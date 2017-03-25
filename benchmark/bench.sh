#!/bin/bash
cd `dirname $0`
BENCH_CSV=KEN_ALL.CSV

if [ ! -e $BENCH_CSV ]; then
    curl -s http://www.post.japanpost.jp/zipcode/dl/kogaki/zip/ken_all.zip|funzip > KEN_ALL.CSV
fi

php -d extension=../modules/php_tc.so -f create_hdb.php
seq 1 8|xargs -I{} -P 2 php -d extension=../modules/php_tc.so -f read_hdb.php