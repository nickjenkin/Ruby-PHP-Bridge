//
//  php_helpers.h
//  ruby-php-bridge
//
//  Created by Nick Jenkin on 4/08/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#ifndef ruby_php_bridge_php_helpers_h
#define ruby_php_bridge_php_helpers_h


#include <php.h>
#include <sapi/embed/php_embed.h>
#include <zend.h>
#include <zend_exceptions.h>
#include <zend_interfaces.h>

zval* rpb_zval_string(char* str);

#endif
