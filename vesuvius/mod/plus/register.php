<?
/**
 * @name         PL User Services
 * @version      25
 * @package      plus
 * @author       Greg Miernicki <g@miernicki.com> <gregory.miernicki@nih.gov>
 * @about        Developed in whole or part by the U.S. National Library of Medicine
 * @link         https://pl.nlm.nih.gov/about
 * @license	 http://www.gnu.org/licenses/lgpl-2.1.html GNU Lesser General Public License (LGPL)
 * @lastModified 2012.0529
 */


global $global;
require_once($global['approot'].'/mod/lpf/lib_lpf.inc');
require_once($global['approot'].'/mod/plus/lib_plus.inc');

// figure out what to do and call the functions to do it
$confirm  = isset($_GET['confirm'])  ? $_GET['confirm']  : null;
$username = isset($_GET['username']) ? $_GET['username'] : null;
$reset    = isset($_GET['reset'])    ? $_GET['reset']    : null;

if($username != null && $reset != null) {
	// first validate reset confirmation code
	$q = "
		SELECT *
		FROM users
		WHERE user_name = '".mysql_real_escape_string($username)."'
		AND confirmation = '".mysql_real_escape_string($reset)."'
		AND status = 'active';
	";
	$r = $global['db']->Execute($q);
	if($row = $r->FetchRow()) {
		shn_plus_resetPasswordAndEmail($username);
		gotoReset();
	} else {
		gotoFourOhFour();
	}

} elseif($username != null && $confirm != null) {
	$q = "
		SELECT *
		FROM users
		WHERE user_name = '".mysql_real_escape_string($username)."';
	";
	$r = $global['db']->Execute($q);
	if($row = $r->FetchRow()) {
		if($row['status'] == "active") {
			gotoAlreadyActive();
		} else if($row['status'] == "pending" && $confirm == $row['confirmation']) {
			shn_plus_activateUser($username);
			gotoActivated();
		} else {
			gotoFourOhFour();
		}
	} else {
		gotoFourOhFour();
	}

} else {
	gotoFourOhFour();
}

function gotoAlreadyActive() {header("Location: ".makeBaseUrl()."resources?page=active");    } //4
function gotoActivated()     {header("Location: ".makeBaseUrl()."resources?page=activated"); } //5
function gotoReset()         {header("Location: ".makeBaseUrl()."resources?page=reset");     } //6
function gotoFourOhFour()    {header("Location: ".makeBaseUrl()."resources?page=404");       } //404
