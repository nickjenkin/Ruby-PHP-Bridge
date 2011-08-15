/*
 *
 * Copyright 2011 Nick Jenkin
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 * http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "type_helpers.h"
#include "ruby_bridge.h"
/** PHP to Ruby type convertions */

VALUE p2r_string(zval* val) {
    return rb_str_new(Z_STRVAL_P(val), Z_STRLEN_P(val));
}

VALUE p2r_int(zval* val) {
    return rb_int_new(Z_LVAL_P(val));
}

VALUE p2r_bool(zval* val) {
    
    if(zval_is_true(val)) {
        return Qtrue;
    }
    
    return Qfalse;
    
}

VALUE p2r_hashtable(zval* val) {
    
    VALUE res = rb_hash_new();
    HashTable *arr_hash;
    HashPosition pointer;
    zval **data;
    
    arr_hash = Z_ARRVAL_P(val);
    /*ZEND_API int zend_hash_get_current_key_ex(const HashTable *ht, char **str_index, uint *str_length, ulong *num_index, zend_bool duplicate, HashPosition *pos);
     ZEND_API int zend_hash_get_current_key_type_ex(HashTable *ht, HashPosition *pos);*/
    
    char *str_key;
    uint key_len;
    ulong int_key;
    
    for(zend_hash_internal_pointer_reset_ex(arr_hash, &pointer); zend_hash_get_current_data_ex(arr_hash, (void**) &data, &pointer) == SUCCESS; zend_hash_move_forward_ex(arr_hash, &pointer)) {
        
        VALUE key;
        zend_hash_get_current_key_ex(arr_hash, &str_key, &key_len, &int_key,0, &pointer);
        if(zend_hash_get_current_key_type_ex(arr_hash, &pointer) == IS_STRING) {
            key = rb_str_new(str_key, key_len);
        }
        else {
            key = rb_int_new(int_key);
        }
        VALUE tmp = p2r_convert(*data);
        
        rb_hash_aset(res, key, tmp);
        
    }
    
    
    return res;
}

VALUE p2r_resource(zval* val) {
    PHPResource* resource;
    VALUE res;
    
    resource = ALLOC(PHPResource);
    resource->resource = val;
    
    res = Data_Wrap_Struct(php_resource, 0, free, resource);
    return res;
}

VALUE p2r_object(zval* val) {
    PHPObjectData* obj;
    VALUE res;
    
    obj = ALLOC(PHPObjectData);
    obj->obj = val;
    res = Data_Wrap_Struct(php_object, 0, free, obj);
    return res;
}

VALUE p2r_convert(zval* val) {
    VALUE res;
    
    if(val == NULL) {
        return Qnil;
    }
    
    
  //  printf("type: %d\n", Z_TYPE_P(val));
  //  return Qnil;
    switch (Z_TYPE_P(val)) {
        
        case IS_STRING:
            res = p2r_string(val);
            break;
            
        case IS_LONG:
            res = p2r_int(val);
            break;
            
        case IS_BOOL:
            res = p2r_bool(val);
            break;
            
        case IS_NULL:
            res = Qnil;
            break;
        
        case IS_ARRAY:
            res = p2r_hashtable(val);
            break;
        
        case IS_RESOURCE:
            res = p2r_resource(val);
            break;
        
        case IS_OBJECT:
            res = p2r_object(val);
            break;
            
        default:
            res = Qnil;
            rb_raise(rb_eTypeError, "unable to convert PHP type to Ruby type");
            break;
    }
    
    return res;
    
}

/** Ruby to PHP type convertions */

zval* r2p_int(VALUE val) {
    
    zval *temp;
    
    MAKE_STD_ZVAL(temp);
    
    ZVAL_LONG(temp, FIX2INT(val));
    
    return temp;
}


zval* r2p_string(VALUE val) {
    return rpb_zval_string(StringValuePtr(val));
}

zval* r2p_bool(VALUE val) {
    
    zval *temp;
    MAKE_STD_ZVAL(temp);
    
    if(TYPE(val) == T_TRUE) {
        ZVAL_BOOL(temp, 1);
    }
    else {
        ZVAL_BOOL(temp, 0);
    }
    return temp;
}

static int r2p_hashtable_each(VALUE key, VALUE value, zval* arr) {
    
    zval* ztmp = r2p_convert(value);
    
    if(TYPE(key) == T_STRING) {
        add_assoc_zval(arr, StringValueCStr(key), ztmp);
    }
    else {
        add_index_zval(arr, FIX2LONG(key), ztmp);
    }
    
    return ST_CONTINUE;
}

zval* r2p_hashtable(VALUE hash) {
    
    zval* arr;
    MAKE_STD_ZVAL(arr);
    array_init(arr);
    
    
    rb_hash_foreach(hash, r2p_hashtable_each, (VALUE)arr);
    
    
    return arr;
}

zval* r2p_array(VALUE val) {
    zval *arr;
//   ALLOC_INIT_ZVAL(arr);
    MAKE_STD_ZVAL(arr);
    array_init(arr);
    
    VALUE tmp = rb_ary_shift(val);
    int i=0;
    while(tmp != Qnil) {
        zval* ztmp = r2p_convert(tmp);
        add_index_zval(arr, i, ztmp);
        i++;
        
        tmp = rb_ary_shift(val);
    }
    
    return arr;
}

zval* r2p_range(VALUE val) {
    
    VALUE arr = rb_funcall(val, rb_intern( "to_a" ), 0);
    
    
    ///printf("type: %x", TYPE(arr));
    //return rpb_zval_null();
    return r2p_array(arr);
}

zval* r2p_object(VALUE val) {
    
    const char* name = rb_obj_classname(val);
    
    if(strcmp(name, "Range") == 0) {
        return r2p_range(val);
    }
    
    return rpb_zval_null();
           
}

zval* r2p_data(VALUE val) {
    
    const char* name = rb_obj_classname(val);
    
    if(strcmp(name, "PHP::PHPBridgeObject") == 0) {
        PHPObjectData *p;
        Data_Get_Struct(val, PHPObjectData, p);
        return p->obj;
    }
    else if(strcmp(name, "PHP::PHPBridgeResourceObject") == 0) {
        PHPResource *p;
        Data_Get_Struct(val, PHPResource, p);
        return p->resource;
    }
    
    return rpb_zval_null();
}

zval* r2p_convert(VALUE val) {
    zval* res;
    
    
    if(val == NULL) {
        return NULL;
    }
    
    switch (TYPE(val)) {
        case T_FIXNUM:
            res = r2p_int(val);
            break;
        case T_STRING:
            res = r2p_string(val);
            break;
            
        case T_TRUE:
        case T_FALSE:
            res = r2p_bool(val);
            break;
        
        case T_HASH:
            res = r2p_hashtable(val);
            break;
            
        case T_ARRAY:
            res = r2p_array(val);
            break; 
        
        case T_NIL:
            res = rpb_zval_null();
            break;
        
        case T_DATA:
            res = r2p_data(val);
            break;
            
        case T_OBJECT:
            res = r2p_object(val);
            break;
            
        default:
            rb_raise(rb_eTypeError, "unable to convert Ruby type to PHP type (%x)", TYPE(val));
            break;
    }
    
    return res;
    
}
