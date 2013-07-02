
<?
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
  
require_once($global['approot']."/inc/lib_security/errors.inc");
require_once($global['approot']."/inc/lib_security/constants.inc");
require_once($global['approot']."/inc/lib_security/lib_acl.inc");
global $global;

	$db = $global['db'];
function shn_demoauth_addUser($username, $fname, $lname, $password, $org_name, $mem_id = null )
{
	
	global $global;
	$db = $global['db'];
	//include_once($global['approot']."/inc/lib_uuid.inc");

	$error = false;
	if($username == null) {
		return false;
	}
	$status = 'active';
	//if($mem_id == null) {
	//	$mem_id = shn_create_uuid($type='person');
	 
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
			INSERT INTO demo(mem_id, password, username, fname, lname, org_name,salt,status)
			values('".$mem_id."','".$stored_password."','".mysql_real_escape_string($username)."', '".mysql_real_escape_string($fname)."',
				'".mysql_real_escape_string($lname)."', '".mysql_real_escape_string($org_name)."', '".$salt."', '".$status."');
		";
		$res = $db->Execute($q);
		if($res == false) {
			add_error($db->ErrorMsg());
		}

		
	}
	if($res) {
			$res = shn_acl_adduser_to_role_uuid($pid, $role);
			shn_acl_log_msg("New user added : ", $pid, $user_name);

			$q = "
				SELECT *
				FROM demo
				WHERE mem_id = '".mysql_real_escape_string($pid)."' ;
			";
			$result = $global['db']->Execute($q);
			if($result === false) { daoErrorLog(__FILE__, __LINE__, __METHOD__, __CLASS__, __FUNCTION__, $global['db']->ErrorMsg(), "add demo find new mem_id ((".$q."))"); }

			if($row = $result->FetchRow()) {
				$res = $row['user_id'];
			}
		}
	function shn_demo_auth_validate()
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

}
/**
 *check the existence of an user
 *@return bool
 *@param string user name
 *@access public
 */
function shn_demo_auth_current_user(){
	global $global;
	$q = "select mem_id from  demo where  username = '{$_SESSION['user']}'";
	$db=$global['db'];
	$res=$db->Execute($q);
	if(($res==null) or ($res->EOF)){
		return null;
	}else {
		return $res->fields["mem_id"];
	}
}
function shn_demo_auth_is_user($username) {

	global $global;

	$db = $global['db'];
	$q = "
		SELECT mem_id
		FROM demo
		WHERE username = '".mysql_real_escape_string($username)."';
	";
	$res = $db->Execute($q);
	if(($res == null) || ($res->EOF)) {
		return false;
	} else {
		return true;
	}
}
function shn_demo_authenticate_user() {

	global $global;
	global $conf;

	$db = $global['db'];
	$user_data = array("user_id" => ANONYMOUS_USER, "user" => "Anonymous");
	$user_data["result"] = null;

	if(isset($_GET['act']) && $_GET['act'] == "logout") {
		$user_data["user_id"] = ANONYMOUS_USER;
		$user_data["user"]    = "Anonymous";
		$user_data["result"]  = LOGGEDOUT;
		return $user_data ;
	}

	// only authenticate if requested
	if(!isset($_GET['yesLogin'])) {
		$user_data["user_id"] = -1;
		return $user_data ;
	}





	// handle Google OAuth login
	 
	// handle a normal login
	 if($_GET['yesLogin'] == "1") {

		//authentication is done only as the user requested to login
		$user = addslashes(strip_tags(trim($_POST{"username"})));
		$pwd = addslashes(strip_tags(trim($_POST{"password"})));
		$q = "
			SELECT mem_id, org_name
			FROM demo
			WHERE username = '$user';
		";
		$res = $db->Execute($q);
		if(($res == null) || ($res->EOF)) {
			add_error("Login Failed : Invalid user name or password.");
			shn_acl_log_msg("Login Failed : Invalid user name or password.", "anonymous", "Anonymous User");
			$user_data["user_id"] = ANONYMOUS_USER;
			$user_data["user"] = "Anonymous";
			$user_data["result"] = LOGINFAILED;
			return $user_data;
		} else {
			$status = $res->fields["status"];
			$salt = $res->fields["salt"];
			$memid = $res->fields["mem_id"];
		}
		/*if($status == 'pending') {
			add_error("Your account is not yet active. Please refer to the registration email you have recieved to activate it.");
			shn_acl_log_msg("Login Failed : Account Pending", "anonymous", "Anonymous User");
			$user_data["user_id"] = ANONYMOUS_USER;
			$user_data["user"] = "Anonymous";
			$user_data["result"] = LOGINFAILED;
			return $user_data;
		}*/
		/*if($status == 'locked') {
			add_error("This account has been locked due to many failed login attempts. Please contact <a href=\"mailto:".$conf['audit_email']."\">".$conf['audit_email']."</a> to have your account unlocked. Remember to provide your username in the email to expedite the process.");
			shn_acl_log_msg("Login Failed : Password lock still valid.",$uid,$user);
			$user_data["user_id"] = ANONYMOUS_USER;
			$user_data["user"] = "Anonymous";
			$user_data["result"] = LOGINFAILED;
			return $user_data;
		}*/
		// banned user
		/*if($status == 'banned') {
			add_error("Login Failed : You have been banned by an administrator of the system.");
			shn_acl_log_msg("Login Failed : Banned user login atttempt.",$uid,$user,1);
			$user_data["user_id"] = ANONYMOUS_USER;
			$user_data["user"] = "Anonymous";
			$user_data["result"] = LOGINFAILED;
			return $user_data;
		}*/

		//$pwd = substr($pwd, 0, 4).$salt.substr($pwd, 4);
		//$user_data["result"] = LOGGEDOUT;

		// Create a digest of the password collected from the challenge
		//$password_digest = md5(trim($pwd));

		// Formulate the SQL to find the user
		$q = "
			SELECT mem_id  FROM demo
			WHERE username = '$user'
		";

		$res = $db->Execute($q);
		if(($res == null) || ($res->EOF)) {

			// no result ,so return 1 ,which is  not a valid user_id , the calling application can identify authentication was attempted,but failed
			shn_acl_log_msg("Login Failed : Invalid Password.", $uid, $user, 1);
			$user_data["mem_id"] = ANONYMOUS_USER;
			$user_data["user"] = "Anonymous";
			$user_data["result"] = LOGINFAILED;
			_shn_auth_lock_user($uid, $status);
			return $user_data;

		} else {
			if($status == 'try1' || $status == 'try2' || $status == 'try3' || $status == 'active') {
				shn_demo_auth_activate_user($uid);
			}

			$e = explode("/", $_POST['return']);
			$f = $e[sizeof($e)-1];
			$user_data["user_id"] = $res->fields["mem_id"];
			$user_data["user"] = $user;
			$user_data["result"] = LOGGEDIN;
			$user_data["doHeader"] = true;
			$user_data["headerLocation"] = $f;
			return $user_data;
		}
	}

function shn_demo_auth_activate_user($pid=null,$uname=null) {

	global $global;

	$db=$global['db'];

	if($pid==null){
		if($uname==null) {
			return false;
		} else {
			$sql = "
				update demo
				set status='active'
				where username='{$uname}';
			";
		}
	} else {
		$sql = "
			update demo
			set status='active'
			where mem_id='{$pid}';
		";
	}
	$res = $db->Execute($sql);
	return $res;
}



