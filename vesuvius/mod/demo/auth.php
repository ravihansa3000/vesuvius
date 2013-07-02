
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
  
function shn_demo_auth_addUser($username, $fname, $lname, $password, $org_name, $mem_id = null )
{
	
	//global $global;
	//$db = $global['db'];
	//include_once($global['approot']."/inc/lib_uuid.inc");

	$error = false;
	if($username == null) {
		return false;
	}
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
			INSERT INTO demo(mem_id, password, username, fname, lname, org_name)
			values('".$mem_id."','".$stored_password."','".mysql_real_escape_string($username)."', '".mysql_real_escape_string($fname)."',
				'".mysql_real_escape_string($lname)."', '".mysql_real_escape_string($org_name)."');
		";
		$res = mysql_query($q);

		
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
function shn_authenticate_demo_user() {

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
	if(!isset($_GET['doLogin'])) {
		$user_data["user_id"] = -1;
		return $user_data ;





	// handle Google OAuth login
	} elseif($_GET['doLogin'] == "2") {

		require_once($global['approot']."/3rd/google-api-php-client/src/apiClient.php");
		require_once($global['approot']."/3rd/google-api-php-client/src/contrib/apiOauth2Service.php");

		$user_id = null;
		$username = null;

		$client = new apiClient();
		$client->setApplicationName($conf['site_name']);
		$client->setClientId($conf['oauth_google_clientId']);
		$client->setClientSecret($conf['oauth_google_clientSecret']);
		$client->setRedirectUri($conf['oauth_google_redirectUri']);
		$client->setDeveloperKey($conf['oauth_google_developerKey']);
		$client->setAccessType('online');

		$oauth2 = new apiOauth2Service($client);
		$authenticated = false;

		if(isset($_GET['code'])) {

			$client->authenticate();
			$_SESSION['token'] = $client->getAccessToken();

			//$redirect = 'http://' . $_SERVER['HTTP_HOST'] . $_SERVER['PHP_SELF'];
			//header('Location: ' . filter_var($redirect, FILTER_SANITIZE_URL));

			if(isset($_SESSION['token'])) {
				$client->setAccessToken($_SESSION['token']);

				if($client->getAccessToken()) {

					// try { $client->verifyIdToken(); }
					// catch(apiAuthException $e) { echo '<br>Unable to verify id_token. Error: '.$e->getMessage().'<br>'; }

					$user = $oauth2->userinfo->get();

					$oauth_id       = isset($user['id'])             ? filter_var($user['id'],             FILTER_SANITIZE_NUMBER_INT) : null;
					$email          = isset($user['email'])          ? filter_var($user['email'],          FILTER_VALIDATE_EMAIL)      : null;
					$verified_email = isset($user['verified_email']) ? filter_var($user['verified_email'], FILTER_VALIDATE_BOOLEAN)    : null;
					$given_name     = isset($user['given_name'])     ? filter_var($user['given_name'],     FILTER_SANITIZE_STRING)     : null;
					$family_name    = isset($user['family_name'])    ? filter_var($user['family_name'],    FILTER_SANITIZE_STRING)     : null;
					$profile_link   = isset($user['link'])           ? filter_var($user['link'],           FILTER_VALIDATE_URL)        : null;
					$profile_pic    = isset($user['picture'])        ? filter_var($user['picture'],        FILTER_VALIDATE_URL)        : null;
					$gender         = isset($user['gender'])         ? filter_var($user['gender'],         FILTER_SANITIZE_STRING)     : null;
					$locale         = isset($user['locale'])         ? filter_var($user['locale'],         FILTER_SANITIZE_STRING)     : null;

					// The access token may have been updated lazily.
					$_SESSION['token'] = $client->getAccessToken();

					if($_SESSION['token'] != null) {
						$authenticated = true;
					}

				} else {
					$authenticated = false;
				}
			} else {
				$authenticated = false;
			}
		} else {
			$authenticated = false;
		}

		if(!$authenticated || (isset($_GET['error']) && $_GET['error'] == "access_denied")) {
			$user_data["user_id"] = ANONYMOUS_USER;
			$user_data["user"] = "Anonymous";
			$user_data["result"] = LOGINFAILED;
			add_error("Login via Google failed.");
			return $user_data;
		}

		/*
		echo '<pre>_SESSION('.print_r(var_export($_SESSION, true), true).')</pre>';
		echo '<pre>_SERVER('.print_r(var_export($_SERVER, true), true).')</pre>';
		echo '<pre>_COOKIE('.print_r(var_export($_COOKIE, true), true).')</pre>';
		echo '<pre>_REQUEST('.print_r(var_export($_REQUEST, true), true).')</pre>';
		die();
		*/

		// logged in successfully

		// find the local user with the corresponding oauth id (if it already exists)
		$q = "
			SELECT *
			FROM `users`
			WHERE oauth_id = '".mysql_real_escape_string($oauth_id)."';
		";
		$result = $global['db']->Execute($q);
		if($result === false) { daoErrorLog(__FILE__, __LINE__, __METHOD__, __CLASS__, __FUNCTION__, $global['db']->ErrorMsg(), "oauth query 1 ((".$q."))"); }

		// if we already have an oauth user with a matching id on our system, log them in
		if($result != null && !$result->EOF) {
			$row = $result->FetchRow();
			$user_id = $row['user_id'];
			$username = $row['user_name'];

		// the oauth user is not already present in our system
		} else {

			// find the local user with a username that matches the google email address
			$q = "
				SELECT *
				FROM `users`
				WHERE user_name = '".mysql_real_escape_string($email)."';
			";
			$result = $global['db']->Execute($q);
			if($result === false) { daoErrorLog(__FILE__, __LINE__, __METHOD__, __CLASS__, __FUNCTION__, $global['db']->ErrorMsg(), "oauth query 2 ((".$q."))"); }

			// we have a user with a username that matches the google email address
			if($result != null && !$result->EOF) {
				$row = $result->FetchRow();
				$user_id = $row['user_id'];
				$username = $row['user_name'];

			// check if a user in the system is using a contact email address the same as the google email
			} else {

				$q = "
					SELECT *
					FROM contact c, users u
					WHERE c.contact_value = '".mysql_real_escape_string($email)."'
					AND c.p_uuid = u.p_uuid;
				";
				$result = $global['db']->Execute($q);
				if($result === false) { daoErrorLog(__FILE__, __LINE__, __METHOD__, __CLASS__, __FUNCTION__, $global['db']->ErrorMsg(), "oauth query 3 ((".$q."))"); }

				// we have a user that is using this google email address
				if($result != null && !$result->EOF) {
					$row = $result->FetchRow();
					$user_id = $row['user_id'];
					$username = $row['user_name'];
				}
			}
		}

		// if not set, we have a new user, so add the new user...
		if($user_id == null && $username == null) {

			// generate a strong random password for the local account...
			$time = time();
			$p1a = rand(10e16, 10e20);
			$p1b = base_convert($p1a, 10, 36);
			$p1c = strtoupper($p1b);
			$part1 = substr($p1c, 0, 8);
			$p2a = rand(10e16, 10e20);
			$p2b = base_convert($p2a, 10, 36);
			$part2 = substr($p2b, 0, 8);
			$password = $part1.$part2;

			$user_id = shn_auth_add_user($given_name, $family_name, $email, $password, REGISTERED, null, null, $email);
			$username = $email;
		}

		// finally, update the user profile information with the current profile data...
		$q = "
			UPDATE users
			SET
				oauth_id        = '".mysql_real_escape_string($oauth_id)."',
				profile_link    = '".mysql_real_escape_string($profile_link)."',
				profile_picture = '".mysql_real_escape_string($profile_pic)."',
				locale          = '".mysql_real_escape_string($locale)."',
				verified_email  = '".mysql_real_escape_string($verified_email)."'

			WHERE user_id = '".mysql_real_escape_string($user_id)."' ;
		";
		$result = $global['db']->Execute($q);
		if($result === false) { daoErrorLog(__FILE__, __LINE__, __METHOD__, __CLASS__, __FUNCTION__, $global['db']->ErrorMsg(), "oauth query 4 ((".$q."))"); }

		// log the user in...
		$user_data["user_id"]  = $user_id;
		$user_data["user"]     = $username;
		$user_data["result"]   = LOGGEDIN;
		$user_data["doHeader"] = true;
		$user_data["headerLocation"] = "settings";
		return $user_data;

	// handle a normal login
	} elseif($_GET['doLogin'] == "1") {

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
			//$status = $res->fields["status"];
			//$salt = $res->fields["salt"];
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
				shn_auth_activate_user($uid);
			}

			$e = explode("/", $_POST['return']);
			$f = $e[sizeof($e)-1];
			$user_data["user_id"] = $res->fields["p_uuid"];
			$user_data["user"] = $user;
			$user_data["result"] = LOGGEDIN;
			$user_data["doHeader"] = true;
			$user_data["headerLocation"] = $f;
			return $user_data;
		}
	}
}


