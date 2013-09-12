<?php
/**
 * @name         Demo Instance
 * @version      1
 * @package      demoM
 * @author       Sneha Bhattacharya <sneha.22.7@gmail.com> 
 * @about        Developed in whole or part by the U.S. National Library of Medicine and the Sahana Foundation
 * @link         https://pl.nlm.nih.gov/about
 * @link         http://sahanafoundation.org
 * @license	 http://www.gnu.org/licenses/lgpl-2.1.html GNU Lesser General Public License (LGPL)
 * @lastModified 2012.0206
 */




$global['approot'] = getcwd()."/../../";
require("../../conf/sahana.conf");
require("./conf.inc");

$q="select * from created;";
$res=mysql_query("$q");
while($row = mysql_fetch_array($res, MYSQL_ASSOC)){
	$exp=$row['exptime'];
	$email=$row['email'];
	$current=now();
	if($exp>$current){
	$to_time = strtotime("$exp");
	$from_time = strtotime("$current");

	$left= round(abs($from_time - $to_time) / 60,2). " minute";
	$subject  = "Your demo instance details...";

				$bodyHTML = "
					Dear ".$username.",<br>
					<br>
					Your demo instance is about to expire
					<br>
					You have ".$left." minutes left
					<br>
					All the best,<br>
					Admins<br>
				";
			$p->sendMessage($email, $email, $subject, $bodyHTML, $bodyHTML);
	}
	else
	{	
		$sname=$row['sname'];
		$dbname=$row['dbname'];
		$output=shell_exec("sh /home/sneha/unclone.sh $sname $dbname");
	}

}


?>