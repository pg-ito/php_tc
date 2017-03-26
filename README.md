# php_tc
php extension for [Tokyo Cabinet](http://fallabs.com/tokyocabinet/index.ja.html, "データベースマネージャ Tokyo Cabinet")


# prepare

## install dependency libraries

`yum install -y php-devel zlib-devel bzip2-devel tokyocabinet-devel`


# build

## make
* make with buld shell  
```bash
./bt.sh
```


* make manualy  
```bash
export PKG_CONFIG_PATH=/usr/local/lib/pkgconfig
# confirm Tokyo Cabinet library exists
pkg-config --modversion tokyocabinet

/path/to/php5.X.XX/bin/phpize 
./configure --enable-php_tc --with-php-config=/path/to/php5.X.XX/bin/php-config
make
# test code
/path/to/php5.X.XX/bin/php -d extension=modules/php_tc.so -f php_tc.php 
```


## install

### copy to configured path
`make install`

### modify php.ini or related *.ini file  
e.g. 
`exntension=php_tc.so`

### restart apache or another webserver
e.g.  
`service httpd restart`


# usage

## create tokyocabinet HashDB  
```php
$php_tc_obj = new phpTC('/dev/shm/casket.tch', true);// open TC HashDB for create and writer mode
$php_tc_obj->set('foo_key','foo_value'));// write record foo_key
$php_tc_obj->set('bar_key','bar_value'));// write record bar_key
unset($php_tc_obj);// close DB
```


## read tokyocabinet HashDB  
```php
$php_tc_obj = new phpTC('/dev/shm/casket.tch');// open TC HashDB for read only without lock mode
var_dump( $php_tc_obj->get('foo_key') );// read record foo_key
var_dump( $php_tc_obj->get('bar_key') );// read record bar_key
unset($php_tc_obj);// close DB
```


# note  
Tokyo Cabinet seems to need mmap.  
Therefore do not recommend using on NFS or similar file systems.