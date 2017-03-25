<?php
error_reporting(E_ALL);
$br = (php_sapi_name() == "cli")? "":"<br>";

if(!extension_loaded('php_tc')) {
	dl(__DIR__.'modules/php_tc.' . PHP_SHLIB_SUFFIX);
}
$module = 'php_tc';
$functions = get_extension_funcs($module);
echo "Functions available in the test extension:$br\n";
foreach($functions as $func) {
    echo $func."$br\n";
}
echo "$br\n";
$function = 'confirm_' . $module . '_compiled';
if (extension_loaded($module)) {
	$str = $function($module);
} else {
	$str = "Module $module is not compiled into PHP";
}

echo "$str\n";

echo php_tc_test('test arg').PHP_EOL;

echo '=========== class test ==========='.PHP_EOL;
$php_tc_obj = new phpTC('/dev/shm/casket.tch', true);
var_dump($php_tc_obj);

var_dump('dbname: '.$php_tc_obj->getFileName());
$unixtime = time();
echo 'unixtimestamp: '.$unixtime.PHP_EOL;
var_dump('set value1: '.$php_tc_obj->set('testkey1','testvalue1_'.$unixtime));
var_dump('set value2: '.$php_tc_obj->set('testkey2','testvalue2_'.$unixtime));
var_dump('get value1: '.$php_tc_obj->get('testkey1'));
var_dump('get value2: '.$php_tc_obj->get('testkey2'));
var_dump('get unknownkey1: '.$php_tc_obj->get('unknownkey1'));
var_dump($php_tc_obj->get('unknownkey2'));
