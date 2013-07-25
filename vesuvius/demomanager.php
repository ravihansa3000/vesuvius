<?php
session_start();

if( (isset($_POST['shortname'])) && (isset($_POST['dbname'])) && (isset($_POST['pswrd'])) && (isset($_POST['hname'])) && (isset($_POST['dbUser'])))
{
$u=$_POST['shortname'];
$t=$_POST['dbname'];
$s=$_POST['pswrd'];
$w=$_POST['hname'];
$v=$_POST['dbUser'];
$output =shell_exec("sh clone.sh $u $t $s $w $v");
//$output =shell_exec("echo " . $uname  );
var_dump ($output);
}

//shell_exec('sh /home/sneha/clone.sh " $uname "');
//echo "<pre>$output</pre>";
//$output = array();
//var_dump($output);





?>

