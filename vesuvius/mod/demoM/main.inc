<?
/**
 * @name         Demo Manager
 * @version      13
 * @package      pref
 * @author       Sneha Bhattacharya sneha.22.7@gmail.com
 * @about        Developed in whole or part by the U.S. National Library of Medicine
 * @link         https://pl.nlm.nih.gov/about
 * @license	 http://www.gnu.org/licenses/lgpl-2.1.html GNU Lesser General Public License (LGPL)
 * @lastModified 2012.0213
 */
global $tabid;
$tabid = isset($_GET['tabid']) ? $_GET['tabid'] : 0;
$tabid = "&tabid=".$tabid;

require_once($global['approot']."/inc/lib_modules.inc");
require_once($global['approot']."/inc/lib_menu.inc");
require_once($global['approot']."/inc/lib_form.inc");
require_once($global['approot']."/inc/lib_validate.inc");
require_once($global['approot']."/inc/lib_errors.inc");
require_once($global['approot']."/mod/lpf/lib_lpf.inc");
require_once($global['approot']."/inc/lib_security/lib_auth.inc");
require_once($global['approot']."/inc/lib_security/constants.inc");
require_once($global['approot']."/inc/lib_security/errors.inc");
require_once($global['approot']."/inc/lib_security/lib_acl.inc");
 
 function shn_demoM_default()
 {	
 	$control = "";
 	if(!isset($_SESSION['group_id']) || $_SESSION['group_id'] != 1){
	shn_tabmenu_open();
	shn_tabmenu_item("session",  _t("Demo-TableHeader|Session Information"), "demoM");
	shn_tabmenu_item("password", _t("Demo-TableHeader|Change My Password"),  "demoM");
	shn_tabmenu_item("reqInstance", _t("Demo-TableHeader|Request for instance"),  "demoM");
	
	
	shn_tabmenu_close();
	if(isset($_GET['reqInstance'])){
		$control ="shn_demoM_reqInstance";
	}
	if(isset($_GET['addUser'])){
		$control ="shn_demoM_addUser";
	}
	if(isset($_GET['request'])){
		$control="shn_demoM_request";
	}
	$control();
	}	

	else if($_SESSION['group_id']==1){

 	shn_tabmenu_open();
	shn_tabmenu_item("session",  _t("Demo-TableHeader|Session Information"), "demoM");
	shn_tabmenu_item("password", _t("Demo-TableHeader|Change My Password"),  "demoM");
	shn_tabmenu_item("adminDashboard", _t("Demo-TableHeader|Admin Dashboard"),  "demoM");
	shn_tabmenu_item("createInstance", _t("Demo-TableHeader|Create Instance"),  "demoM");
	shn_tabmenu_item("testInstance", _t("Demo-TableHeader|Test Instance"),  "demoM");
	shn_tabmenu_item("pendingNotifications", _t("Demo-TableHeader|View pending notifications"),  "demoM");
	shn_tabmenu_item("show_notifications", _t("Demo-TableHeader|Show notifications"),  "demoM");
	shn_tabmenu_close();

	

	if(isset($_GET['forgot'])) {
		$control = "shn_demo_forgotPassword";

	} elseif(isset($_GET['password'])) {
		$control = "shn_demo_ch_passwd";

	} elseif(isset($_GET['passwdp'])) {
		$control = "shn_demo_ch_passwd_p";

	} elseif(isset($_GET['create2'])) {
		$control = "shn_demoM_create2";

	}
	elseif(isset($_GET['create3'])) {
		$control = "shn_demoM_create3";

	} elseif(isset($_GET['login'])) {
		$control = "shn_demo_loginForm";
	}
	elseif(isset($_GET['adminDashboard'])){
        $control="shn_demoM_adminDashboard";
	}
	elseif(isset($_GET['pendingNotifications'])){
        $control="shn_demoM_pendingNotifications";
	}

	elseif(isset($_GET['createInstance'])){
        $control="shn_demoM_createInstance";
	}
	elseif(isset($_GET['testInstance'])){
	$control="shn_demoM_testInstance";
}
	elseif(isset($_GET['test2'])){
		$control="shn_demoM_test2";
	}
	elseif(isset($_GET['test3'])){
		$control="shn_demoM_test3";
	}
	elseif(isset($_GET['show_notifications'])){
		$control="shn_demoM_show_notifications";
	}

	if($control == "") {
		$control = "shn_demo_session";
	}
	

	$control();
}
 }
function shn_demoM_reqInstance(){
	global $global;

	$user=$_SESSION['user'];
	
	$query="select created from flag where user = '$user';";
	$res=mysql_query($query);
	$row=mysql_fetch_array($res);
	$val=$row['created'];
	
	if($val==1){
		$result="select * from created where sname = '$user';";
		$res=mysql_query($result);
		echo "<table class=\"emTable\">
			<tr>
				<td style=\"font-size: 120%; padding-left: 8px;\" ><b>Organisation</b></td>
				<td style=\"font-size: 120%; padding-left: 8px;text-align: center;\" ><b>Short Name</b></td>
				<td style=\"font-size: 120%; padding-left: 8px;text-align: center;\" ><b>Time left</b></td>
			</tr>";
			echo "<meta http-equiv='refresh' content='10;'>";
		
		
		  		

		while($row = mysql_fetch_array($res, MYSQL_ASSOC)){
	
		 $temp=$row['exptime'];
         $date = new DateTime("$temp");
         $now = new DateTime();
         echo "<tr><td>" . $row['sname'] . "</td><td>" . $row['username'] . "</td><td>" . $date->diff($now)->format("%d days, %h hours and %i minutes") . "</td></tr>";
    
		//echo "<tr><td>" . $row['sname'] . "</td><td>" . $row['Db_name'] . "</td><td>" . $left . "</td></tr>";

	}

	}

	
	else{
	$policy = shn_get_password_policy();

	if(count($policy) > 0){
		echo "<br><br><h4>Passwords must adhere to the following rules :</h4>";
		for($i=0; $i < count($policy); $i++) {
			echo ($i+1).". ".$policy[$i]."<br/>";
		}
	}
	echo "<div id=\"formcontainer\">";
	shn_form_fopen("request");
	$extra_opts['req']=true;


	shn_form_text("Shortname",                     'sname',   'size="30"', $extra_opts);
	shn_form_text("Organisation name",             'org_name','size="30"', $extra_opts);
	shn_form_text("Username",                      'username','size="30"', $extra_opts);
	shn_form_text("Email id",                      'email',   'size="30"', $extra_opts);
	
	if(isset($_POST['password']) || isset($_POST['re_password'])) {
		unset($_POST['password']);
		unset($_POST['re_password']);
	}

	shn_form_password("Password for Login and db", "pswrd",    null, $extra_opts);
	shn_form_password("Re-enter password",         "pswrd",    null, $extra_opts);
	

	shn_form_fsclose();
	$extra_opts['req']=true;


 	shn_form_submit(_t("Submit"), "class=\"styleTehButton\"");
	echo "</br><br/><br/>";
	shn_form_fclose();
	echo "</div>";
	}

}
function shn_demoM_request()
{
	if ((isset($_POST['sname'])) && (isset($_POST['org_name']))&& (isset($_POST['username'])) && (isset($_POST['pswrd'])) && (isset($_POST['email'])) )
{
	$ret = shn_demoM_addUser($_POST['sname'],($_POST['org_name']),($_POST['username']),($_POST['pswrd']) , ($_POST['email']));
	if($ret) {
			$msg = "A new user account has been created for you and is now active. Please login below using your credentials.";
			add_confirmation($msg);
			
		}
}
}
function shn_demoM_addUser($sname,$org_name,$username,$pswrd,$email){

		
		// Create the encrypted password
		//$salt1           = _shn_generateSalt();
		//$salt2           = _shn_generateSalt();
		//$salt            = $salt1.$salt2;
		//$user_password   = substr($pswrd, 0, 4).$salt.substr($user_password, 4);
		$stored_password = md5(trim($pswrd));
		//$time            = now();
		$confirmation    = md5(uniqid(rand(), true)); // code used to activate account

		$q = "
			INSERT INTO DemoUser(sname,org_name,time,username,pswrd,email)
			VALUES('".mysql_real_escape_string($sname)."','".mysql_real_escape_string($org_name)."', now(),'".mysql_real_escape_string($username)."','".$stored_password."','".mysql_real_escape_string($email)."');
		";
		
		$res = mysql_query($q);
		if($res==true)
		{
			$msg = "Your request will be processed by the admin";
			add_confirmation($msg);
		}
		if($res == false) {
			add_error($db->ErrorMsg());
			
		}

		$p ="INSERT INTO notify(sname,org_name,time,username,pswrd,email)
			VALUES('".mysql_real_escape_string($sname)."','".mysql_real_escape_string($org_name)."', now(),'".mysql_real_escape_string($username)."','".$stored_password."','".mysql_real_escape_string($email)."');";
               
		$res=mysql_query($p);

		$r = "INSERT INTO flag (user) VALUES ('".mysql_real_escape_string($username)."');";
		mysql_query($r);
}
 function shn_demoM_adminDashboard() {

	global $global;
	$q="select * from created;";
	$res=mysql_query($q);
	echo "<table class=\"emTable\">
			<tr>
				<td style=\"font-size: 120%; padding-left: 8px;\" ><b>Short name</b></td>
				<td style=\"font-size: 120%; padding-left: 8px;text-align: center;\" ><b>Database</b></td>
				<td style=\"font-size: 120%; padding-left: 8px;text-align: center;\" ><b>Time</b></td>
			</tr>";
	while($row = mysql_fetch_array($res, MYSQL_ASSOC))
	{
		echo "<tr><td>" . $row['sname'] . "</td><td>" . $row['Db_name'] . "</td><td>" . $row['ctime'] ."</td></tr>";

	}
	echo "</table>";

	
}

function shn_demoM_pendingNotifications(){
	global $global;
	$q="select * from notify;";
	$res=mysql_query($q);
	echo "<table class=\"emTable\">
			<tr>
				<td style=\"font-size: 120%; padding-left: 8px;\" ><b>Organisation</b></td>
				<td style=\"font-size: 120%; padding-left: 8px;text-align: center;\" ><b>Short Name</b></td>
				<td style=\"font-size: 120%; padding-left: 8px;text-align: center;\" ><b>Requested on</b></td>
			</tr>";
	$users = array();
	$count = 0;
	while($row = mysql_fetch_array($res, MYSQL_ASSOC))
	{
		echo "<tr><td>" . $row['org_name'] . "</td><td>" . $row['sname'] . "</td><td>" . $row['time'] ."</td></tr>";
		$users[$count] = $row;
		$count++;

	}

	$total=$count;
	$i=1;
	$eo=0;
	while($i <= $count) {
		$row           = $users[$i-1];
		$type          = "<b><span style=\"color: blue;\">".$char."</span> &nbsp; </b>";
		$uuid          = $row['uuid'];
		$name          = "<b>".$row['org_name']."</b>";
		$shortname     = $row['sname'];
		
		// find class to color the row...
		if(($eo % 2) == 0) {
			$evenOddClass = "mainRowEven";
		} else {
			$evenOddClass = "mainRowOdd";
		}

		
		echo "<tr>
				<td style=\"padding-left: 8px;\" class=\"".$evenOddClass."\">".$type."<a href=\"\" onclick=\"javascript: shn_form_fopen('createInstance');\">".$name."<a/></td>
				<td style=\"padding-left: 8px; text-align: center;\" class=\"".$evenOddClass."\"><input class=\"styleTehButton\" type=\"button\" onclick=\"javascript: demoM_createInstance(".$uuid.");\" value=\"Create\"></td>
				</tr>
		";
		//shn_form_fopen("create2");
		$i++;
		$eo++;
	}

	echo "</table>";
	

}

function shn_demoM_incTime(){
//$url = makeBaseUrl() ."localhost/".$sname.;
}
function shn_demoM_createInstance($uuid) {

	global $global;

	echo "<div id=\"formcontainer\">";
	shn_form_fopen("create2");
	$extra_opts['req']=true;
	
	shn_form_text("Shortname", 'sname', 'size="30"', $extra_opts);
	shn_form_text("Database name", 'db_name','size="30"', $extra_opts);
	shn_form_text("Db User", 'db_user','size="30"', $extra_opts);
	shn_form_password("Password for Login and db", "pswrd", null, $extra_opts);
	shn_form_text("Hostname", 'hname', 'size="30"', $extra_opts);
	shn_form_text("Timelimit", 'exptime', 'size="30"', $extra_opts);
	shn_form_text("Email", 'email', 'size="30"', $extra_opts);
	

	shn_form_fsclose();
	$extra_opts['req']=true;


 	shn_form_submit(_t("Submit"), "class=\"styleTehButton\"");
	echo "</br><br/><br/>";
	shn_form_fclose();
	echo "</div>";
}

function shn_demoM_create2() {

	shn_demoM_createInstance();
	if ((isset($_POST['sname'])) && (isset($_POST['db_name']))&& (isset($_POST['db_user'])) && (isset($_POST['pswrd'])) && (isset($_POST['hname'])) && (isset($_POST['exptime'])) && (isset($_POST['email'])))
	{	
	   	$ret = shn_demoM_create3($_POST['sname'], $_POST['db_name'], $_POST['pswrd'], $_POST['hname'], $_POST['db_user'], $_POST['exptime']);
	   	 $p ="INSERT INTO created(sname,db_name,pswrd,ctime,exptime,email)
			VALUES('".mysql_real_escape_string($_POST['sname'])."','".mysql_real_escape_string($_POST['db_name'])."','".$_POST['pswrd']."',now(),'".$_POST['exptime']."','".mysql_real_escape_string($_POST['email'])."');";
		$res=mysql_query($p);
		$user=$_SESSION['user'];
		$q="update flag set created = 1 where user = '".$user."';";
		mysql_query($q);
		$msg = "New instance created.";	
	   	
		
		} 
		shn_demoM_sendUserMail($_POST['sname']);
	}



	
function shn_demoM_create3($sname,$db_name,$pswrd,$hname,$db_user,$exptime)
{	
	$output =shell_exec("sh /var/www/project_vesu/vesuvius/vesuvius/mod/demoM/clone1.sh $sname $db_name $pswrd $hname $db_user");
	
}	
		/**	add_confirmation($msg);
			//shn_demoM_adminDashboard();
	$del="delete from notify where sname = '$sname';";
	mysql_query($del);*/
	
function shn_demoM_sendUserMail($sname){
	$q="select * from created where sname= '".$sname."';";
	$res=mysql_query($q);
	$row = mysql_fetch_array($res, MYSQL_ASSOC);
	$email=$row['email'];
	$url = makeBaseUrl() .$shortname;
	
	$subject  = "Your demo instance details...";

				$bodyHTML = "
					Dear ".$username.",<br>
					<br>
					You have requested a new password for our site. If you did not request this, please ignore this email. It will expire and become useless in 24 hours time.<br>
					<br>
					To reset your password, please visit the following page:<br>
					<a href=\"".$url."\">".$url."</a><br>
					<br>
					When you visit that page, your password will be reset, and the new password will be emailed to you.<br>
					<br>
					Your username is: <b>".$username."</b><br>
					<br>
					All the best,<br>
					Admins<br>
				";

				$bodyAlt = "
					Dear ".$username.",\n
					\n
					You have requested a new password for our site. If you did not request this, please ignore this email. It will expire and become useless in 24 hours time.\n
					\n
					To reset your password, please visit the following page:\n
					".$url."\n
					\n
					When you visit that page, your password will be reset, and the new password will be emailed to you.\n
					\n
					Your username is: ".$username."\n
					\n
					All the best,\n
					Admins\n
				";
				$p->sendMessage($email, $email, $subject, $bodyHTML, $bodyAlt);

}
function shn_demoM_testInstance()
{
	global $global;

	echo "<div id=\"formcontainer\">";
	shn_form_fopen("test2", null);
	
	shn_form_text("Shortname",    'sname',    'size="30"', $extra_opts);
	shn_form_password("Password for Login and db", "pswrd", null, $extra_opts);

	shn_form_fsclose();
	shn_form_submit(_t("Submit"), "class=\"styleTehButton\"");
	echo "</br><br/><br/>";
	shn_form_fclose();
	echo "</div>";
}
function shn_demoM_test2()
{
	if(isset($_POST['sname']))
	{
		
		$ret=shn_demoM_test3($_POST['sname']);


	}
}
function shn_demoM_test3($sname)
{
	header('Location: ' . $sname );
}
function shn_demoM_deleteInstance()
{
	shn_form_text("Shortname",    'sname',    'size="30"', $extra_opts);
	//shn_demoM_delete($sname);
}
function shn_demoM_delete2(){
	$ret=shn_demoM_delete3($sname);
}
function shn_demoM_delete3($sname)
{
	$out =shell_exec("sh /home/sneha/unclone.sh $sname $val");
}
function demoM_show_notifications($internal = FALSE) {

	global $conf;
	global $global;
	$htmlLog = "";
	$htmlMain = "";
	$total = 0;
	$char = "<span style=\"text-shadow: 1px 1px 1px #000;\">▶</span>";

	
		echo "<table class=\"emTable\">
			<tr>
				<td style=\"font-size: 120%; padding-left: 8px;\" ><b>Organisation</b></td>
				<td style=\"font-size: 120%; padding-left: 8px;text-align: center;\" ><b>Short Name</b></td>
				<td style=\"font-size: 120%; padding-left: 8px;text-align: center;\" ><b>Requested on</b></td>
			</tr>";

	// get list of hospitals
	$q = "
		SELECT *
		FROM notify;
	";
	$result=mysql_query($q);

	// find out how many hospitals we have and load them into an array
	$notify = array();
	$count = 0;
	while($row=mysql_fetch_assoc($result) ){
		$notify[$count] = $row;
		$count++;
	}
	$total = $count;

	$i = 1; // we start with the 2nd row of teh table (header?) :)
	$eo = 0; // 2nd row is odd ~ of course u knew that computers start counting at 0 :P
	while($i <= $count) {
		$row           = $notify[$i-1];
		$type          = "<b><span style=\"color: blue;\">".$char."</span> &nbsp; </b>";
		$org_name          = "<b>".$row['org_name']."</b>";
		$sname     = $row['sname'];
		//$npi           = $row['npi'];

		// find class to color the row...
		if(($eo % 2) == 0) {
			$evenOddClass = "mainRowEven";
		} else {
			$evenOddClass = "mainRowOdd";
		}


		$i++;
		$eo++;
	}
	// end hospitals
	////////////////////////////////////////////////////////////////////////////////////////////////..................................

	
	$htmlMain .= "</table>";

	

	//---- internally, no ajax, just pass back data
	if ( $internal ) {
		return $htmlMain;
	}
}


?>




