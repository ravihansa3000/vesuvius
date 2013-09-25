<?php

/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

/**
 * Description of main
 *
 * @name    Native Synchronization
 * @package sync
 * @author  Gurutharshan Nadarajah <gurutharshan@gmail.com>
 */

//include_once ($global['approot'].'/inc/lib_menu.inc');
//include_once ($global['approot'].'/inc/lib_errors.inc');
//require_once ($global['approot'].'/inc/handler_db.inc');
//include_once ($global['approot'].'/inc/lib_xajax.inc');

include_once ($global['approot'].'/mod/sync/SyncConfDB.php');

require_once($global['approot']."mod/lpf/class.person.php"); 
require_once($global['approot']."mod/lpf/class.syncPerson.php"); 
require_once($global['approot']."mod/lpf/class.personEdxl.php");
require_once($global['approot']."mod/lpf/class.personImage.php");
require_once($global['approot']."mod/lpf/class.personImageTag.php");
require_once($global['approot'].'3rd/adodb/adodb.inc.php');
require_once($global['approot'].'3rd/nusoap/lib/nusoap.php');
    
/**
 * Check Credz
 *
 * @access public
 * @return void
 */
function shn_sync_checkCredz() {
	$good = false;
	if(isset($_SESSION['logged_in']) && $_SESSION['logged_in'] == 1) {
		if(isset($_SESSION['group_id']) && $_SESSION['group_id'] != 3) { // not anonymous
			$good = true; // allowed to report
		}
	}
	return $good;
}

function shn_sync_default() {
    if(shn_sync_checkCredz()) {
//        __synchronize(32);
		echo '<a href="index.php?mod=sync&act=new"><span class="styleTehButton">Create new synchronization</span></a> <br /> <br />';
        shn_sync_view();
	}
}

/**
 * This is the controller
 * @global type $global
 * @return type
 */
function shn_sync_controller() {
	global $global;

	//Controller
	$case = isset($_REQUEST['seq']) ? $_REQUEST['seq'] : null;
	switch($case) {
		case 'new' :
			if (shn_sync_validate()) {
				//clean the $_POST to remove the conflicts
				$_POST = null;
				// saving and sync_req part
                $confID = shn_conf_establish($_REQUEST['incident_short'], $_REQUEST['instance_uuid'], $_REQUEST['mode']);
//                testEmail();   
			} else {
				shn_sync_new();
			}
		break;
    
        case 'show' :
            shn_sync_show();
        break;
        
        case 'sync_update' :
            shn_sync_update();
        break;
        
        case 'sync_start' :
            __synchronize($_REQUEST['confID']);
        break;
        
		default :
			shn_sync_default();
		break;
	}
}

function testEmail() {
    global $conf;
    global $global;

//    $toEmail  = "gurutharshan@gmail.com";
//    $toName  = "Guru";
    
    $q = "
                SELECT  p.p_uuid, p.full_name, c.contact_value
                FROM    sys_user_to_group s, person_uuid p, contact c
                WHERE   p.p_uuid = s.p_uuid
                AND     s.p_uuid = c.p_uuid
                AND     s.group_id = 1
                AND     c.opt_contact_type = 'email'
                ";                                                      // GROUP_ID - 1: admin contact details
    $result = $global['db']->Execute($q);
    $toEmail = $result->fields['contact_value'];
    $toName = $result->fields['full_name'];
        
    $p = new pop();
    $link = "WWW-tst";
    $subject = "Please confirm the registration of your new PL user account";
    $bodyHTML =
            "Thank you for registering.<br><br>"
            . "You <b>must</b> click on this link in order for the registration process to be completed:<br><a href=\"" . $link . "\">" . $link . "</a><br><br>"
            . "Once completed, your account will be active allowing you to login the Person Locator web site and other devices which utilize its web services.<br><br><br>"
            . "<br><img src=\"http://goo.gl/BqfCv\">";
    $bodyAlt =
            "Thank you for registering.\n\n"
            . "You must click on this link in order for the registration process to be completed:\n" . $link . "\n"
            . "Once completed, your account will be active allowing you to login the Person Locator web site and other devices which utilize its web services.\n\n\n"
            . "- " . $conf['site_name'];
    $p->sendMessage($toEmail, $toName, $subject, $bodyHTML, $bodyAlt);
}

/**
 * This is the Synchronization form.
 * 
 */
function shn_sync_new() {
    $incident_shortName = $_REQUEST["shortname"];
    shn_form_fopen("controller", null, array('enctype'=>'enctype="multipart/form-data"', 'req_message' => true));
        shn_form_hidden(array('seq'=>'new'));
        shn_form_fsopen(_t('Native Synchronization Details'));
            ?>
            <p><?php echo _t('Please enter synchronization details to configure Native Sync with another vesuvius instance:'); ?></p>    
            <?php
            shn_form_hidden(array('incident_short'=> $incident_shortName));
            shn_form_text(_t('Instance UUID '), 'instance_uuid', 'size="50"', array('req' => true, 'help' => 
                'Enter instance UUID which is going to be synched all the missing/found person details for '.$incident_shortName.' incident ', 'public' => true));
//            shn_form_radio2(array("master" => "Master", "slave" => "Slave"), "Role ", "role", "master");
            shn_form_radio2(array("two_way" => "Two way", "one_way" => "One way"), "Mode ", "mode", "two_way");
        shn_form_fsclose();        
        shn_form_submit("Next", "class=\"styleTehButton\"");
    shn_form_fclose();
}

/**
 * show all the sync confs details
 * @global type $global
 * @global type $conf
 */
function shn_sync_view() {
    global $global;
    global $conf;
    // show all ///////////////////////////////////////////////////////////////////
    echo "
		<div class=\"form-container\"><form><fieldset>
		<legend>" . "Configured Synchronization </legend>
	";

    $q = "
        SELECT  *
        FROM    sync_conf;
		";
    $r = $global['db']->Execute($q);

    $count = 0;
    while ($row = $r->FetchRow()) {
        if ($count == 0) {
            echo "
				<table id=\"repLog\">
				<tr>
				<td class=\"noBorderLeft evener\" >Incident Name</td>
				<td class=\"noBorderLeft evener\" >Instance UUID</td>
				<td class=\"noBorderLeft evener\" >Mode</td>
				<td class=\"noBorderLeft evener\" >Role</td>
				<td class=\"noBorderLeft evener\" >Status</td>
				</tr>
			";
        }
        if ($count % 2 == 0) {
            $class = "odder";
        } else {
            $class = "evener";
        }
        echo "
			<tr>
			<td style=\"width: 150px;\" class=\"noBorderLeft " . $class . "\" >
                <a href=\""."index.php?mod=sync&act=show&id=".$row['id']."\" >" //".$row['id']."
                . $row['incident_name'] . "</a></td>
			<td class=\"noBorderLeft " . $class . "\" >" 
                . $row['instance_uuid'] . "</td>
			<td class=\"noBorderLeft " . $class . "\" >" 
                . $row['mode'] . "</td>
			<td class=\"noBorderLeft " . $class . "\" >" 
                . $row['role'] . "</td>
			<td class=\"noBorderLeft " . $class . "\" >" 
                . $row['status'] . "</td>
			</tr>
		";
        $count++;
    }
    if ($count == 0) {
        echo "<tr><td class=\"noBorderLeft\" colspan=3>No Synchronization configured</td></tr>";
    }
    echo "</table>";
    echo "</fieldset>";
    echo "</form></div><br>";
}

/**
 * Show more details about configuration parameters and able to change.
 */
function shn_sync_show() {
    $syncConfDB = new SyncConfDB();
    $syncConfDB->id = $_REQUEST['id'];
    $syncConfDB->load();
    
    $action = FALSE;
    shn_form_fopen("controller", null, array('enctype'=>'enctype="multipart/form-data"', 'req_message' => true));
        
        shn_form_hidden(array('confID'=>$syncConfDB->id));
        shn_form_fsopen(_t('Edit Native sync configurations'));
            ?>
            <p><?php echo _t('First slave instance can only able to accept the sync request:'); ?></p>    
            <p><?php echo _t('Once sync established master/slave can able to change status:'); ?></p>  
            <?php if(($syncConfDB->role == "master") && ($syncConfDB->status == "reg")) { ?>
                <p><b class="red"> * </b> <?php echo _t("Waiting for slave's response.."); ?></p>
            <?php } ?>
            <?php
//            shn_form_hidden(array('incident_short'=> $incident_shortName));
            shn_form_text(_t('Instance UUID '), 'instance_uuid', 'size="50"; disabled=disabled;', array('value' => $syncConfDB->instance_uuid));
            shn_form_text(_t('Incident Name'), 'incident_name', 'size="50"; disabled=disabled;', array('value' => $syncConfDB->incident_name));
            shn_form_text(_t('Mode'), 'mode', 'size="50"; disabled=disabled;', array('value' => $syncConfDB->mode));
            shn_form_text(_t('Role'), "role", 'size="50"; disabled=disabled', array('value' => $syncConfDB->role));
            if(($syncConfDB->role == "slave") && ($syncConfDB->status == "reg")) {
                $action = TRUE;
                shn_form_radio2(array("reg" => "Registered", "ack" => "Acknowledged"), "Status ", "status", $syncConfDB->status);
            }
            else 
                shn_form_text(_t('Status'), "status", 'size="50"; disabled=disabled', array('value' => $syncConfDB->status));
        shn_form_fsclose();        
        if($action) {
            shn_form_hidden(array('seq'=>'sync_update'));
            shn_form_submit("Next", "class=\"styleTehButton\"");
        } elseif (($syncConfDB->role == "master") && ($syncConfDB->status == "ack")) {
            shn_form_hidden(array('seq'=>'sync_start'));
            shn_form_submit("Start Sync", "class=\"styleTehButton\"");
        }
    shn_form_fclose();
    
}

/**
 * Updates the sync details -- status
 */
function shn_sync_update() {
    global $global;

    $syncConfDB = new SyncConfDB();
    $syncConfDB->id = $_REQUEST['confID'];
    $syncConfDB->load();
    $res_stat;
    if (($syncConfDB->role == "slave") && ($syncConfDB->status == "reg")) {
        if ($_REQUEST['status'] == "ack") {
            $syncConfDB->status = $_REQUEST['status'];
            $syncConfDB->preshared_key .= $syncConfDB->id; //while reg we add master id+## and now we add slave confID
            $syncConfDB->save();

            //send ack soap call to master
            $client = new nusoap_client($syncConfDB->instance_uuid);
            $x = file_get_contents($global['approot'] . "mod/plus/reference_REUNITE4.xml");
            $syncReq = $client->call('sync_ack', array(
                'preshared_key' => $syncConfDB->preshared_key,
                'status'        => $syncConfDB->status,
                'activated_time'=> date('Y-m-d H:i:s'))
            );
            $res_stat = json_decode($syncReq['respond']);
        }
    }
    
    if($res_stat != NULL) {
        add_confirmation($res_stat);
    } else {
        add_error("Failed from master response..");
    }
    shn_sync_default();
}

/**
 * This is the new sync creation Validation function
 * @access public
 * @return void
 */
function shn_sync_validate() {
	global $global;
	global $conf;
	require_once($global['approot']."/inc/lib_validate.inc");

	$error_flag = false;

	//clean the post -- trim them all
	foreach($_POST as $k => $v) {
		$v = trim($v);
		if($v != '') {
			$local_post[$k] = $v;
		}
	}

	$_SESSION['sync']['new'] = $local_post;
	if(!isset($_SESSION['sync']['new']['p_uuid'])) {
		$_SESSION['sync']['new']['p_uuid'] = shn_create_uuid();
	}

	// anything entered?
	if(empty($local_post)) {
		add_error(_t("Please fill out the form with as much information as possible about the person you are reporting."));
		return false;
	}

	// UUID entered?
	if(!isset($local_post['instance_uuid'])) {
		add_error(_t("Please enter instance UUID to sync configuration."));
		$error_flag = true;
	} else {
        // check whether already created or Not
        if(isAlreadyConfigured($local_post['instance_uuid'])) {
            add_error(_t("Entered instance uuid was already configured to sync."));
//              $error_flag = true;
        }
    }

	if($error_flag) {
		return false;
	} else {
		// set session
		return true;
	}
}

/**
 * Check whether does this foreign uuid is already configured or not.
 * @param type $foreign_uuid
 * @return boolean
 */
function isAlreadyConfigured($foreign_uuid) {
    $syncConfDB = new SyncConfDB();
    $configured = $syncConfDB->isInstanceConfigured($foreign_uuid);
    return $configured;
}

/**
 * Creating local configuration for synchronization.
 * @param type $incident_short
 * @param type $instanceUUID
 * @param type $mode
 */
function shn_conf_establish($incident_short, $instanceUUID, $mode) {
    global $global;
    global $conf;
    
    // configuration saving..
    $q = "
        SELECT `incident_id` , `name`
        FROM `incident`
        WHERE `shortname` = '".mysql_real_escape_string($incident_short)."'
        ";
    $incident     = $global['db']->Execute($q);
    $incidentId   = $incident->fields['incident_id'];
    $incidentName = $incident->fields['name'];
    
    $syncConfDB = new SyncConfDB();
    $syncConfDB->created_time   = date('Y-m-d H:i:s');
    $syncConfDB->role           = "master";
    $syncConfDB->incident_name  = $incidentName;
    $syncConfDB->incident_id    = $incidentId;
    $syncConfDB->instance_uuid  = $instanceUUID;  
    $syncConfDB->preshared_key  = "";
    $syncConfDB->mode           = $mode;
    $syncConfDB->status         = "reg";    //indicates that it's in registration state OR waiting for slave instance response
    
    $confID = $syncConfDB->save();
    
    // soap call for sync request
    $client = new nusoap_client($instanceUUID);
    $x = file_get_contents($global['approot'] . "mod/plus/reference_REUNITE4.xml");
    $syncReq = $client->call('sync_req', array(
        'incidentName'  => $syncConfDB->incident_name,
        'instanceUUID'  => $conf['wsdl'],
        'mode'          => $syncConfDB->mode,
        'role'          => "slave",
        'other_confID'  => (string)$confID,
        'status'        => $syncConfDB->status,
        'created_time'  => $syncConfDB->created_time)
        );
    $res_stat = json_decode($syncReq['req_status']);
    $res_msg  = json_decode($syncReq['message']);
    
//    if($res_stat == "SUCCESS") {
//        add_confirmation($res_msg);
//    } else {
//        add_error("Failed from slave ". $res_msg);
//    }    
    shn_sync_default();
}

/**
 * 
 * @global type $global
 * @param type $sync_conf_id
 */
function __synchronize($sync_conf_id) {
    global $global;
    
//    $sync_conf_id = 1;
    $syncConfDB = new SyncConfDB();
    $syncConfDB->id = $sync_conf_id;
    $syncConfDB->load();
    
    if(($syncConfDB->status == "ack") && ($syncConfDB->role == "master")) {
        $localIncidentId= $syncConfDB->incident_id;
        $preshared_key  = $syncConfDB->preshared_key;
        $syncMode       = $syncConfDB->mode;
        $slave_wsdl     = $syncConfDB->instance_uuid;
        /** Last sync date-time. */
        $lastSyncDate = '2012-01-01 00:00:00';
        $q = "
            SELECT  MAX(`update_time`) AS uTime
            FROM    `sync_updates`
            WHERE   `conf_id` = '" . mysql_real_escape_string($sync_conf_id) . "'
        ";
        $res = $global['db']->Execute($q);
        if ($res->fields['uTime'] != NULL){
            $lastSyncDate = $res->fields['uTime'];
        }            

        $currentSyncDateTime = date('Y-m-d H:i:s');
        $client = new nusoap_client($slave_wsdl);
        $x = file_get_contents($global['approot'] . "mod/plus/reference_REUNITE4.xml");
        $pullResult = $client->call('pull', array('presharedKey' => $preshared_key, 'lastSyncDate' => $lastSyncDate));
        
        $pulledTime = json_decode($pullResult['pulledTime']);
        $lastPushed = json_decode($pullResult['lastPushed']);
        
//        echo "
//            <h2>wsdl: " . $slave_wsdl . "</h2>
//            <pre>" . var_export($pullResult, true) . "</pre> 
//        ";

        $skipForPush = "";
        $updatesForPush = array();
        /* Resolve conflicts for each person. */
        $slave_updates = json_decode($pullResult['updates']);
        foreach ($slave_updates as $serP) {
            $slave_person = unserialize($serP);
            $slave_person->postSerialize($global['db']);
            /* Remove redundundant data on update_history */
            $slave_person->removeUpdateRedundancy();
            $local = new syncPerson();
            $local->p_uuid = $slave_person->p_uuid;
            $local->synced_instance = $syncConfDB->preshared_key;
            $local->last_sync = $lastSyncDate;
            $local->loadWithUpdates();
            $local->removeUpdateRedundancy();

            /**
             * conflicts resolving algo 
             * loop all the elements in slave's update history
             *  checks with local update elements
             *  if nothing same table & elem
             *      then just add new values to person obj and save
             *  else if any same t&e found 
             *      if local greater date
             *          then dont do anything
             *      else 
             *          add slave's value to person obj & save.. also delete from local array which is gonna push to slave later 
             *  
             *  
             */
            foreach ($slave_person->update_history as $s_up) {
                $matched = FALSE;
                foreach ($local->update_history as $key => $l_up) {
                    if (($s_up['updated_table'] == $l_up['updated_table']) && ($s_up['updated_column'] == $l_up['updated_column'])) {
                        if ($s_up['update_time'] > $l_up['update_time']) {
                            //set slave value to person based on recent update...  
                            $local->setAttr($s_up['updated_column'], $s_up['new_value']);
                            array_splice($local->update_history, $key, 1);
                        }
                        $matched = TRUE;
                        break;
                    }
                }
                //set slave value if there is no matched
                if (!$matched)
                    $local->setAttr($s_up['updated_column'], $s_up['new_value']);
            }

            // Add relavent local objects to push data...
            if (sizeof($local->update_history) > 0) {
                $local->prepareSerialize();
                array_push($updatesForPush, serialize($local));
            }
            // collect local p_uuid to avoid while fetching for push...
            $p_uuid = $local->p_uuid . ", ";
            $skipForPush .= $p_uuid;

            $local->update();            //update the content
        }

        if ($syncMode == "two_way") {
            /** 
            * For push Newly created person records from last sync. 
            * This record taken before save from slave to avoid similar data gathering.
            */            
            $q = "
            SELECT  p.`p_uuid`
            FROM    `person_uuid` p, `person_status` s
            WHERE   p.`incident_id`='" . mysql_real_escape_string($localIncidentId) . "'
            AND     p.`p_uuid` = s.`p_uuid`
            AND     s.`creation_time` > '" . mysql_real_escape_string($lastPushed) . "'
            ";
            //    $sql = "SELECT * from person_uuid where incident_id='".mysql_real_escape_string($localIncidentId)."'";
            $result = $global['db']->Execute($q);
            $newRecordList = array();
            while (!$result == NULL && !$result->EOF) {
                $syncP = new syncPerson();
                $syncP->p_uuid = $result->fields['p_uuid'];
                $syncP->load();
                $syncP->prepareSerialize();
                $newRecordList[] = serialize($syncP);
                $result->MoveNext();
            }
            
            /**
             * Fetch other (other than conflicts) local updates to send push request.
             * query with exception from already resolved p_uuid 
             */
            $q = "
                SELECT  p.`p_uuid`
                FROM    `person_uuid` p, `person_status` s
                WHERE   p.`incident_id`='" . mysql_real_escape_string($localIncidentId) . "'
                AND     p.`p_uuid` = s.`p_uuid`
                AND     s.`last_updated` > '" . mysql_real_escape_string($lastPushed) . "'
                AND     s.`creation_time` < '" . mysql_real_escape_string($lastPushed) . "'
                ";
            if($skipForPush != NULL) 
                $q .= "NOT IN  ('" . $skipForPush . "')";
            $q .= "ORDER BY last_updated DESC";
            
            $result = $global['db']->Execute($q);
            while (!$result == NULL && !$result->EOF) {
                $syncP = new syncPerson();
                $syncP->p_uuid = $result->fields['p_uuid'];
                $syncP->loadWithUpdates();
                $syncP->prepareSerialize();
                array_push($updatesForPush, serialize($syncP));
                $result->MoveNext();
            }

//            echo "
//                <h2> Push dataset </h2>
//                 New: "     ."<pre>" . var_export($newRecordList, true) . "</pre> 
//                 Updates: " ."<pre>" . var_export($updatesForPush, true)."</pre> 
//            ";
            
            /** push request to slave... */
            $pushResult = $client->call('push', array(
                'new'           => (string) json_encode($newRecordList),
                'updates'       => (string) json_encode($updatesForPush),
                'pushedTime'  => (string) json_encode($currentSyncDateTime),
                'presharedKey'  => (string) json_encode($preshared_key),
            ));
            $msg = json_decode($pushResult['message']);
        }
                
        // Add new records from slave
        __addNewRecords(json_decode($pullResult['new']));

        /**
         * update to the sync into sync_updates table.
         */
        $q = "INSERT INTO `sync_updates` (`id`, `conf_id`, `update_time`)
                VALUES (NULL , '".$sync_conf_id."', '".$pulledTime."');";
        $result = $global['db']->Execute($q);
        if ($result === false) {
            daoErrorLog(__FILE__, __LINE__, __METHOD__, __CLASS__, __FUNCTION__, $global['db']->ErrorMsg(), "synchronizatoin update sync_updates insert ((" . $q . "))");
        }
        
        echo "successfully synced.";
    } else {
        echo "Sync is not properly configured yet, cannot sync now.. ";
    }
}

/** Save new person records which created on slave from last sync. */
function __addNewRecords($newRecords) {
    global $global;
    
    foreach ($newRecords as $serP) {
        $per = unserialize($serP);
        $per->postSerialize($global['db']);
        $per->init();       // these are new records... 
        $per->insert();
        /* echo 'test: ' . $per->full_name;
        echo '<pre>';
        print_r($per);
        */
    }
}


?>
