#!/bin/bash -x

# rm -f Makefile.*
${HOME}/php-ext/bin/phpize 
./configure --enable-php_tc --enable-debug --with-php-config=${HOME}/php-ext/bin/php-config 
if [ "$1" = "clean" ]; then
    make clean
fi
make -j 4 

${HOME}/php-ext/bin/php -d extension=modules/php_tc.so -f php_tc.php