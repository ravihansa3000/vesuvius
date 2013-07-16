<?php
/**
 * @name         Edit A Person
 * @version      14
 * @package      eap
 * @author       Greg Miernicki <g@miernicki.com> <gregory.miernicki@nih.gov>
 * @about        Developed in whole or part by the U.S. National Library of Medicine and the Sahana Foundation
 * @link         https://pl.nlm.nih.gov/about
 * @link         http://sahanafoundation.org
 * @license	 http://www.gnu.org/licenses/lgpl-2.1.html GNU Lesser General Public License (LGPL)
 * @lastModified 2012.1022
 */

global $global;
global $revisionCount;

require_once($global['approot']."/inc/lib_uuid.inc");
require_once($global['approot']."/mod/lpf/lib_lpf.inc");

$revisionCount = 0;
$global['uuid'] = "";
$global['makeStaticPfifNote'] = false;


// Entry point for mod_rewrite urls
function shn_eap_default() {

	if(isset($_GET['addNote'])) {
		shn_eap_addNote();
	} elseif(isset($_GET['follow'])) {
		shn_eap_follow();
	} elseif(isset($_GET['unfollow'])) {
		shn_eap_unfollow();
	} else {
		shn_eap_edit();
	}
}


// Begin following a person.....
function shn_eap_follow() {
	global $global;
	shn_eap_findRights();

	$q0  = "
		SELECT COUNT(*)
		FROM person_followers
		WHERE follower_p_uuid = '".$_SESSION['user_p_uuid']."'
		AND p_uuid = '".$global['uuid']."';
	";
	$r0  = $global['db']->Execute($q0);
	if($r0 === false) { daoErrorLog(__FILE__, __LINE__, __METHOD__, __CLASS__, __FUNCTION__, $global['db']->ErrorMsg(), "edit follow 1 ((".$q."))"); }
	$row0 = $r0->FetchRow();
	$alreadyFollowing = (bool)$row0['COUNT(*)']; // true if logged in user is already following the person

	if($global['startFollow']) {
		add_confirmation("You successfully began following this person.");
		if(!$alreadyFollowing) {
			$q = "
				INSERT INTO person_followers (p_uuid, follower_p_uuid)
				VALUES ('".$global['uuid']."','".$_SESSION['user_p_uuid']."');
			";
			$r = $global['db']->Execute($q);
			if($r === false) { daoErrorLog(__FILE__, __LINE__, __METHOD__, __CLASS__, __FUNCTION__, $global['db']->ErrorMsg(), "edit follow 2 ((".$q."))"); }
		}
	} else {
		add_error("You are not allowed to follow this person... you must first login.");
	}
	shn_eap_edit();
}



/**
 * Stop following a person.....
 *
 * @access public
 * @return void
 */
function shn_eap_unfollow() {
	global $global;
	shn_eap_findRights();

	if($global['startFollow']) {
		add_confirmation("You are no longer following this person.");
		$q = "
			DELETE FROM person_followers
			WHERE `p_uuid` = '".$global['uuid']."'
			AND `follower_p_uuid` = '".$_SESSION['user_p_uuid']."';
		";
		$r = $global['db']->Execute($q);
		if($r === false) { daoErrorLog(__FILE__, __LINE__, __METHOD__, __CLASS__, __FUNCTION__, $global['db']->ErrorMsg(), "edit unfollow 1 ((".$q."))"); }

	} else {
		add_error("You are not allowed to unfollow this person.");
	}
	shn_eap_edit();
}




/**
 * Adds a note to a person.....
 *
 * @access public
 * @return void
 */
function shn_eap_addNote() {
	global $global;
	shn_eap_findRights();

	if($global['addNote']) {
		$note = trim($_POST['addNote']);
		if(strlen($note)>1000) {
			$note = substr($note, 0, 999);
		}
		$note = trim(strip_tags($note));
		$note = mysql_real_escape_string($note);

		if(strlen($note) > 0) {
			$q  = "
				INSERT INTO person_notes (note_about_p_uuid, note_written_by_p_uuid, note)
				VALUES ('".$global['uuid']."', '".$_SESSION['user_p_uuid']."', '".$note."');
			";
			$r  = $global['db']->Execute($q);
			if($r === false) { daoErrorLog(__FILE__, __LINE__, __METHOD__, __CLASS__, __FUNCTION__, $global['db']->ErrorMsg(), "edit add note 1 ((".$q."))"); }

			// update record's timestamp
			$q = "
				UPDATE person_status
				SET
					last_updated = CURRENT_TIMESTAMP,
					last_updated_db = '".date('Y-m-d H:i:s')."'
				WHERE p_uuid = '".$global['uuid']."';
			";
			$r = $global['db']->Execute($q);
			if($r === false) { daoErrorLog(__FILE__, __LINE__, __METHOD__, __CLASS__, __FUNCTION__, $global['db']->ErrorMsg(), "edit add note 2 ((".$q."))"); }

			$row = $r->FetchRow();
			add_confirmation("Your note has been added.");
		} else {
			add_warning("Your note was not added because it was blank.");
		}
	} else {
		add_warning("You cannot add notes without being logged in, please login first.");
	}
	shn_eap_edit();
}




/**
 * Deletes a note from a person.....
 *
 * @access public
 * @return void
 */
function shn_eap_deleteNote() {
	global $global;
	shn_eap_findRights();

	if($global['privateEdit']) {
		$id = mysql_real_escape_string($_GET['noteId']);
		$q  = "
			DELETE FROM person_notes
			WHERE note_id = '".$id."'
			AND note_about_p_uuid = '".$global['uuid']."';
		";
		$r  = $global['db']->Execute($q);
		if($r === false) { daoErrorLog(__FILE__, __LINE__, __METHOD__, __CLASS__, __FUNCTION__, $global['db']->ErrorMsg(), "edite delete note 1 ((".$q."))"); }
		$row = $r->FetchRow();
		add_confirmation("Note <b>#".$id."</b> has been deleted.");
	} else {
		add_warning("You do not have permission to delete notes.");
	}
	shn_eap_edit();
}



/**
 * This function defines the menu list.
 * @access public
 * @return void
 */
function shn_eap_mainmenu() {
	global $global;
	$module = $global['module'];
	include $global['approot']."/inc/handler_mainmenu.inc";
	// no menu... this module is only one page :)
}



/**
 * findRights
 *
 * @access public
 * @return void
 */
function shn_eap_findRights() {

	global $global;
	global $conf;

	// if we came in via a mod_rewrite rule for a uuid formed from a url, use its $val here...
	if(isset($_GET['val'])) {
		$global['uuid'] = mysql_real_escape_string($conf['base_uuid'].$_GET['val']);

	} elseif(isset($_GET['puuid'])) {
		$global['uuid'] = mysql_real_escape_string(urldecode($_GET['puuid']));

	} else {
		$global['uuid'] = 0;
	}

	// first and foremost, check if the user has permissions to view the record
	$q = "
		SELECT *
		FROM person_uuid p, incident i
		WHERE p.incident_id = i.incident_id
		AND p.p_uuid = '".$global['uuid']."';
	";
	$r  = $global['db']->Execute($q);
	if($r === false) { daoErrorLog(__FILE__, __LINE__, __METHOD__, __CLASS__, __FUNCTION__, $global['db']->ErrorMsg(), "edit find rights 1 ((".$q."))"); }
	$row = $r->FetchRow();
	if((!isset($_SESSION['group_id']) || ($_SESSION['group_id'] != (int)$row['private_group'])) && $row['private_group'] != NULL) {
		$global['permissionToView'] = false;
	} else {
		$global['permissionToView'] = true;
	}


	// anonymous user ~ not logged in
	if(($_SESSION['logged_in'] != 1) || ($_SESSION['user'] == 'Anonymous')) {
		$global['publicView']  = true;
		$global['publicEdit']  = false;
		$global['privateView'] = false;
		$global['privateEdit'] = false;
		$global['addNote']     = false;
		$global['startFollow'] = false;
		$global['role']        = "anonymous";


	// logged in
	} else {
		$q  = "
			SELECT COUNT(*)
			FROM users u
			LEFT JOIN person_to_report r ON u.p_uuid = r.rep_uuid
			WHERE u.user_id = '".$_SESSION['user_id']."'
			AND r.p_uuid = '".$global['uuid']."';
		";
		$r  = $global['db']->Execute($q);
		if($r === false) { daoErrorLog(__FILE__, __LINE__, __METHOD__, __CLASS__, __FUNCTION__, $global['db']->ErrorMsg(), "edit find rights 2 ((".$q."))"); }
		$row = $r->FetchRow();
		$global['isReporter'] = (bool)$row['COUNT(*)'];

		// reporter ~ logged in
		if($global['isReporter']) {
			$global['publicView']  = true;
			$global['publicEdit']  = true;
			$global['privateView'] = true;
			$global['privateEdit'] = true;
			$global['addNote']     = true;
			$global['startFollow'] = false;
			$global['role']        = "reporter";

		// registered user ~ logged in
		} else if(($_SESSION['group_name'] == "Registered User") && ($_SESSION['logged_in'] == 1)) {
			$global['publicView']  = true;
			$global['publicEdit']  = false;
			$global['privateView'] = false;
			$global['privateEdit'] = false;
			$global['addNote']     = true;
			$global['startFollow'] = true;
			$global['role']        = "registered";

		// Hospital Staff / Hospital Staff Admin ~ logged in
		} else if(($_SESSION['group_id'] == "5" || $_SESSION['group_id'] == "6") && ($_SESSION['logged_in'] == 1)) {
			$global['publicView']  = true;
			$global['publicEdit']  = true;
			$global['privateView'] = true;
			$global['privateEdit'] = true;
			$global['addNote']     = true;
			$global['startFollow'] = true;
			$global['role']        = "staff";
		}
	}
	// Admin ~ logged in ~ has right to everything
	if(isset($_SESSION['group_id']) && ($_SESSION['group_id'] == "1") && ($_SESSION['logged_in'] == 1)) {
		$global['permissionToView'] = true;
		$global['publicView']  = true;
		$global['publicEdit']  = true;
		$global['privateView'] = true;
		$global['privateEdit'] = true;
		$global['addNote']     = true;
		$global['startFollow'] = true;
		$global['role']        = "admin";
	}
}



/**
 * emailUpdates
 *
 * @access public
 * @return void
 */
function emailUpdates() {
	global $global;
	global $revisionCount;

	// only email if revisions...
	if($revisionCount > 0) {

		$p = new pop();
		$subject  = "NLM People Locator update for someone you reported or are following";
		$bodyHTML = "
			<br>
			Changes have been made to the details of a person you have reported or are following.<br>
		";
		$bodyAlt = "
			\n
			Changes have been made to the details of a person you have reported or are following.\n
		";

		$bodyHTML .= "
			<br>
			<br>
			You can view the page for this person here: <a href=\"".makePersonUrl($global['uuid'])."\">".makePersonUrl($global['uuid'])."</a><br>
			<br>
			<br>
			<i>If you would like to unsubscribe from these email alerts, click the button to un-follow this person from the link provided above.</i><br>
			<br>
			<img src=\"http://goo.gl/BqfCv\">
		";
		$bodyAlt .= "
			\n
			\n
			You can view the page for this person here: ".makePersonUrl($global['uuid'])."\n
			\n
			\n
			If you would like to unsubscribe from these email alerts, click the button to un-follow this person from the link provided above.\n
			\n
			\n
			\n
			- People Locator
		";

		// email followers......
		$q = "
			SELECT *
			FROM person_followers f, person_uuid p, contact c
			WHERE f.p_uuid = '".$global['uuid']."'
			AND p.p_uuid = f.follower_p_uuid
			AND c.p_uuid = f.follower_p_uuid
			AND c.opt_contact_type = 'email';
		";
		$r = $global['db']->Execute($q);
		if($r === false) { daoErrorLog(__FILE__, __LINE__, __METHOD__, __CLASS__, __FUNCTION__, $global['db']->ErrorMsg(), "edit email updates 1 ((".$q."))"); }

		while($row = $r->FetchRow()) {
			$p->sendMessage($row['contact_value'], $row['full_name'], $subject, $bodyHTML, $bodyAlt);
		}

		// email reporter........
		$q = "
			SELECT *
			FROM person_to_report p, contact c, person_uuid pp
			WHERE p.p_uuid = '".$global['uuid']."'
			AND p.rep_uuid = c.p_uuid
			AND c.opt_contact_type = 'email'
			AND pp.p_uuid = p.rep_uuid;
		";
		$r = $global['db']->Execute($q);
		if($r === false) { daoErrorLog(__FILE__, __LINE__, __METHOD__, __CLASS__, __FUNCTION__, $global['db']->ErrorMsg(), "edit email updates 2 ((".$q."))"); }

		while($row = $r->FetchRow()) {
			$p->sendMessage($row['contact_value'], $row['full_name'], $subject, $bodyHTML, $bodyAlt);
		}
	}
}



/**
 * saveRevision
 *
 * @access public
 * @return void
 */
function saveRevision($table, $column, $name, $oldValue, $newValue) {
	global $global;
	global $revisionCount;

	if($revisionCount == 0) {
		add_confirmation("Changes saved.");
	}

	$revisionCount++;

	// turn empty strings into NULL
	if(trim($oldValue) == "") {
		$old = "NULL";
	} else {
		$old = "'".(string)mysql_real_escape_string($oldValue)."'";
	}
	if(trim($newValue) == "") {
		$new = "NULL";
	} else {
		$new = "'".(string)mysql_real_escape_string($newValue)."'";
	}

	// we stopped using on duplicate b/c it causes problems with triggers!
	/*
	// save the revision
	$q = "
		INSERT INTO ".$table." (p_uuid, ".$column.")
		VALUES ('".$global['uuid']."', ".$new.")
		ON DUPLICATE KEY UPDATE ".$column." = ".$new." ;
	";
	$r = $global['db']->Execute($q);
	if($r === false) { daoErrorLog(__FILE__, __LINE__, __METHOD__, __CLASS__, __FUNCTION__, $global['db']->ErrorMsg(), "edit save revision 1 ((".$q."))"); }
	*/
	
	$q = "
		SELECT COUNT(*)
		FROM ".$table."
		WHERE p_uuid = '".$global['uuid']."';
	";
	$r = $global['db']->Execute($q);
	if($r === false) { daoErrorLog(__FILE__, __LINE__, __METHOD__, __CLASS__, __FUNCTION__, $global['db']->ErrorMsg(), "edit revise 0a ((".$q."))");	}
	$row = $r->FetchRow();
	
	// if row exists, update
	if((int)$row['COUNT(*)'] > 0) {
		$q2 = "
			UPDATE ".$table."
			SET ".$column." = ".$new."
			WHERE p_uuid = '".$global['uuid']."';
		";
		$r2 = $global['db']->Execute($q2);
		if($r2 === false) { aoErrorLog(__FILE__, __LINE__, __METHOD__, __CLASS__, __FUNCTION__, $global['db']->ErrorMsg(), "edit save revision 0b ((".$q2."))"); }
		
	// no row exists, so insert instead
	} else {
		$q2 = "
			INSERT INTO ".$table." (p_uuid, ".$column.")
			VALUES ('".$global['uuid']."', ".$new.");
		";
		$r2 = $global['db']->Execute($q2);
		if($r2 === false) { daoErrorLog(__FILE__, __LINE__, __METHOD__, __CLASS__, __FUNCTION__, $global['db']->ErrorMsg(), "edit save revision 0c ((".$q2."))"); }	
	}

	// note the revision
	$q = "
		INSERT into person_updates (`p_uuid`, `updated_table`, `updated_column`, `old_value`, `new_value`, `updated_by_p_uuid`)
		VALUES ('".$global['uuid']."', '".$table."', '".$column."', ".$old.", ".$new.", '".$_SESSION['user_p_uuid']."');
	";
	$r = $global['db']->Execute($q);
	if($r === false) { daoErrorLog(__FILE__, __LINE__, __METHOD__, __CLASS__, __FUNCTION__, $global['db']->ErrorMsg(), "edit save revision 2 ((".$q."))"); }

	// update timestamp
	$q = "
		UPDATE person_status
		SET
			last_updated = CURRENT_TIMESTAMP,
			last_updated_db = '".date('Y-m-d H:i:s')."'
		WHERE p_uuid = '".$global['uuid']."';
	";
	$r = $global['db']->Execute($q);
	if($r === false) { daoErrorLog(__FILE__, __LINE__, __METHOD__, __CLASS__, __FUNCTION__, $global['db']->ErrorMsg(), "edit save revision 3 ((".$q."))"); }
}



//  at a first save, make the record expire after 1 year... temp hack!
function updateExpiryDate() {
	global $global;
	$e = (date("Y")+1) ."-".date("m-d G:i:s");
	$q = "
		UPDATE person_uuid
		SET expiry_date = '".$e."'
		WHERE p_uuid = '".$global['uuid']."';
	";
	$r = $global['db']->Execute($q);
	if($r === false) { daoErrorLog(__FILE__, __LINE__, __METHOD__, __CLASS__, __FUNCTION__, $global['db']->ErrorMsg(), "edit update expiry date 1 ((".$q."))"); }
	return $e;
}



/**
 * saveContactRevision
 *
 * @access public
 * @return void
 */
function saveContactRevision($type, $name, $oldValue, $newValue) {
	global $global;
	global $revisionCount;

	if($revisionCount == 0) {
		add_confirmation("Changes saved.");
	}

	$revisionCount++;

	// turn empty strings into NULL
	if(trim($oldValue) == "") {
		$old = "NULL";
	} else {
		$old = "'".mysql_real_escape_string($oldValue)."'";
	}
	if(trim($newValue) == "") {
		$new = "NULL";
	} else {
		$new = "'".mysql_real_escape_string($newValue)."'";
	}

	// we delete and then insert here because sometimes the contact value is not yet set, so an update would fail~!

	// delete the old value
	$q = "
		DELETE FROM contact
		WHERE p_uuid = '".$global['uuid']."'
		AND opt_contact_type = '".$type."';
	";
	$r = $global['db']->Execute($q);
	if($r === false) { daoErrorLog(__FILE__, __LINE__, __METHOD__, __CLASS__, __FUNCTION__, $global['db']->ErrorMsg(), "edit save contact revision 1 ((".$q."))"); }


	// insert the new value
	$q = "
		INSERT INTO contact (`p_uuid`, `opt_contact_type`, `contact_value`)
		VALUE ('".$global['uuid']."', '".$type."', '".mysql_real_escape_string($newValue)."');
	";
	$r = $global['db']->Execute($q);
	if($r === false) { daoErrorLog(__FILE__, __LINE__, __METHOD__, __CLASS__, __FUNCTION__, $global['db']->ErrorMsg(), "edit save contact revision 2 ((".$q."))"); }

	// note the revision
	$q = "
		INSERT INTO person_updates (`p_uuid`, `updated_table`, `updated_column`, `old_value`, `new_value`, `updated_by_p_uuid`)
		VALUES ('".$global['uuid']."', 'contact', 'contact_value', '".$type."<::>".$oldValue."', '".$type."<::>".$newValue."', '".$_SESSION['user_p_uuid']."');
	";
	$r = $global['db']->Execute($q);
	if($r === false) { daoErrorLog(__FILE__, __LINE__, __METHOD__, __CLASS__, __FUNCTION__, $global['db']->ErrorMsg(), "edit save contact revision 3 ((".$q."))"); }

	// update timestamp
	$q = "
		UPDATE person_status
		SET
			last_updated = CURRENT_TIMESTAMP,
			last_updated_db = '".date('Y-m-d H:i:s')."'
		WHERE p_uuid = '".$global['uuid']."';
	";
	$r = $global['db']->Execute($q);
	if($r === false) { daoErrorLog(__FILE__, __LINE__, __METHOD__, __CLASS__, __FUNCTION__, $global['db']->ErrorMsg(), "edit save contact revision 4 ((".$q."))"); }
}



/**
 * updateFullName ~ full_name is derived, so we update it when given or family names are updated
 *
 * @access public
 * @return void
 */
function updateFullName($editGivenName, $editFamilyName) {
	global $global;

	$q = "
		UPDATE person_uuid
		SET full_name = '".mysql_real_escape_string($editGivenName)." ".mysql_real_escape_string($editFamilyName)."'
		WHERE p_uuid = '".$global['uuid']."';
	";
	$r = $global['db']->Execute($q);
	if($r === false) { daoErrorLog(__FILE__, __LINE__, __METHOD__, __CLASS__, __FUNCTION__, $global['db']->ErrorMsg(), "edit update full name 1 ((".$q."))"); }
}



/**
 * Main Controller function
 *
 * @access public
 * @return void
 */
function shn_eap_edit() {

	global $global;
	global $conf;
	global $shn_tabindex;

	shn_eap_findRights();

	echo "
	<script type=\"text/javascript\" src=\"res/js/jquery.js\"></script>
	<script type=\"text/javascript\" src=\"res/js/jquery.qrcode.min.js\"></script>
	<script type=\"text/javascript\" src=\"res/js/jquery.lightbox-0.5.js\"></script>
	<link rel=\"stylesheet\" type=\"text/css\" href=\"res/jquery.lightbox-0.5.css\" media=\"screen\" />
	";

	// if the person record is assigned to a private event and the user does not have permission to view it...
	if(!$global['permissionToView']) {
		echo "
			<h2>Error</h2>
			You do not have permission to view this record or the record does not exist.
		";
		return;
	}

	// check if the person record exists or has expired...
	$q  = "
		SELECT COUNT(*), expiry_date
		FROM person_uuid
		WHERE p_uuid = '".$global['uuid']."'
		AND (expiry_date > NOW()
		OR expiry_date is null
		OR expiry_date = '2000-01-01 01:01:01');
	";
	$r   = $global['db']->Execute($q);
	if($r === false) { daoErrorLog(__FILE__, __LINE__, __METHOD__, __CLASS__, __FUNCTION__, $global['db']->ErrorMsg(), "edit edit 1 ((".$q."))"); }
	$row = $r->FetchRow();
	$num = $row['COUNT(*)'];
	$isRap2 = $row['expiry_date'];

	// we has images
	if($num != 1) {
		echo "
			<h2>Error</h2>
			This person's record does not exist or it has been deleted.
		";
		return;
	}

	// make goo.gl short url
	$googl = new goo_gl(makePageUrl());
	$global['googl'] = $googl->result();

	$debug = false;
	$debugParam = "";
	if(isset($_GET['debug'])) {
		$debug = true;
		$debugParam = "&debug";
	}

	// HEADER ////////////////////////////////////////////////////////////////////////////////////
	$q  = "
		SELECT *
		FROM person_uuid p
		WHERE p_uuid = '".$global['uuid']."';
	";
	$r                   = $global['db']->Execute($q);
	if($r === false) { daoErrorLog(__FILE__, __LINE__, __METHOD__, __CLASS__, __FUNCTION__, $global['db']->ErrorMsg(), "edit edit 2 ((".$q."))"); }
	$row                 = $r->FetchRow();
	$full_name           = $row['full_name'];
	$person_incident     = array();
	$person_incident['incident_id'] = $row['incident_id'];
	$hospital            = $row['hospital_uuid'];
	$global['full_name'] = $full_name;
	$person_uuid         = $row;

	// if hospital not assigned yet...
	if($hospital == NULL) {
		$person_to_hospital = array();
		$person_to_hospital['hospital_uuid'] = null;
		$person_to_hospital['name']          = null;
	} else {
		$q = "
			SELECT *
			FROM hospital
			WHERE hospital_uuid = '".$hospital."';
		";
		$r = $global['db']->Execute($q);
		if($r === false) { daoErrorLog(__FILE__, __LINE__, __METHOD__, __CLASS__, __FUNCTION__, $global['db']->ErrorMsg(), "edit edit 3 ((".$q."))"); }
		$row = $r->FetchRow();
		$person_to_hospital = array();
		$person_to_hospital['hospital_uuid'] = $hospital;
		$person_to_hospital['name']          = $row['name'];
		$person_to_hospital['icon_url']      = $row['icon_url'];
	}


	$q  = "
		SELECT *
		FROM person_details
		WHERE p_uuid = '".$global['uuid']."';
	";
	$r = $global['db']->Execute($q);
	if($r === false) { daoErrorLog(__FILE__, __LINE__, __METHOD__, __CLASS__, __FUNCTION__, $global['db']->ErrorMsg(), "edit edit 4 ((".$q."))"); }
	$person_details = $r->FetchRow();

	// show the full name
	echo "
		<div class=\"form-container\">
			<form>
				<fieldset id=\"editHeader\">
					<legend>Person Record</legend>
					<div id=\"moveShitHere\"> </div>
	";



	// figure out the name (username) of the reporter
	$q = "
		SELECT *
		FROM person_to_report r, person_uuid p, users u
		WHERE r.p_uuid = '".$global['uuid']."'
		AND p.p_uuid = r.rep_uuid
		AND p.p_uuid = u.p_uuid;
	";
	$result = $global['db']->Execute($q);
	if($result === false) { daoErrorLog(__FILE__, __LINE__, __METHOD__, __CLASS__, __FUNCTION__, $global['db']->ErrorMsg(), "edit edit 5 ((".$q."))"); }
	if($result != NULL && !$result->EOF) {
		$row = $result->FetchRow();
		$reporterName = $row['full_name']." (".$row['user_name'].")";
		if($row['user_name'] == "mpres") {
			$reporterName = $row['full_name'];
		}
		echo "
					<label for=\"opt_status\"><b>Reported by (username)</b> </label>
					<input type=\"text\" value=\"".$reporterName."\" size=\"64\" class=\"nonedit2 public\" READONLY>
					<br>
		";
	}


	// show the reporting method if available
	// check mpres
	$appName = "";
	$q = "
		SELECT *
		FROM mpres_log
		WHERE p_uuid = '".$global['uuid']."';
	";
	$result = $global['db']->Execute($q);
	if($r === false) { daoErrorLog(__FILE__, __LINE__, __METHOD__, __CLASS__, __FUNCTION__, $global['db']->ErrorMsg(), "edit edit 6 ((".$q."))"); }
	if($result != NULL && !$result->EOF) {
		$row = $result->FetchRow();
		$appName = $row['xml_format'];

	// check plus
	} else {
		$q = "
			SELECT *
			FROM plus_report_log
			WHERE p_uuid = '".$global['uuid']."';
		";
		$result = $global['db']->Execute($q);
		if($result === false) { daoErrorLog(__FILE__, __LINE__, __METHOD__, __CLASS__, __FUNCTION__, $global['db']->ErrorMsg(), "edit edit 7 ((".$q."))"); }
		if($result != NULL && !$result->EOF) {
			$row = $result->FetchRow();
			$appName = $row['enum'];

		// check rap
		} else {
			$q = "
				SELECT *
				FROM rap_log
				WHERE p_uuid = '".$global['uuid']."';
			";
			$result = $global['db']->Execute($q);
			if($result === false) { daoErrorLog(__FILE__, __LINE__, __METHOD__, __CLASS__, __FUNCTION__, $global['db']->ErrorMsg(), "edit edit 8 ((".$q."))"); }
			if($result != NULL && !$result->EOF) {
				$row = $result->FetchRow();
				$appName = "RAP";

			// check pfif
			} else {
				$q = "
					SELECT *
					FROM pfif_person
					WHERE p_uuid = '".$global['uuid']."';
				";
				$result = $global['db']->Execute($q);
				if($result === false) { daoErrorLog(__FILE__, __LINE__, __METHOD__, __CLASS__, __FUNCTION__, $global['db']->ErrorMsg(), "edit edit 9 ((".$q."))"); }
				if($result != NULL && !$result->EOF) {
					$row = $result->FetchRow();
					$appName = "PF";
				}
			}
		}
	}

	if($appName == "TRIAGEPIC0") {
		$appTitle = "TriagePic (Format 0)";
	} elseif($appName == "TRIAGEPIC1") {
		$appTitle = "TriagePic (Format 1)";
	} elseif($appName == "TRIAGEPIC2") {
		$appTitle = "TriagePic (Format 2)";

	} elseif($appName == "REUNITE2") {
		$appTitle = "Reunite (Format 2)";
	} elseif($appName == "REUNITE3") {
		$appTitle = "Reunite (Format 3)";
	} elseif($appName == "REUNITE4") {
		$appTitle = "Reunite (Format 4)";

	} elseif($appName == "RAP") {
		$appTitle = "People Locator (Report A Person)";

	} elseif($appName == "PF") {
		$appTitle = "Google Person Finder";

	} elseif($appName == "MPRES") {
		$appTitle = "Unstructured Email";

	} else {
		$appTitle = "";
	}

	if($appTitle != "") {
		echo "
					<label for=\"opt_status\"><b>Reported via</b> </label>
					<input type=\"text\" value=\"".$appTitle."\" size=\"64\" class=\"nonedit2 public\" READONLY>
					<br>
		";
	}


	// show the mass casualty ID if we have it
	$q  = "
		SELECT count(*)
		FROM edxl_co_lpf
		WHERE p_uuid = '".$global['uuid']."';
	";
	$r   = $global['db']->Execute($q);
	if($r === false) { daoErrorLog(__FILE__, __LINE__, __METHOD__, __CLASS__, __FUNCTION__, $global['db']->ErrorMsg(), "edit edit 10 ((".$q."))"); }
	$row = $r->FetchRow();
	$num = $row['count(*)'];

	// we have a mass casualty id
	if($num == 1) {
		$q  = "
			SELECT *
			FROM edxl_co_lpf
			WHERE p_uuid = '".$global['uuid']."';
		";
		$r = $global['db']->Execute($q);
		if($r === false) { daoErrorLog(__FILE__, __LINE__, __METHOD__, __CLASS__, __FUNCTION__, $global['db']->ErrorMsg(), "edit edit 11 ((".$q."))"); }
		$row = $r->FetchRow();
		$did = $row['person_id'];

		$tct = $row['triage_category'];
		$tcc = $tct;
		if($tct == "BH Green") {
			$tcc = "palegreen";
		} elseif($tct == "Gray") {
			$tcc = "darkgrey";
		} else if($tct == "Green") {
			$tcc = "lime";
		}

		echo "
					<label><b>Mass Casualty ID</b> </label>
					<input type=\"text\" value=\"".$did."\" size=\"64\" class=\"nonedit2 public\" READONLY>
					<br>
		";

		echo "
					<label><b>Initial Triage Category</b> </label>
					<input type=\"text\" value=\"\" class=\"nonedit public tct\" style=\"border: 1px solid #777; border-radius: 20px; -moz-border-radius: 20px; width: 2px; margin-left: 10px; background-color: ".$tcc.";\" READONLY>
					<input type=\"text\" value=\"".$tct."\" size=\"8\" class=\"nonedit2 public\" style=\"padding-left: 0px; margin-left: 0px;\" READONLY>
					<br>
		";
	}


	global $conf;
	// if the origin of the record is on this server, show a combined origin id / origin url
	if(preg_match("+".$conf['base_uuid']."+", $global['uuid']) > 0) {
		echo "
					<label><b>Origin ID</b> </label>
					<input style=\"float: left;\" type=\"text\" value=\"".$global['uuid']."\" size=\"96\" class=\"nonedit2 public\" READONLY>
					<br>

					<label><b>Origin URL</b> </label>
					<div class=\"originUrl\"><a href=\"".$conf['https'].$global['uuid']."\">".$conf['https'].$global['uuid']."</a></div>
					<br>
		";

	// record has foreign origin, so show separate url and uuid fields
	} else {
		$q = "
			SELECT *
			FROM pfif_person
			WHERE p_uuid = '".$global['uuid']."';
		";
		$r = $global['db']->Execute($q);
		if($r === false) { daoErrorLog(__FILE__, __LINE__, __METHOD__, __CLASS__, __FUNCTION__, $global['db']->ErrorMsg(), "edit edit 12 ((".$q."))"); }
		if($row = $r->FetchRow()) {
			$url = $row['source_url'];
		} else {
			$url = "";
		}
		// fix bad data
		if($url == "NULL" || $url == "null") {
			$url = "";
		}

		if(trim($url) == "") {
			$linkToShow = "<div class=\"originUrl\">N/A</div>";
		} else {
			$linkToShow = "<div class=\"originUrl\"><a href=\"".$url."\">".$url."</a></div>";
		}

		echo "
					<label for=\"opt_status\"><b>Origin ID</b> </label>
					<input type=\"text\" name=\"opt_status\" id=\"opt_status\" value=\"".$global['uuid']."\" size=\"64\" class=\"nonedit2 public\" READONLY>
					<br>

					<label for=\"opt_status\"><b>Origin URL</b> </label>
					".$linkToShow."
					<br>
		";
	}


	echo "
				</fieldset>
			</form>
		</div>
	";
	// HEADER END ////////////////////////////////////////////////////////////////////////////////


	// IMAGES ////////////////////////////////////////////////////////////////////////////////////

	// figure out if the person has any images ...
	$q  = "
		SELECT COUNT(*)
		FROM image
		WHERE p_uuid = '".$global['uuid']."'
    AND note_id = '0';
	";
	$r   = $global['db']->Execute($q);
	if($r === false) { daoErrorLog(__FILE__, __LINE__, __METHOD__, __CLASS__, __FUNCTION__, $global['db']->ErrorMsg(), "edit edit 13 ((".$q."))"); }
	$row = $r->FetchRow();
	$num = (int)$row['COUNT(*)'];

	$imagesFull  = array();
	$imagesThumb = array();
	$primary     = array();

	// set default silhouettes

	$boy     = "res/img/s0boy.png";
	$girl    = "res/img/s1girl.png";
	$man     = "res/img/s2man.png";
	$woman   = "res/img/s3woman.png";
	$unknown = "res/img/s4unknown.png";


	// add a report abuse button
	echo '<div style="clear:both;"></div>';
	echo '<div style="float: right;" id="edit2abuse" onclick="abuse(\''.$global['uuid'].'\');">report abuse <img src="res/img/flag_hover.png"></div>';
	echo '
		<script>
			function abuse(uuid) {
				window.location.href = "'.$conf['https'].$conf['base_uuid']."edit?puuid=".urlencode($global['uuid']).'&abuse=1";
			}
		</script>
	';
	
	// handle abuse requests
	if(isset($_GET['abuse']) && (int)$_GET['abuse'] == 1) {
		add_confirmation("Your request to report this record as abuse has been sent to our site staff.");
		
		$ip = isset($_SERVER['REMOTE_ADDR']) ? "'".mysql_real_escape_string($_SERVER['REMOTE_ADDR'])."'" : "0.0.0.0";
		$q = "
			SELECT *
			FROM `abuse_queue`
			WHERE p_uuid = '".mysql_real_escape_string($global['uuid'])."'
			AND ip = ".$ip.";
		";
		$result = $global['db']->Execute($q);
		if($result === false) { daoErrorLog(__FILE__, __LINE__, __METHOD__, __CLASS__, __FUNCTION__, $global['db']->ErrorMsg(), "reportAbuse via EAP 1 ((".$q."))"); }
		if($result != null && !$result->EOF) {
			$skipAbuse = true;
		} else {
			$skipAbuse = false;
		}

		// ok, so its a valid record and the user hasn't already reported it, so add it to the queue
		if(!$skipAbuse) {
			
			// figure out of anonymous or logged in user
			if(($_SESSION['logged_in'] != 1) || ($_SESSION['user'] == 'Anonymous')) {
				$identifier = 'anonymous';
			} else {
				$identifier = $_SESSION['user'];
			}
			
			$q = "
				INSERT INTO `abuse_queue` (
					`p_uuid`,
					`identifier`,
					`ip`
				) VALUES (
					'".mysql_real_escape_string((string)$global['uuid'])."',
					'".$identifier."',
					".$ip."
				);
			";
			$result = $global['db']->Execute($q);
			if($result === false) { daoErrorLog(__FILE__, __LINE__, __METHOD__, __CLASS__, __FUNCTION__, $global['db']->ErrorMsg(), "reportAbuse via EAP 2 ((".$q."))"); }
		}
	}

	
	// begin output
	echo '<div style="clear:both;"></div>';	
	echo '<hr>';
	echo "
		<div class=\"form-container\">
			<form method=\"post\" id=\"editPublic\" name=\"editPublic\" action=\"edit?puuid=".urlencode($global['uuid'])."\" enctype=\"multipart/form-data\">
				<fieldset id=\"editImages\">
					<legend>Images</legend>
	";

	if($global['publicEdit']) {

		// add images...
		if((count($_POST) > 0) && isset($_POST['UPLOAD'])) {

			//echo "<pre>".print_r($_FILES, true)."</pre>";

			$name     = $_FILES["add_image"]["name"];
			$type     = $_FILES["add_image"]["type"];
			$tmp_name = $_FILES["add_image"]["tmp_name"];
			$error    = $_FILES["add_image"]["error"];
			$size     = $_FILES["add_image"]["size"];

			if($size > 10000000) {
				add_error("Uploaded file is too large. Please upload an image file smaller than 10 MB.");

			} elseif($type == "image/gif" || $type == "image/jpeg" || $type == "image/png" || $type == "image/x-png" || $type == "image/pjpeg") {

				if ($_FILES["add_image"]["error"] > 0) {
					echo "Return Code: " . $_FILES["file"]["error"] . "<br />";

				} else {
					// get the file contents...
					$image = file_get_contents($tmp_name);

/*
					echo "Upload: ".$name."<br />";
					echo "Type: ".$type."<br />";
					echo "Size: ".($size/1000000)." MB<br />";
					echo "Temp file: ".$tmp_name."<br />";
*/
					require_once($global['approot']."inc/lib_image.inc");

					// generate path and filename portion
					$a = explode("/", $global['uuid']);

					$image_id = shn_create_uuid("image");

					$filename = $a[0]."_".$a[1]; // make pl.nlm.nih.gov/person.123456 into pl.nlm.nih.gov_person.123456
					$filename = $filename."_".$image_id."_"; // filename now like pl.nlm.nih.gov_person.123456_112233_
					$path = $global['approot']."www/tmp/plus_cache/".$filename; // path is now like /opt/pl/www/tmp/plus_cache/pl.nlm.nih.gov_person.123456_112233_

					// save original like /opt/pl/www/tmp/plus_cache/person.123456_112233_original
					file_put_contents($path."original", $image);

					// get information from original file
					$info = getimagesize($path."original");
					$image_width  = $info[0];
					$image_height = $info[1];
					list(,$mime) = explode("/",$info['mime']);
					$mime = strtolower($mime);
					$image_type = $mime;
					if(stripos($mime,"png") !== FALSE) {
						$ext = ".png";
					} elseif(stripos($mime,"gif") !== FALSE) {
						$ext = ".gif";
					} else {
						$ext = ".jpg";
					}

					// save full size resampled image like /opt/pl/www/tmp/plus_cache/person.123456_112233_full.ext
					shn_image_resize($path."original", $path."full".$ext, $image_width, $image_height);

					// save thumb resampled image (320px height) like /opt/pl/www/tmp/plus_cache/person.123456_112233_thumb.ext
					shn_image_resize_height($path."original", $path."thumb".$ext, 320);

					$fullSizePath  = $path."full".$ext;
					$thumbnailPath = $path."thumb".$ext;

					// update URLs
					$url       = "tmp/plus_cache/".$filename."full".$ext;
					$url_thumb = "tmp/plus_cache/".$filename."thumb".$ext;

					// make the files world writeable for other users/applications and to handle deletes
					chmod($path."original",   0777);
					chmod($path."full".$ext,  0777);
					chmod($path."thumb".$ext, 0777);

					// if the person has image(s) already, make this a secondary image, otherwise primary
					if($num > 0) {
						$principal = (int)0;
					} else {
						$principal = (int)1;
					}

					$q = "
						INSERT INTO image (
							image_id,
							p_uuid,
							image_type,
							image_height,
							image_width,
							url,
							url_thumb,
							original_filename,
							principal )
						VALUES (
							'".$image_id."',
							'".$global['uuid']."',
							'".$image_type."',
							'".$image_height."',
							'".$image_width."',
							'".$url."',
							'".$url_thumb."',
							'".mysql_real_escape_string((string)$tmp_name)."',
							".$principal." );
					";
					$result = $global['db']->Execute($q);
					if($result === false) { daoErrorLog(__FILE__, __LINE__, __METHOD__, __CLASS__, __FUNCTION__, $global['db']->ErrorMsg(), "eap insert image ((".$q."))"); } else {
						$num++; // we just added an image
					}

					add_confirmation("Your image has been uploaded successfully.");
				}
			} else {
				add_error("Invalid filetype uploaded. Supported image filetypes are jpeg, png, and gif.");
			}
		}
		echo '
			<div style="margin: 10px;" id="eapUploadImage">
				<br>
				Add an additional image to this record:<br>
				<br>
				<input type="file" name="add_image" id="add_image" onchange="enableUploadButton();" /><br>
				<br>
				<input type="submit" name="UPLOAD" id="UPLOAD" value="UPLOAD" class="styleTehButton" style="opacity: 0.2;" DISABLED/><br>
				<br>
				<i>Please save any changes to this record first<br>
				before uploading a new image or they will be lost.</i>
				<script>
					function enableUploadButton() {
						var d = document.getElementById("UPLOAD");
						d.disabled = false;
						d.style.opacity = "1";
					}
				</script>
			</div>
			<div style="clear: both;"></div>
		';
	}

	// show all images

	$num = 0;
	$q  = "
		SELECT *
		FROM image
		WHERE p_uuid = '".$global['uuid']."'
    AND note_id = '0';
	";
	$r   = $global['db']->Execute($q);
	if($r === false) { daoErrorLog(__FILE__, __LINE__, __METHOD__, __CLASS__, __FUNCTION__, $global['db']->ErrorMsg(), "edit edit 14 ((".$q."))"); }
	while ($row = $r->FetchRow()) {
		$imagesFull[]  = $row['url'];
		$imagesThumb[] = $row['url_thumb'];

		// find the first image tag for this image and call it the caption....
		$caption = "";
		$q2  = "
			SELECT *
			FROM image_tag
			WHERE image_id = '".$row['image_id']."'
			LIMIT 1;
		";
		$r2   = $global['db']->Execute($q2);
		if($r2 === false) { daoErrorLog(__FILE__, __LINE__, __METHOD__, __CLASS__, __FUNCTION__, $global['db']->ErrorMsg(), "edit edit image tag 14b ((".$q2."))"); }
		while(!$r2->EOF) {
			$row2 = $r2->FetchRow();
			$caption = $row2['tag_text'];
		}

		// formulate the image footer
		if((int)$row['principal'] == 1) {
			$primary[] = "PRIMARY IMAGE<br>".$caption;
		} else {
			$primary[] = "SECONDARY IMAGE<br>".$caption;
		}
		$num++;
	}

	if($num == 0) {
		echo '
			<div style="float: left; margin-right: 10px;">
			This record does not have an image associated with it.
			</div>
		';
	} else {
		echo "<span id=\"gallery\">";
		for($i = 0; $i < $num; $i++) {
			echo '
				<a href="'.$imagesFull[$i].'" target="_blank" title="'.$primary[$i].'">
					<div style="float: left; width: 200px !important; padding: 5px; margin: 10px; box-shadow: 1px 1px 8px rgba(127,127,127,0.5);">
						<img style="margin-bottom: 5px;" width="200" src="'.$imagesThumb[$i].'">
						<div style="clear: both;"></div>
					<span style="color: #777 !important;">'.$primary[$i].'</span>
					</div>
				</a>
			';
		}
		echo "</span>";
		echo "
			<script type=\"text/javascript\">
			$(function() {
				$('#gallery a').lightBox(); // Select all links in object with gallery ID
			});
			</script>

		";
	}

	echo "
				</fieldset>
			</form>
		</div>
	";
	// IMAGES END /////////////////////////////////////////////////////////////////////////////////////


	// VOICENOTE BEGIN ////////////////////////////////////////////////////////////////////////////////

	// figure out if the person have a voicenote

	$q  = "
		SELECT COUNT(*)
		FROM voice_note
		WHERE p_uuid = '".$global['uuid']."';
	";
	$r   = $global['db']->Execute($q);
	if($r === false) { daoErrorLog(__FILE__, __LINE__, __METHOD__, __CLASS__, __FUNCTION__, $global['db']->ErrorMsg(), "edit edit 15 ((".$q."))"); }
	$row = $r->FetchRow();
	$num = $row['COUNT(*)'];

	// we have voicenote
	if($num > 0) {
		$q  = "
			SELECT *
			FROM voice_note
			WHERE p_uuid = '".$global['uuid']."';
		";
		$r   = $global['db']->Execute($q);
		if($r === false) { daoErrorLog(__FILE__, __LINE__, __METHOD__, __CLASS__, __FUNCTION__, $global['db']->ErrorMsg(), "edit edit 16 ((".$q."))"); }

		echo "
				<div style=\"clear:both;\"></div>
				<hr>
				<div class=\"form-container\" id=\"voiceNoteDiv\">
					<form>
						<fieldset id=\"editImages\">
							<legend>Voice Note</legend>
		";

		while($row = $r->FetchRow()) {
			$url_mp3 = $row['url_resampled_mp3'];
			$url_ogg = $row['url_resampled_ogg'];
			echo "
							<audio controls preload=\"auto\" autobuffer>
								<source src=\"".$url_mp3."\" type=\"audio/mp3\" />
								<source src=\"".$url_ogg."\" type=\"audio/ogg\" />
								<br>Your browser doesn't support the HTML5 audio tag.<br>
								<a href=\"".$url_mp3."\">Click here to download the media instead.</a>
							</audio>
							<div style=\"height: 5px; clear:both;\"></div>
			";
		}

		echo "
						</fieldset>
					</form>
				</div>
		";
	}
	// VOICENOTE END //////////////////////////////////////////////////////////////////////////////////
	
	
	// MAP BEGIN //////////////////////////////////////////////////////////////////////////////////////
	
	
	// get LAT-LON from reunite data first
	$q  = "
		SELECT *
		FROM person_status
		WHERE p_uuid = '".$global['uuid']."';
	";
	$r = $global['db']->Execute($q);
	if($r === false) { daoErrorLog(__FILE__, __LINE__, __METHOD__, __CLASS__, __FUNCTION__, $global['db']->ErrorMsg(), "edit edit 9999 ((".$q."))"); }
	$row = $r->FetchRow();
	$lat = $row['latitude'];
	$lon = $row['longitude'];

	$street2 = $row['street2'];
	if($street2 != null && strlen(trim($street2)) > 0) {
		$street2 = "<br> &nbsp; ".$row['street2'];
	} else {
		$street2 = "";
	}
	
	$info = "<b>GPS: ".$lat.", ".$lon."</b><br> &nbsp; ".$row['street1'].$street2."<br> &nbsp; ".$row['city']."<br> &nbsp; ".$row['region']." ".$row['postal_code']."<br> &nbsp; ".$row['country'];

	
	//next try and get lat/lon for a hospital
	if($lat == null && $lon == null && $person_to_hospital['hospital_uuid'] != null) {
		
		$q = "
			SELECT *
			FROM person_uuid p, hospital h
			WHERE p.p_uuid = '".$global['uuid']."'
			AND p.hospital_uuid = h.hospital_uuid ;
		";
		$r = $global['db']->Execute($q);
		if($r === false) { daoErrorLog(__FILE__, __LINE__, __METHOD__, __CLASS__, __FUNCTION__, $global['db']->ErrorMsg(), "edit edit 9998 ((".$q."))"); }
		$row = $r->FetchRow();
		
		$lat = $row['latitude'];
		$lon = $row['longitude'];

		$street2 = $row['street2'];
		if($street2 != null && strlen(trim($street2)) > 0) {
			$street2 = "<br> &nbsp; ".$row['street2'];
		} else {
			$street2 = "";
		}
				
		$info = "<b>GPS: ".$lat.", ".$lon."</b><br> &nbsp; ".$row['name']."<br> &nbsp; ".$row['street1'].$street2."<br> &nbsp; ".$row['city']."<br> &nbsp; ".$row['region']." ".$row['postal_code']."<br> &nbsp; ".$row['country'];
	}
	

	// valid point?
	if($lat != null && $lon != null && (float)$lat > -90 && (float)$lat < 90 && (float)$lon > -90 && $lon < 90) {
		
		
		global $conf;
		if(isset($conf['gmaps_api_string'])) {
			echo $conf['gmaps_api_string'];
		} else {
			echo '<script type="text/javascript" src="http://maps.google.com/maps/api/js?v=3&sensor=true"></script>';
		}
		
		
		echo "
			<div style=\"clear:both;\"></div>
			<hr>
			<div class=\"form-container\" id=\"locationDiv\" style=\"width: 100%;\">
				<form>
					<fieldset id=\"editImages\">
						<legend>Last Known Location</legend>
		";
		
		echo '
			<div id="mapCanvas" style="width: 600px; height: 500px;"></div>
		';
		
		echo "

<script>
		
// Google Maps junx

var latitude = '".$lat."';
var longitude = '".$lon."';
var map;
var marker;

var latlng = new google.maps.LatLng(latitude, longitude);
var config = {
	zoom: 12,
	center: latlng,
	disableDefaultUI: true,
	navigationControl: false,
	navigationControlOptions: {
		style: google.maps.NavigationControlStyle.ZOOM_PAN
	},
	zoomControl: true,
	zoomControlOptions: {
		style: google.maps.ZoomControlStyle.LARGE
	},
	mapTypeControl: false,
	mapTypeId: google.maps.MapTypeId.ROADMAP,
	draggable: false,
	disableDoubleClickZoom: true,
	panControl: false,
	scrollwheel: false,
	scaleControl: false	
};
map = new google.maps.Map(document.getElementById('mapCanvas'), config);

marker = new google.maps.Marker({
	map: map,
	draggable: false,
});

var info = new google.maps.InfoWindow( {
    content: '".$info."'
});

info.open(map, marker);

google.maps.event.addListener(marker, 'click', function() { info.open(map, marker); });

marker.setPosition(latlng);
map.setCenter(latlng);

</script>
		";
		
		echo "
					</fieldset>
				</form>
			</div>
		";
	}	
	// MAP END ////////////////////////////////////////////////////////////////////////////////////////


	// LOAD PUBLIC DATA ///////////////////////////////////////////////////////////////////////////////
	$gender = $person_details['opt_gender'];
	$skin = null;
	$eye = null;
	$hair = null;

	// load physical
	$q  = "
		SELECT *
		FROM person_physical
		WHERE p_uuid = '".$global['uuid']."';
	";
	$r = $global['db']->Execute($q);
	if($r === false) { daoErrorLog(__FILE__, __LINE__, __METHOD__, __CLASS__, __FUNCTION__, $global['db']->ErrorMsg(), "edit edit 17 ((".$q."))"); }
	$person_physical = $r->FetchRow();

	// load status
	$q  = "
		SELECT *
		FROM person_status s, field_options f
		WHERE s.p_uuid = '".$global['uuid']."'
		AND f.field_name = 'opt_status'
		AND s.opt_status = f.option_code;
	";
	$r = $global['db']->Execute($q);
	if($r === false) { daoErrorLog(__FILE__, __LINE__, __METHOD__, __CLASS__, __FUNCTION__, $global['db']->ErrorMsg(), "edit edit 18 ((".$q."))"); }
	$person_status  = $r->FetchRow();

	// check for not sets
	if(!isset($person_status['opt_status'])) {
		$person_status['opt_status'] = "unk";
	}
	if(!isset($person_status['creation_time'])) {
		$person_status['creation_time'] = null;
	}
	if(!isset($person_status['last_updated'])) {
		$person_status['last_updated'] = null;
	}

	// map field_options to real values
	// map gender
	$q  = "
		SELECT *
		FROM field_options
		WHERE option_code = '".$gender."'
		AND field_name = 'opt_gender';
	";
	$r   = $global['db']->Execute($q);
	if($r === false) { daoErrorLog(__FILE__, __LINE__, __METHOD__, __CLASS__, __FUNCTION__, $global['db']->ErrorMsg(), "edit edit 19 ((".$q."))"); }
	$row = $r->FetchRow();
	$genderEnum = $row['option_description'];

	// map eyes
	$q  = "
		SELECT *
		FROM field_options
		WHERE option_code = '".$eye."'
		AND field_name = 'opt_eye_color';
	";
	$r   = $global['db']->Execute($q);
	if($r === false) { daoErrorLog(__FILE__, __LINE__, __METHOD__, __CLASS__, __FUNCTION__, $global['db']->ErrorMsg(), "edit edit 20 ((".$q."))"); }
	$row = $r->FetchRow();
	$eye = $row['option_description'];

	// map skin color
	$q  = "
		SELECT *
		FROM field_options
		WHERE option_code = '".$skin."'
		AND field_name = 'opt_skin_color';
	";
	$r   = $global['db']->Execute($q);
	if($r === false) { daoErrorLog(__FILE__, __LINE__, __METHOD__, __CLASS__, __FUNCTION__, $global['db']->ErrorMsg(), "edit edit 21 ((".$q."))"); }
	$row = $r->FetchRow();
	$skin = $row['option_description'];

	// map hair color
	$q  = "
		SELECT *
		FROM field_options
		WHERE option_code = '".$hair."'
		AND field_name = 'opt_hair_color';
	";
	$r   = $global['db']->Execute($q);
	if($r === false) { daoErrorLog(__FILE__, __LINE__, __METHOD__, __CLASS__, __FUNCTION__, $global['db']->ErrorMsg(), "edit edit 22 ((".$q."))"); }
	$row = $r->FetchRow();
	$hair = $row['option_description'];
	// LOAD PUBLIC DATA END ///////////////////////////////////////////////////////////////////////////


	// PUBLIC EDIT ////////////////////////////////////////////////////////////////////////////////////
	if($global['publicEdit']) {

		// edit...
		if((count($_POST) > 0) && isset($_POST['given_name'])) {

			// check if this is the first save...
			$q  = "
				SELECT *
				FROM person_uuid
				WHERE p_uuid = '".$global['uuid']."';
			";
			$r = $global['db']->Execute($q);
			if($r === false) { daoErrorLog(__FILE__, __LINE__, __METHOD__, __CLASS__, __FUNCTION__, $global['db']->ErrorMsg(), "edit edit 18b ((".$q."))"); }
			$checkcheck  = $r->FetchRow();
			if($checkcheck['expiry_date'] == "2000-01-01 01:01:01") {
				$global['makeStaticPfifNote'] = true;
				$person_uuid['expiry_date'] = updateExpiryDate();
			}

			$editGivenName     = trim($_POST['given_name']);
			$editFamilyName    = trim($_POST['family_name']);

            // we only save age now, no longer ranges (but ranges can be inserted into age column)
            $editAge           = trim($_POST['age']);
            $editMinAge        = null;
            $editMaxAge        = null;

            $editGender        = trim($_POST['opt_gender']);
			$editEye           = trim($_POST['opt_eye_color']);
			$editSkin          = trim($_POST['opt_skin_color']);
			$editHair          = trim($_POST['opt_hair_color']);
			$editHeight        = trim($_POST['height']);
			$editWeight        = trim($_POST['weight']);
			$editComments      = trim($_POST['comments']);
			$editStatus        = trim($_POST['opt_status']);
			$editHospital      = trim($_POST['hospital']);
			$editEvent         = trim($_POST['event']);
			$editLastSeen      = trim($_POST['last_seen']);
			$editLastClothing  = trim($_POST['last_clothing']);
			$editOtherComments = trim($_POST['other_comments']);

			if($editGivenName == '')     { $editGivenName = null; }
			if($editFamilyName == '')    { $editFamilyName = null; }
			if($editAge == '')           { $editAge = null; }
			if($editMinAge == '')        { $editMinAge = null; }
			if($editMaxAge == '')        { $editMaxAge = null; }
			if($editGender == '')        { $editGender = null; }
			if($editEye == '')           { $editEye = null; }
			if($editSkin == '')          { $editSkin = null; }
			if($editHair == '')          { $editHair = null; }
			if($editWeight == '')        { $editWeight = null; }
			if($editHeight == '')        { $editHeight = null; }
			if($editComments == '')      { $editComments = null; }
			if($editStatus == '')        { $editStatus = 'unk'; }
			if($editHospital == '')      { $editHospital = null; }
			if($editEvent == '')         { $editEvent = null; }
			if($editLastSeen == '')      { $editLastSeen = null; }
			if($editLastClothing == '')  { $editLastClothing = null; }
			if($editOtherComments == '') { $editOtherComments = null; }

			// check for revisions and save dem
			if($editGivenName  !== $person_uuid['given_name'])  {
				saveRevision("person_uuid", "given_name", "Given Name", $person_uuid['given_name'],  $editGivenName);
				updateFullName($editGivenName, $editFamilyName);
			}
			if($editFamilyName !== $person_uuid['family_name']) {
				saveRevision("person_uuid", "family_name", "Family Name", $person_uuid['family_name'], $editFamilyName);
				updateFullName($editGivenName, $editFamilyName);
			}

			if($editLastSeen      !== $person_details['last_seen'])      {
				saveRevision("person_details", "last_seen", "Last Seen", $person_details['last_seen'], $editLastSeen);
				$global['makeStaticPfifNote'] = true;
			}
			if($editLastClothing  !== $person_details['last_clothing'])  { saveRevision("person_details",       "last_clothing",  "Last Clothing",  $person_details['last_clothing'],  $editLastClothing);  }
			if($editOtherComments !== $person_details['other_comments']) { saveRevision("person_details",       "other_comments", "Other Comments", $person_details['other_comments'], $editOtherComments); }
			if($editAge           !== $person_details['years_old'])      { saveRevision("person_details",       "years_old",      "Age",            $person_details['years_old'],      $editAge);           }
			if($editMinAge        !== $person_details['minAge'])         { saveRevision("person_details",       "minAge",         "Minimum Age",    $person_details['minAge'],         $editMinAge);        }
			if($editMaxAge        !== $person_details['maxAge'])         { saveRevision("person_details",       "maxAge",         "Maximum Age",    $person_details['maxAge'],         $editMaxAge);        }
			if($editGender        !== $person_details['opt_gender'])     { saveRevision("person_details",       "opt_gender",     "Gender",         $person_details['opt_gender'],     $editGender);        }

			if($editEye        !== $person_physical['opt_eye_color'])    { saveRevision("person_physical",      "opt_eye_color",  "Eye Color",   $person_physical['opt_eye_color'],    $editEye);      }
			if($editSkin       !== $person_physical['opt_skin_color'])   { saveRevision("person_physical",      "opt_skin_color", "Skin Color",  $person_physical['opt_skin_color'],   $editSkin);     }
			if($editHair       !== $person_physical['opt_hair_color'])   { saveRevision("person_physical",      "opt_hair_color", "Hair Color",  $person_physical['opt_hair_color'],   $editHair);     }
			if($editHeight     !== $person_physical['height'])           { saveRevision("person_physical",      "height",         "Height",      $person_physical['height'],           $editHeight);   }
			if($editWeight     !== $person_physical['weight'])           { saveRevision("person_physical",      "weight",         "Weight",      $person_physical['weight'],           $editWeight);   }
			if($editComments   !== $person_physical['comments'])         { saveRevision("person_physical",      "comments",       "Comments",    $person_physical['comments'],         $editComments); }

			if($editStatus     !== $person_status['opt_status'])         {
				saveRevision("person_status", "opt_status", "Status", $person_status['opt_status'], $editStatus);
				$global['makeStaticPfifNote'] = true;
			}

			if($editEvent      !== $person_incident['incident_id'])      { saveRevision("person_uuid",          "incident_id",    "Event",       $person_incident['incident_id'],      $editEvent);    }
			if($editHospital   !== $person_to_hospital['hospital_uuid']) { saveRevision("person_uuid",          "hospital_uuid",  "Hospital",    $person_to_hospital['hospital_uuid'], $editHospital); }

			// reload status since changes made......
			$q  = "
				SELECT *
				FROM person_status s, field_options f
				WHERE s.p_uuid = '".$global['uuid']."'
				AND f.field_name = 'opt_status'
				AND s.opt_status = f.option_code;
			";
			$r              = $global['db']->Execute($q);
			if($r === false) { daoErrorLog(__FILE__, __LINE__, __METHOD__, __CLASS__, __FUNCTION__, $global['db']->ErrorMsg(), "edit edit 23 ((".$q."))"); }
			$person_status  = $r->FetchRow();

			// we use last updated in the case creation time is unavailable
			if($person_status['creation_time'] == null) {
				$person_status['creation_time'] = $person_status['last_updated'];
			}

		// non-edit...load data
		} else {
			$editGivenName  = $person_uuid['given_name'];
			$editFamilyName = $person_uuid['family_name'];

			$editAge           = $person_details['years_old'];
			$editMinAge        = $person_details['minAge'];
			$editMaxAge        = $person_details['maxAge'];
			$editGender        = $person_details['opt_gender'];
			$editLastSeen      = $person_details['last_seen'];
			$editLastClothing  = $person_details['last_clothing'];
			$editOtherComments = $person_details['other_comments'];

			$editEye        = $person_physical['opt_eye_color'];
			$editSkin       = $person_physical['opt_skin_color'];
			$editHair       = $person_physical['opt_hair_color'];
			$editHeight     = $person_physical['height'];
			$editWeight     = $person_physical['weight'];
			$editComments   = $person_physical['comments'];

			$editStatus     = isset($person_status['opt_status']) ?  $person_status['opt_status'] : null;

			$editHospital   = $person_to_hospital['hospital_uuid'];
			$editEvent      = $person_incident['incident_id'];
		}

		$timezone = new DateTimeZone( "UTC" );
		$date = new DateTime();
		$date->setTimezone( $timezone );

		$date->setTimestamp(strtotime($person_status['creation_time']));
		$creation_time =  $date->format('l, F j, Y g:i a T');
		if($person_status['creation_time'] == null) {
			$creation_time = "---";
		}

		$date->setTimestamp(strtotime($person_status['last_updated']));
		$last_updated =  $date->format('l, F j, Y g:i a T');
		if($person_status['last_updated'] == null) {
			$last_updated = "---";
		}

		$date->setTimestamp(strtotime($person_uuid['expiry_date']));
		$expiry_date =  $date->format('l, F j, Y g:i a T');
		if($person_uuid['expiry_date'] == null || $person_uuid['expiry_date'] == "2000-01-01 01:01:01") {
			$expiry_date = "---";
		}

echo '<div style="clear:both;"></div>';
echo '<hr>';
		echo "
			<div class=\"form-container\">
			<form method=\"post\" id=\"editPublic\" name=\"editPublic\" action=\"edit?puuid=".urlencode($global['uuid'])."\">
			<fieldset id=\"editPublic\">
			<legend><span class=\"publicText\">Public Information</span></legend>

			<label for=\"opt_status\"><b>Record Created</b> </label>
			<input type=\"text\" name=\"opt_status\" id=\"opt_status\" value=\"".$creation_time."\" size=\"50\" tabindex=\"".++$shn_tabindex."\" class=\"nonedit2 public\" READONLY>
			<br>

			<label for=\"opt_status\"><b>Last Updated</b> </label>
			<input type=\"text\" name=\"opt_status\" id=\"opt_status\" value=\"".$last_updated."\" size=\"50\" tabindex=\"".++$shn_tabindex."\" class=\"nonedit2 public\" READONLY>
			<br>

			<label for=\"opt_status\"><b>Record Expires</b> </label>
			<input type=\"text\" name=\"opt_status\" id=\"opt_status\" value=\"".$expiry_date."\" size=\"50\" tabindex=\"".++$shn_tabindex."\" class=\"nonedit2 public\" READONLY>
			<br>
			<br>
		";
		// if the person was just reported via rap2, creation_time and last_updated are equal, so notify the user...
		if($person_status['creation_time'] == $person_status['last_updated'] && $global['publicEdit'] == true) {
			// check if the person was reported via rap2
			$q = "
				SELECT count(*)
				FROM rap_log
				WHERE p_uuid = '".$global['uuid']."';
			";
			$r = $global['db']->Execute($q);
			if($r === false) { daoErrorLog(__FILE__, __LINE__, __METHOD__, __CLASS__, __FUNCTION__, $global['db']->ErrorMsg(), "edit edit 23b ((".$q."))"); }
			$row = $r->FetchRow();
			$count = $row['count(*)'];
			if($count > 0) {
				add_confirmation("A new person record has been created. Please fill in as much information as you can about this person and then save the changes.");
				$global['makeStaticPfifNote'] = false;
			}
		}


		// generate status dropdown
		$q = "
			SELECT *
			FROM field_options
			WHERE field_name = 'opt_status'
		";
		//AND option_code != 'unk';
		$r = $global['db']->Execute($q);
		if($r === false) { daoErrorLog(__FILE__, __LINE__, __METHOD__, __CLASS__, __FUNCTION__, $global['db']->ErrorMsg(), "edit edit 24 ((".$q."))"); }
		$opt_status_array = array();
		while(!$r->EOF) {
			$opt_status_array[$r->fields['option_code']] = $r->fields['option_description'];
			$r->MoveNext();
		}
		shn_form_select($opt_status_array, "<b>Health / Locational Status</b>", "opt_status", "onchange=\"\"", array('value'=>$editStatus));


		// generate hospital dropdown
		$q = "
			SELECT *
			FROM hospital;
		";
		$r = $global['db']->Execute($q);
		if($r === false) { daoErrorLog(__FILE__, __LINE__, __METHOD__, __CLASS__, __FUNCTION__, $global['db']->ErrorMsg(), "edit edit 25 ((".$q."))"); }
		$hospital_array = array();
		$hospital_array[null] = '---';
		while(!$r->EOF) {
			$hospital_array[$r->fields['hospital_uuid']] = $r->fields['name'];
			$r->MoveNext();
		}
		if($global['role'] != "staff" && $global['role'] != "admin") {
				$readOnlyH = ' disabled style="opacity: 0.2;"';
		} else {
				$readOnlyH = "";
		}
		
		shn_form_select($hospital_array, "<b>Located at Hospital</b>", "hospital", $readOnlyH, array('value'=>$editHospital));


		// generate incident dropdown
		$q = "
			SELECT *
			FROM incident;
		";
		$r = $global['db']->Execute($q);
		if($r === false) { daoErrorLog(__FILE__, __LINE__, __METHOD__, __CLASS__, __FUNCTION__, $global['db']->ErrorMsg(), "edit edit 26 ((".$q."))"); }
		$incident_array = array();
		while(!$r->EOF) {
			$incident_array[$r->fields['incident_id']] = $r->fields['name'];
      if($r->fields['incident_id'] == $editEvent) {
        $newTitle = $r->fields['name'];
      }
			$r->MoveNext();
		}
		shn_form_select($incident_array, "<b>Related to Event:</b>", "event", "onchange=\"\"", array('value'=>$editEvent));
    echo "
      <script>
        // they want the title to show the event name, so we update it here:
        document.title = document.title + ' :: ".$newTitle."';
      </script>
    ";

		// rest of public info......

        // On Display:
        // If we have an exact age, display it
        // else If we have an age range display it instead
        // else display nothing

        if($editAge != null) {
            $editAgeAll = $editAge;
        } elseif($editMinAge != null && $editMaxAge != null) {
            $editAgeAll = $editMinAge."-".$editMaxAge;
        } else {
            $editAgeAll = null;
        }

		$editGivenName  = htmlspecialchars($editGivenName);
		$editFamilyName = htmlspecialchars($editFamilyName);
		$global['full_name'] = $editGivenName." ".$editFamilyName;
		echo "
			<script>
				var shit = document.getElementById('moveShitHere');
				shit.innerHTML = '<b><label for=\"given_name\">Given Name </label></b><input onchange=\"updateGiven(this.value);\" onblur=\"updateGiven(this.value);\" type=\"text\" name=\"given_name2\" id=\"given_name2\" value=\"".$editGivenName."\" size=\"50\" tabindex=\"".++$shn_tabindex."\" class=\"public\"><br><b><label for=\"family_name\">Family Name </label></b><input onchange=\"updateFamily(this.value);\" onblur=\"updateFamily(this.value);\" type=\"text\" name=\"family_name2\" id=\"family_name2\" value=\"".$editFamilyName."\" size=\"50\" tabindex=\"".++$shn_tabindex."\" class=\"public\"><br>';

				function updateGiven(value) {
					var gv = document.getElementById('given_name');
					gv.value = value;
				}
				function updateFamily(value) {
					var fv = document.getElementById('family_name');
					fv.value = value;
				}
			</script>

			<input type=\"hidden\" id=\"given_name\" name=\"given_name\" value=\"".$editGivenName."\">
			<input type=\"hidden\" id=\"family_name\" name=\"family_name\" value=\"".$editFamilyName."\">

			<label for=\"age\">Age</label>
			<input size=\"6\" type=\"text\" name=\"age\" id=\"age\" value=\"".$editAgeAll."\" size=\"6\" tabindex=\"".++$shn_tabindex."\" class=\"public\">
			<br>
		";

		shn_form_opt_select("opt_gender",     "Gender",     null, array('sort'=>'display_order, option_description ASC', 'value'=>$editGender));
		shn_form_opt_select("opt_eye_color",  "Eye Color",  null, array('sort'=>'display_order, option_description ASC', 'value'=>$editEye   ));
		shn_form_opt_select("opt_skin_color", "Skin Color", null, array('sort'=>'display_order, option_description ASC', 'value'=>$editSkin  ));
		shn_form_opt_select("opt_hair_color", "Hair Color", null, array('sort'=>'display_order, option_description ASC', 'value'=>$editHair  ));


		echo "
			<label for=\"height\">Height</label>
			<input type=\"text\" name=\"height\" id=\"height\" value=\"".htmlspecialchars($editHeight)."\" size=\"10\" maxlength=10 tabindex=\"".++$shn_tabindex."\" class=\"public\">
			<br>

			<label for=\"height\">Weight</label>
			<input type=\"text\" name=\"weight\" id=\"weight\" value=\"".htmlspecialchars($editWeight)."\" size=\"10\" maxlength=10 tabindex=\"".++$shn_tabindex."\" class=\"public\">
			<br>

			<label for=\"height\">Distinctive Features</label>
			<textarea name=\"comments\" id=\"comments\" cols=\"56\" rows=\"4\" tabindex=\"".++$shn_tabindex."\" class=\"public\">".$editComments."</textarea>
			<br>

			<label for=\"height\">Last Seen Location</label>
			<textarea name=\"last_seen\" id=\"last_seen\" cols=\"56\" rows=\"4\" tabindex=\"".++$shn_tabindex."\" class=\"public\">".$editLastSeen."</textarea>
			<br>

			<label for=\"height\">Last Seen Clothing/Appearance</label>
			<textarea name=\"last_clothing\" id=\"last_clothing\" cols=\"56\" rows=\"4\" tabindex=\"".++$shn_tabindex."\" class=\"public\">".$editLastClothing."</textarea>
			<br>

			<label for=\"height\">Other Comments</label>
			<textarea name=\"other_comments\" id=\"other_comments\" cols=\"56\" rows=\"4\" tabindex=\"".++$shn_tabindex."\" class=\"public\">".$editOtherComments."</textarea>
			<br>
			<br>

			<label for=\"height\">&nbsp;</label>
			<input type=\"submit\" value=\"Save Public Information\" class=\"styleTehButton\" tabindex=\"".++$shn_tabindex."\">

			</fieldset>
			</form></div>
		";
	// PUBLIC EDIT END ////////////////////////////////////////////////////////////////////////////////


	// PUBLIC VIEW ////////////////////////////////////////////////////////////////////////////////////
	} else {

		$timezone = new DateTimeZone( "UTC" );
		$date = new DateTime();
		$date->setTimezone( $timezone );

		$date->setTimestamp(strtotime($person_status['creation_time']));
		$creation_time =  $date->format('l, F j, Y g:i a T');
		if($person_status['creation_time'] == null) {
			$creation_time = "---";
		}

		$date->setTimestamp(strtotime($person_status['last_updated']));
		$last_updated =  $date->format('l, F j, Y g:i a T');
		if($person_status['last_updated'] == null) {
			$last_updated = "---";
		}

		$date->setTimestamp(strtotime($person_uuid['expiry_date']));
		$expiry_date =  $date->format('l, F j, Y g:i a T');
		if($person_uuid['expiry_date'] == null) {
			$expiry_date = "---";
		}

		$editMinAge = $person_details['minAge'];
		$editMaxAge = $person_details['maxAge'];
echo '<div style="clear:both;"></div>';
echo '<hr>';
		echo "
			<div class=\"form-container\">
			<form>
			<fieldset id=\"editPublic\">
			<legend><span class=\"publicText\">Public Information</span></legend>

			<label for=\"opt_status\"><b>Record Created</b> </label>
			<input type=\"text\" name=\"opt_status\" id=\"opt_status\" value=\"".$creation_time."\" size=\"50\" tabindex=\"".++$shn_tabindex."\" class=\"nonedit public\" READONLY>
			<br>

			<label for=\"opt_status\"><b>Last Updated</b> </label>
			<input type=\"text\" name=\"opt_status\" id=\"opt_status\" value=\"".$last_updated."\" size=\"50\" tabindex=\"".++$shn_tabindex."\" class=\"nonedit public\" READONLY>
			<br>

			<label for=\"opt_status\"><b>Record Expires</b> </label>
			<input type=\"text\" name=\"opt_status\" id=\"opt_status\" value=\"".$expiry_date."\" size=\"50\" tabindex=\"".++$shn_tabindex."\" class=\"nonedit public\" READONLY>
			<br>

			<label for=\"opt_status\"><b>Health / Locational Status</b> </label>
			<input type=\"text\" name=\"opt_status\" id=\"opt_status\" value=\"".$person_status['option_description']."\" size=\"50\" tabindex=\"".++$shn_tabindex."\" class=\"nonedit public\" READONLY>
			<br>
		";

		if($person_to_hospital['hospital_uuid'] != null) {
			/*
			if(strlen($person_to_hospital['icon_url']) > 5) {
				echo "<img style=\"float: left;\" src=\"".$person_to_hospital['icon_url']."\"><br>";
			}
			*/
			echo "
				<label for=\"opt_status\"><b>Located at Hospital</b> </label>
				<input type=\"text\" name=\"opt_status\" id=\"opt_status\" value=\"".$person_to_hospital['name']."\" size=\"50\" tabindex=\"".++$shn_tabindex."\" class=\"nonedit public\" READONLY>
				<br>
			";
		}

		// generate incident dropdown
		$q = "
			SELECT *
			FROM incident
			WHERE incident_id = '".$person_incident['incident_id']."';
		";
		$r = $global['db']->Execute($q);
		if($r === false) { daoErrorLog(__FILE__, __LINE__, __METHOD__, __CLASS__, __FUNCTION__, $global['db']->ErrorMsg(), "edit edit 27 ((".$q."))"); }
		$row = $r->FetchRow();
		$eventName = $row['name'];

		echo "
			<label for=\"opt_status\"><b>Related to Event</b> </label>
			<input type=\"text\" name=\"opt_status\" id=\"opt_status\" value=\"".$eventName."\" size=\"50\" tabindex=\"".++$shn_tabindex."\" class=\"nonedit public\" READONLY>
      <script>
        // they want the title to show the event name, so we update it here:
        document.title = document.title + ' :: ".$eventName."';
      </script>
			<br>
		";

		// no more auto camel casing...
		//$person_uuid['given_name']  = htmlspecialchars(mb_convert_case($person_uuid['given_name'],  MB_CASE_TITLE, "UTF-8"));
		//$person_uuid['family_name'] = htmlspecialchars(mb_convert_case($person_uuid['family_name'], MB_CASE_TITLE, "UTF-8"));
		$person_uuid['given_name']  = htmlspecialchars($person_uuid['given_name']);
		$person_uuid['family_name'] = htmlspecialchars($person_uuid['family_name']);
		echo "
			<script>
				var shit = document.getElementById('moveShitHere');
				shit.innerHTML = '<b><label for=\"given_name\">Given Name </label></b><input type=\"text\" name=\"given_name\" id=\"given_name\" value=\"".$person_uuid['given_name']."\" size=\"50\" tabindex=\"".++$shn_tabindex."\" class=\"nonedit public\" READONLY><br><b><label for=\"given_name\">Family Name </label></b><input type=\"text\" name=\"family_name\" id=\"family_name\" value=\"".$person_uuid['family_name']."\" size=\"50\" tabindex=\"".++$shn_tabindex."\" class=\"nonedit public\" READONLY><br>';
			</script>

			<label for=\"age\">Age</label>
			<input type=\"text\" name=\"age\" id=\"age\" value=\"".$person_details['years_old']."\" size=\"50\" tabindex=\"".++$shn_tabindex."\" class=\"nonedit public\" READONLY>
			<br>

			<label for=\"age_lower\">Lower Age Range</label>
			<input type=\"text\" name=\"age_lower\" id=\"age_upper\" value=\"".$editMinAge."\" size=\"50\" tabindex=\"".++$shn_tabindex."\" class=\"nonedit public\">
			<br>

			<label for=\"age_upper\">Upper Age Range</label>
			<input type=\"text\" name=\"age_upper\" id=\"age_upper\" value=\"".$editMaxAge."\" size=\"50\" tabindex=\"".++$shn_tabindex."\" class=\"nonedit public\">
			<br>
		";

		$a = (int)$person_details['years_old'];
		(($a >= 0) && ($a <= 150)) ? $hide = true : $hide = false;

		if(!$hide) {
			echo "
				<label for=\"age_lower\">Lower Age Range</label>
				<input type=\"text\" name=\"age_lower\" id=\"age_upper\" value=\"".$person_details['minAge']."\" size=\"50\" tabindex=\"".++$shn_tabindex."\" class=\"nonedit public\" READONLY>
				<br>

				<label for=\"age_upper\">Upper Age Range</label>
				<input type=\"text\" name=\"age_upper\" id=\"age_upper\" value=\"".$person_details['maxAge']."\" size=\"50\" tabindex=\"".++$shn_tabindex."\" class=\"nonedit public\" READONLY>
				<br>
			";
		}

		echo "
			<label for=\"gender\">Gender</label>
			<input type=\"text\" name=\"gender\" id=\"gender\" value=\"".$genderEnum."\" size=\"50\" tabindex=\"".++$shn_tabindex."\" class=\"nonedit public\" READONLY>
			<br>

			<label for=\"eye_color\">Eye Color</label>
			<input type=\"text\" name=\"eye_color\" id=\"eye_color\" value=\"".$eye."\" size=\"50\" tabindex=\"".++$shn_tabindex."\" class=\"nonedit public\" READONLY>
			<br>

			<label for=\"skin_color\">Skin Color</label>
			<input type=\"text\" name=\"skin_color\" id=\"skin_color\" value=\"".$skin."\" size=\"50\" tabindex=\"".++$shn_tabindex."\" class=\"nonedit public\" READONLY>
			<br>

			<label for=\"hair_color\">Hair Color</label>
			<input type=\"text\" name=\"hair_color\" id=\"hair_color\" value=\"".$hair."\" size=\"50\" tabindex=\"".++$shn_tabindex."\" class=\"nonedit public\" READONLY>
			<br>

			<label for=\"height\">Height</label>
			<input type=\"text\" name=\"height\" id=\"height\" value=\"".htmlspecialchars($person_physical['height'])."\" size=\"50\" tabindex=\"".++$shn_tabindex."\" class=\"nonedit public\" READONLY>
			<br>

			<label for=\"height\">Weight</label>
			<input type=\"text\" name=\"weight\" id=\"weight\" value=\"".htmlspecialchars($person_physical['weight'])."\" size=\"50\" tabindex=\"".++$shn_tabindex."\" class=\"nonedit public\" READONLY>
			<br>

			<label for=\"height\">Distinctive Features</label>
			<textarea name=\"comments\" id=\"comments\" cols=\"56\" rows=\"4\" tabindex=\"".++$shn_tabindex."\" class=\"nonedit public\" READONLY>".$person_physical['comments']."</textarea>
			<br>

			<label for=\"height\">Last Seen Location</label>
			<textarea name=\"last_seen\" id=\"last_seen\" cols=\"56\" rows=\"4\" tabindex=\"".++$shn_tabindex."\" class=\"nonedit public\" READONLY>".$person_details['last_seen']."</textarea>
			<br>

			<label for=\"height\">Last Seen Clothing/Appearance</label>
			<textarea name=\"last_clothing\" id=\"last_clothing\" cols=\"56\" rows=\"4\" tabindex=\"".++$shn_tabindex."\" class=\"nonedit public\" READONLY>".$person_details['last_clothing']."</textarea>
			<br>

			<label for=\"height\">Other Comments</label>
			<textarea name=\"other_comments\" id=\"other_comments\" cols=\"56\" rows=\"4\" tabindex=\"".++$shn_tabindex."\" class=\"nonedit public\" READONLY>".$person_details['other_comments']."</textarea>
			<br>

			</fieldset></form></div>
		";
	}
	// PUBLIC VIEW END ////////////////////////////////////////////////////////////////////////////////////


	// NOTES BEGIN ////////////////////////////////////////////////////////////////////////////////////////
echo '<div style="clear:both;"></div>';
echo '<hr>';
	echo "
		<div class=\"form-container\">
		<form method=\"post\" id=\"notesPerson\" name=\"notesPerson\" action=\"edit?addNote&puuid=".urlencode($global['uuid'])."\">
		<fieldset id=\"editNotes\" style=\"min-width: 700px;\">
		<legend><span class=\"notesText\">Comments</span></legend>
	";

	$q  = "
		SELECT *
		FROM person_notes n, person_uuid p
		WHERE n.note_written_by_p_uuid = p.p_uuid
		AND note_about_p_uuid = '".$global['uuid']."'
		ORDER BY n.when ASC;
	";
	$r  = $global['db']->Execute($q);
	if($r === false) { daoErrorLog(__FILE__, __LINE__, __METHOD__, __CLASS__, __FUNCTION__, $global['db']->ErrorMsg(), "edit edit 28 ((".$q."))"); }

	while($row = $r->FetchRow()) {
		$delete = "";
		if($global['privateEdit'] || (isset($_SESSION['user_p_uuid']) && ($_SESSION['user_p_uuid'] == $row['note_written_by_p_uuid']))) {
			$delete = "&nbsp;&nbsp;&nbsp;<a style=\"color: red;\" href=\"index.php?mod=eap&act=deleteNote&noteId=".$row['note_id'].$debugParam."&uuid=".base64_encode($global['uuid'])."\">DELETE</a>";
		}

		$timezone = new DateTimeZone( "UTC" );
		$date = new DateTime();
		$date->setTimezone( $timezone );

		$date->setTimestamp(strtotime($row['when']));
		$utime =  $date->format('l, F j, Y g:i a T');

		$noteUrl = makeBaseUrl()."note.".$row['note_id'];
		$noteUrl = str_replace("https://", "", $noteUrl);
		$noteUrl = str_replace("http://", "", $noteUrl);

		if($row['suggested_status'] == null) {
			$ss = "";
		} else {
			$ss = "<br>Suggested Status: <b>";

			// HACK ~ fix this to use the DB to get these values!
			if($row['suggested_status'] == "ali") {
				$ss .= "Alive & Well";

			} elseif($row['suggested_status'] == "inj") {
				$ss .= "Injured";

			} elseif($row['suggested_status'] == "mis") {
				$ss .= "Missing";

			} elseif($row['suggested_status'] == "dec") {
				$ss .= "Deceased";

			} elseif($row['suggested_status'] == "fnd") {
				$ss .= "Found";

			} else {
				$ss .= "Unknown";
			}

			$ss .= "</b><br>";
		}

		echo "
			<p class=\"triangle-obtuse\">
				".htmlentities($row['note'])."<br>
				".$ss."
				<span class=\"noteFooter\">".$noteUrl."</span>
			</p>
			<div style=\"height: 25px;\">Posted by <b>".$row['full_name']."</b> on ".$utime."</div>
			<br>
			<br>
		";
	}

  // if user is allowed to add notes... show the tool for this
	if($global['addNote']) {
		echo "
			<div id=\"addYourOwnComment\">
				<b>Add your own comment...</b><br>
				<center>
					<textarea name=\"addNote\" id=\"addNote\" style=\"width: 95%; \" rows=\"5\" tabindex=\"".++$shn_tabindex."\" ></textarea><br>
					<input type=\"submit\" value=\"Save Comment\" class=\"styleTehButton\" tabindex=\"".++$shn_tabindex."\">
				</center>
			</div>
		";

	// if user is not allowed to add notes, tell them
	} else {
		echo "
			<div id=\"addYourOwnComment\">
				<b> &nbsp; To leave a note, you must first <a href=\"login\">login or register</a>.</b>
			</div>
		";
	}

	// close notes
	echo "
		<label for=\"height\">&nbsp;</label>
		</fieldset>
		</form></div>
	";
	// NOTES END ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////




	// LEGACY PFIF NOTES BEGIN //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  $pfifNoteCount = 0;
	$global['db']->EXECUTE("set names 'utf8'");
	$q = "
		SELECT note_record_id, entry_date, author_name, author_made_contact,
		REPLACE(status, '_', ' ') as status, last_known_location, text
		FROM pfif_note
		WHERE p_uuid = '".$global['uuid']."'
		ORDER BY entry_date DESC;
	";
	$result = $global['db']->Execute($q);
	if($r === false) { daoErrorLog(__FILE__, __LINE__, __METHOD__, __CLASS__, __FUNCTION__, $global['db']->ErrorMsg(), "edit edit 29 ((".$q."))"); }
	while (!$result == NULL && !$result->EOF) {

    $pfifNoteCount++;
    
    if($pfifNoteCount == 1) {
      echo '
        <div style="clear:both;"></div>
        <hr>
    		<div class="form-container">
    		<form method="post" id="notesPerson" name="notesPerson" action="edit?addNote&puuid='.urlencode($global['uuid']).'">
    		<fieldset id="editNotes">
    		<legend><span class="notesText">Legacy Comments</span></legend>
    	';
    }

		$timezone = new DateTimeZone( "UTC" );
		$date = new DateTime();
		$date->setTimezone( $timezone );

		$date->setTimestamp(strtotime($result->fields["entry_date"]));
		$utime =  $date->format('l, F j, Y g:i a T');

		echo "
				<b>Note:</b> ".$result->fields["text"]."<br>
		";

		if($result->fields["status"] == null) {
			$status = "unknown";
		} else {
			$status = $result->fields["status"];
		}
		echo "<b>Status:</b> ".$status."<br>";

		if($result->fields["author_made_contact"] == true) {
			echo "<b>FOUND</b><br>";
		}
		if(trim($result->fields["last_known_location"]) != "") {
			echo "<b>Last Known Location: </b>".$result->fields["last_known_location"]."<br>";
		}

		echo "
			<b>Comment Author:</b> ".$result->fields["author_name"]."</b> on ".$utime."<br>
		";

		/*
		'note_id'=>$result->fields["note_record_id"],
		'date'=>$result->fields["entry_date"],
		'author'=>$result->fields["author_name"],
		'found'=>$result->fields["found"],
		'status'=>$result->fields["status"],
		'lastSeen'=>$result->fields["last_known_location"],
		'text'=>$result->fields["text"]
		*/
		$result->MoveNext();
	}
  if($pfifNoteCount > 0) {
    echo "
      <label for=\"height\">&nbsp;</label>
      </fieldset>
      </form></div>
	  ";
  }
	// EXTERNAL PFIF END






	// LOAD PRIVATE DATA //////////////////////////////////////////////////////////////////////////////////
	if($global['privateEdit']) {

		$address = "";
		$zip     = "";
		$phone   = "";
		$mobile  = "";
		$email   = "";

		$q  = "
			SELECT *
			FROM contact
			WHERE p_uuid = '".$global['uuid']."';
		";
		$r   = $global['db']->Execute($q);
		if($r === false) { daoErrorLog(__FILE__, __LINE__, __METHOD__, __CLASS__, __FUNCTION__, $global['db']->ErrorMsg(), "edit edit 30 ((".$q."))"); }
		while ($row = $r->FetchRow()) {
			$type  = $row['opt_contact_type'];
			$value = $row['contact_value'];
			if($type == "home") {
				$address = $value;
			} else if($type == "zip") {
				$zip = $value;
			} else if($type == "curr") {
				$phone = $value;
			} else if($type == "cmob") {
				$mobile = $value;
			} else if($type == "email") {
				$email = $value;
			}
		}
	// LOAD PRIVATE DATA END ///////////////////////////////////////////////////////////////////////////


	// PRIVATE EDIT ////////////////////////////////////////////////////////////////////////////////////
		if((count($_POST) > 0) && isset($_POST['opt_race'])) {
			$editAddress    = trim($_POST['address']);
			$editZip        = trim($_POST['zip']);
			$editPhone      = trim($_POST['phone']);
			$editCell       = trim($_POST['mobile']);
			$editEmail      = trim($_POST['email']);
			$editReligion   = trim($_POST['opt_religion']);
			$editRace       = trim($_POST['opt_race']);

			if($editAddress  == '') { $editAddress  = null; }
			if($editZip      == '') { $editZip      = null; }
			if($editPhone    == '') { $editPhone    = null; }
			if($editCell     == '') { $editCell     = null; }
			if($editEmail    == '') { $editEmail    = null; }
			if($editReligion == '') { $editReligion = null; }
			if($editRace     == '') { $editRace     = null; }

			if($editAddress   != $address)                        { saveContactRevision("home",  "Address",  $address, $editAddress); }
			if($editZip       != $zip)                            { saveContactRevision("zip",   "Zip Code", $zip,     $editZip);     }
			if($editPhone     != $phone)                          { saveContactRevision("curr",  "Phone",    $phone,   $editPhone);   }
			if($editCell      != $mobile)                         { saveContactRevision("cmob",  "Mobile",   $mobile,  $editCell);    }
			if($editEmail     != $email)                          { saveContactRevision("email", "Email",    $email,   $editEmail)  ; }
			if($editReligion  != $person_details['opt_religion']) { saveRevision("person_details", "opt_religion", "Religion",  $person_details['opt_religion'], $editReligion); }
			if($editRace      != $person_details['opt_race'])     { saveRevision("person_details", "opt_race",     "Race",      $person_details['opt_race'],     $editRace);     }

		} else {
			$editAddress    = $address;
			$editZip        = $zip;
			$editPhone      = $phone;
			$editCell       = $mobile;
			$editEmail      = $email;
			$editReligion   = $person_details['opt_religion'];
			$editRace       = $person_details['opt_race'];
		}
echo '<div style="clear:both;"></div>';
echo '<hr>';
		echo "
			<div class=\"form-container\">
			<form method=\"post\" id=\"editPrivateForm\" name=\"editPrivateForm\" action=\"edit?puuid=".urlencode($global['uuid'])."\">
			<fieldset id=\"editPrivate\">
			<legend><span class=\"privateText\">Private Contact Information (not visible to the public)</span></legend>
			<label for=\"address\">Street Address</label>
			<textarea name=\"address\" id=\"address\" cols=\"80\" rows=\"4\" tabindex=\"".++$shn_tabindex."\" class=\"public\">".$editAddress."</textarea>
			<br>
		";

		shn_form_text(    "Zip/Postal Code",          "zip",    'size="20"', array('value'=>$editZip  ));
		shn_form_text(    "Home Phone Number",        "phone",  'size="20"', array('value'=>$editPhone));
		shn_form_text(    "Cell/Mobile Phone Number", "mobile", 'size="20"', array('value'=>$editCell ));
		shn_form_text(    "Email Address",            "email",  'size="20"', array('value'=>$editEmail));

		shn_form_opt_select("opt_race", "Race ",         null, array('sort'=>'display_order, option_description ASC', 'value'=>$editRace    ));
		shn_form_opt_select("opt_religion", "Religion ", null, array('sort'=>'display_order, option_description ASC', 'value'=>$editReligion));

		echo "
			<br>
			<label for=\"height\">&nbsp;</label>
			<input type=\"submit\" value=\"Save\" class=\"styleTehButton\" tabindex=\"".++$shn_tabindex."\">

			</fieldset>
			</form></div>
		";
	}
	// PRIVATE EDIT END ////////////////////////////////////////////////////////////////////////////////


	// REVISIONS START//////////////////////////////////////////////////////////////////////////////////
	if($global['privateEdit']) {
echo '<div style="clear:both;"></div>';
echo '<hr>';
		echo "
			<div class=\"form-container\">
			<form id=\"editRevisionsForm\" name=\"editRevisionsForm\">
			<fieldset id=\"editPrivate\">
			<legend><span class=\"revisionsText\">Revisions to this Record</span></legend>
		";

		$out = "<table>
			<tr>
				<td style=\"text-align: left; border: none; font-weight: bold;\" class=\"odder\">Revision by (username)</td>
				<td style=\"text-align: left; border: none; font-weight: bold;\" class=\"odder\">Updated When</td>
				<td style=\"text-align: left; border: none; font-weight: bold;\" class=\"odder\">Field Changed</td>
				<td style=\"text-align: left; border: none; font-weight: bold;\" class=\"odder\">Old Value</td>
				<td style=\"text-align: left; border: none; font-weight: bold;\" class=\"odder\"> &nbsp; </td>
				<td style=\"text-align: left; border: none; font-weight: bold;\" class=\"odder\">New Value</td>


			</tr>
		";

		$q  = "
			SELECT *
			FROM person_updates u, person_uuid p, users uu
			WHERE u.p_uuid = '".$global['uuid']."'
			AND p.p_uuid = u.updated_by_p_uuid
			AND p.p_uuid = uu.p_uuid
			ORDER BY update_time DESC, updated_column DESC;
		";
		$r  = $global['db']->Execute($q);
		if($r === false) { daoErrorLog(__FILE__, __LINE__, __METHOD__, __CLASS__, __FUNCTION__, $global['db']->ErrorMsg(), "edit edit 31 ((".$q."))"); }
		$count = 0;
		while($row = $r->FetchRow()) {
			if(($count % 2) == 0) {
				$class = "evener";
			} else {
				$class = "odder";
			}
			$out .= "
				<tr>
					<td style=\"text-align: left; border: none;\" class=\"".$class."\"><b>".$row['full_name']."</b> (".$row['user_name'].")</td>
					<td style=\"text-align: left; border: none;\" class=\"".$class."\">".$row['update_time']."</td>
					<td style=\"text-align: left; border: none;\" class=\"".$class."\">".$row['updated_table']." :: ".$row['updated_column']."</td>
					<td style=\"text-align: left; border: none;\" class=\"".$class."\">".$row['old_value']."</td>
					<td style=\"text-align: left; border: none;\" class=\"".$class."\">=></td>
					<td style=\"text-align: left; border: none;\" class=\"".$class."\">".$row['new_value']."</td>

				</tr>
			";
			$count++;
		}
		$out .= "</table>";
		if($count == 0) {
			echo "<b>This record has no revisions.</b>";
		} else {
			echo $out;
		}
		echo "
			</fieldset>
			</form></div>
		";
	}
	// REVISIONS END ///////////////////////////////////////////////////////////////////////////////////


	// FOLLOW //////////////////////////////////////////////////////////////////////////////////////////
echo '<div style="clear:both;"></div>';
echo '<hr>';
	echo "<div id=\"followDiv\">";
	if($global['startFollow'] && $global['role'] != "reporter") {

		$q  = "
			SELECT COUNT(*)
			FROM person_followers
			WHERE follower_p_uuid = '".$_SESSION['user_p_uuid']."'
			AND p_uuid = '".$global['uuid']."';
		";
		$r  = $global['db']->Execute($q);
		if($r === false) { daoErrorLog(__FILE__, __LINE__, __METHOD__, __CLASS__, __FUNCTION__, $global['db']->ErrorMsg(), "edit edit 32 ((".$q."))"); }
		$row = $r->FetchRow();
		$alreadyFollowing = (bool)$row['COUNT(*)']; // true if logged in user is already following the person

		if(!$alreadyFollowing) {
			echo "
				<script>
					function followPersonCall() {
						var fP = document.getElementById('followPerson');
						fP.submit();
					}
				</script>
				<div class=\"form-container\">
				<form method=\"post\" id=\"followPerson\" name=\"followPerson\" action=\"edit?follow&puuid=".urlencode($global['uuid'])."\">
				<fieldset id=\"editFollow\">
				<legend><span class=\"followText\">Follow / Share</span></legend>
				To receive email alerts when this record is updated with new information, you can elect to follow this person. You will only receive an email when the reporter or staff change any of the information in this person's record or when a user of the site leaves a note on this page.<br>
				<br>
				<input id=\"followButton\" class=\"styleTehButton\" type=\"button\" value=\"Click here to begin following this person's record via email\" onclick=\"followPersonCall();\"><br>
				<br>
				Be aware that at any time you may visit your <a href=\"pref?following\">People I'm Following</a> page to manage which people you are following.
				</fieldset>
				</form>
				</div>
			";
		} else {
			echo "
				<script>
					function unFollowPersonCall() {
						var ufP = document.getElementById('unFollowPerson');
						ufP.submit();
					}
				</script>
				<div class=\"form-container\">
				<form method=\"post\" id=\"unFollowPerson\" name=\"unFollowPerson\" action=\"edit?unfollow&puuid=".urlencode($global['uuid'])."\">
				<fieldset id=\"editFollow\">
				<legend><span class=\"followText\">Follow</span></legend>
				To stop receiving email alerts when this record is updated with new information or notes, you can elect to unfollow this person.<br>
				<br>
				<input id=\"unFollowButton\" class=\"styleTehButton\" type=\"button\" value=\"Click here to STOP following this person's record via email\" onclick=\"unFollowPersonCall();\"><br>
				<br>
				</fieldset>
				</form>
				</div>
			";
		}
	}

	// if not allowed to follow, then we ask the user to first login/signup
	if(!$global['startFollow'] && $global['role'] != "reporter") {
		echo "
			<div class=\"form-container\">
			<form method=\"post\" id=\"followPerson\" name=\"followPerson\">
			<fieldset id=\"editFollow\">
			<legend><span class=\"followText\">Follow / Share</span></legend>
			To receive email alerts when this record is updated with new information, you can elect to follow this person. You will only receive an email when the reporter or staff change any of the information in this person's record or when a user of the site leaves a note on this page.<br>
			<br>
			<b> &nbsp; To follow this person, you must first <a href=\"login\">login or register</a>.</b>
			</fieldset>
			</form>
			</div>
		";
	}
	echo "</div>";
	// FOLLOW END ///////////////////////////////////////////////////////////////////////////////


	// DEBUG ////////////////////////////////////////////////////////////////////////////////////
	// only allow debugging when enabled and only by the root user....
	if($debug && $_SESSION['user_p_uuid'] == "1") {
		echo "<div class=\"form-container\" style=\"overflow: hidden;\"><form><fieldset>";
		echo "<legend>debug</legend>";
		echo "<pre>";
		echo "user_id.....(".$_SESSION['user_id'].       ")<br>";
		echo "uuid........(".$global['uuid'].            ")<br>";
		echo "publicView..(".(int)$global['publicView']. ")<br>";
		echo "publicEdit..(".(int)$global['publicEdit']. ")<br>";
		echo "privateView.(".(int)$global['privateView'].")<br>";
		echo "privateEdit.(".(int)$global['privateEdit'].")<br>";
		echo "addNote.....(".(int)$global['addNote'].    ")<br>";
		echo "startFollow.(".(int)$global['startFollow'].")<br>";
		echo "role........(".$global['role']. ")<br>";
		echo "<br>";
		echo $googl->result()." --> ".makePageUrl()."<br><br>";
		echo "\$_SESSION[] = ".print_r($_SESSION, true)."\n\n";
		echo "\$_GET[] = ".print_r($_GET, true)."\n\n";
		echo "\$_POST[] = ".print_r($_POST, true)."\n\n";
		echo "\$global[] = ".print_r($global, true)."\n\n";
		echo "</pre>";
		echo "</fieldset></form></div>";
	}
	// DEBUG END //////////////////////////////////////////////////////////////////////////////////

	echo '
			<div>
				<g:plusone size="medium" annotation="none"></g:plusone>
				<a id="tweet" href="https://twitter.com/share" class="twitter-share-button" data-size="small" data-lang="en" data-count="none"></a>
				<div class="fb-send" data-href="http://example.com" data-font="verdana"></div><div id="fb-root"></div>
			</div>
			<br>
			<script>

// g+ button
(function() {
	var po = document.createElement("script"); po.type = "text/javascript";
	po.async = true;
	po.src = "https://apis.google.com/js/plusone.js";
	var s = document.getElementsByTagName("script")[0];
	s.parentNode.insertBefore(po, s);
})();

// twitter button
!function(d,s,id){
	var js,fjs=d.getElementsByTagName(s)[0];
	if(!d.getElementById(id)){
		js=d.createElement(s);
		js.id=id;
		js.src="//platform.twitter.com/widgets.js";
		fjs.parentNode.insertBefore(js,fjs);
	}
}(document,"script","twitter-wjs");

// facebook button
(function(d, s, id) {
  var js, fjs = d.getElementsByTagName(s)[0];
  if (d.getElementById(id)) return;
  js = d.createElement(s); js.id = id;
  js.src = "//connect.facebook.net/en_US/all.js#xfbml=1";
  fjs.parentNode.insertBefore(js, fjs);
}(document, "script", "facebook-jssdk"));

			</script>
	';


	emailUpdates();

	// make pfif note on new records (for the first save)
	if($global['makeStaticPfifNote'] == true) {

		require_once($global['approot']."inc/lib_uuid.inc");
		require_once($global['approot']."mod/pfif/pfif.inc");
		require_once($global['approot']."mod/pfif/util.inc");

		$p = new Pfif();

		$n = new Pfif_Note();
		$n->note_record_id          = shn_create_uuid('pfif_note');
		$n->person_record_id        = $global['uuid'];
		$n->linked_person_record_id = null;
		$n->source_date             = $person_status['last_updated']; // since we are now creating the note,
		$n->entry_date              = $person_status['last_updated']; // we use the last_updated for both values
		$n->author_phone            = null;
		$n->email_of_found_person   = null;
		$n->phone_of_found_person   = null;
		$n->last_known_location     = $person_details['last_seen'];
		$n->text                    = $person_details['other_comments'];
		$n->author_made_contact     = null; // we have no way to know if the reporter had direct contact (hence we leave this null)

		// we map found = true for the cases when we know a person is alive (PL-318)
		if($person_status['opt_status'] == "ali") {
			$n->author_made_contact = true;
		}

		// figure out the person's pfif status
		$n->status = shn_map_status_to_pfif($person_status['opt_status']);

		// find author name and email...
		$q = "
			SELECT *
			FROM contact c, person_uuid p
			WHERE p.p_uuid = c.p_uuid
			AND c.opt_contact_type = 'email'
			AND p.p_uuid = '".$_SESSION['user_p_uuid']."';
		";
		$result = $global['db']->Execute($q);
		if($result === false) { daoErrorLog(__FILE__, __LINE__, __METHOD__, __CLASS__, __FUNCTION__, $global['db']->ErrorMsg(), "edit make static pfif note ((".$q."))"); }

		if($result != NULL && !$result->EOF) {
			$n->author_name  = $result->fields['full_name'];
			$n->author_email = $result->fields['contact_value'];
		} else {
			$n->author_name  = null;
			$n->author_email = null;
		}

		$p->setNote($n);
		$p->setIncidentId($person_incident['incident_id']);
		$p->storeNotesInDatabase();
	}
}




