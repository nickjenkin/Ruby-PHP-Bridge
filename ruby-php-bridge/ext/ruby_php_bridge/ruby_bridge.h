//
//  ruby_bridge.h
//  ruby-php-bridge
//
//  Created by Nick Jenkin on 4/08/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#ifndef ruby_php_bridge_ruby_bridge_h
#define ruby_php_bridge_ruby_bridge_h



#include <Ruby/ruby.h>

#define RBT VALUE


#include "php_helpers.h"
#include "type_helpers.h"


RBT bridge_module;

void Init_ruby_php_bridge();
static VALUE ruby_hello_world(VALUE module);
static VALUE rpb_method_missing(int argc, VALUE *argv, VALUE self);
static VALUE p2r_string(zval* val);


#endif
