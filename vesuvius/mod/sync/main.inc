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

require_once($global['approot']."mod/lpf/class.person.php"); 
require_once($global['approot']."mod/lpf/class.syncPerson.php"); 
require_once($global['approot']."mod/lpf/class.personEdxl.php");
require_once($global['approot']."mod/lpf/class.personImage.php");
require_once($global['approot']."mod/lpf/class.personImageTag.php");
require_once($global['approot'].'3rd/adodb/adodb.inc.php');
require_once($global['approot'].'3rd/nusoap/lib/nusoap.php');
    
function shn_sync_default() {
    global $global;
    
    $sync_conf_id = 1;
    $q = "
        SELECT  *
        FROM    `sync_conf`
        WHERE   `id` = '".mysql_real_escape_string($sync_conf_id)."'
       ";
    $sync_conf = $global['db']->Execute($q); 
    $localIncidentId = $sync_conf->fields['incident_id'];
    $foreignIncidentId = $sync_conf->fields['foreign_incident_id'];
    $syncMode = $sync_conf->fields['mode'];
    $slave_wsdl = $sync_conf->fields['instance_uuid'];
    
    /** Last sync date-time. */
    $q = "
        SELECT  MAX(`update_time`)
        FROM    `sync_updates`
        WHERE   `conf_id` = '".mysql_real_escape_string($sync_conf_id)."'
        ";
    $res = $global['db']->Execute($q);
    if (!$res == NULL)
        $lastSyncDate = $res->fields['update_time']; 
    else
        $lastSyncDate = '2012-01-01 00:00:00';
    
    $currentSyncDateTime = date('Y-m-d H:i:s');
//    $slave_wsdl = "http://localhost/vesuvius_dev/vesuvius/www/?wsdl&api=29";
    $client = new nusoap_client($slave_wsdl);
    $x = file_get_contents($global['approot']."mod/plus/reference_REUNITE4.xml");
    $pullResult = $client->call('pull', array('incidentId' => $foreignIncidentId, 'lastSyncDate' => $lastSyncDate));
    
    
    echo "
        <h2>wsdl: ".$slave_wsdl."</h2>
        <pre>".var_export($pullResult, true)."</pre> 
    ";

    __addNewRecords(json_decode($pullResult['new']));
    
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
           foreach ($local->update_history as $key=>$l_up) {
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
               $local->setAttr($l_up['updated_column'], $s_up['new_value']);
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
    
    /** Newly created person records from last sync. */
    $q = "
        SELECT  p.`p_uuid`
        FROM    `person_uuid` p, `person_status` s
        WHERE   p.`incident_id`='".mysql_real_escape_string($localIncidentId)."'
        AND     p.`p_uuid` = s.`p_uuid`
        AND     s.`creation_time` > '".mysql_real_escape_string($lastSyncDate)."'
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
     * 
     * query with exception from already resolved p_uuid 
     */
    $q = "
            SELECT  p.`p_uuid`
            FROM    `person_uuid` p, `person_status` s
            WHERE   p.`incident_id`='".mysql_real_escape_string($localIncidentId)."'
            AND     p.`p_uuid` = s.`p_uuid`
            AND     s.`last_updated` > '".mysql_real_escape_string($lastSyncDate)."'
            AND     s.`creation_time` < '".mysql_real_escape_string($lastSyncDate)."'
            NOT IN  ('".$skipForPush."')
            ORDER BY last_updated DESC
        ";
    $result = $global['db']->Execute($q);
	while (!$result == NULL && !$result->EOF) {
        $syncP = new syncPerson();
        $syncP->p_uuid = $result->fields['p_uuid'];
        $syncP->loadWithUpdates();
        $syncP->prepareSerialize();
        array_push($updatesForPush, serialize($syncP));   
        $result->MoveNext();
	}
        
    /** push request to slave... */
    $this_wsdl = "localhost/vesuvius_dev/vesuvius/29";      // TO-DO fetch from conf file.
    $pushResult = $client->call('push', array('new'         => (string)json_encode($newRecordList),
                                              'updates'     => (string)json_encode($updatesForPush),
                                              'syncDateTime'=> (string)json_encode($currentSyncDateTime),
                                              'incidentId'  => (string)json_encode($foreignIncidentId),
                                              'master_wsdl' => (string)json_encode($this_wsdl),
        ));
    $msg = json_decode($pushResult['message']);
    
    /**
     * update to the sync into sync_updates table.
     */
    $q = "
        INSERT INTO sync_updates (
            conf_id,
            update_time,)
        VALUES (
            ". $sync_conf_id .",
            ". $currentSyncDateTime .",
    ";
    $result = $global['db']->Execute($q);
    if($result === false) { daoErrorLog(__FILE__, __LINE__, __METHOD__, __CLASS__, __FUNCTION__, $global['db']->ErrorMsg(), "synchronizatoin update sync_updates insert ((".$q."))"); }

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
