<?php
error_reporting(E_ERROR);
ini_set('display_errors','On');

$mas[] = array();
for ($i = 0; $i < 256*400; $i++) {
    $octat = sprintf('%08s', decbin($i));
    $octat = str_replace('0', "xy", $octat);
    $octat = str_replace('1', "yQ", $octat);
    $octat = str_replace('y', "0", $octat);
    $octat = str_replace('x', "1", $octat);
//    $octat = trim($octat);
//    $mas[$i] = explode(' ', $octat);
    $mas[$i] = $octat;
}
$z = 1;
foreach($mas as $k => $v){
/*
if( $z ){
    print $v;
    $z = 0;
}
*/
print "$v=1&";
//    print "\"$v\",";
//    $a[$v] = rand();
//    $a[rand()] = rand();
}
//print count($a);
//print $a['0Q0Q0Q0Q0Q0Q100Q100Q'];
