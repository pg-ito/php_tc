<?php
if(!extension_loaded('php_tc')) {
    error_log('ERROR!! php_tc module is not loaded.');
    exit(255);
}
$tchfile = '/dev/tmpfile/benchfile.tch';
$keylistfile = 'keylistfile.txt';
$csvfile = 'KEN_ALL.CSV';
$tc = new phpTC($tchfile, true);


$fp = fopen($csvfile, 'r');
$keys = [];
if (!$fp) {
    exit(255);
}
$count = 0;
while (($line = fgets($fp, 4096)) !== false) {
    $key = hash('sha256',$line);
    $keys[] = $key;
    $tc->set($key, serialize(explode(',', $line) ));
    ++$count;
}
echo 'count : '.$count.PHP_EOL;
if (!feof($fp)) {
    echo 'Error: unexpected fgets() fail'.PHP_EOL;
}
fclose($fp);

// check values
foreach($keys as $k){
    $v = $tc->get($k);
    // var_dump($v);
    if(false === $v){
        error_log('Error!! cannot get '.$k);
    }
}
file_put_contents($keylistfile, implode(PHP_EOL, $keys));
