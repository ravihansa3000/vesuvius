<?php

/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

class SyncConfDB {
    
    public 
        $db,  
            
        $id,
        $created_time,
        $role,
        $incident_name,
        $incident_id,
        $instance_uuid,
        $preshared_key,
        $mode,
        $status,
        
        $sql_id,
        $sql_created_time,
        $sql_role,
        $sql_incident_name,
        $sql_incident_id,
        $sql_instance_uuid,
        $sql_preshared_key,
        $sql_mode,
        $sql_status;
    
    public function	__construct() {
		global $global;
		$this->db = $global['db'];
        
        $this->id = NULL;
    }
    
    /**
     * 
     * @global type $global
     * @param type $confId
     */
    public function load($confId = NULL) {
        if(isset($confId))
            $this->id = $confId;
        
        $q = "
            SELECT  *
            FROM    `sync_conf`
            WHERE   `id` = '".mysql_real_escape_string($this->id)."'
        ";
        $sync_conf = $this->db->Execute($q); 
        
        $this->created_time  =  $sync_conf->fields['created_time'];
        $this->role          =  $sync_conf->fields['role'];
        $this->incident_name =  $sync_conf->fields['incident_name'];
        $this->incident_id   =  $sync_conf->fields['incident_id'];
        $this->instance_uuid =  $sync_conf->fields['instance_uuid'];
        $this->preshared_key =  $sync_conf->fields['preshared_key'];
        $this->mode          =  $sync_conf->fields['mode'];
        $this->status        =  $sync_conf->fields['status'];
        
    }
    
    /**
     * 
     * @param type $uuid
     * @return boolean
     */
    public function isInstanceConfigured($uuid) {
        $q = " 
            SELECT *
            FROM `sync_conf`
            WHERE `instance_uuid` = '".mysql_real_escape_string($uuid)."'
            ";
        $result = $this->db->Execute($q);
        if($result === false) { daoErrorLog(__FILE__, __LINE__, __METHOD__, __CLASS__, __FUNCTION__, $this->db->ErrorMsg(), "Instance UUID check ((".$q."))"); }
        
        if($result != NULL && !$result->EOF) {
			return TRUE;            
		} else {
            return FALSE;
        }
    }

    public function save() {
        if ($this->id == NULL) {
            $this->insert();
            return $this->id;
        } else {
            // UPDATE
            $this->update();
        }
        
    }
    
    public function insert() {
        echo 'insert '.'<br />';
        $this->setSQLparams();
        if(!$this->isInstanceConfigured($this->instance_uuid)) {
            $q = "
                INSERT INTO sync_conf (
                    created_time,
                    role,
                    incident_name,
                    incident_id,
                    instance_uuid,
                    preshared_key,
                    mode,
                    status )
                VALUES (
                    ".$this->sql_created_time.",
                    ".$this->sql_role.",
                    ".$this->sql_incident_name.",
                    ".$this->sql_incident_id.",
                    ".$this->sql_instance_uuid.",
                    ".$this->sql_preshared_key.",
                    ".$this->sql_mode.",
                    ".$this->sql_status." );
            ";
            $result = $this->db->Execute($q);
            $this->id = mysql_insert_id();
            if($result === false) { daoErrorLog(__FILE__, __LINE__, __METHOD__, __CLASS__, __FUNCTION__, $this->db->ErrorMsg(), "Native Sync sync_conf insert ((".$q."))"); }
        }
    }
    
    public function update() {
        $this->setSQLparams();
        $q = "
            UPDATE sync_conf
            SET
                status          = ".$this->sql_status.",
                preshared_key   = ".$this->sql_preshared_key."
            WHERE id = ".$this->sql_id.";
        ";
        $result = $this->db->Execute($q);
        if($result === false) { daoErrorLog(__FILE__, __LINE__, __METHOD__, __CLASS__, __FUNCTION__, $this->db->ErrorMsg(), "Native Sync sync_conf update ((".$q."))"); }
    }


    private function setSQLparams() {
        $this->sql_id           = ($this->id            === null) ? "NULL" : "'".htmlentities(mysql_real_escape_string((string)$this->id))."'";
        $this->sql_created_time = ($this->created_time  === null) ? "NULL" : "'".htmlentities(mysql_real_escape_string((string)$this->created_time))."'";
        $this->sql_role         = ($this->role          === null) ? "NULL" : "'".htmlentities(mysql_real_escape_string((string)$this->role))."'";
        $this->sql_incident_name= ($this->incident_name === null) ? "NULL" : "'".htmlentities(mysql_real_escape_string((string)$this->incident_name))."'";
        $this->sql_incident_id  = ($this->incident_id   === null) ? "NULL" : "'".htmlentities(mysql_real_escape_string((string)$this->incident_id))."'";
        $this->sql_instance_uuid= ($this->instance_uuid === null) ? "NULL" : "'".htmlentities(mysql_real_escape_string((string)$this->instance_uuid))."'";
        $this->sql_preshared_key= ($this->preshared_key === null) ? "NULL" : "'".htmlentities(mysql_real_escape_string((string)$this->preshared_key))."'";
        $this->sql_mode         = ($this->mode          === null) ? "NULL" : "'".htmlentities(mysql_real_escape_string((string)$this->mode))."'";
        $this->sql_status       = ($this->status        === null) ? "NULL" : "'".htmlentities(mysql_real_escape_string((string)$this->status))."'";
    }
    
}

?>
