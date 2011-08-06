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

VALUE p2r_array(zval* val) {
    
    VALUE res = rb_ary_new();
    
    
    
    return res;
}

VALUE p2r_convert(zval* val) {
    VALUE res;
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
    
    ALLOC_INIT_ZVAL(temp);
    
    ZVAL_LONG(temp, FIX2INT(val));
    
    return temp;
}


zval* r2p_string(VALUE val) {
    return rpb_zval_string(StringValuePtr(val));
}

zval* r2p_bool(VALUE val) {
    
    zval *temp;
    ALLOC_INIT_ZVAL(temp);
    
    if(TYPE(val) == T_TRUE) {
        ZVAL_BOOL(temp, 1);
    }
    else {
        ZVAL_BOOL(temp, 0);
    }
    return temp;
}

zval* r2p_convert(VALUE val) {
    zval* res;
    
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
            
        default:
            rb_raise(rb_eTypeError, "unable to convert Ruby type to PHP type");
            break;
    }
    
    return res;
    
}
