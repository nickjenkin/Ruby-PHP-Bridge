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

#ifndef ruby_php_bridge_ruby_bridge_h
#define ruby_php_bridge_ruby_bridge_h



#include <Ruby/ruby.h>

#define RBT VALUE


#include "php_helpers.h"
#include "type_helpers.h"


VALUE bridge_module;
VALUE php_exception;
VALUE php_resource;
VALUE php_object;
VALUE var_class;

void Init_ruby_php_bridge();
static VALUE rpb_method_missing(int argc, VALUE *argv, VALUE self);
static VALUE rpb_eval(VALUE class, VALUE code);
static VALUE rpb_include(VALUE mod, VALUE filename);
static VALUE rpb_var_bridge(int argc, VALUE *argv, VALUE self);
static VALUE rpb_var(VALUE mod);

static VALUE rpb_object_method(int argc, VALUE *argv, VALUE self);


#endif
