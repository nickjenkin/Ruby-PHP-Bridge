//
//  main.c
//  ruby-php-bridge
//
//  Created by Nick Jenkin on 4/08/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#include <stdio.h>
#include "ruby_bridge.h"

int main (int argc, const char * argv[])
{

    ruby_init();

    Init_ruby_php_bridge();
    ruby_script("embedded");

    rb_load_file("test.rb");
    ruby_run();
    
    printf("\n");
    return 0;
    
    
}

