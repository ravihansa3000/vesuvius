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

require_once($global['approot']."mod/lpf/class.person.php"); 
require_once($global['approot']."mod/lpf/class.syncPerson.php"); 
require_once($global['approot']."mod/lpf/class.personEdxl.php");
require_once($global['approot']."mod/lpf/class.personImage.php");
require_once($global['approot']."mod/lpf/class.personImageTag.php");
require_once($global['approot'].'3rd/adodb/adodb.inc.php');
require_once($global['approot'].'3rd/nusoap/lib/nusoap.php');
    
function shn_sync_default() {
    global $global;
    
    /** Last sync date-time. */
    // todo query from sync update table...
    $lastSyncDate = '2012-01-01 00:00:00';
    
    $wsdl = "http://localhost/vesuvius_dev/vesuvius/www/?wsdl&api=29";
    $client = new nusoap_client($wsdl);
    
    $x = file_get_contents($global['approot']."mod/plus/reference_REUNITE4.xml");
    $result = $client->call('pull', array('incidentId' => 1, 'lastSyncDate' => $lastSyncDate));
        
    echo "
        <h2>wsdl: ".$wsdl."</h2>
        <pre>".var_export($result, true)."</pre> 
    ";

    __addNewRecords(json_decode($result['new']));

    /** Fetch updates for existing person record to send push request. */
    $q = "
            SELECT  p.`p_uuid`
            FROM    `person_uuid` p, `person_status` s
            WHERE   p.`incident_id`='".mysql_real_escape_string($incidentId)."'
            AND     p.`p_uuid` = s.`p_uuid`
            AND     s.`last_updated` > '".mysql_real_escape_string($lastSyncDate)."'
            AND     s.`creation_time` < '".mysql_real_escape_string($lastSyncDate)."'
        ";
    $result = $global['db']->Execute($q); 
    $updatesForPush = array();
	while (!$result == NULL && !$result->EOF) {
        $syncP = new syncPerson();
        $syncP->p_uuid = $result->fields['p_uuid'];
        $syncP->loadWithUpdates();
        $syncP->prepareSerialize();
        $updatesForPush[] = serialize($syncP);            
        $result->MoveNext();
	}
    
    /* Resolve conflicts for each person. */
    $slave_updates = json_decode($result['updates']);
    foreach ($slave_updates as $serP) {
        $slave_person = unserialize($serP);
        $slave_person->postSerialize($global['db']);
//        echo 'test: ' . $slave_person->full_name;
//        echo '<pre>';
//        print_r($slave_person);
        
        $local = new syncPerson();
        $local->p_uuid = $slave_person->p_uuid;
        $local->last_sync = $lastSyncDate;
        $local->loadWithUpdates();
        
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
            foreach ($local->update_history as syncPerson::$l_up) {
                if(($s_up['updated_table']==$l_up['updated_table']) && ($s_up['updated_column']==$l_up['updated_column'])) {
                    if($s_up['update_time'] > $l_up['update_time']) {
                        //set slave value to person
                        $l_up->setAttr($s_up['updated_column'], $s_up['new_value']);
                    } else {
                        // No changes need for local instance but change on push data to slave.
                        
                    }
                } else {
                    //set slave value to person
                    $l_up->setAttr($s_up['updated_column'], $s_up['new_value']);
                }
                
            }
        }
        
    }
    
    /*
    echo "
        <h2>Request</h2>
        <pre>".htmlspecialchars($client->request, ENT_QUOTES)."</pre>
        <h2>Response</h2>
        <pre>".htmlspecialchars($client->response, ENT_QUOTES)."</pre>
        <h2>Debug</h2>
        <pre>".htmlspecialchars($client->debug_str, ENT_QUOTES)."</pre>
    "; 
     */
    
}

/** Save new person records which created on slave from last sync. */
function __addNewRecords($newRecords) {
    global $global;
    
    foreach ($newRecords as $serP) {
        $per = unserialize($serP);
        $per->postSerialize($global['db']);
               
        $per->init();       // these are new records... 
        $per->insert();
//
        echo 'test: ' . $per->full_name;
        echo '<pre>';
        print_r($per);
    }
}

?>
