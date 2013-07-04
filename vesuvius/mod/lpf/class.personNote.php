<?php
/** ******************************************************************************************************************************************************************
*********************************************************************************************************************************************************************
********************************************************************************************************************************************************************
*
* @class        personNote
* @version      1
* @author       Greg Miernicki <g@miernicki.com>
*
********************************************************************************************************************************************************************
*********************************************************************************************************************************************************************
**********************************************************************************************************************************************************************/

class personNote {

  public $note_id;
  public $note_about_p_uuid;
  public $note_written_by_p_uuid;
  public $note;
  public $when;
  public $suggested_status;
  public $suggested_location;
  
  public $Onote_id;
  public $Onote_about_p_uuid;
  public $Onote_written_by_p_uuid;
  public $Onote;
  public $Owhen;
  public $Osuggested_status;
  public $Osuggested_location;
  
  public $sql_note_id;
  public $sql_note_about_p_uuid;
  public $sql_note_written_by_p_uuid;
  public $sql_note;
  public $sql_when;
  public $sql_suggested_status;
  public $sql_suggested_location;
  
  public $sql_Onote_id;
  public $sql_Onote_about_p_uuid;
  public $sql_Onote_written_by_p_uuid;
  public $sql_Onote;
  public $sql_Owhen;
  public $sql_Osuggested_status;
  public $sql_Osuggested_location;

	private $saved;
	private $modified;


	// Constructor
	public function __construct() {

		// init db
		global $global;
		$this->db = $global['db'];

		// init values
    $this->note_id = null;
    $this->note_about_p_uuid = null;
    $this->note_written_by_p_uuid = null;
    $this->note = null;
    $this->when = null;
    $this->suggested_status = null;
    $this->suggested_location = null;
  
    $this->Onote_id = null;
    $this->Onote_about_p_uuid = null;
    $this->Onote_written_by_p_uuid = null;
    $this->Onote = null;
    $this->Owhen = null;
    $this->Osuggested_status = null;
    $this->Osuggested_location = null;

    $this->sql_note_id = null;
    $this->sql_note_about_p_uuid = null;
    $this->sql_note_written_by_p_uuid = null;
    $this->sql_note = null;
    $this->sql_when = null;
    $this->sql_suggested_status = null;
    $this->sql_suggested_location = null;
  
    $this->sql_Onote_id = null;
    $this->sql_Onote_about_p_uuid = null;
    $this->sql_Onote_written_by_p_uuid = null;
    $this->sql_Onote = null;
    $this->sql_Owhen = null;
    $this->sql_Osuggested_status = null;
    $this->sql_Osuggested_location = null;

		$this->modified = false;
		$this->saved    = false;
	}


	// Destructor
	public function __destruct() {

    $this->note_id = null;
    $this->note_about_p_uuid = null;
    $this->note_written_by_p_uuid = null;
    $this->note = null;
    $this->when = null;
    $this->suggested_status = null;
    $this->suggested_location = null;
  
    $this->Onote_id = null;
    $this->Onote_about_p_uuid = null;
    $this->Onote_written_by_p_uuid = null;
    $this->Onote = null;
    $this->Owhen = null;
    $this->Osuggested_status = null;
    $this->Osuggested_location = null;

    $this->sql_note_id = null;
    $this->sql_note_about_p_uuid = null;
    $this->sql_note_written_by_p_uuid = null;
    $this->sql_note = null;
    $this->sql_when = null;
    $this->sql_suggested_status = null;
    $this->sql_suggested_location = null;
  
    $this->sql_Onote_id = null;
    $this->sql_Onote_about_p_uuid = null;
    $this->sql_Onote_written_by_p_uuid = null;
    $this->sql_Onote = null;
    $this->sql_Owhen = null;
    $this->sql_Osuggested_status = null;
    $this->sql_Osuggested_location = null;

		$this->modified = null;
		$this->saved    = null;
	}


	// initializes some values for a new instance (instead of when we load a previous instance)
	public function init() {

		$this->saved = false;
		$this->note_id = shn_create_uuid("person_notes");
	}


	// load data from db
	public function load() {

		global $global;

		$q = "
			SELECT *
			FROM person_notes
			WHERE note_id = '".mysql_real_escape_string((string)$this->note_id)."' ;
		";
		$result = $this->db->Execute($q);
		if($result === false) { daoErrorLog(__FILE__, __LINE__, __METHOD__, __CLASS__, __FUNCTION__, $this->db->ErrorMsg(), "personNote load 1 ((".$q."))"); }

		if($result != NULL && !$result->EOF) {

      $this->note_id                = $result->fields['note_id'];
      $this->note_about_p_uuid      = $result->fields['note_about_p_uuid'];
      $this->note_written_by_p_uuid = $result->fields['note_written_by_p_uuid'];
      $this->note                   = $result->fields['note'];
      $this->when                   = $result->fields['when'];
      $this->suggested_status       = $result->fields['suggested_status'];
      $this->suggested_location     = $result->fields['suggested_location'];

			// original values for updates...
      $this->Onote_id                = $result->fields['note_id'];
      $this->Onote_about_p_uuid      = $result->fields['note_about_p_uuid'];
      $this->Onote_written_by_p_uuid = $result->fields['note_written_by_p_uuid'];
      $this->Onote                   = $result->fields['note'];
      $this->Owhen                   = $result->fields['when'];
      $this->Osuggested_status       = $result->fields['suggested_status'];
      $this->Osuggested_location     = $result->fields['suggested_location'];

			$this->saved = true;

		} else {
			// we failed to load a de object for this person, so fail the load (indicate to person class there is no voice note for this person)
			return false;
		}
	}
	

	public function makeArrayObject() {
		$r = array();
	
		$r['note_id']                = $this->note_id;
    $r['note_about_p_uuid']      = $this->note_about_p_uuid;
    $r['note_written_by_p_uuid'] = $this->note_written_by_p_uuid;
    $r['note']                   = $this->note;
    $r['when']                   = $this->when;
    $r['suggested_status']       = $this->suggested_status;
    $r['suggested_location']     = $this->suggested_location;

		return $r;
	}
		

	// Delete function
	public function delete() {

		// just to mysql-ready the data nodes...
		$this->sync();

		// delete from db
		$q = "
			DELETE FROM person_notes
			WHERE note_id = ".$this->sql_note_id.";
		";
		$result = $this->db->Execute($q);
		if($result === false) { daoErrorLog(__FILE__, __LINE__, __METHOD__, __CLASS__, __FUNCTION__, $this->db->ErrorMsg(), "person note delete 1 ((".$q."))"); }
	}


	// synchronize SQL value strings with public attributes
	private function sync() {

		global $global;

    // validate status
    if($this->suggested_status != "ali" && $this->suggested_status != "mis" && $this->suggested_status != "inj" && $this->suggested_status != "dec" && $this->suggested_status != "unk" && $this->suggested_status != "fnd") {
      $this->suggested_status = null;
    }
    
    // truncate large notes
    if(strlen($this->note) > 1024) {
      $this->note = substr($this->note, 0, 1023);
    }

		// build SQL value strings
		$this->sql_note_id                = ($this->note_id                === null) ? "NULL" : (int)$this->note_id;
    $this->sql_note_about_p_uuid      = ($this->note_about_p_uuid      === null) ? "NULL" : "'".mysql_real_escape_string((string)$this->note_about_p_uuid)."'";
    $this->sql_note_written_by_p_uuid = ($this->note_written_by_p_uuid === null) ? "NULL" : "'".mysql_real_escape_string((string)$this->note_written_by_p_uuid)."'";
    $this->sql_note                   = ($this->note                   === null) ? "NULL" : "'".htmlentities(mysql_real_escape_string((string)$this->note))."'";
    $this->sql_when                   = ($this->when                   === null) ? "NULL" : "'".htmlentities(mysql_real_escape_string((string)$this->when))."'";
    $this->sql_suggested_status       = ($this->suggested_status       === null) ? "NULL" : "'".$this->suggested_status."'";
    $this->sql_suggested_location     = ($this->suggested_location     === null) ? "NULL" : "'".mysql_real_escape_string((string)$this->suggested_location)."'";

  	$this->sql_Onote_id                = ($this->Onote_id                === null) ? "NULL" : (int)$this->Onote_id;
    $this->sql_Onote_about_p_uuid      = ($this->Onote_about_p_uuid      === null) ? "NULL" : "'".mysql_real_escape_string((string)$this->Onote_about_p_uuid)."'";
    $this->sql_Onote_written_by_p_uuid = ($this->Onote_written_by_p_uuid === null) ? "NULL" : "'".mysql_real_escape_string((string)$this->Onote_written_by_p_uuid)."'";
    $this->sql_Onote                   = ($this->Onote                   === null) ? "NULL" : "'".htmlentities(mysql_real_escape_string((string)$this->Onote))."'";
    $this->sql_Owhen                   = ($this->Owhen                   === null) ? "NULL" : "'".htmlentities(mysql_real_escape_string((string)$this->Owhen))."'";
    $this->sql_Osuggested_status       = ($this->Osuggested_status       === null) ? "NULL" : "'".$this->Osuggested_status."'";
    $this->sql_Osuggested_location     = ($this->Osuggested_location     === null) ? "NULL" : "'".mysql_real_escape_string((string)$this->Osuggested_location)."'";
	}



	/** initial save the note */
	public function insert() {

		// if this object is in the db, update it instead
		if($this->saved) {
			$this->update();
		} else {
			$this->sync();
			$q = "
				INSERT INTO person_notes (
					note_id,
					note_about_p_uuid,
          note_written_by_p_uuid,
          note,
          `when`,
          suggested_status,
          suggested_location )
				VALUES (
					".$this->sql_note_id.",
					".$this->sql_note_about_p_uuid.",
					".$this->sql_note_written_by_p_uuid.",
					".$this->sql_note.",
					".$this->sql_when.",
					".$this->sql_suggested_status.",
          ".$this->sql_suggested_location." );
			";
			$result = $this->db->Execute($q);
			if($result === false) { daoErrorLog(__FILE__, __LINE__, __METHOD__, __CLASS__, __FUNCTION__, $this->db->ErrorMsg(), "personNote insert ((".$q."))"); }

			$this->saved = true;
		}
	}


	/** Update / Save Functions ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// Update / Save Functions ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// Update / Save Functions ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// */


	// save the note (subsequent save = update)
	public function update() {

		// if we've never saved this record before, we can't update it, so insert() instead
		if(!$this->saved) {
			$this->insert();
		} else {
			$this->sync();
			$this->saveRevisions();
      
			if($this->modified) {
				$q = "
					UPDATE person_notes
					SET
						note_id                = ".$this->sql_note_id.",
						note_about_p_uuid      = ".$this->sql_note_about_p_uuid.",
						note_written_by_p_uuid = ".$this->sql_note_written_by_p_uuid.",
						note                   = ".$this->sql_note.",
						`when`                 = ".$this->sql_when.",
						suggested_status       = ".$this->sql_suggested_status.",
            suggested_location     = ".$this->sql_suggested_location."
            
					WHERE note_id = ".$this->sql_note_id.";
				";
				$result = $this->db->Execute($q);
				if($result === false) { daoErrorLog(__FILE__, __LINE__, __METHOD__, __CLASS__, __FUNCTION__, $this->db->ErrorMsg(), "person personNote update ((".$q."))"); }
			}

			$this->modified = false;
		}
	}


	// save any changes since object was loaded as revisions
	function saveRevisions() {

		if($this->note_about_p_uuid      != $this->Onote_about_p_uuid)      { $this->saveRevision($this->sql_note_about_p_uuid,      $this->sql_Onote_about_p_uuid,      'person_notes', 'note_about_p_uuid'      ); }
		if($this->note_written_by_p_uuid != $this->Onote_written_by_p_uuid) { $this->saveRevision($this->sql_note_written_by_p_uuid, $this->sql_Onote_written_by_p_uuid, 'person_notes', 'note_written_by_p_uuid' ); }
		if($this->note                   != $this->Onote)                   { $this->saveRevision($this->sql_note,                   $this->sql_Onote,                   'person_notes', 'note'                   ); }
		if($this->when                   != $this->Owhen)                   { $this->saveRevision($this->sql_when,                   $this->sql_Owhen,                   'person_notes', 'when'                   ); }
		if($this->suggested_status       != $this->Osuggested_status)       { $this->saveRevision($this->sql_suggested_status,       $this->sql_Osuggested_status,       'person_notes', 'suggested_status'       ); }
    if($this->suggested_location     != $this->Osuggested_location)     { $this->saveRevision($this->sql_suggested_location,     $this->sql_Osuggested_location,     'person_notes', 'suggested_location'     ); }
	}


	// save the revision
	function saveRevision($newValue, $oldValue, $table, $column) {

		$this->modified = true;

		// note the revision
		$q = "
			INSERT into person_updates (`p_uuid`, `updated_table`, `updated_column`, `old_value`, `new_value`, `updated_by_p_uuid`)
			VALUES (".$this->sql_note_about_p_uuid.", '".$table."', '".$column."', ".$oldValue.", ".$newValue.", '".$this->note_written_by_p_uuid."');
		";
		$result = $this->db->Execute($q);
		if($result === false) { daoErrorLog(__FILE__, __LINE__, __METHOD__, __CLASS__, __FUNCTION__, $this->db->ErrorMsg(), "person personNote saveRevision ((".$q."))"); }
	}
}



