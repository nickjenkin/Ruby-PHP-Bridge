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
#ifndef ruby_php_bridge_php_helpers_h
#define ruby_php_bridge_php_helpers_h


#include <php.h>
#include <sapi/embed/php_embed.h>
#include <zend.h>
#include <zend_exceptions.h>
#include <zend_interfaces.h>

zval* rpb_zval_string(char* str);
void rpb_global_var_set(char* name, zval* value);
zval* rpb_zval_null();
zval* rpb_global_var_get(char* name);


#endif
