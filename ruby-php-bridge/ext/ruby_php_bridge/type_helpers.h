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

#ifndef ruby_php_bridge_type_helpers_h
#define ruby_php_bridge_type_helpers_h

#include <Ruby/ruby.h>
#include "php_helpers.h"

VALUE p2r_string(zval* val);
VALUE p2r_int(zval* val);
VALUE p2r_bool(zval* val);
VALUE p2r_float(zval* val);
VALUE p2r_long(zval* val);
VALUE p2r_double(zval* val);
VALUE p2r_array(zval* val);
VALUE p2r_object(zval* val);
VALUE p2r_resource(zval* val);


zval* r2p_int(VALUE val);
zval* r2p_string(VALUE val);
zval* r2p_bool(VALUE val);
zval* r2p_float(VALUE val);
zval* r2p_long(VALUE val);
zval* r2p_double(VALUE val);
zval* r2p_array(VALUE val);
zval* r2p_object(VALUE val);
zval* r2p_resource(VALUE val);


VALUE p2r_convert(zval* val);
zval* r2p_convert(VALUE val);


#endif
