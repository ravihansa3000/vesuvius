<?php
$host="localhost";
$db="new";
$user="root";
$pass="sneha227";
$bd = mysql_connect($host, $user, $pass) or die("Could not connect database");
    mysql_select_db($db, $bd) or die("Could not select database");
?>