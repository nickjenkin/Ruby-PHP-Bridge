//
//  php_helpers.c
//  ruby-php-bridge
//
//  Created by Nick Jenkin on 4/08/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

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
 