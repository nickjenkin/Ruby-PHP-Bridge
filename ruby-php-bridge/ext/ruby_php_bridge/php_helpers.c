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

#include <stdio.h>
#include "php_helpers.h"

/*typedef struct _zend_fcall_info {
 size_t size;
 HashTable *function_table;
 zval *function_name;
 HashTable *symbol_table;
 zval **retval_ptr_ptr;
 zend_uint param_count;
 zval ***params;
 zval *object_ptr;
 zend_bool no_separation;
 } zend_fcall_info;
*/



zval* rpb_zval_string(char* str) {
    zval *new_string;
    
    MAKE_STD_ZVAL(new_string);
    ZVAL_STRING(new_string, str, 1);
    
    return new_string;
}

zval* rpb_zval_null() {
    zval *res;
    MAKE_STD_ZVAL(res);
    ZVAL_NULL(res);
    return res;
}

void rpb_global_var_set(char* name, zval* value) {
    
    zend_hash_add(&EG(symbol_table), name, strlen(name) + 1, &value, sizeof(zval*), NULL);

}

zval* rpb_global_var_get(char* name) {
    
    zval **value;
    if (zend_hash_find(&EG(symbol_table), name, strlen(name) + 1, (void**)&value) == FAILURE) {
        return rpb_zval_null();
    }
    
    return *value;
}


void rpb_print_hashtable(HashTable *arr_hash) {
    HashPosition pointer;

        char *str_key;
    uint key_len;
    ulong int_key;
    zval **data;
    
    for(zend_hash_internal_pointer_reset_ex(arr_hash, &pointer); zend_hash_get_current_data_ex(arr_hash, (void**) &data, &pointer) == SUCCESS; zend_hash_move_forward_ex(arr_hash, &pointer)) {
        

        zend_hash_get_current_key_ex(arr_hash, &str_key, &key_len, &int_key,0, &pointer);
        if(zend_hash_get_current_key_type_ex(arr_hash, &pointer) == IS_STRING) {
            zend_print_pval_r(data,0);
        }
        else {
            zend_print_pval_r(data,0);
        }
        
        
        
    }

}
