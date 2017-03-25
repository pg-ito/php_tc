<?php
$benchset = 10;
$tchfile = '/dev/tmpfile/benchfile.tch';
$keylistfile = 'keylistfile.txt';

$tc = new phpTC($tchfile, false);// read only open without lock.
$fp = fopen($keylistfile, 'r');

$keys = [];
if (!$fp) {
    exit(255);
}

if(!$tc){
    error_log('tc open error!');
    exit(255);
}
while (($key = fgets($fp, 4096)) !== false) {
    $keys[] = trim($key);
    
}
if (!feof($fp)) {
    echo 'Error: unexpected fgets() fail'.PHP_EOL;
}
fclose($fp);
// echo 'count($keys) : '.count($keys).PHP_EOL;
shuffle($keys);// prepare random read
$results = [];
for($i=0;$i<$benchset;++$i){
    $results[] = bench($tc, $keys);
}
$total = (object)[
    'total_sec'=>0,
    'total_count'=>0,
    'total_error'=>0,
    'total_qps'=>0,
    'benchset'=>$benchset,
    'db_records'=>count($keys)
];

foreach($results as $k=>$res){
    $total->total_sec+=$res->sec;
    $total->total_count+=$res->count;
    $total->total_error+=$res->error;
}
$total->total_qps = round($total->total_count/$total->total_sec, 3);
$total->total_sec = round($total->total_sec, 6);
$view = print_r($total, true);
echo $view.PHP_EOL;
function bench($tc, $keys){
    $count = 0;
    $error = 0;
    $start_mtime = microtime(true);
    foreach($keys as $k){
        // var_dump($k);
        $val = $tc->get($k);
        // var_dump($val);
        if($val===false){
            ++$error;
        }
        ++$count;
    }

    $end_mtime = microtime(true);
    $elapsed = ($end_mtime-$start_mtime);
    return (object)['sec'=>$elapsed, 
    'count'=>$count,
    'error'=>$error,
    'qps'=>round($count/$elapsed, 3)];
}



