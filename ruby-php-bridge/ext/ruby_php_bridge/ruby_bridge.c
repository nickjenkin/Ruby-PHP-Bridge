//
//  ruby_bridge.c
//  ruby-php-bridge
//
//  Created by Nick Jenkin on 4/08/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#include <stdio.h>
#include "ruby_bridge.h"

void Init_ruby_php_bridge() {
    
    
    static char* argv[] = {"ruby_php_bridge", NULL};
    php_embed_init(1, argv PTSRMLS_CC);
    
    bridge_module = rb_define_class("PHP", rb_cObject);
    //bridge_module = rb_define_module("PHP");
  //  rb_include_module(bridge_module);
    

    
    rb_define_singleton_method(bridge_module, "hello", ruby_hello_world, 0);
    rb_define_singleton_method(bridge_module, "method_missing", rpb_method_missing, -1);
    
    
    printf("good to go commander!\n");
    
    
}

static VALUE rpb_method_missing(int argc, VALUE *argv, VALUE self) {

    VALUE r_name, args;
    rb_scan_args(argc, argv, "11*", &r_name, &args);
    const char* name = rb_id2name(SYM2ID(r_name)); 
    
    
    zval* arg1 = rpb_zval_string(StringValuePtr(args));
    zval* funcname = rpb_zval_string(name);
    
    zval* retval;
    
    zval **params[2];
    params[0] = &arg1;

    zend_fcall_info fci = empty_fcall_info;
    zend_fcall_info_cache fcc = empty_fcall_info_cache;
    
    fci.function_name = funcname;
    fci.retval_ptr_ptr = &retval;
    fci.function_table =  EG(function_table);
    fci.size = sizeof(fci);
    fci.params = params;
    fci.param_count = 1;
    zend_call_function(&fci, &fcc TSRMLS_CC);
    
    
    //return rb_str_new(Z_STRVAL_P(retval), Z_STRLEN_P(retval));
    return p2r_string(retval);
    //return T_NIL;
}


static VALUE ruby_hello_world(VALUE module) {
    
    printf("Hello World!\n");
    
    return T_NIL;
}

static VALUE p2r_string(zval* val) {
    return rb_str_new(Z_STRVAL_P(val), Z_STRLEN_P(val));
    //   return T_NIL;
}

