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

#include "ruby_bridge.h"
#include <stdio.h>
#include <string.h>

void Init_ruby_php_bridge() {
    
    
    static char* argv[] = {"ruby_php_bridge"};
    php_embed_init(1, argv PTSRMLS_CC);
    
    
    bridge_module = rb_define_module("PHP");

    
    /* exceptions */
    php_exception = rb_define_class("PHPBridgeException", rb_eScriptError);
    
    /* classes */
    var_class = rb_define_class_under(bridge_module, "PHPGlobalVariableBridge", rb_cObject);
    php_resource = rb_define_class_under(bridge_module, "PHPBridgeResourceObject", rb_cObject);
    php_object = rb_define_class_under(bridge_module, "PHPBridgeObject", rb_cObject);

    
    /* function/method accessors */
    rb_define_singleton_method(bridge_module, "method_missing", rpb_method_missing, -1);
    rb_define_method(php_object, "method_missing", rpb_object_method, -1);
    
    /* language construct implementations */
    rb_define_singleton_method(bridge_module, "eval", rpb_eval, 1);
    rb_define_singleton_method(bridge_module, "include", rpb_include, 1);
    
    /* variable/type accessors */
    rb_define_singleton_method(bridge_module, "var", rpb_var, 0);
    rb_define_singleton_method(var_class, "method_missing", rpb_var_bridge, -1);


    
}

static VALUE rpb_method_missing(int argc, VALUE *argv, VALUE self) {
    
    VALUE r_name, *args;
    rb_scan_args(argc, argv, "1*", &r_name, &args);
    char* name = (char*)rb_id2name(SYM2ID(r_name)); 
    
    printf("PHP.%s(%d)\n", name, argc-1);
    zval* funcname = rpb_zval_string(name);
    
    zval* retval;
    zend_fcall_info fci = empty_fcall_info;
    zend_fcall_info_cache fcc = empty_fcall_info_cache;
    fci.param_count = argc-1;

    zval ***params = ecalloc(fci.param_count, sizeof(zval **));

    for(int i=1; i < argc; i++) {
        
        zval *pzval = r2p_convert(argv[i]);
        
        params[i-1] = emalloc(sizeof(zval *));
        *params[i-1] = pzval;
        
    }
    
    fci.params = params;
    fci.function_name = funcname;
    fci.retval_ptr_ptr = &retval;
    fci.function_table =  EG(function_table);
    fci.size = sizeof(fci);
    
    
    zend_call_function(&fci, &fcc TSRMLS_CC);
    
    

    
    // clean up
    for (int i = 0; i < fci.param_count; i++) {
   //     zval_ptr_dtor(params[i]);
        efree(params[i]);
    }
    
    efree(params);
    zval_ptr_dtor(&funcname);
    
    VALUE result = Qnil;
    
    if(retval) {
        result = p2r_convert(retval);
        
        if(Z_TYPE_P(retval) != IS_RESOURCE) {
            zval_ptr_dtor(&retval);
        }
    }
    return result;

}

static VALUE rpb_eval(VALUE class, VALUE code) {

    
    Check_Type(code, T_STRING);
    char* ccode = StringValueCStr(code);
    zval retval;

    
    if (zend_eval_string_ex(ccode, &retval, "Ruby.PHP.eval", 1) == FAILURE) {
        //zend_throw_exception(NULL, "eval_stmt() from Ruby failed", 0);
        return Qnil;
    }
    
    VALUE res = Qnil;
    
    
    res = p2r_convert(&retval);
    zval_dtor(&retval);

    
    return res;
}


static VALUE rpb_include(VALUE mod, VALUE filename) {
    return Qnil;
}

static VALUE rpb_var_bridge(int argc, VALUE *argv, VALUE self) {
    
    VALUE r_name, value;
    rb_scan_args(argc, argv, "11", &r_name, &value);
    char* name = (char*)rb_id2name(SYM2ID(r_name)); 

    
    
    if(strrchr(name, '=') != NULL) {
        char *varname = (char*) malloc(strlen(name)-1);
        strncpy(varname, name, strlen(name)-1);
        
        rpb_global_var_set(varname, r2p_convert(value));
        
        free(varname);
    }
    else {

        zval* val = rpb_global_var_get(name);
        return p2r_convert(val);
    
    }
    
    
    return Qnil;
}
/*ZEND_API zval* zend_call_method(zval **object_pp, zend_class_entry *obj_ce, zend_function **fn_proxy, char *function_name, int function_name_len, zval **retval_ptr_ptr, int param_count, zval* arg1, zval* arg2 TSRMLS_DC);
*/
static VALUE rpb_object_method(int argc, VALUE *argv, VALUE self) {
    
    
    zval* obj = r2p_data(self);
    
    VALUE rname, args;
    rb_scan_args(argc, argv, "1*", &rname, &args);
    
    char* method_name = (char*)rb_id2name(SYM2ID(rname));
    rpb_lower(method_name);
    
    zval* z_method_name = rpb_zval_string(method_name);
    
    zval *retval;
    
   // zend_class_entry* ce = Z_OBJ_HT(*obj)->get_class_entry;
    zend_class_entry *ce = Z_OBJCE(*obj);
    
    
    //printf("%s.%s\n", ce->name, method_name);
    
    
    zend_fcall_info fci = empty_fcall_info;
    zend_fcall_info_cache fcc = empty_fcall_info_cache;
    
    
    fci.size = sizeof(fci);
    fci.function_table = &ce->function_table;
    
    fcc.initialized = 1;
    fcc.calling_scope = ce;
    fcc.called_scope = Z_OBJCE_PP(&obj);
    fcc.object_ptr = obj;
    
    fci.param_count = argc-1;
    
    zval ***params = ecalloc(fci.param_count, sizeof(zval **));
    
    for(int i=1; i < argc; i++) {
        
        zval *pzval = r2p_convert(argv[i]);
        
        params[i-1] = emalloc(sizeof(zval *));
        *params[i-1] = pzval;
        
    }
    
    fci.params = params;
    fci.function_name = z_method_name;
    fci.retval_ptr_ptr = &retval;
    fci.no_separation = 1;
    fci.symbol_table = NULL;
    //zend_print_pval(fci.function_name,0);
    //rpb_print_hashtable(fci.function_table);
    
    if(zend_hash_find(fci.function_table, method_name, strlen(method_name)+1, (void **) &fcc.function_handler) == FAILURE) {
        rb_raise(php_exception, "Unable to find implementation for method %s::%s", ce->name, method_name);
        
    }
    
    int result = zend_call_function(&fci, &fcc TSRMLS_CC);

    if (result == FAILURE) {
   //     printf("failure\n");  
        rb_raise(php_exception, "Unable to call method %s::%s", ce->name, method_name);
    }
        
    efree(params);
    
    return p2r_convert(retval);
}

static VALUE rpb_var(VALUE mod) {
    return var_class;
}


