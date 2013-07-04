<?php
/** ******************************************************************************************************************************************************************
*********************************************************************************************************************************************************************
********************************************************************************************************************************************************************
*
* @class        voiceNote
* @version      12
* @author       Greg Miernicki <g@miernicki.com>
* @note         Usage of this class **REQUIRES** that ffmpeg is installed on the system.
*
********************************************************************************************************************************************************************
*********************************************************************************************************************************************************************
**********************************************************************************************************************************************************************/

class voiceNote {

	public $voice_id;
	public $p_uuid;
	public $length;
	public $format;
	public $sample_rate;
	public $channels;
	public $speaker;
	public $url_original;
	public $url_resampled_mp3;
	public $url_resampled_ogg;
	public $sha1original;

	public $Ovoice_id;
	public $Op_uuid;
	public $Olength;
	public $Oformat;
	public $Osample_rate;
	public $Ochannels;
	public $Ospeaker;
	public $Ourl_original;
	public $Ourl_resampled_mp3;
	public $Ourl_resampled_ogg;
	public $Osha1original;

	private $sql_voice_id;
	private $sql_p_uuid;
	private $sql_length;
	private $sql_format;
	private $sql_sample_rate;
	private $sql_channels;
	private $sql_speaker;
	private $sql_url_original;
	private $sql_url_resampled_mp3;
	private $sql_url_resampled_ogg;
	private $sql_sha1original;

	private $sql_Ovoice_id;
	private $sql_Op_uuid;
	private $sql_Olength;
	private $sql_Oformat;
	private $sql_Osample_rate;
	private $sql_Ochannels;
	private $sql_Ospeaker;
	private $sql_Ourl_original;
	private $sql_Ourl_resampled_mp3;
	private $sql_Ourl_resampled_ogg;
	private $sql_Osha1original;

	public $dataBase64;
	public $data;
	public $Odata;

	public $updated_by_p_uuid;

	private $saved;
	private $modified;


	// Constructor
	public function __construct() {

		// init db
		global $global;
		$this->db = $global['db'];

		// init values
		$this->voice_note_id     = null;
		$this->p_uuid            = null;
		$this->length            = null;
		$this->format            = null;
		$this->sample_rate       = null;
		$this->channels          = null;
		$this->speaker           = null;
		$this->url_original      = null;
		$this->url_resampled_mp3 = null;
		$this->url_resampled_ogg = null;
		$this->sha1original      = null;

		$this->Ovoice_note_id     = null;
		$this->Op_uuid            = null;
		$this->Olength            = null;
		$this->Oformat            = null;
		$this->Osample_rate       = null;
		$this->Ochannels          = null;
		$this->Ospeaker           = null;
		$this->Ourl_original      = null;
		$this->Ourl_resampled_mp3 = null;
		$this->Ourl_resampled_ogg = null;
		$this->Osha1original      = null;

		$this->sql_voice_note_id     = null;
		$this->sql_p_uuid            = null;
		$this->sql_length            = null;
		$this->sql_format            = null;
		$this->sql_sample_rate       = null;
		$this->sql_channels          = null;
		$this->sql_speaker           = null;
		$this->sql_url_original      = null;
		$this->sql_url_resampled_mp3 = null;
		$this->sql_url_resampled_ogg = null;
		$this->sql_sha1original      = null;

		$this->sql_Ovoice_note_id     = null;
		$this->sql_Op_uuid            = null;
		$this->sql_Olength            = null;
		$this->sql_Oformat            = null;
		$this->sql_Osample_rate       = null;
		$this->sql_Ochannels          = null;
		$this->sql_Ospeaker           = null;
		$this->sql_Ourl_original      = null;
		$this->sql_Ourl_resampled_mp3 = null;
		$this->sql_Ourl_resampled_ogg = null;
		$this->sql_Osha1original      = null;

		$this->dataBase64        = null;
		$this->data              = null;
		$this->Odata             = null;
		$this->updated_by_p_uuid = null;

		$this->modified = false;
		$this->saved    = false;
	}


	// Destructor
	public function __destruct() {

		$this->voice_note_id     = null;
		$this->p_uuid            = null;
		$this->length            = null;
		$this->format            = null;
		$this->sample_rate       = null;
		$this->channels          = null;
		$this->speaker           = null;
		$this->url_original      = null;
		$this->url_resampled_mp3 = null;
		$this->url_resampled_ogg = null;
		$this->sha1original      = null;

		$this->Ovoice_note_id     = null;
		$this->Op_uuid            = null;
		$this->Olength            = null;
		$this->Oformat            = null;
		$this->Osample_rate       = null;
		$this->Ochannels          = null;
		$this->Ospeaker           = null;
		$this->Ourl_original      = null;
		$this->Ourl_resampled_mp3 = null;
		$this->Ourl_resampled_ogg = null;
		$this->Osha1original      = null;

		$this->sql_voice_note_id     = null;
		$this->sql_p_uuid            = null;
		$this->sql_length            = null;
		$this->sql_format            = null;
		$this->sql_sample_rate       = null;
		$this->sql_channels          = null;
		$this->sql_speaker           = null;
		$this->sql_url_original      = null;
		$this->sql_url_resampled_mp3 = null;
		$this->sql_url_resampled_ogg = null;
		$this->sql_sha1original      = null;

		$this->sql_Ovoice_note_id     = null;
		$this->sql_Op_uuid            = null;
		$this->sql_Olength            = null;
		$this->sql_Oformat            = null;
		$this->sql_Osample_rate       = null;
		$this->sql_Ochannels          = null;
		$this->sql_Ospeaker           = null;
		$this->sql_Ourl_original      = null;
		$this->sql_Ourl_resampled_mp3 = null;
		$this->sql_Ourl_resampled_ogg = null;
		$this->sql_Osha1original      = null;

		$this->dataBase64        = null;
		$this->data              = null;
		$this->Odata             = null;
		$this->updated_by_p_uuid = null;

		$this->modified = null;
		$this->saved    = null;
	}


	// initializes some values for a new instance (instead of when we load a previous instance)
	public function init() {

		$this->saved = false;
		$this->voice_note_id = shn_create_uuid("voice_note");
	}


	// load data from db
	public function load() {

		global $global;

		$q = "
			SELECT *
			FROM voice_note
			WHERE p_uuid = '".mysql_real_escape_string((string)$this->p_uuid)."' ;
		";
		$result = $this->db->Execute($q);
		if($result === false) { daoErrorLog(__FILE__, __LINE__, __METHOD__, __CLASS__, __FUNCTION__, $this->db->ErrorMsg(), "voiceNote load 1 ((".$q."))"); }

		if($result != NULL && !$result->EOF) {

			$this->voice_note_id     = $result->fields['voice_note_id'];
			$this->p_uuid            = $result->fields['p_uuid'];
			$this->length            = $result->fields['length'];
			$this->format            = $result->fields['format'];
			$this->sample_rate       = $result->fields['sample_rate'];
			$this->channels          = $result->fields['channels'];
			$this->speaker           = $result->fields['speaker'];
			$this->url_original      = $result->fields['url_original'];
			$this->url_resampled_mp3 = $result->fields['url_resampled_mp3'];
			$this->url_resampled_ogg = $result->fields['url_resampled_ogg'];
			$this->sha1original      = $result->fields['sha1original'];

			// original values for updates...
			$this->Ovoice_note_id     = $result->fields['voice_note_id'];
			$this->Op_uuid            = $result->fields['p_uuid'];
			$this->Olength            = $result->fields['length'];
			$this->Oformat            = $result->fields['format'];
			$this->Osample_rate       = $result->fields['sample_rate'];
			$this->Ochannels          = $result->fields['channels'];
			$this->Ospeaker           = $result->fields['speaker'];
			$this->Ourl_original      = $result->fields['url_original'];
			$this->Ourl_resampled_mp3 = $result->fields['url_resampled_mp3'];
			$this->Ourl_resampled_ogg = $result->fields['url_resampled_ogg'];
			$this->Osha1original      = $result->fields['sha1original'];

			$path = $global['approot']."www/";
			$this->data = file_get_contents($path.$this->url_original);
			$this->Odata = file_get_contents($path.$this->url_original);
			$this->encode();

			$this->saved = true;

		} else {
			// we failed to load a de object for this person, so fail the load (indicate to person class there is no voice note for this person)
			return false;
		}
	}
	

	public function makeArrayObject() {
		$r = array();
	
		$r['voice_note_id']     = $this->voice_note_id;
		$r['length']            = $this->length;
		$r['format']            = $this->format;
		$r['sample_rate']       = $this->sample_rate;
		$r['channels']          = $this->channels;
		$r['speaker']           = $this->speaker;
		$r['original']          = $this->url_original;
		$r['url_resampled_mp3'] = $this->url_resampled_mp3;
		$r['url_resampled_ogg'] = $this->url_resampled_ogg;
		$r['sha1original']      = $this->sha1original;
		
		return $r;
	}
		

	// Delete function
	public function delete() {

		// just to mysql-ready the data nodes...
		$this->sync();

		// remove from filesystem this image
		$this->unwrite();

		// delete from db
		$q = "
			DELETE FROM voice_note
			WHERE voice_note_id = ".$this->sql_voice_note_id.";
		";
		$result = $this->db->Execute($q);
		if($result === false) { daoErrorLog(__FILE__, __LINE__, __METHOD__, __CLASS__, __FUNCTION__, $this->db->ErrorMsg(), "person voice note delete 1 ((".$q."))"); }
	}


	// synchronize SQL value strings with public attributes
	private function sync() {

		global $global;

		// build SQL value strings

		$this->sql_voice_note_id     = ($this->voice_note_id     === null) ? "NULL" : (int)$this->voice_note_id;
		$this->sql_p_uuid            = ($this->p_uuid            === null) ? "NULL" : "'".mysql_real_escape_string((string)$this->p_uuid)."'";
		$this->sql_length            = ($this->length            === null) ? "NULL" : (int)$this->length;
		$this->sql_format            = ($this->format            === null) ? "NULL" : "'".mysql_real_escape_string((string)$this->format)."'";
		$this->sql_sample_rate       = ($this->sample_rate       === null) ? "NULL" : (int)$this->sample_rate;
		$this->sql_channels          = ($this->channels          === null) ? "NULL" : (int)$this->channels;
		$this->sql_speaker           = ($this->speaker           === null) ? "NULL" : "'".mysql_real_escape_string((string)$this->speaker)."'";
		$this->sql_url_original      = ($this->url_original      === null) ? "NULL" : "'".mysql_real_escape_string((string)$this->url_original)."'";
		$this->sql_url_resampled_mp3 = ($this->url_resampled_mp3 === null) ? "NULL" : "'".mysql_real_escape_string((string)$this->url_resampled_mp3)."'";
		$this->sql_url_resampled_ogg = ($this->url_resampled_ogg === null) ? "NULL" : "'".mysql_real_escape_string((string)$this->url_resampled_ogg)."'";
		$this->sql_sha1original      = ($this->sha1original      === null) ? "NULL" : "'".mysql_real_escape_string((string)$this->sha1original)."'";

		$this->sql_Ovoice_note_id     = ($this->Ovoice_note_id     === null) ? "NULL" : (int)$this->Ovoice_note_id;
		$this->sql_Op_uuid            = ($this->Op_uuid            === null) ? "NULL" : "'".mysql_real_escape_string((string)$this->Op_uuid)."'";
		$this->sql_Olength            = ($this->Olength            === null) ? "NULL" : (int)$this->Olength;
		$this->sql_Oformat            = ($this->Oformat            === null) ? "NULL" : "'".mysql_real_escape_string((string)$this->Oformat)."'";
		$this->sql_Osample_rate       = ($this->Osample_rate       === null) ? "NULL" : (int)$this->Osample_rate;
		$this->sql_Ochannels          = ($this->Ochannels          === null) ? "NULL" : (int)$this->Ochannels;
		$this->sql_Ospeaker           = ($this->Ospeaker           === null) ? "NULL" : "'".mysql_real_escape_string((string)$this->Ospeaker)."'";
		$this->sql_Ourl_original      = ($this->Ourl_original      === null) ? "NULL" : "'".mysql_real_escape_string((string)$this->Ourl_original)."'";
		$this->sql_Ourl_resampled_mp3 = ($this->Ourl_resampled_mp3 === null) ? "NULL" : "'".mysql_real_escape_string((string)$this->Ourl_resampled_mp3)."'";
		$this->sql_Ourl_resampled_ogg = ($this->Ourl_resampled_ogg === null) ? "NULL" : "'".mysql_real_escape_string((string)$this->Ourl_resampled_ogg)."'";
		$this->sql_Osha1original      = ($this->Osha1original      === null) ? "NULL" : "'".mysql_real_escape_string((string)$this->Osha1original)."'";
	}


	public function decode() {

		$this->data = base64_decode($this->dataBase64);
		$this->sha1original = sha1($this->data);
	}


	public function encode() {

		$this->dataBase64 = base64_encode($this->data);
	}


	private function unwrite() {

		global $global;

		$webroot  = $global['approot']."www/";
		$original = $webroot.$this->url_original;
		$mp3      = $webroot.$this->url_resampled_mp3;
		$ogg      = $webroot.$this->url_resampled_ogg;

		if(!unlink($original)) {
			daoErrorLog(__FILE__, __LINE__, __METHOD__, __CLASS__, __FUNCTION__, "unable to delete file", "person voicenote unwrite 1 ((".$original."))");
		}
		if(!unlink($mp3)) {
			daoErrorLog(__FILE__, __LINE__, __METHOD__, __CLASS__, __FUNCTION__, "unable to delete file", "person voicenote unwrite 2 ((".$mp3."))");
		}
		if(!unlink($ogg)) {
			daoErrorLog(__FILE__, __LINE__, __METHOD__, __CLASS__, __FUNCTION__, "unable to delete file", "person voicenote unwrite 3 ((".$ogg."))");
		}
	}


	private function write() {

		global $global;

		// base64 to hex
		$this->decode();

		// generate path and filename portion
		$a = explode("/", $this->p_uuid);
		$filename = $a[0]."_".$a[1]; // make pl.nlm.nih.gov/person.123456 into pl.nlm.nih.gov_person.123456
		$filename = $filename."_vn".$this->voice_note_id."_"; // filename now like pl.nlm.nih.gov_person.123456_vn112233_
		$path = $global['approot']."www/tmp/plus_cache/".$filename; // path is now like /opt/pl/www/tmp/plus_cache/pl.nlm.nih.gov_person.123456_vn112233_

		// save original like /opt/pl/www/tmp/plus_cache/pl.nlm.nih.gov_person.123456_vn112233_original
		file_put_contents($path."original", $this->data);
		chmod($path."original", 0777);
		$this->url_original  = "tmp/plus_cache/".$filename."original";

		// save sha1 of the original file for later identification
		$this->sha1original = sha1($this->data);

		// use ffmpeg to resample the file to wav for html5 audio (supported in all browsers)
		shell_exec("ffmpeg -i ".$path."original ".$path."resampled.mp3 >/dev/null 2>&1 ;");
		shell_exec("ffmpeg -i ".$path."original ".$path."resampled.ogg >/dev/null 2>&1 ;");
		chmod($path."resampled.mp3", 0777);
		chmod($path."resampled.ogg", 0777);
		$this->url_resampled_mp3 = "tmp/plus_cache/".$filename."resampled.mp3";
		$this->url_resampled_ogg = "tmp/plus_cache/".$filename."resampled.ogg";
	}


	/** initial save the voice note */
	public function insert() {

		// if this object is in the db, update it instead
		if($this->saved) {
			$this->update();
		} else {

			$this->write();
			$this->sync();
			$q = "
				INSERT INTO voice_note (
					voice_note_id,
					p_uuid,
					length,
					format,
					sample_rate,
					channels,
					speaker,
					url_original,
					url_resampled_mp3,
					url_resampled_ogg,
					sha1original )
				VALUES (
					".$this->sql_voice_note_id.",
					".$this->sql_p_uuid.",
					".$this->sql_length.",
					".$this->sql_format.",
					".$this->sql_sample_rate.",
					".$this->sql_channels.",
					".$this->sql_speaker.",
					".$this->sql_url_original.",
					".$this->sql_url_resampled_mp3.",
					".$this->sql_url_resampled_ogg.",
					".$this->sql_sha1original." );
			";
			$result = $this->db->Execute($q);
			if($result === false) { daoErrorLog(__FILE__, __LINE__, __METHOD__, __CLASS__, __FUNCTION__, $this->db->ErrorMsg(), "voiceNote insert ((".$q."))"); }

			$this->saved = true;
		}
	}


	/** Update / Save Functions ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// Update / Save Functions ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// Update / Save Functions ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// */


	// save the person (subsequent save = update)
	public function update() {

		// if we've never saved this record before, we can't update it, so insert() instead
		if(!$this->saved) {
			$this->insert();
		} else {

			$this->sync();

			// if the binary data changed.... lets save it!
			if($this->data != $this->Odata) {
				$this->write();
			}

			$this->saveRevisions();

			if($this->modified) {
				$q = "
					UPDATE voice_note
					SET
						voice_note_id     = ".$this->sql_voice_note_id.",
						p_uuid            = ".$this->sql_p_uuid.",
						length            = ".$this->sql_length.",
						format            = ".$this->sql_format.",
						sample_rate       = ".$this->sql_sample_rate.",
						channels          = ".$this->sql_channels.",
						speaker           = ".$this->sql_speaker.",
						url_original      = ".$this->sql_url_original.",
						url_resampled_mp3 = ".$this->sql_url_resampled_mp3.",
						url_resampled_ogg = ".$this->sql_url_resampled_ogg.",
						sha1original      = ".$this->sql_sha1original."

					WHERE voice_note_id = ".$this->sql_voice_note_id.";
				";
				$result = $this->db->Execute($q);
				if($result === false) { daoErrorLog(__FILE__, __LINE__, __METHOD__, __CLASS__, __FUNCTION__, $this->db->ErrorMsg(), "person voiceNote update ((".$q."))"); }
			}

			$this->modified = false;
		}
	}


	// save any changes since object was loaded as revisions
	function saveRevisions() {

		if($this->p_uuid            != $this->Op_uuid)            { $this->saveRevision($this->sql_p_uuid,            $this->sql_Op_uuid,            'voice_note', 'p_uuid'            ); }
		if($this->length            != $this->Olength)            { $this->saveRevision($this->sql_length,            $this->sql_Olength,            'voice_note', 'length'            ); }
		if($this->format            != $this->Oformat)            { $this->saveRevision($this->sql_format,            $this->sql_Oformat,            'voice_note', 'format'            ); }
		if($this->sample_rate       != $this->Osample_rate)       { $this->saveRevision($this->sql_sample_rate,       $this->sql_Osample_rate,       'voice_note', 'sample_rate'       ); }
		if($this->channels          != $this->Ochannels)          { $this->saveRevision($this->sql_channels,          $this->sql_Ochannels,          'voice_note', 'channels'          ); }
		if($this->speaker           != $this->Ospeaker)           { $this->saveRevision($this->sql_speaker,           $this->sql_Ospeaker,           'voice_note', 'speaker'           ); }
		if($this->url_original      != $this->Ourl_original)      { $this->saveRevision($this->sql_url_original,      $this->sql_Ourl_original,      'voice_note', 'url_original'      ); }
		if($this->url_resampled_mp3 != $this->Ourl_resampled_mp3) { $this->saveRevision($this->sql_url_resampled_mp3, $this->sql_Ourl_resampled_mp3, 'voice_note', 'url_resampled_mp3' ); }
		if($this->url_resampled_ogg != $this->Ourl_resampled_ogg) { $this->saveRevision($this->sql_url_resampled_ogg, $this->sql_Ourl_resampled_ogg, 'voice_note', 'url_resampled_ogg' ); }
		if($this->sha1original      != $this->Osha1original)      { $this->saveRevision($this->sql_sha1original,      $this->sql_Osha1original,      'voice_note', 'sha1original'      ); }
	}


	// save the revision
	function saveRevision($newValue, $oldValue, $table, $column) {

		$this->modified = true;

		// note the revision
		$q = "
			INSERT into person_updates (`p_uuid`, `updated_table`, `updated_column`, `old_value`, `new_value`, `updated_by_p_uuid`)
			VALUES (".$this->sql_p_uuid.", '".$table."', '".$column."', ".$oldValue.", ".$newValue.", '".$this->updated_by_p_uuid."');
		";
		$result = $this->db->Execute($q);
		if($result === false) { daoErrorLog(__FILE__, __LINE__, __METHOD__, __CLASS__, __FUNCTION__, $this->db->ErrorMsg(), "person voiceNote saveRevision ((".$q."))"); }
	}
}



