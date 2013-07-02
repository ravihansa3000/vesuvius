<?php
/**
 * @name         Demo Instance
 * @version      1.0	
 * @package      demo
 * @author       Sneha Bhattacharya <g@miernicki.com> <gregory.miernicki@nih.gov>
 * @about        Developed in whole or part by the U.S. National Library of Medicine and the Sahana Foundation
 * @link         https://pl.nlm.nih.gov/about
 * @link         http://sahanafoundation.org
 * @license	 http://www.gnu.org/licenses/lgpl-2.1.html GNU Lesser General Public License (LGPL)
 * @lastModified 2011.1205
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
	include_once($global['approot']."/inc/lib_uuid.inc");
	include_once($global['approot']."/inc/lib_security/lib_auth.inc");
	include_once($global['approot']."/mod/demo/demoauth.inc");
function shn_demo_default() {

	global $global;

	shn_tabmenu_open();
	shn_tabmenu_item("session",  _t("Demo-TableHeader|Session Information"), "demo");
	shn_tabmenu_item("password", _t("Demo-TableHeader|Change My Password"),  "demo");
	shn_tabmenu_close();

	$control = "";

	//if(isset($_GET['forgot'])) {
	//	$control = "shn_pref_forgotPassword";

	//} elseif(isset($_GET['password'])) {
	//	$control = "shn_pref_ch_passwd";

	//} elseif(isset($_GET['passwdp'])) {
	//	$control = "shn_pref_ch_passwd_p";

	//}
	if(isset($_GET['register'])) {
		$control = "shn_demo_register56";

	} 
	if(isset($_GET['login'])) {
		$control = "shn_demo_loginForm";
	}

	if($control == "") {
		$control = "shn_demo_session";
	}

	//$control();
}








function shn_demo_signup($title = "Create your demo user account" , $mod = "demo", $act = "register56", $error= false)
{
	
	if($error) {
		display_errors();
	}
	
	echo "<h2> Sign up for a demo user account</h2>";
	

	$policy = shn_get_password_policy();
	if(count($policy) > 0){
		echo "<br><br><h4>Passwords must adhere to the following rules :</h4>";
		for($i=0; $i < count($policy); $i++) {
			echo ($i+1).". ".$policy[$i]."<br/>";
		}
	}
	echo "<div id=\"formcontainer\">";
	shn_form_fopen("register56");
	shn_form_fsopen("Account Details");
	$extra_opts['req'] = true;
	shn_form_text("First Name ",    'fname',    'size="30"', $extra_opts);
	shn_form_text("Last Name ",     'lname',   'size="30"', $extra_opts);
	shn_form_text("User Name ",     'username',     'size="30"', $extra_opts);
	//shn_form_text("Email Address ", 'email_address', 'size="30"', $extra_opts);
	shn_form_text("Organisation name",	'org_name',	'size="40"', $extra_opts);
	if(isset($_POST['password']) || isset($_POST['re_password'])) {
		unset($_POST['password']);
		unset($_POST['re_password']);
	}
	shn_form_password("Password for Login", "password", null, $extra_opts);
	shn_form_password("Confirm Password", "re_password", null, $extra_opts);
	shn_form_fsclose();
	$extra_opts['req']=true;
	
	
	

 	shn_form_submit(_t("Demo-addUser-Button|Submit"), "class=\"styleTehButton\"");
	echo "</br><br/><br/>";
	shn_form_fclose();
	echo "</div>";
    


	
}

function shn_demo_image_captcha() { shn_auth_gen_captcha(); }
function shn_demo_register56() {

	//if(shn_auth_self_signup_cr() == true){
	//	shn_demo_signup();
		
		//	return;
		//}
		//shn_demo_signup();

	 
	
    //shn_demo_auth_addUser($username, $fname, $lname, $password, $org_name,null );
    $ret = shn_demo_demoauth_addUser($_POST['fname'], $_POST['lname'], $_POST['username'], $_POST['password'],$_POST['org_name'] ,null );
    if($ret) {
			$msg = "A new user account has been created for you and is now active. Please login below using your credentials.";
			echo $msg;
			add_confirmation($msg);
			shn_demo_loginForm();
		}
		else {
			shn_demo_signup();
		}

  /** $error = false;
	if($username == null) {
		return false;
	}
	//if($mem_id == null) {
		$mem_id = shn_create_uuid($type='person');
	//}
	 
	if(!$error) {
		// Create the encrypted password
		$salt1           = _shn_generateSalt();
		$salt2           = _shn_generateSalt();
		$salt            = $salt1.$salt2;
		$password   = substr($password, 0, 4).$salt.substr($password, 4);
		$stored_password = md5(trim($password));
		$time            = time();
		//$confirmation    = md5(uniqid(rand(), true)); // code used to activate account
		$q = "
			INSERT INTO demo(mem_id, password, username, fname, lname, org_name)
			values('".$mem_id."','".$stored_password."','".mysql_real_escape_string($username)."', '".mysql_real_escape_string($fname)."',
				'".mysql_real_escape_string($lname)."', '".mysql_real_escape_string($org_name)."');
		";
		$res = mysql_query($q);

   if($res) {
			$msg = "A new user account has been created for you and is now active. Please login below using your credentials.";
			add_confirmation($msg);
			shn_loginForm('settings');
		}
	}*/

}



function shn_demo_loginForm($return = null) {

	global $conf;
	global $global;

	if ($_SESSION['logged_in'] != true ) {

		if($return != null) {
			$returnVal = $return;
		} else {
			$returnVal = $_SERVER['REQUEST_URI'];
		}

		echo '
			<div class="form-container" style="padding: auto;">
				<form method="post" action="demoauth" id="form0" name="form0">
				<fieldset id="noIdDefined" style="width: 500px;">
				<legend>Login</legend>
				<label for="username">Username </label><input type="text" name="username" id="username" tabindex="1" autocomplete="off"><br>
				<div class="brake"></div>
				<label for="pswd">Password </label><input type="password" name="password" id="pswd" autocomplete="off" tabindex="2"><br>
				<div class="brake"></div>
				<br>
				<div style="text-align: center;">
					<input type="submit" value="login" class="styleTehButton" tabindex="3">
				</div>
				</fieldset>
				<input type="hidden" name="return" value="'.$returnVal.'">
				</form>
			</div>
		';
		echo '
			<div class="form-container" style="padding: auto;">
				<form>
					<fieldset style="width: 500px;">
					<legend>Register / Forgot Password</legend>
					<div style="text-align: center;">
						<input id="followButton" class="styleTehButton" type="button" value="Register as a new user" onclick="signup();">
						&nbsp; &nbsp; &nbsp; &nbsp;
						<input id="followButton" class="styleTehButton" type="button" value="Forgot my password" onclick="forgot();">
					</div>
					<hr class="loginHr">
					<span class="loginText">You can register for a new user account or reset your password in the event it is lost.</span>
					</fieldset>
				</form>
			</div>
			<script>
				function register() {
					window.location = \'signup\';
				}
				function forgot() {
					window.location = \'forgot\';
				}
			</script>
		';
		echo "
			<script>
				window.onload = function() {
					document.getElementById(\"username\").focus();
				};
			</script>
		";

	} else {
		echo "
			<div id=\"loggedIn\">
					<b>Currently logged in as :</b><br>
					".$_SESSION['org_name']." / ".$_SESSION['username']."<br><br>
					<span id=\"logoutLink\" class=\"styleTehButton\"><a href=\"logout\">Logout</a></span>
			</div>
		";
	}
}

function shn_demo_session() {

	global $global;

	echo "<br><div id=\"home\">";
	echo '
		<div id="rezMain" class="mainArea" style="opacity: 1; width: 300px;">
			<table class="emTable">
				<tbody>
				<tr>
					<td style="padding-left: 8px; text-align: left; font-weight: bold;" colspan=2 class="mainRowOdd">Currently Logged in as...</td>
				</tr>
				<tr>
					<td style="padding-left: 8px; text-align: right;" class="mainRowEven">Username</td>
					<td style="padding-left: 8px; text-align: left; font-weight: bold;" class="mainRowEven">'.$_SESSION['username'].'</td>
				</tr>
				<tr>
					<td style="padding-left: 8px; text-align: right;" class="mainRowOdd">Full Name</td>
					<td style="padding-left: 8px; text-align: left; font-weight: bold;" class="mainRowOdd">'.$_SESSION['fname'].'</td>
				</tr>
				<tr>
					<td style="padding-left: 8px; text-align: right;" class="mainRowEven">User Group</td>
					<td style="padding-left: 8px; text-align: left; font-weight: bold;" class="mainRowEven">'.$_SESSION['org_name'].'</td>
				</tr>
				<tr>
					<td style="padding-left: 8px; text-align: right;" class="mainRowOdd">Locale</td>
					<td style="padding-left: 8px; text-align: left; font-weight: bold;" class="mainRowOdd">'.$_SESSION['locale'].'</td>
				</tr>
				</tbody>
			</table>
		</div>
	';
	echo "</div>";
}


	function shn_demo_validate()
{
	$username=$_POST['username'];
    $password=$_POST['password'];
    $sql="SELECT * FROM demo WHERE username='$username' and password='$password'";
    $r = mysql_query($sql);
    $count = mysql_num_rows($r);

    if(count == 1)
    {
    	
			session_register("username");
			session_register("password");

    }
    if(isset($_SESSION['username']))
    {
    	echo " <div id=\"loggedIn\">
    			<b>Currently logged in as: </b><br>
    			".$_SESSION['org_name']." / ".$_SESSION['username']."<br><br>
    			<span id=\"logoutLink\" class=\"styleTehButton\"><a href=\"logout\">Logout</a></span>
			</div>
			";
		shn_demo_session();
    }

}

shn_session_start();

// authenticate user
$user_data = shn_authenticate_user();

switch($user_data["result"]) {

	case LOGGEDIN:
		$global['module']   = isset($conf['default_module']) ? $conf['default_module'] : "home";
		$global['action']   = isset($conf['default_action']) ? $conf['default_action'] : "default";
		$global['previous'] = true;
		shn_session_change($user_data);
		break;

	case LOGGEDOUT:
		shn_session_end($user_data);
		header("Location: ".makeBaseUrlMinusEvent());
		break;

	case LOGINFAILED:
		$global['module']   = "demo";
		$global['action']   = "loginForm";
		$global['previous'] = true;
		break;

	default:
		break;
}
session_update_activity();

if(isset($user_data["doHeader"]) && $user_data["doHeader"] === true) {
	if($user_data["headerLocation"] == "login" || $user_data["headerLocation"] = "auth" || $user_data["headerLocation"] == "index.php?doLogin=1" || $user_data["headerLocation"] == "index.php?doLogin=2"|| $user_data["headerLoction"]=="demoauth") {
		header("Location: settings");
	} else {
		header("Location: ".$_POST['return']);
	}
}





