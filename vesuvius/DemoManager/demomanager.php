<?php

include('connection.php');
global $output,$u,$t,$s,$w,$v,$d;
if( (isset($_POST['shortname'])) && (isset($_POST['dbname'])) && (isset($_POST['pswrd'])) && (isset($_POST['hname'])) && (isset($_POST['dbUser'])))
{
$u=$_POST['shortname'];
$t=$_POST['dbname'];
$s=$_POST['pswrd'];
$w=$_POST['hname'];
$v=$_POST['dbUser'];
$d=$_POST['expdate'];
$output =shell_exec("sh /home/sneha/clone.sh $u $t $s $w $v");
if(!$output)
{
  echo "error occured";
}
}
//$output =shell_exec("echo " . $uname  );



$res=mysql_query("INSERT INTO demoDb(shortname, username, password, dbUser,ctime,expDate)VALUES('$u', '$t', '$s', '$v',now(),'$d')");
//require 'inc/db-connect.php';

header('location:adminpage.php');
  




?>