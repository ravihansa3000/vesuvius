<?php

/**
 * 
 * child class of person class uses for Native synchronization..
 * 
 * @class   syncPerson
 * @author  Gurutharshan Nadarajah <gurutharshan at gmail.com>
 * 
 */
class syncPerson extends person {
    
    public $update_history;
    public $last_sync;
    public $synced_instance;

	// Constructor:
	public function	__construct() {
        parent::__construct();
        $this->update_history = array();
    }
    
    // Destructor
	public function __destruct() {
        parent::__destruct();
        $this->update_history = null;
    }
    
    // calls parent load from person object and get update histpry
	public function loadWithUpdates() {
        parent::load();
        
        //Fetch update history from person_updates table
        $q = "
            SELECT  * 
            FROM    person_updates 
            WHERE   p_uuid='".mysql_real_escape_string((string)$this->p_uuid)."'
            AND     `update_time` > '".mysql_real_escape_string((string)$this->last_sync)."'
            ORDER BY update_time DESC
        ";
        $results = $this->db->Execute($q);
        
        while (!$results == NULL && !$results->EOF) {
            $p_his = array(
                'update_index'      => (string)$results->fields["update_index"],
                'p_uuid'            => (string)$results->fields["p_uuid"],
                'update_time'       => (string)$results->fields["update_time"],
                'updated_table'     => (string)$results->fields["updated_table"],
                'updated_column'    => (string)$results->fields["updated_column"],
                'old_value'         => (string)$results->fields["old_value"],
                'new_value'         => (string)$results->fields["new_value"],
                'updated_by_p_uuid' => (string)$results->fields["updated_by_p_uuid"],
            );
            $this->update_history[] = $p_his;
            $results->MoveNext();
        }
    }
    
    /* 
     * Remove redundant data on update_history -- largest index-->> old change
     * it's costly --->>  TO-DO performance improvement  
     */
    public function removeUpdateRedundancy() {
        for($i=0; $i < sizeof($this->update_history); $i++) {
            for($j=$i+1; $j < sizeof($this->update_history); $j++) {
                if (($this->update_history[$i]['updated_table'] == $this->update_history[$j]['updated_table']) && 
                        ($this->update_history[$i]['updated_column'] == $this->update_history[$j]['updated_column'])) {
                    // remove the element and rearrange the array index elements..
                    array_splice($this->update_history, $j, 1);
                }               
            }
        }
    }
    
    // Updates due to synchronizatoin with other instance... 
	function saveRevision($newValue, $oldValue, $table, $column) {
		$this->modified = true;
		// note the revision
        
        $sql_p_uuid = ($this->p_uuid === null) ? "NULL" : "'".mysql_real_escape_string((string)$this->p_uuid)."'";
		$q = "
			INSERT into person_updates (`p_uuid`, `updated_table`, `updated_column`, `old_value`, `new_value`, `updated_by_p_uuid`)
			VALUES (".$sql_p_uuid.", '".$table."', '".$column."', ".$oldValue.", ".$newValue.", '".$this->synced_instance."');
		";
		$result = $this->db->Execute($q);
		if($result === false) { daoErrorLog(__FILE__, __LINE__, __METHOD__, __CLASS__, __FUNCTION__, $this->db->ErrorMsg(), "person saveRevision ((".$q."))"); }
	}
    
    /*
     * Set null for filecontent string and database details
     */
    public function prepareSerialize() {
        foreach ($this->images as $personImage) {
            $personImage->fileContent = null;
            $personImage->OfileContent = null;
            
            $personImage->db = null;
        }
        $this->db = null;
    }
    
    /*
     * resetting deleted values
     */
    public function postSerialize() {
        global $global;
        foreach ($this->images as $personImage) {
            $personImage->decode();
            $personImage->OfileContent = $personImage->fileContent;
            $personImage->db = $global['db'];
        }
        $this->db = $global['db'];
    }
        
    /*
     * 
     */
    public function setAttr($attr, $val) {
        if($attr == 'p_uuid') 
            $this->p_uuid = $val;
        else if($attr == 'full_name') 
            $this->full_name = $val;
        else if($attr == 'family_name') 
            $this->family_name = $val;
        else if($attr == 'given_name') 
            $this->given_name = $val;
        else if($attr == 'alternate_names') 
            $this->alternate_names = $val;
        else if($attr == 'profile_urls') 
            $this->profile_urls = $val;
        else if($attr == 'incident_id') 
            $this->incident_id = $val;
        else if($attr == 'hospital_uuid') 
            $this->hospital_uuid = $val;
        else if($attr == 'expiry_date') 
            $this->expiry_date = $val;
        
        else if($attr == 'opt_status') 
            $this->opt_status = $val;
        else if($attr == 'last_updated') 
            $this->last_updated = $val;
        else if($attr == 'creation_time') 
            $this->creation_time = $val;
        else if($attr == 'street1') 
            $this->street1 = $val;
        else if($attr == 'street2') 
            $this->street2 = $val;
        else if($attr == 'neighborhood') 
            $this->neighborhood = $val;
        else if($attr == 'city') 
            $this->city = $val;
        else if($attr == 'region') 
            $this->region = $val;
        else if($attr == 'postal_code') 
            $this->postal_code = $val;
        else if($attr == 'country') 
            $this->country = $val;
        else if($attr == 'latitude') 
            $this->latitude = $val;
        else if($attr == 'longitude') 
            $this->longitude = $val;
        else if($attr == 'birth_date') 
            $this->birth_date = $val;
        else if($attr == 'opt_race') 
            $this->opt_race = $val;
        else if($attr == 'opt_religion') 
            $this->opt_religion = $val;
        else if($attr == 'opt_gender') 
            $this->opt_gender = $val;
        else if($attr == 'years_old') 
            $this->years_old = $val;
        else if($attr == 'minAge') 
            $this->minAge = $val;
        else if($attr == 'maxAge') 
            $this->maxAge = $val;
        else if($attr == 'last_seen') 
            $this->last_seen = $val;
        else if($attr == 'last_clothing') 
            $this->last_clothing = $val;
        else if($attr == 'other_comments') 
            $this->other_comments = $val;
        else if($attr == 'rep_uuid') 
            $this->rep_uuid = $val;
        
        else if($attr == 'opt_blood_type') 
            $this->opt_blood_type = $val;
        else if($attr == 'height') 
            $this->height = $val;
        else if($attr == 'weight') 
            $this->weight = $val;
        else if($attr == 'opt_eye_color') 
            $this->opt_eye_color = $val;
        else if($attr == 'opt_skin_color') 
            $this->opt_skin_color = $val;
        else if($attr == 'opt_hair_color') 
            $this->opt_hair_color = $val;
        else if($attr == 'injuries') 
            $this->injuries = $val;
        else if($attr == 'comments') 
            $this->comments = $val;
        
//        else if($attr == 'contact_type_value') 
//            $this->contact_type_value = $val;
    }
    
    
    //end class
     
}
?>
