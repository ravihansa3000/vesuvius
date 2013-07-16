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
    public $update_startDate;

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
	public function load() {
        parent::load();
        
        //Fetch update history from person_updates table
        $q = "
            SELECT  * 
            FROM    person_updates 
            WHERE   p_uuid='".mysql_real_escape_string((string)$this->p_uuid)."'
            AND     `update_time` > '".mysql_real_escape_string((string)$this->update_startDate)."'
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
    
    //end class
     
}
?>
