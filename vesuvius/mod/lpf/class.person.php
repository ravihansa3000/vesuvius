<?
/** ******************************************************************************************************************************************************************
*********************************************************************************************************************************************************************
********************************************************************************************************************************************************************
*
* @class        person
* @version      18
* @author       Greg Miernicki <g@miernicki.com>
*
********************************************************************************************************************************************************************
*********************************************************************************************************************************************************************
**********************************************************************************************************************************************************************/

class person {

	// holds the XML/format if used to instantiate in this manner
	public $theString; // object is initialized as a string first, then parsed into an array
	public $xmlFormat; // enumerated constant denoting type of the XML being loaded ~ REUNITE, TRIAGEPIC
	public $a; // holds the array of the parsed xml
    
	// table person_uuid
	public $p_uuid;
	public $full_name;
	public $family_name;
	public $given_name;
	public $alternate_names;
	public $profile_urls;
	public $incident_id;
	public $hospital_uuid;
	public $expiry_date;

	// original values (initialized at load and checked against when saved)
	private $Op_uuid;
	private $Ofull_name;
	private $Ofamily_name;
	private $Ogiven_name;
	private $Oalternate_names;
	private $Oprofile_urls;
	private $Oincident_id;
	private $Ohospital_uuid;
	private $Oexpiry_date;

	// table person_status
	public $opt_status;
	public $last_updated;
	public $creation_time;
	public $street1;
	public $street2;
	public $neighborhood;
	public $city;
	public $region;
	public $postal_code;
	public $country;
	public $latitude;
	public $longitude;

	// original values (initialized at load and checked against when saved)
	private $Oopt_status;
	private $Olast_updated;
	private $Ocreation_time;
	private $Ostreet1;
	private $Ostreet2;
	private $Oneighborhood;
	private $Ocity;
	private $Oregion;
	private $Opostal_code;
	private $Ocountry;
	private $Olatitude;
	private $Olongitude;

	// when true we set the last_updated_db to null (holds record from solr indexing)
	public $useNullLastUpdatedDb;

	// ignore duplicate check
	public $ignoreDupeUuid;

	// table person_details
	public $birth_date;
	public $opt_race;
	public $opt_religion;
	public $opt_gender;
	public $years_old;
	public $minAge;
	public $maxAge;
	public $last_seen;
	public $last_clothing;
	public $other_comments;

	// original values (initialized at load and checked against when saved)
	private $Obirth_date;
	private $Oopt_race;
	private $Oopt_religion;
	private $Oopt_gender;
	private $Oyears_old;
	private $OminAge;
	private $OmaxAge;
	private $Olast_seen;
	private $Olast_clothing;
	private $Oother_comments;
    
    
    // HACK table person_physical
    public $opt_blood_type;
    public $height;
    public $weight;
    public $opt_eye_color;
    public $opt_skin_color;
    public $opt_hair_color;
    public $injuries;
    public $comments;
    
    // HACK original values (initialized at load and checked against when saved)
    public $Oopt_blood_type;
    public $Oheight;
    public $Oweight;
    public $Oopt_eye_color;
    public $Oopt_skin_color;
    public $Oopt_hair_color;
    public $Oinjuries;
    public $Ocomments;
    
    // HACK table contact
    public $contact_type_value;
    public $Ocontact_type_value;
    

	// table person_to_report
	public $rep_uuid;
	// original values (initialized at load and checked against when saved)
	private $Orep_uuid;

	// person's images
	public $images;

	// person's edxl components
	public $edxl;

	// person's voice_notes
	public $voice_notes;
  
  //person's notes
  public $person_notes;

	// sql strings of the objetct's attributes
	private $sql_p_uuid;
	private $sql_full_name;
	private $sql_family_name;
	private $sql_given_name;
	private $sql_alternate_names;
	private $sql_profile_urls;
	private $sql_incident_id;
	private $sql_hospital_uuid;
	private $sql_expiry_date;
	private $sql_opt_status;
	private $sql_last_updated;
	private $sql_creation_time;
	private $sql_street1;
	private $sql_street2;
	private $sql_neighborhood;
	private $sql_city;
	private $sql_region;
	private $sql_postal_code;
	private $sql_country;
	private $sql_latitude;
	private $sql_longitude;
	private $sql_birth_date;
	private $sql_opt_race;
	private $sql_opt_religion;
	private $sql_opt_gender;
	private $sql_years_old;
	private $sql_minAge;
	private $sql_maxAge;
	private $sql_last_seen;
	private $sql_last_clothing;
	private $sql_other_comments;
	private $sql_rep_uuid;

	// and for original values..
	private $sql_Op_uuid;
	private $sql_Ofull_name;
	private $sql_Ofamily_name;
	private $sql_Ogiven_name;
	private $sql_Oalternate_names;
	private $sql_Oprofile_urls;
	private $sql_Oincident_id;
	private $sql_Ohospital_uuid;
	private $sql_Oexpiry_date;
	private $sql_Oopt_status;
	private $sql_Olast_updated;
	private $sql_Ocreation_time;
	private $sql_Ostreet1;
	private $sql_Ostreet2;
	private $sql_Oneighborhood;
	private $sql_Ocity;
	private $sql_Oregion;
	private $sql_Opostal_code;
	private $sql_Ocountry;
	private $sql_Olatitude;
	private $sql_Olongitude;
	private $sql_Obirth_date;
	private $sql_Oopt_race;
	private $sql_Oopt_religion;
	private $sql_Oopt_gender;
	private $sql_Oyears_old;
	private $sql_OminAge;
	private $sql_OmaxAge;
	private $sql_Olast_seen;
	private $sql_Olast_clothing;
	private $sql_Oother_comments;
	private $sql_Orep_uuid;
    
	// used for when we recieve emails from mpres to make a pfif_note
	public $author_name;
	public $author_email;

	// whether to make a static PFIF not upon insertion
	public $makePfifNote;

	// if we encounter an error anywhere along the way, its value will be stored here ~ no error, value = 0
	public $ecode;

	// if this object has been modified or saved (inserted)
	private $modified;
	private $saved;

	// we hold the p_uuid of the person making modifications to the record
	public $updated_by_p_uuid;

	// boolean values to denote the origin of the person (for statistical purposes)
	public $arrival_triagepic;
	public $arrival_reunite;
	public $arrival_website;
	public $arrival_pfif;
	public $arrival_vanilla_email;
    
	// Constructor:
	public function	__construct() {

		global $global;
		$this->db = $global['db'];

		$this->theString      = null;
		$this->xmlFormat      = null;
		$this->a              = null;

		$this->p_uuid         = null;
		$this->full_name      = null;
		$this->family_name    = null;
		$this->given_name     = null;
		$this->alternate_names = null;
		$this->profile_urls   = null;
		$this->incident_id    = null;
		$this->hospital_uuid  = null;
		$this->expiry_date    = null;
		$this->opt_status     = null;
		$this->last_updated   = null;
		$this->creation_time  = null;
		$this->street1      = null;
		$this->street2      = null;
		$this->neighborhood = null;
		$this->city         = null;
		$this->region       = null;
		$this->postal_code  = null;
		$this->country      = null;
		$this->latitude     = null;
		$this->longitude    = null;
		$this->birth_date     = null;
		$this->opt_race       = null;
		$this->opt_religion   = null;
		$this->opt_gender     = null;
		$this->years_old      = null;
		$this->minAge         = null;
		$this->maxAge         = null;
		$this->last_seen      = null;
		$this->last_clothing  = null;
		$this->other_comments = null;
		$this->rep_uuid       = null;
        
		$this->Op_uuid         = null;
		$this->Ofull_name      = null;
		$this->Ofamily_name    = null;
		$this->Ogiven_name     = null;
		$this->Oalternate_names = null;
		$this->Oprofile_urls   = null;		
		$this->Oincident_id    = null;
		$this->Ohospital_uuid  = null;
		$this->Oexpiry_date    = null;
		$this->Oopt_status     = null;
		$this->Olast_updated   = null;
		$this->Ocreation_time  = null;
		$this->Ostreet1      = null;
		$this->Ostreet2      = null;
		$this->Oneighborhood = null;
		$this->Ocity         = null;
		$this->Oregion       = null;
		$this->Opostal_code  = null;
		$this->Ocountry      = null;
		$this->Olatitude     = null;
		$this->Olongitude    = null;
		$this->Obirth_date     = null;
		$this->Oopt_race       = null;
		$this->Oopt_religion   = null;
		$this->Oopt_gender     = null;
		$this->Oyears_old      = null;
		$this->OminAge         = null;
		$this->OmaxAge         = null;
		$this->Olast_seen      = null;
		$this->Olast_clothing  = null;
		$this->Oother_comments = null;
		$this->Orep_uuid       = null;

        // ---------- HACK person_physical and contact variables ---------------
        $this->opt_blood_type   = null;
        $this->height           = null;
        $this->weight           = null;
        $this->opt_eye_color    = null;
        $this->opt_skin_color   = null;
        $this->opt_hair_color   = null;
        $this->injuries         = null;
        $this->comments         = null;
        
        $this->Oopt_blood_type   = null;
        $this->Oheight           = null;
        $this->Oweight           = null;
        $this->Oopt_eye_color    = null;
        $this->Oopt_skin_color   = null;
        $this->Oopt_hair_color   = null;
        $this->Oinjuries         = null;
        $this->Ocomments         = null;
        
        $this->contact_type_value   = array();
        $this->Ocontact_type_value  = array();
        
        // ------------ End HACK -----------------------------------------------
        
		$this->images         = array();
		$this->edxl           = null;
		$this->voice_notes    = array();
    $this->person_notes   = array();

		$this->sql_p_uuid         = null;
		$this->sql_full_name      = null;
		$this->sql_family_name    = null;
		$this->sql_given_name     = null;
		$this->sql_alternate_names = null;
		$this->sql_profile_urls   = null;
		$this->sql_incident_id    = null;
		$this->sql_hospital_uuid  = null;
		$this->sql_expiry_date    = null;
		$this->sql_opt_status     = null;
		$this->sql_last_updated   = null;
		$this->sql_creation_time  = null;
		$this->sql_street1      = null;
		$this->sql_street2      = null;
		$this->sql_neighborhood = null;
		$this->sql_city         = null;
		$this->sql_region       = null;
		$this->sql_postal_code  = null;
		$this->sql_country      = null;
		$this->sql_latitude     = null;
		$this->sql_longitude    = null;
		$this->sql_birth_date     = null;
		$this->sql_opt_race       = null;
		$this->sql_opt_religion   = null;
		$this->sql_opt_gender     = null;
		$this->sql_years_old      = null;
		$this->sql_minAge         = null;
		$this->sql_maxAge         = null;
		$this->sql_last_seen      = null;
		$this->sql_last_clothing  = null;
		$this->sql_other_comments = null;
		$this->sql_rep_uuid       = null;

		$this->sql_Op_uuid         = null;
		$this->sql_Ofull_name      = null;
		$this->sql_Ofamily_name    = null;
		$this->sql_Ogiven_name     = null;
		$this->sql_Oalternate_names = null;
		$this->sql_Oprofile_urls   = null;
		$this->sql_Oincident_id    = null;
		$this->sql_Ohospital_uuid  = null;
		$this->sql_Oexpiry_date    = null;
		$this->sql_Oopt_status     = null;
		$this->sql_Olast_updated   = null;
		$this->sql_Ocreation_time  = null;
		$this->sql_Ostreet1      = null;
		$this->sql_Ostreet2      = null;
		$this->sql_Oneighborhood = null;
		$this->sql_Ocity         = null;
		$this->sql_Oregion       = null;
		$this->sql_Opostal_code  = null;
		$this->sql_Ocountry      = null;
		$this->sql_Olatitude     = null;
		$this->sql_Olongitude    = null;		
		$this->sql_Obirth_date     = null;
		$this->sql_Oopt_race       = null;
		$this->sql_Oopt_religion   = null;
		$this->sql_Oopt_gender     = null;
		$this->sql_Oyears_old      = null;
		$this->sql_OminAge         = null;
		$this->sql_OmaxAge         = null;
		$this->sql_Olast_seen      = null;
		$this->sql_Olast_clothing  = null;
		$this->sql_Oother_comments = null;
		$this->sql_Orep_uuid       = null;

		$this->author_name           = null;
		$this->author_email          = null;
		$this->makePfifNote          = true;
		$this->useNullLastUpdatedDb  = false;
		$this->ignoreDupeUuid        = false;
		$this->ecode                 = 0;
		$this->updated_by_p_uuid     = null;

		$this->saved                 = false;
		$this->modified              = false;

		$this->arrival_triagepic     = false;
		$this->arrival_reunite       = false;
		$this->arrival_website       = false;
		$this->arrival_pfif          = false;
		$this->arrival_vanilla_email = false;
	}



	// Destructor
	public function __destruct() {
		$this->theString      = null;
		$this->xmlFormat      = null;
		$this->a              = null;

		$this->p_uuid         = null;
		$this->full_name      = null;
		$this->family_name    = null;
		$this->given_name     = null;
		$this->alternate_names = null;
		$this->profile_urls   = null;		
		$this->incident_id    = null;
		$this->hospital_uuid  = null;
		$this->expiry_date    = null;
		$this->opt_status     = null;
		$this->last_updated   = null;
		$this->creation_time  = null;
		$this->street1      = null;
		$this->street2      = null;
		$this->neighborhood = null;
		$this->city         = null;
		$this->region       = null;
		$this->postal_code  = null;
		$this->country      = null;
		$this->latitude     = null;
		$this->longitude    = null;
		$this->birth_date     = null;
		$this->opt_race       = null;
		$this->opt_religion   = null;
		$this->opt_gender     = null;
		$this->years_old      = null;
		$this->minAge         = null;
		$this->maxAge         = null;
		$this->last_seen      = null;
		$this->last_clothing  = null;
		$this->other_comments = null;
		$this->rep_uuid       = null;

		$this->Op_uuid         = null;
		$this->Ofull_name      = null;
		$this->Ofamily_name    = null;
		$this->Ogiven_name     = null;
		$this->Oalternate_names = null;
		$this->Oprofile_urls   = null;		
		$this->Oincident_id    = null;
		$this->Ohospital_uuid  = null;
		$this->Oexpiry_date    = null;
		$this->Oopt_status     = null;
		$this->Olast_updated   = null;
		$this->Ocreation_time  = null;
		$this->Ostreet1      = null;
		$this->Ostreet2      = null;
		$this->Oneighborhood = null;
		$this->Ocity         = null;
		$this->Oregion       = null;
		$this->Opostal_code  = null;
		$this->Ocountry      = null;
		$this->Olatitude     = null;
		$this->Olongitude    = null;
		$this->Obirth_date     = null;
		$this->Oopt_race       = null;
		$this->Oopt_religion   = null;
		$this->Oopt_gender     = null;
		$this->Oyears_old      = null;
		$this->OminAge         = null;
		$this->OmaxAge         = null;
		$this->Olast_seen      = null;
		$this->Olast_clothing  = null;
		$this->Oother_comments = null;
		$this->Orep_uuid       = null;

        // ------------ HACK person_physical and contact variables -------------
        $this->opt_blood_type   = null;
        $this->height           = null;
        $this->weight           = null;
        $this->opt_eye_color    = null;
        $this->opt_skin_color   = null;
        $this->opt_hair_color   = null;
        $this->injuries         = null;
        $this->comments         = null;
        
        $this->Oopt_blood_type   = null;
        $this->Oheight           = null;
        $this->Oweight           = null;
        $this->Oopt_eye_color    = null;
        $this->Oopt_skin_color   = null;
        $this->Oopt_hair_color   = null;
        $this->Oinjuries         = null;
        $this->Ocomments         = null;
        
        $this->contact_type_value   = null;
        $this->Ocontact_type_value  = null;
        
        // -------------------- End HACK ----------------------
        
		$this->images          = null;
		$this->edxl            = null;
		$this->voice_notes      = null;
    $this->person_notes     = null;

		$this->sql_p_uuid         = null;
		$this->sql_full_name      = null;
		$this->sql_family_name    = null;
		$this->sql_given_name     = null;
		$this->sql_alternate_names = null;
		$this->sql_profile_urls   = null;		
		$this->sql_incident_id    = null;
		$this->sql_hospital_uuid  = null;
		$this->sql_expiry_date    = null;
		$this->sql_opt_status     = null;
		$this->sql_last_updated   = null;
		$this->sql_creation_time  = null;
		$this->sql_street1      = null;
		$this->sql_street2      = null;
		$this->sql_neighborhood = null;
		$this->sql_city         = null;
		$this->sql_region       = null;
		$this->sql_postal_code  = null;
		$this->sql_country      = null;
		$this->sql_latitude     = null;
		$this->sql_longitude    = null;		
		$this->sql_birth_date     = null;
		$this->sql_opt_race       = null;
		$this->sql_opt_religion   = null;
		$this->sql_opt_gender     = null;
		$this->sql_years_old      = null;
		$this->sql_minAge         = null;
		$this->sql_maxAge         = null;
		$this->sql_last_seen      = null;
		$this->sql_last_clothing  = null;
		$this->sql_other_comments = null;
		$this->sql_rep_uuid       = null;

		$this->sql_Op_uuid         = null;
		$this->sql_Ofull_name      = null;
		$this->sql_Ofamily_name    = null;
		$this->sql_Ogiven_name     = null;
		$this->sql_Oalternate_names = null;
		$this->sql_Oprofile_urls   = null;		
		$this->sql_Oincident_id    = null;
		$this->sql_Ohospital_uuid  = null;
		$this->sql_Oexpiry_date    = null;
		$this->sql_Oopt_status     = null;
		$this->sql_Olast_updated   = null;
		$this->sql_Ocreation_time  = null;
		$this->sql_Ostreet1      = null;
		$this->sql_Ostreet2      = null;
		$this->sql_Oneighborhood = null;
		$this->sql_Ocity         = null;
		$this->sql_Oregion       = null;
		$this->sql_Opostal_code  = null;
		$this->sql_Ocountry      = null;
		$this->sql_Olatitude     = null;
		$this->sql_Olongitude    = null;		
		$this->sql_Obirth_date     = null;
		$this->sql_Oopt_race       = null;
		$this->sql_Oopt_religion   = null;
		$this->sql_Oopt_gender     = null;
		$this->sql_Oyears_old      = null;
		$this->sql_OminAge         = null;
		$this->sql_OmaxAge         = null;
		$this->sql_Olast_seen      = null;
		$this->sql_Olast_clothing  = null;
		$this->sql_Oother_comments = null;
		$this->sql_Orep_uuid       = null;

		$this->author_name           = null;
		$this->author_email          = null;
		$this->makePfifNote          = null;
		$this->useNullLastUpdatedDb  = null;
		$this->ignoreDupeUuid        = null;
		$this->ecode                 = null;
		$this->updated_by_p_uuid     = null;

		$this->saved                 = null;
		$this->modified              = null;

		$this->arrival_triagepic     = null;
		$this->arrival_reunite       = null;
		$this->arrival_website       = null;
		$this->arrival_pfif          = null;
		$this->arrival_vanilla_email = null;
	}


	// initializes..
	public function init() {
		$this->saved = false;
	}


	// Load Functions //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Load Functions //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Load Functions //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	// loads the data from a person in the database
	public function load() {

		global $global;

		$q = "
			SELECT *
			FROM person_uuid
			WHERE p_uuid = '".mysql_real_escape_string((string)$this->p_uuid)."' ;
		";
		$result = $this->db->Execute($q);
		if($result === false) { daoErrorLog(__FILE__, __LINE__, __METHOD__, __CLASS__, __FUNCTION__, $this->db->ErrorMsg(), "person load person_uuid ((".$q."))"); }

		if($result != NULL && !$result->EOF) {
			$this->full_name     = $result->fields['full_name'];
			$this->family_name   = $result->fields['family_name'];
			$this->given_name    = $result->fields['given_name'];
			$this->alternate_names = $result->fields['alternate_names'];
			$this->profile_urls  = $result->fields['profile_urls'];
			$this->incident_id   = $result->fields['incident_id'];
			$this->hospital_uuid = $result->fields['hospital_uuid'];
			$this->expiry_date   = $result->fields['expiry_date'];

			// save them as original values too...
			$this->Ofull_name     = $result->fields['full_name'];
			$this->Ofamily_name   = $result->fields['family_name'];
			$this->Ogiven_name    = $result->fields['given_name'];
			$this->Oalternate_names = $result->fields['alternate_names'];
			$this->Oprofile_urls  = $result->fields['profile_urls'];
			$this->Oincident_id   = $result->fields['incident_id'];
			$this->Ohospital_uuid = $result->fields['hospital_uuid'];
			$this->Oexpiry_date   = $result->fields['expiry_date'];
		} else {
			$this->ecode = 9000;
		}


		$q = "
			SELECT *
			FROM person_status
			WHERE p_uuid = '".mysql_real_escape_string((string)$this->p_uuid)."' ;
		";
		$result = $this->db->Execute($q);
		if($result === false) { daoErrorLog(__FILE__, __LINE__, __METHOD__, __CLASS__, __FUNCTION__, $this->db->ErrorMsg(), "person load person_status ((".$q."))"); }

		if($result != NULL && !$result->EOF) {
			$this->opt_status    = $result->fields['opt_status'];
			$this->last_updated  = $result->fields['last_updated'];
			$this->creation_time = $result->fields['creation_time'];
			$this->street1      = $result->fields['street1'];
			$this->street2      = $result->fields['street2'];
			$this->neighborhood = $result->fields['neighborhood'];
			$this->city         = $result->fields['city'];
			$this->region       = $result->fields['region'];
			$this->postal_code  = $result->fields['postal_code'];
			$this->country      = $result->fields['country'];
			$this->latitude     = $result->fields['latitude'];
			$this->longitude    = $result->fields['longitude'];

			// save them as original values too...
			$this->Oopt_status    = $result->fields['opt_status'];
			$this->Olast_updated  = $result->fields['last_updated'];
			$this->Ocreation_time = $result->fields['creation_time'];
			$this->Ostreet1      = $result->fields['street1'];
			$this->Ostreet2      = $result->fields['street2'];
			$this->Oneighborhood = $result->fields['neighborhood'];
			$this->Ocity         = $result->fields['city'];
			$this->Oregion       = $result->fields['region'];
			$this->Opostal_code  = $result->fields['postal_code'];
			$this->Ocountry      = $result->fields['country'];
			$this->Olatitude     = $result->fields['latitude'];
			$this->Olongitude    = $result->fields['longitude'];
		} else {
			$this->ecode = 9000;
		}


		$q = "
			SELECT *
			FROM person_details
			WHERE p_uuid = '".mysql_real_escape_string((string)$this->p_uuid)."' ;
		";
		$result = $this->db->Execute($q);
		if($result === false) { daoErrorLog(__FILE__, __LINE__, __METHOD__, __CLASS__, __FUNCTION__, $this->db->ErrorMsg(), "person load person_details ((".$q."))"); }

		if($result != NULL && !$result->EOF) {
			$this->birth_date     = $result->fields['birth_date'];
			$this->opt_race       = $result->fields['opt_race'];
			$this->opt_religion   = $result->fields['opt_religion'];
			$this->opt_gender     = $result->fields['opt_gender'];
			$this->years_old      = $result->fields['years_old'];
			$this->minAge         = $result->fields['minAge'];
			$this->maxAge         = $result->fields['maxAge'];
			$this->last_seen      = $result->fields['last_seen'];
			$this->last_clothing  = $result->fields['last_clothing'];
			$this->other_comments = $result->fields['other_comments'];

			// save them as original values too...
			$this->Obirth_date     = $result->fields['birth_date'];
			$this->Oopt_race       = $result->fields['opt_race'];
			$this->Oopt_religion   = $result->fields['opt_religion'];
			$this->Oopt_gender     = $result->fields['opt_gender'];
			$this->Oyears_old      = $result->fields['years_old'];
			$this->OminAge         = $result->fields['minAge'];
			$this->OmaxAge         = $result->fields['maxAge'];
			$this->Olast_seen      = $result->fields['last_seen'];
			$this->Olast_clothing  = $result->fields['last_clothing'];
			$this->Oother_comments = $result->fields['other_comments'];
		} else {
			$this->ecode = 9000;
		}


		$q = "
			SELECT *
			FROM person_to_report
			WHERE p_uuid = '".mysql_real_escape_string((string)$this->p_uuid)."' ;
		";
		$result = $this->db->Execute($q);
		if($result === false) { daoErrorLog(__FILE__, __LINE__, __METHOD__, __CLASS__, __FUNCTION__, $this->db->ErrorMsg(), "person load person_to_report ((".$q."))"); }

		if($result != NULL && !$result->EOF) {
			$this->rep_uuid = $result->fields['rep_uuid'];
			// original value too...
			$this->Orep_uuid = $result->fields['rep_uuid'];
		} else {
			$this->ecode = 9000;
		}

		// object exists in the db
		if($this->ecode != 9000) {
			$this->saved = true;
		}

		$this->loadImages();
		$this->loadEdxl();
		$this->loadPfif();
		$this->loadVoiceNotes();
        $this->loadPersonNotes();
        
        /* HACK for person_physical and contact table detail */
        $this->loadPhysical();
        $this->loadContactDetails();
	}
    
    /* HACK load contents from person_physical table */
    private function loadPhysical() {
        
        // fetch all person_physical record for this person
		$q = "
			SELECT  *
			FROM    person_physical
			WHERE   p_uuid = '".mysql_real_escape_string((string)$this->p_uuid)."' ;
		";
        $result = $this->db->Execute($q);
		if($result === false) { daoErrorLog(__FILE__, __LINE__, __METHOD__, __CLASS__, __FUNCTION__, $this->db->ErrorMsg(), "person_physical 1"); }
		if($result != NULL && !$result->EOF) {
			$this->opt_blood_type   = $result->fields['opt_blood_type'];
            $this->height           = $result->fields['height'];
            $this->weight           = $result->fields['weight'];
            $this->opt_eye_color    = $result->fields['opt_eye_color'];
            $this->opt_skin_color   = $result->fields['opt_skin_color'];
            $this->opt_hair_color   = $result->fields['opt_hair_color'];
            $this->injuries         = $result->fields['injuries'];
            $this->comments         = $result->fields['comments'];

            $this->Oopt_blood_type   = $result->fields['opt_blood_type'];
            $this->Oheight           = $result->fields['height'];
            $this->Oweight           = $result->fields['weight'];
            $this->Oopt_eye_color    = $result->fields['opt_eye_color'];
            $this->Oopt_skin_color   = $result->fields['opt_skin_color'];
            $this->Oopt_hair_color   = $result->fields['opt_hair_color'];
            $this->Oinjuries         = $result->fields['injuries'];
            $this->Ocomments         = $result->fields['comments'];
		}
    }
    
    /* HACK load contents from contact table */
    private function loadContactDetails() {
        // fetch all person_physical record for this person
		$q = "
			SELECT *
			FROM contact
			WHERE p_uuid = '".mysql_real_escape_string((string)$this->p_uuid)."' ;
		";
        $result = $this->db->Execute($q);
		if($result === false) { daoErrorLog(__FILE__, __LINE__, __METHOD__, __CLASS__, __FUNCTION__, $this->db->ErrorMsg(), "person_physical 1"); }
		while(!$result == NULL && !$result->EOF) {
            $this->contact_type_value[]   = $result->fields['opt_contact_type'] . '<::>' . $result->fields['contact_value'];
            $this->Ocontact_type_value[]  = $result->fields['opt_contact_type'] . '<::>' . $result->fields['contact_value'];
            $result->MoveNext();
        }
    }


	private function loadImages() {

		// find all images for this person
		$q = "
			SELECT *
			FROM image
			WHERE p_uuid = '".mysql_real_escape_string((string)$this->p_uuid)."' ;
		";
		$result = $this->db->Execute($q);
		if($result === false) { daoErrorLog(__FILE__, __LINE__, __METHOD__, __CLASS__, __FUNCTION__, $this->db->ErrorMsg(), "loadImages 1"); }
		while(!$result == NULL && !$result->EOF) {

			$i = new personImage();
			$i->p_uuid = $this->p_uuid;
			$i->updated_by_p_uuid = $this->updated_by_p_uuid;
			$i->image_id = $result->fields['image_id'];
			$i->load();
			$this->images[] = $i;
			$result->MoveNext();
		}
	}


	private function loadEdxl() {

		$this->edxl = new personEdxl();
		$this->edxl->p_uuid = $this->p_uuid;
		$this->edxl->updated_by_p_uuid = $this->updated_by_p_uuid;
		$recordHasEdxl = $this->edxl->load();
		if(!$recordHasEdxl) {
			$this->edxl = null;
		}
	}


	private function loadVoiceNotes() {

		// find all voice_notes for this person
		$q = "
			SELECT *
			FROM voice_note
			WHERE p_uuid = '".mysql_real_escape_string((string)$this->p_uuid)."' ;
		";
		$result = $this->db->Execute($q);
		if($result === false) { daoErrorLog(__FILE__, __LINE__, __METHOD__, __CLASS__, __FUNCTION__, $this->db->ErrorMsg(), "loadVoiceNotes 1"); }
		while(!$result == NULL && !$result->EOF) {

			$v = new voiceNote();
			$v->p_uuid = $this->p_uuid;
			$v->updated_by_p_uuid = $this->updated_by_p_uuid;
			$v->voice_note_id = $result->fields['voice_note_id'];
			$v->load();
			$this->voice_notes[] = $v;
			$result->MoveNext();
		}
	}


  private function loadPersonNotes() {

		// find/load all person_notes for this person
		$q = "
			SELECT *
			FROM person_notes
			WHERE note_about_p_uuid = '".mysql_real_escape_string((string)$this->p_uuid)."' ;
		";
		$result = $this->db->Execute($q);
		if($result === false) { daoErrorLog(__FILE__, __LINE__, __METHOD__, __CLASS__, __FUNCTION__, $this->db->ErrorMsg(), "loadPersonNotes 1"); }
		while(!$result == NULL && !$result->EOF) {

			$pn = new personNote();
			$pn->note_about_p_uuid = $this->p_uuid;
			$pn->note_written_by_p_uuid = $this->updated_by_p_uuid;
			$pn->note_id = $result->fields['note_id'];
			$pn->load();
			$this->person_notes[] = $pn;
			$result->MoveNext();
		}
	}


	private function loadPfif() {
		// to do....
	}


	/** Insert / FirstSave Functions ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// Insert / FirstSave Functions ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// Insert / FirstSave Functions ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// */


	// save the person (initial save = insert)
	public function insert() {

		// if this object is in the db, update it instead
		if($this->saved) {
			$this->update();
		} else {

			$this->sync();
			$q = "
				INSERT INTO person_uuid (
					p_uuid,
					full_name,
					family_name,
					given_name,
					alternate_names,
					profile_urls,
					incident_id,
					hospital_uuid,
					expiry_date )
				VALUES (
					".$this->sql_p_uuid.",
					".$this->sql_full_name.",
					".$this->sql_family_name.",
					".$this->sql_given_name.",
					".$this->sql_alternate_names.",
					".$this->sql_profile_urls.",
					".$this->sql_incident_id.",
					".$this->sql_hospital_uuid.",
					".$this->sql_expiry_date." );
			";
			$result = $this->db->Execute($q);
			if($result === false) { daoErrorLog(__FILE__, __LINE__, __METHOD__, __CLASS__, __FUNCTION__, $this->db->ErrorMsg(), "person person_uuid insert ((".$q."))"); }

			if($this->useNullLastUpdatedDb) {
				$ludb = "NULL";
			} else {
				$ludb = "'".date('Y-m-d H:i:s')."'";
			}

			$q = "
				INSERT INTO person_status (
					p_uuid,
					opt_status,
					last_updated,
					creation_time,
					last_updated_db,
					street1,
					street2,
					neighborhood,
					city,
					region,
					postal_code,
					country,
					latitude,
					longitude
				)
				VALUES (
					".$this->sql_p_uuid.",
					".$this->sql_opt_status.",
					".$this->sql_last_updated.",
					".$this->sql_creation_time.",
					".$ludb.",
					".$this->sql_street1.",
					".$this->sql_street2.",
					".$this->sql_neighborhood.",
					".$this->sql_city.",
					".$this->sql_region.",
					".$this->sql_postal_code.",
					".$this->sql_country.",
					".$this->sql_latitude.",
					".$this->sql_longitude."
				);
			";
			$result = $this->db->Execute($q);
			if($result === false) { daoErrorLog(__FILE__, __LINE__, __METHOD__, __CLASS__, __FUNCTION__, $this->db->ErrorMsg(), "person person_status insert ((".$q."))"); }

			$q = "
				INSERT INTO person_details (
					p_uuid,
					birth_date,
					opt_race,
					opt_religion,
					opt_gender,
					years_old,
					minAge,
					maxAge,
					last_seen,
					last_clothing,
					other_comments )
				VALUES (
					".$this->sql_p_uuid.",
					".$this->sql_birth_date.",
					".$this->sql_opt_race.",
					".$this->sql_opt_religion.",
					".$this->sql_opt_gender.",
					".$this->sql_years_old.",
					".$this->sql_minAge.",
					".$this->sql_maxAge.",
					".$this->sql_last_seen.",
					".$this->sql_last_clothing.",
					".$this->sql_other_comments." );
			";
			$result = $this->db->Execute($q);
			if($result === false) { daoErrorLog(__FILE__, __LINE__, __METHOD__, __CLASS__, __FUNCTION__, $this->db->ErrorMsg(), "person person_details insert ((".$q."))"); }

			$q = "
				INSERT INTO  `person_to_report` (`p_uuid`, `rep_uuid`)
				VALUES (".$this->sql_p_uuid.", ".$this->sql_rep_uuid.");
			";
			$result = $this->db->Execute($q);
			if($result === false) { daoErrorLog(__FILE__, __LINE__, __METHOD__, __CLASS__, __FUNCTION__, $this->db->ErrorMsg(), "person person_to_report insert ((".$q."))"); }

			$this->insertImages();
			$this->insertEdxl();
			$this->makeStaticPfifNote();
			$this->insertVoiceNotes();
      $this->insertPersonNotes();
      
      /*
            // -------- HACK person_physical and contact updates ---------------
            $q = "
				INSERT INTO `person_physical` (
                    `p_uuid`,
                    `opt_blood_type`,
                    `height`, 
                    `weight`, 
                    `opt_eye_color`, 
                    `opt_skin_color`, 
                    `opt_hair_color`, 
                    `injuries`, 
                    `comments`)
                VALUES (
                    ".$this->p_uuid.",
                    ".$this->opt_blood_type.",
                    ".$this->height.",
                    ".$this->weight.",
                    ".$this->opt_eye_color.",
                    ".$this->opt_skin_color.",
                    ".$this->opt_hair_color.",
                    ".$this->injuries.",
                    ".$this->comments." );
			";
            $result = $this->db->Execute($q);
            if ($result === false) {
                daoErrorLog(__FILE__, __LINE__, __METHOD__, __CLASS__, __FUNCTION__, $this->db->ErrorMsg(), "person person_details update ((" . $q . "))");
            }
        */
            // to-do contact array updates later

			$this->saved = true;
			$this->modified = false;

			// keep arrival rate Statistics...
			updateArrivalRate($this->p_uuid, $this->incident_id, $this->arrival_triagepic, $this->arrival_reunite, $this->arrival_website, $this->arrival_pfif, $this->arrival_vanilla_email);
		}
	}


	private function insertImages() {

		foreach($this->images as $image) {
			$image->insert();
		}
	}


	private function insertEdxl() {

		if($this->edxl != null) {
			$this->edxl->insert();
		}
	}


	private function insertVoiceNotes() {

		foreach($this->voice_notes as $voice_note) {
			$voice_note->insert();
		}
	}


  private function insertPersonNotes() {

		foreach($this->person_notes as $person_note) {
			$person_note->insert();
		}
	}


	public function makeStaticPfifNote() {

		// make the note unless we are explicitly asked not to...
		if(!$this->makePfifNote) {
			return;
		}

		global $global;
		require_once($global['approot']."inc/lib_uuid.inc");
		require_once($global['approot']."mod/pfif/pfif.inc");
		require_once($global['approot']."mod/pfif/util.inc");

		$p = new Pfif();

		$n = new Pfif_Note();
		$n->note_record_id          = shn_create_uuid('pfif_note');
		$n->person_record_id        = $this->p_uuid;
		$n->linked_person_record_id = null;
		$n->source_date             = $this->last_updated; // since we are now creating the note,
		$n->entry_date              = $this->last_updated; // we use the last_updated for both values
		$n->author_phone            = null;
		$n->email_of_found_person   = null;
		$n->phone_of_found_person   = null;
		$n->last_known_location     = $this->last_seen;
		$n->text                    = $this->other_comments;
		$n->found                   = null; // we have no way to know if the reporter had direct contact (hence we leave this null)

		// figure out the person's pfif status
		$n->status = shn_map_status_to_pfif($this->opt_status);

		// find author name and email...
		$q = "
			SELECT *
			FROM contact c, person_uuid p
			WHERE p.p_uuid = c.p_uuid
			AND c.opt_contact_type = 'email'
			AND p.p_uuid = '".$this->rep_uuid."';
		";
		$result = $this->db->Execute($q);
		if($result === false) { daoErrorLog(__FILE__, __LINE__, __METHOD__, __CLASS__, __FUNCTION__, $this->db->ErrorMsg(), "person get contact for pfif note ((".$q."))"); }

		if($result != NULL && !$result->EOF) {
			$n->author_name  = $result->fields['full_name'];
			$n->author_email = $result->fields['contact_value'];
		} elseif($this->author_name != null) {
			$n->author_name  = $this->author_name;
			$n->author_email = $this->author_email;
		} else {
			$n->author_name  = null;
			$n->author_email = null;
		}

		$p->setNote($n);
		$p->setIncidentId($this->incident_id);
		$p->storeNotesInDatabase();
	}


	/** Delete Functions ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// Delete Functions ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// Delete Functions //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// */


	public function delete() {

		// just to mysql-ready the data nodes...
		$this->sync();

		$this->deleteImages();
		$this->deleteEdxl();
		$this->deleteVoiceNotes();
    $this->deletePersonNotes();

		$q = "
			DELETE FROM person_status
			WHERE  p_uuid = ".$this->sql_p_uuid.";
		";
		$result = $this->db->Execute($q);
		if($result === false) { daoErrorLog(__FILE__, __LINE__, __METHOD__, __CLASS__, __FUNCTION__, $this->db->ErrorMsg(), "person delete person 1 ((".$q."))"); }

		$q = "
			DELETE FROM person_details
			WHERE  p_uuid = ".$this->sql_p_uuid.";
		";
		$result = $this->db->Execute($q);
		if($result === false) { daoErrorLog(__FILE__, __LINE__, __METHOD__, __CLASS__, __FUNCTION__, $this->db->ErrorMsg(), "person delete person 2 ((".$q."))"); }

		$q = "
			DELETE FROM person_to_report
			WHERE  p_uuid = ".$this->sql_p_uuid.";
		";
		$result = $this->db->Execute($q);
		if($result === false) { daoErrorLog(__FILE__, __LINE__, __METHOD__, __CLASS__, __FUNCTION__, $this->db->ErrorMsg(), "person delete person 3 ((".$q."))"); }

		$q = "
			DELETE FROM person_uuid
			WHERE  p_uuid = ".$this->sql_p_uuid.";
		";
		$result = $this->db->Execute($q);
		if($result === false) { daoErrorLog(__FILE__, __LINE__, __METHOD__, __CLASS__, __FUNCTION__, $this->db->ErrorMsg(), "person delete person 4 ((".$q."))"); }

		$this->saved = false;

	  $this->deletePfif();
    
    // call a solr delete operation to remove this record from the search index
    global $conf;
    if($conf['enable_solr_for_search'] == true) {
    
      $solrQuery = $conf["SOLR_root"]."update?stream.body=<delete><id>".$this->p_uuid."</id></delete>&commit=true";
      $ch = curl_init();
      curl_setopt($ch, CURLOPT_URL, $solrQuery . "&wt=json");
      curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);
      curl_setopt($ch, CURLOPT_VERBOSE, 1);
      curl_setopt($ch, CURLOPT_PORT, $conf['SOLR_port']);
      $temp = json_decode(curl_exec($ch));
      curl_close($ch);
    }
	}


	private function deletePfif() {

		$q = "
			DELETE FROM pfif_person
			WHERE  p_uuid = ".$this->sql_p_uuid.";
		";
		$result = $this->db->Execute($q);
		if($result === false) { daoErrorLog(__FILE__, __LINE__, __METHOD__, __CLASS__, __FUNCTION__, $this->db->ErrorMsg(), "person delete pfif 1 ((".$q."))"); }

		$q = "
			DELETE FROM pfif_note
			WHERE  p_uuid = ".$this->sql_p_uuid.";
		";
		$result = $this->db->Execute($q);
		if($result === false) { daoErrorLog(__FILE__, __LINE__, __METHOD__, __CLASS__, __FUNCTION__, $this->db->ErrorMsg(), "person delete pfif 2 ((".$q."))"); }

		$q = "
			UPDATE pfif_note
			SET linked_person_record_id = NULL
			WHERE linked_person_record_id = ".$this->sql_p_uuid.";
		";
		$result = $this->db->Execute($q);
		if($result === false) { daoErrorLog(__FILE__, __LINE__, __METHOD__, __CLASS__, __FUNCTION__, $this->db->ErrorMsg(), "person delete pfif 3 ((".$q."))"); }

		$q = "
			CALL delete_reported_person('$this->p_uuid', 1);
		";
		$result = $this->db->Execute($q);
		if($result === false) { daoErrorLog(__FILE__, __LINE__, __METHOD__, __CLASS__, __FUNCTION__, $this->db->ErrorMsg(), "person delete pfif 4 ((".$q."))"); }
	}


	private function deleteImages() {

		foreach($this->images as $image) {
			$image->delete();
		}
	}


	private function deleteEdxl() {

		if($this->edxl != null) {
			$this->edxl->delete();
		}
	}


	private function deleteVoiceNotes() {

		foreach($this->voice_notes as $voice_note) {
			$voice_note->delete();
		}
	}


  private function deletePersonNotes() {

		foreach($this->person_notes as $person_note) {
			$person_note->delete();
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
			$this->saveRevisions();

			$q = "
				UPDATE person_uuid
				SET
					full_name       = ".$this->sql_full_name.",
					family_name     = ".$this->sql_family_name.",
					given_name      = ".$this->sql_given_name.",
					alternate_names = ".$this->sql_alternate_names.",
					profile_urls    = ".$this->sql_profile_urls.",
					incident_id     = ".$this->sql_incident_id.",
					hospital_uuid   = ".$this->sql_hospital_uuid.",
					expiry_date     = ".$this->sql_expiry_date."
				WHERE p_uuid = ".$this->sql_p_uuid.";
			";
			$result = $this->db->Execute($q);
			if($result === false) { daoErrorLog(__FILE__, __LINE__, __METHOD__, __CLASS__, __FUNCTION__, $this->db->ErrorMsg(), "person person_uuid update ((".$q."))"); }

			// we always update the last_updated to current time when saving(updating)
			$q = "
				UPDATE person_status
				SET
					opt_status      = ".$this->sql_opt_status.",
					last_updated    = '".date('Y-m-d H:i:s')."',
					creation_time   = ".$this->sql_creation_time.",
					last_updated_db = '".date('Y-m-d H:i:s')."',
					street1         = ".$this->sql_street1.",
					street2         = ".$this->sql_street2.",
					neighborhood    = ".$this->sql_neighborhood.",
					city            = ".$this->sql_city.",
					region          = ".$this->sql_region.",
					postal_code     = ".$this->sql_postal_code.",
					country         = ".$this->sql_country.",
					latitude        = ".$this->sql_latitude.",
					longitude       = ".$this->sql_longitude."

				WHERE p_uuid = ".$this->sql_p_uuid.";
			";
			$result = $this->db->Execute($q);
			if($result === false) { daoErrorLog(__FILE__, __LINE__, __METHOD__, __CLASS__, __FUNCTION__, $this->db->ErrorMsg(), "person person_status update ((".$q."))"); }

			$q = "
				UPDATE person_details
				SET
					birth_date     = ".$this->sql_birth_date.",
					opt_race       = ".$this->sql_opt_race.",
					opt_religion   = ".$this->sql_opt_religion.",
					opt_gender     = ".$this->sql_opt_gender.",
					years_old      = ".$this->sql_years_old.",
					minAge         = ".$this->sql_minAge.",
					maxAge         = ".$this->sql_maxAge.",
					last_seen      = ".$this->sql_last_seen.",
					last_clothing  = ".$this->sql_last_clothing.",
					other_comments = ".$this->sql_other_comments."
				WHERE p_uuid = ".$this->sql_p_uuid.";
			";
			$result = $this->db->Execute($q);
			if($result === false) { daoErrorLog(__FILE__, __LINE__, __METHOD__, __CLASS__, __FUNCTION__, $this->db->ErrorMsg(), "person person_details update ((".$q."))"); }


			$q = "
				UPDATE person_to_report
				SET
					rep_uuid = ".$this->sql_rep_uuid."
				WHERE p_uuid = ".$this->sql_p_uuid.";
			";
			$result = $this->db->Execute($q);
			if($result === false) { daoErrorLog(__FILE__, __LINE__, __METHOD__, __CLASS__, __FUNCTION__, $this->db->ErrorMsg(), "person person_to_report update ((".$q."))"); }

      $this->updatePersonNotes();
			$this->updateImages();
			$this->updateEdxl();
			$this->updatePfif();
			$this->updateVoiceNotes();

        /*
            // -------- HACK person_physical and contact updates ---------------
            $q = "
				UPDATE person_physical
				SET
					opt_blood_type  = ".$this->opt_blood_type.",
                    height          = ".$this->height.",
                    weight          = ".$this->weight.",
                    opt_eye_color   = ".$this->opt_eye_color.",
                    opt_skin_color  = ".$this->opt_skin_color.",
                    opt_hair_color  = ".$this->opt_hair_color.",
                    injuries        = ".$this->injuries.",
                    comments        = ".$this->comments.",
				WHERE p_uuid = ".$this->sql_p_uuid.";
			";
			$result = $this->db->Execute($q);
			if($result === false) { daoErrorLog(__FILE__, __LINE__, __METHOD__, __CLASS__, __FUNCTION__, $this->db->ErrorMsg(), "person person_details update ((".$q."))"); }
            
            // to-do contact array updates later
        */
            
			$this->modified = false;
		}
	}


	private function updateImages() {

		foreach($this->images as $image) {
			if($image !== null) {
				$image->updated_by_p_uuid = $this->updated_by_p_uuid;
				$image->update();
			}
		}
	}


	private function updateEdxl() {

		if($this->edxl != null) {
			$this->edxl->updated_by_p_uuid = $this->updated_by_p_uuid;
			$this->edxl->update();
		}
	}

	private function updatePfif() {
		// to do....
	}


	private function updateVoiceNotes() {

		foreach($this->voice_notes as $voice_note) {
			if($voice_note !== null) {
				$voice_note->updated_by_p_uuid = $this->updated_by_p_uuid;
				$voice_note->update();
			}
		}
	}


  private function updatePersonNotes() {

		foreach($this->person_notes as $person_note) {
			if($person_note !== null) {
				$person_note->update();
			}
		}
	}



	// save any changes since object was loaded as revisions
	function saveRevisions() {

		global $global;
		global $revisionCount;

		if($this->full_name       != $this->Ofull_name)       { $this->saveRevision($this->sql_full_name,       $this->sql_Ofull_name,       'person_uuid',      'full_name'     ); }
		if($this->family_name     != $this->Ofamily_name)     { $this->saveRevision($this->sql_family_name,     $this->sql_Ofamily_name,     'person_uuid',      'family_name'   ); }
		if($this->given_name      != $this->Ogiven_name)      { $this->saveRevision($this->sql_given_name,      $this->sql_Ogiven_name,      'person_uuid',      'given_name'    ); }
		if($this->alternate_names != $this->Oalternate_names) { $this->saveRevision($this->sql_alternate_names, $this->sql_Oalternate_names, 'person_uuid',      'alternate_names');}
		if($this->profile_urls    != $this->Oprofile_urls)    { $this->saveRevision($this->sql_profile_urls,    $this->sql_Oprofile_urls,    'person_uuid',      'profile_urls'  ); }
		if($this->incident_id     != $this->Oincident_id)     { $this->saveRevision($this->sql_incident_id,     $this->sql_Oincident_id,     'person_uuid',      'incident_id'   ); }

		if($this->hospital_uuid   != $this->Ohospital_uuid)   {
			$this->saveRevision($this->sql_hospital_uuid,   $this->sql_Ohospital_uuid,   'person_uuid',      'hospital_uuid' );
			// update person location if hospital changes
			
			$q = "
				SELECT *
				FROM hospital
				WHERE hospital_uuid = '".$this->hospital_uuid."';
			";
			$result = $this->db->Execute($q);
			if($result === false) {	daoErrorLog(__FILE__, __LINE__, __METHOD__, __CLASS__, __FUNCTION__, $this->db->ErrorMsg(), "savreRevision hospitalChangeLocation ((".$q."))");	}
			
			if($result != NULL && !$result->EOF) {
				
				$this->street1      = $result->fields['street1'];
				$this->street2      = $result->fields['street2'];
				$this->neighborhood = null;
				$this->city         = $result->fields['city'];
				$this->region       = $result->fields['region'];
				$this->postal_code  = $result->fields['postal_code'];
				$this->country      = $result->fields['country'];
				$this->latitude     = $result->fields['latitude'];
				$this->longitude    = $result->fields['longitude'];
				
			// invalid or null hospital
			} else {
				$this->street1      = null;
				$this->street2      = null;
				$this->neighborhood = null;
				$this->city         = null;
				$this->region       = null;
				$this->postal_code  = null;
				$this->country      = null;
				$this->latitude     = null;
				$this->longitude    = null;
			}
			// resync the changed address fields
			$this->sync();
		}

		if($this->expiry_date     != $this->Oexpiry_date)     { $this->saveRevision($this->sql_expiry_date,     $this->sql_Oexpiry_date,     'person_uuid',      'expiry_date'   ); }
		if($this->opt_status      != $this->Oopt_status)      { $this->saveRevision($this->sql_opt_status,      $this->sql_Oopt_status,      'person_status',    'opt_status'    ); }
		//if($this->last_updated   != $this->Olast_updated)   { $this->saveRevision($this->sql_last_updated,    $this->sql_Olast_updated,    'person_status',    'last_updated'  ); }
		if($this->creation_time   != $this->Ocreation_time)   { $this->saveRevision($this->sql_creation_time,   $this->sql_Ocreation_time,   'person_status',    'creation_time' ); }
		if($this->street1         != $this->Ostreet1)         { $this->saveRevision($this->sql_street1,         $this->sql_Ostreet1,         'person_status',    'street1'       ); }
		if($this->street2         != $this->Ostreet2)         { $this->saveRevision($this->sql_street2,         $this->sql_Ostreet2,         'person_status',    'street2'       ); }
		if($this->neighborhood    != $this->Oneighborhood)    { $this->saveRevision($this->sql_neighborhood,    $this->sql_Oneighborhood,    'person_status',    'neighborhood'  ); }
		if($this->city            != $this->Ocity)            { $this->saveRevision($this->sql_city,            $this->sql_Ocity,            'person_status',    'city'          ); }
		if($this->region          != $this->Oregion)          { $this->saveRevision($this->sql_region,          $this->sql_Oregion,          'person_status',    'region'        ); }
		if($this->postal_code     != $this->Opostal_code)     { $this->saveRevision($this->sql_postal_code,     $this->sql_Opostal_code,     'person_status',    'postal_code'   ); }
		if($this->country         != $this->Ocountry)         { $this->saveRevision($this->sql_country,         $this->sql_Ocountry,         'person_status',    'country'       ); }
		if($this->latitude        != $this->Olatitude)        { $this->saveRevision($this->sql_latitude,        $this->sql_Olatitude,        'person_status',    'latitude'      ); }
		if($this->longitude       != $this->Olongitude)       { $this->saveRevision($this->sql_longitude,       $this->sql_Olongitude,       'person_status',    'longitude'     ); }
		if($this->birth_date      != $this->Obirth_date)      { $this->saveRevision($this->sql_birth_date,      $this->sql_Obirth_date,      'person_details',   'birth_date'    ); }
		if($this->opt_race        != $this->Oopt_race)        { $this->saveRevision($this->sql_opt_race,        $this->sql_Oopt_race,        'person_details',   'opt_race'      ); }
		if($this->opt_religion    != $this->Oopt_religion)    { $this->saveRevision($this->sql_opt_religion,    $this->sql_Oopt_religion,    'person_details',   'opt_religion'  ); }
		if($this->opt_gender      != $this->Oopt_gender)      { $this->saveRevision($this->sql_opt_gender,      $this->sql_Oopt_gender,      'person_details',   'opt_gender'    ); }
		if($this->years_old       != $this->Oyears_old)       { $this->saveRevision($this->sql_years_old,       $this->sql_Oyears_old,       'person_details',   'years_old'     ); }
		if($this->minAge          != $this->OminAge)          { $this->saveRevision($this->sql_minAge,          $this->sql_OminAge,          'person_details',   'minAge'        ); }
		if($this->maxAge          != $this->OmaxAge)          { $this->saveRevision($this->sql_maxAge,          $this->sql_OmaxAge,          'person_details',   'maxAge'        ); }
		if($this->last_seen       != $this->Olast_seen)       { $this->saveRevision($this->sql_last_seen,       $this->sql_Olast_seen,       'person_details',   'last_seen'     ); makeStaticPfifNote(); }
		if($this->last_clothing   != $this->Olast_clothing)   { $this->saveRevision($this->sql_last_clothing,   $this->sql_Olast_clothing,   'person_details',   'last_clothing' ); }
		if($this->other_comments  != $this->Oother_comments)  { $this->saveRevision($this->sql_other_comments,  $this->sql_Oother_comments,  'person_details',   'other_comments'); }
		if($this->rep_uuid        != $this->Orep_uuid)        { $this->saveRevision($this->sql_rep_uuid,        $this->sql_Orep_uuid,        'person_to_report', 'rep_uuid'      ); }
		
        // -------------- HACK person_physical data updates --------------------
        if($this->opt_blood_type  != $this->Oopt_blood_type)  { $this->saveRevision($this->opt_blood_type,      $this->Oopt_blood_type,      'person_physical',  'opt_blood_type'); }
        if($this->height          != $this->Oheight)          { $this->saveRevision($this->height,              $this->Oheight,              'person_physical',  'height'        ); }
        if($this->weight          != $this->Oweight)          { $this->saveRevision($this->weight,              $this->Oweight,              'person_physical',  'weight'        ); }
        if($this->opt_eye_color   != $this->Oopt_eye_color)   { $this->saveRevision($this->opt_eye_color,       $this->Oopt_eye_color,       'person_physical',  'opt_eye_color' ); }
        if($this->opt_skin_color  != $this->Oopt_skin_color)  { $this->saveRevision($this->opt_skin_color,      $this->Oopt_skin_color,      'person_physical',  'opt_skin_color'); }
        if($this->opt_hair_color  != $this->Oopt_hair_color)  { $this->saveRevision($this->opt_hair_color,      $this->Oopt_hair_color,      'person_physical',  'opt_hair_color'); }
        if($this->injuries        != $this->Oinjuries)        { $this->saveRevision($this->injuries,            $this->Oinjuries,            'person_physical',  'injuries'      ); }
        if($this->comments        != $this->Ocomments)        { $this->saveRevision($this->comments,            $this->Ocomments,            'person_physical',  'comments'      ); }
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
		if($result === false) { daoErrorLog(__FILE__, __LINE__, __METHOD__, __CLASS__, __FUNCTION__, $this->db->ErrorMsg(), "person saveRevision ((".$q."))"); }
	}


	/** Other Members Functions //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// Other Members Functions //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// Other Members Functions /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// */


	// synchronize SQL value strings with public attributes
	private function sync() {

		global $global;
		
		// validate expiry_date
		if (date('Y-m-d H:i:s', strtotime($this->expiry_date)) == $this->expiry_date) {
			// passed!
		} else {
			$this->expiry_date = null;
		}
		

		// map enum types

		if($this->opt_gender == "M") {
			$this->opt_gender = "mal";

		} elseif($this->opt_gender == "F") {
			$this->opt_gender = "fml";

		} elseif($this->opt_gender == "C") {
			$this->opt_gender = "cpx";

		} elseif($this->opt_gender == "U") {
			$this->opt_gender = null;

		} elseif($this->opt_gender == "mal" || $this->opt_gender == "fml" || $this->opt_gender == "cpx") {
			// do nothing... we are good :)

		} else {
			$this->opt_gender = null;
		}

		$this->full_name = $this->given_name." ".$this->family_name;
		if($this->given_name === null) {
			$this->full_name = $this->family_name;
		}
		if($this->family_name === null) {
			$this->full_name = $this->given_name;
		}
		if($this->given_name === null && $this->family_name === null) {
			$this->full_name = null;
		}

		// build SQL value strings
		$this->sql_p_uuid         = ($this->p_uuid         === null) ? "NULL" : "'".mysql_real_escape_string((string)$this->p_uuid)."'";
		$this->sql_full_name      = ($this->full_name      === null) ? "NULL" : "'".htmlentities(mysql_real_escape_string((string)$this->full_name))."'";
		$this->sql_family_name    = ($this->family_name    === null) ? "NULL" : "'".htmlentities(mysql_real_escape_string((string)$this->family_name))."'";
		$this->sql_given_name     = ($this->given_name     === null) ? "NULL" : "'".htmlentities(mysql_real_escape_string((string)$this->given_name))."'";
		$this->sql_alternate_names = ($this->alternate_names === null) ? "NULL" : "'".htmlentities(mysql_real_escape_string((string)$this->alternate_names))."'";
		$this->sql_profile_urls   = ($this->profile_urls   === null) ? "NULL" : "'".htmlentities(mysql_real_escape_string((string)$this->profile_urls))."'";
		$this->sql_incident_id    = ($this->incident_id    === null) ? "NULL" : (int)$this->incident_id;
		$this->sql_hospital_uuid  = ($this->hospital_uuid  === null) ? "NULL" : "'".mysql_real_escape_string((string)$this->hospital_uuid)."'";
		$this->sql_expiry_date    = ($this->expiry_date    === null) ? "NULL" : "'".htmlentities(mysql_real_escape_string((string)$this->expiry_date))."'";

		$this->sql_opt_status     = ($this->opt_status     === null) ? "NULL" : "'".htmlentities(mysql_real_escape_string((string)$this->opt_status))."'";
		$this->sql_last_updated   = ($this->last_updated   === null) ? "NULL" : "'".htmlentities(mysql_real_escape_string((string)$this->last_updated))."'";
		$this->sql_creation_time  = ($this->creation_time  === null) ? "NULL" : "'".htmlentities(mysql_real_escape_string((string)$this->creation_time))."'";
		
		$this->sql_street1        = ($this->street1        === null) ? "NULL" : "'".htmlentities(mysql_real_escape_string((string)$this->street1))."'";
		$this->sql_street2        = ($this->street2        === null) ? "NULL" : "'".htmlentities(mysql_real_escape_string((string)$this->street2))."'";
		$this->sql_neighborhood   = ($this->neighborhood   === null) ? "NULL" : "'".htmlentities(mysql_real_escape_string((string)$this->neighborhood))."'";
		$this->sql_city           = ($this->city           === null) ? "NULL" : "'".htmlentities(mysql_real_escape_string((string)$this->city))."'";
		$this->sql_region         = ($this->region         === null) ? "NULL" : "'".htmlentities(mysql_real_escape_string((string)$this->region))."'";
		$this->sql_postal_code    = ($this->postal_code    === null) ? "NULL" : "'".htmlentities(mysql_real_escape_string((string)$this->postal_code))."'";
		$this->sql_country        = ($this->country        === null) ? "NULL" : "'".htmlentities(mysql_real_escape_string((string)$this->country))."'";
		$this->sql_latitude       = ($this->latitude       === null) ? "NULL" : "'".htmlentities(mysql_real_escape_string((string)$this->latitude))."'";
		$this->sql_longitude      = ($this->longitude      === null) ? "NULL" : "'".htmlentities(mysql_real_escape_string((string)$this->longitude))."'";

		$this->sql_birth_date     = ($this->birth_date     === null) ? "NULL" : "'".htmlentities(mysql_real_escape_string((string)$this->birth_date))."'";
		$this->sql_opt_race       = ($this->opt_race       === null) ? "NULL" : "'".htmlentities(mysql_real_escape_string((string)$this->opt_race))."'";
		$this->sql_opt_religion   = ($this->opt_religion   === null) ? "NULL" : "'".htmlentities(mysql_real_escape_string((string)$this->opt_religion))."'";
		$this->sql_opt_gender     = ($this->opt_gender     === null) ? "NULL" : "'".htmlentities(mysql_real_escape_string((string)$this->opt_gender))."'";
		$this->sql_years_old      = ($this->years_old      === null) ? "NULL" : (int)$this->years_old;
		$this->sql_minAge         = ($this->minAge         === null) ? "NULL" : (int)$this->minAge;
		$this->sql_maxAge         = ($this->maxAge         === null) ? "NULL" : (int)$this->maxAge;
		$this->sql_last_seen      = ($this->last_seen      === null) ? "NULL" : "'".htmlentities(mysql_real_escape_string((string)$this->last_seen))."'";
		$this->sql_last_clothing  = ($this->last_clothing  === null) ? "NULL" : "'".htmlentities(mysql_real_escape_string((string)$this->last_clothing))."'";
		$this->sql_other_comments = ($this->other_comments === null) ? "NULL" : "'".htmlentities(mysql_real_escape_string((string)$this->other_comments))."'";

		$this->sql_rep_uuid       = ($this->rep_uuid       === null) ? "NULL" : "'".mysql_real_escape_string((string)$this->rep_uuid)."'";

		// do the same for original values...
		$this->sql_Op_uuid         = ($this->Op_uuid         === null) ? "NULL" : "'".htmlentities(mysql_real_escape_string((string)$this->Op_uuid))."'";
		$this->sql_Ofull_name      = ($this->Ofull_name      === null) ? "NULL" : "'".htmlentities(mysql_real_escape_string((string)$this->Ofull_name))."'";
		$this->sql_Ofamily_name    = ($this->Ofamily_name    === null) ? "NULL" : "'".htmlentities(mysql_real_escape_string((string)$this->Ofamily_name))."'";
		$this->sql_Ogiven_name     = ($this->Ogiven_name     === null) ? "NULL" : "'".htmlentities(mysql_real_escape_string((string)$this->Ogiven_name))."'";
		$this->sql_Oalternate_names = ($this->Oalternate_names === null) ? "NULL" : "'".htmlentities(mysql_real_escape_string((string)$this->Oalternate_names))."'";
		$this->sql_Oprofile_urls   = ($this->Oprofile_urls   === null) ? "NULL" : "'".htmlentities(mysql_real_escape_string((string)$this->Oprofile_urls))."'";
		$this->sql_Oincident_id    = ($this->Oincident_id    === null) ? "NULL" : (int)$this->Oincident_id;
		$this->sql_Ohospital_uuid  = ($this->Ohospital_uuid  === null) ? "NULL" : "'".mysql_real_escape_string((string)$this->Ohospital_uuid)."'";
		$this->sql_Oexpiry_date    = ($this->Oexpiry_date    === null) ? "NULL" : "'".htmlentities(mysql_real_escape_string((string)$this->Oexpiry_date))."'";

		$this->sql_Oopt_status     = ($this->Oopt_status     === null) ? "NULL" : "'".htmlentities(mysql_real_escape_string((string)$this->Oopt_status))."'";
		$this->sql_Olast_updated   = ($this->Olast_updated   === null) ? "NULL" : "'".htmlentities(mysql_real_escape_string((string)$this->Olast_updated))."'";
		$this->sql_Ocreation_time  = ($this->Ocreation_time  === null) ? "NULL" : "'".htmlentities(mysql_real_escape_string((string)$this->Ocreation_time))."'";
		
		$this->sql_Ostreet1        = ($this->Ostreet1        === null) ? "NULL" : "'".htmlentities(mysql_real_escape_string((string)$this->Ostreet1))."'";
		$this->sql_Ostreet2        = ($this->Ostreet2        === null) ? "NULL" : "'".htmlentities(mysql_real_escape_string((string)$this->Ostreet2))."'";
		$this->sql_Oneighborhood   = ($this->Oneighborhood   === null) ? "NULL" : "'".htmlentities(mysql_real_escape_string((string)$this->Oneighborhood))."'";
		$this->sql_Ocity           = ($this->Ocity           === null) ? "NULL" : "'".htmlentities(mysql_real_escape_string((string)$this->Ocity))."'";
		$this->sql_Oregion         = ($this->Oregion         === null) ? "NULL" : "'".htmlentities(mysql_real_escape_string((string)$this->Oregion))."'";
		$this->sql_Opostal_code    = ($this->Opostal_code    === null) ? "NULL" : "'".htmlentities(mysql_real_escape_string((string)$this->Opostal_code))."'";
		$this->sql_Ocountry        = ($this->Ocountry        === null) ? "NULL" : "'".htmlentities(mysql_real_escape_string((string)$this->Ocountry))."'";
		$this->sql_Olatitude       = ($this->Olatitude       === null) ? "NULL" : "'".htmlentities(mysql_real_escape_string((string)$this->Olatitude))."'";
		$this->sql_Olongitude      = ($this->Olongitude      === null) ? "NULL" : "'".htmlentities(mysql_real_escape_string((string)$this->Olongitude))."'";
		
		$this->sql_Obirth_date     = ($this->Obirth_date     === null) ? "NULL" : "'".htmlentities(mysql_real_escape_string((string)$this->Obirth_date))."'";
		$this->sql_Oopt_race       = ($this->Oopt_race       === null) ? "NULL" : "'".htmlentities(mysql_real_escape_string((string)$this->Oopt_race))."'";
		$this->sql_Oopt_religion   = ($this->Oopt_religion   === null) ? "NULL" : "'".htmlentities(mysql_real_escape_string((string)$this->Oopt_religion))."'";
		$this->sql_Oopt_gender     = ($this->Oopt_gender     === null) ? "NULL" : "'".htmlentities(mysql_real_escape_string((string)$this->Oopt_gender))."'";
		$this->sql_Oyears_old      = ($this->Oyears_old      === null) ? "NULL" : (int)$this->Oyears_old;
		$this->sql_OminAge         = ($this->OminAge         === null) ? "NULL" : (int)$this->OminAge;
		$this->sql_OmaxAge         = ($this->OmaxAge         === null) ? "NULL" : (int)$this->OmaxAge;
		$this->sql_Olast_seen      = ($this->Olast_seen      === null) ? "NULL" : "'".htmlentities(mysql_real_escape_string((string)$this->Olast_seen))."'";
		$this->sql_Olast_clothing  = ($this->Olast_clothing  === null) ? "NULL" : "'".htmlentities(mysql_real_escape_string((string)$this->Olast_clothing))."'";
		$this->sql_Oother_comments = ($this->Oother_comments === null) ? "NULL" : "'".htmlentities(mysql_real_escape_string((string)$this->Oother_comments))."'";

		$this->sql_Orep_uuid       = ($this->Orep_uuid       === null) ? "NULL" : "'".mysql_real_escape_string((string)$this->Orep_uuid)."'";
	}


	// expires a person...
	function expire($user_id, $explanation) {

		// first we clear out all pending expiration requests...
		$q = "
			DELETE FROM expiry_queue
			WHERE p_uuid = ".$this->sql_p_uuid.";
		";
		$result = $this->db->Execute($q);
		if($result === false) { daoErrorLog(__FILE__, __LINE__, __METHOD__, __CLASS__, __FUNCTION__, $this->db->ErrorMsg(), "person expire 1 ((".$q."))"); }

		// next we insert a row to indicate who expired this person record
		$q = "
			INSERT into expiry_queue (`p_uuid`, `requested_by_user_id`, `requested_when`, `queued`, `approved_by_user_id`, `approved_when`, `approved_why`, `expired`)
			VALUES (".$this->sql_p_uuid.", NULL, NULL, 0, '".$user_id."', ".$this->sql_expiry_date.", '".htmlentities(mysql_real_escape_string($explanation))."', 1);
		";
		$result = $this->db->Execute($q);
		if($result === false) { daoErrorLog(__FILE__, __LINE__, __METHOD__, __CLASS__, __FUNCTION__, $this->db->ErrorMsg(), "person expire 2 ((".$q."))"); }
		
		// then we set the expiration time to now
		$this->expiry_date = date('Y-m-d H:i:s');
		
		// lastly, when a person is expired, we randomize the image (full and thumb) urls to make it impossible to find the image of an expired person
		foreach($this->images as $image) {
			$image->randomize();
		}
		
		// save all changes
		$this->update();
	}


	// queues the expiration of a person
	function expireQueue($user_id, $explanation) {

		$this->sync();
		$q = "
			INSERT into expiry_queue (`p_uuid`, `requested_by_user_id`, `requested_when`, `requested_why`, `queued`, `approved_by_user_id`, `approved_when`, `expired`)
			VALUES (".$this->sql_p_uuid.", '".$user_id."', '".date('Y-m-d H:i:s')."', '".htmlentities(mysql_real_escape_string($explanation))."', 1, NULL, NULL, 0);
		";
		$result = $this->db->Execute($q);
		if($result === false) { daoErrorLog(__FILE__, __LINE__, __METHOD__, __CLASS__, __FUNCTION__, $this->db->ErrorMsg(), "person expireQueue ((".$q."))"); }
	}


	// updates a expiry_date
	function setExpiryDate($expiryDate) {

		$this->expiry_date = $expiryDate;
		$this->update();
	}


	// updates a expiry_date to one year from now
	function setExpiryDateOneYear() {

		$this->expiry_date = date('Y-m-d H:i:s', time()+(60*60*24*365));
		$this->update();
	}


	// checks if the person record has already expired (expiry_date is in the past)
	function isAlreadyExpired() {

		$currentTime = date('Y-m-d H:i:s');

		$d1 = new DateTime($this->expiry_date);
		$d2 = new DateTime($currentTime);

		if($this->expiry_date === null) {
			return false;
		} else if($d1 == $d2 || $d1 < $d2) {
			return true;
		} else if($d1 > $d2) {
			return false;
		}
	}


	// sets a new massCasualtyId on a person... HACK!!!!!
	function setMassCasualtyId($newMcid) {

		// we must revise this to work once DAO load/update is completed on all objects!!!
		// HACK REMOVAL NOTICE !!! REMOVE THIS HACK SOMEDAY!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!11111111111111

		$this->sync();

		$q = "
			UPDATE edxl_co_lpf
			SET person_id = '".htmlentities(mysql_real_escape_string($newMcid))."'
			WHERE p_uuid = ".$this->sql_p_uuid.";
		";
		$result = $this->db->Execute($q);
		if($result === false) { daoErrorLog(__FILE__, __LINE__, __METHOD__, __CLASS__, __FUNCTION__, $this->db->ErrorMsg(), "person setMassCasualtyId HACK 1 ((".$q."))"); }

		// note the revision
		$q = "
			INSERT into person_updates (`p_uuid`, `updated_table`, `updated_column`, `old_value`, `new_value`, `updated_by_p_uuid`)
			VALUES (".$this->sql_p_uuid.", 'edxl_co_lpf', 'person_id', 'NOT_YET_SET', '".htmlentities(mysql_real_escape_string($newMcid))."', '".$this->updated_by_p_uuid."');
		";
		$result = $this->db->Execute($q);
		if($result === false) { daoErrorLog(__FILE__, __LINE__, __METHOD__, __CLASS__, __FUNCTION__, $this->db->ErrorMsg(), "person setMassCasultyId HACK 2 ((".$q."))"); }
	}


	// insert into plus_log
	public function plusReportLog() {

		$q = "
			INSERT INTO plus_report_log (p_uuid, enum)
			VALUES ('".$this->p_uuid."', '".$this->xmlFormat."');
		";
		$result = $this->db->Execute($q);
		if($result === false) { daoErrorLog(__FILE__, __LINE__, __METHOD__, __CLASS__, __FUNCTION__, $this->db->ErrorMsg(), "person plus_report insert ((".$q."))"); }
	}


	// insert into rap_log
	public function rapReportLog() {

		$q = "
			INSERT INTO rap_log (p_uuid)
			VALUES ('".$this->p_uuid."');
		";
		$result = $this->db->Execute($q);
		if($result === false) { daoErrorLog(__FILE__, __LINE__, __METHOD__, __CLASS__, __FUNCTION__, $this->db->ErrorMsg(), "person rap_log insert ((".$q."))"); }
	}



	public function isEventOpen() {

		// find if this event is open/closed
		$q = "
			SELECT *
			FROM incident
			WHERE incident_id = '".$this->incident_id."';
		";
		$result = $this->db->Execute($q);
		if($result === false) { daoErrorLog(__FILE__, __LINE__, __METHOD__, __CLASS__, __FUNCTION__, $this->db->ErrorMsg(), "person check event open ((".$q."))"); }

		if($result != NULL && !$result->EOF) {
			$row = $result->FetchRow();
		} else {
			return false;
		}
		if($row['closed'] != 0) {
			return false;
		} else {
			return true;
		}
	}


	public function getOwner() {

		// find the username of the user to report this person
		$q = "
			SELECT *
			FROM `users`
			WHERE p_uuid = '".$this->rep_uuid."';
		";
		$result = $this->db->Execute($q);
		if($result === false) { daoErrorLog(__FILE__, __LINE__, __METHOD__, __CLASS__, __FUNCTION__, $this->db->ErrorMsg(), "person getOwner ((".$q."))"); }
		if($result != NULL && !$result->EOF) {
			$row = $result->FetchRow();
		} else {
			return false;
		}
		return $row['user_name'];
	}



	public function getRevisionPermissionGroupIDs() {

		// for now this is a hack, we allow records to be revised by admin, hs, hsa
		// so, we will just report this back
		// in the future, a better group manager will allow finer grained control of record revisions
		$list = array();
		$list[] = 1;
		$list[] = 5;
		$list[] = 6;
		return(json_encode($list));
	}


	public function addImage($fileContentBase64, $filename) {

		if(trim($fileContentBase64) != "") {
			$i = new personImage();
			$i->init();
			$i->p_uuid = $this->p_uuid;
			$i->fileContentBase64 = $fileContentBase64;
			$i->original_filename = $filename;
			$this->images[] = $i;
		}
	}


  public function addComment($comment, $suggested_status, $suggested_location, $suggested_image, $authorUuid) {

    $pn = new personNote();
    $pn->init();
    $pn->note_about_p_uuid = $this->p_uuid;
    $pn->note_written_by_p_uuid = $authorUuid;
    $pn->note = $comment;
    $pn->when = date('Y-m-d H:i:s', time());
    $pn->suggested_status = $suggested_status;
    $pn->suggested_location = $suggested_location;
    $pn->insert();
    if(($suggested_image != null) && (trim($suggested_image) != "")) {
      $i = new personImage();
      $i->init();
			$i->p_uuid = $this->p_uuid;
			$i->fileContentBase64 = $suggested_image;
			$i->original_filename = null;
      $i->note_id = $pn->note_id;
      $i->principal = 0;
			$this->images[] = $i;
		}
  }


	public function createUUID() {

		if($this->p_uuid === null || $this->p_uuid == "") {
			$this->p_uuid = shn_create_uuid("person");
		}
	}


	// set the event id (which will be ignored by XML parser)
	public function setEvent($eventShortName) {

		$q = "
			SELECT *
			FROM `incident`
			WHERE shortname = '".mysql_real_escape_string($eventShortName)."';
		";
		$result = $this->db->Execute($q);
		if($result === false) { daoErrorLog(__FILE__, __LINE__, __METHOD__, __CLASS__, __FUNCTION__, $this->db->ErrorMsg(), "person get incident ((".$q."))"); }

		$this->incident_id = $result->fields['incident_id'];
	}


	// cleanse data to improve integrity
	private function cleanInput() {

		// zero pad the patient_id string if hospital says we should...
		if($this->hospital_uuid != null) {

			// strip the hospital prefix from the patient_id (we will replace it with our own)
			$tmp = $this->edxl->person_id = explode("-", $this->edxl->person_id);
			$this->edxl->person_id = $tmp[sizeof($tmp)-1];

			// no prefix...
			if(sizeof($tmp) == 1) {
				$prefix = null;
			} else {
				$prefix = $tmp[0]."-";
			}

			$q  = "
				SELECT *
				FROM hospital
				WHERE hospital_uuid = '".$this->hospital_uuid."';
			";

			$result = $this->db->Execute($q);
			if($result === false) { daoErrorLog(__FILE__, __LINE__, __METHOD__, __CLASS__, __FUNCTION__, $this->db->ErrorMsg(), "cleanData after parseXML ((".$q."))"); }
			$row = $result->FetchRow();

			if((int)$row['patient_id_suffix_variable'] == 0) {
				$this->edxl->person_id = str_pad($this->edxl->person_id, $row['patient_id_suffix_fixed_length'], "0", STR_PAD_LEFT);
			}

			// re-add the prefix...
			// save practice prefixes (dont over-write them)
			if($prefix == "Practice-") {
				$this->edxl->person_id = $prefix.$this->edxl->person_id;
			} else {
			// detect triagepic misbehaving and log it...
				if($prefix != $row['patient_id_prefix']) {
					daoErrorLog('', '', '', '', '', '', "TP misbehaving! sent prefix(".$prefix.") and HA has prefix(".$row['patient_id_prefix'].")");
				}
				$this->edxl->person_id = $row['patient_id_prefix'].$this->edxl->person_id;
			}
		}
	}
	
	
	public function makeArrayObject() {
		$r = array();
		
		$r['p_uuid']         = $this->p_uuid;
		$r['full_name']      = $this->full_name;
		$r['family_name']    = $this->family_name;
		$r['given_name']     = $this->given_name;
		$r['alternate_names'] = $this->alternate_names;
		$r['profile_urls']   = $this->profile_urls;
		$r['incident_id']    = $this->incident_id;
		$r['hospital_uuid']  = $this->hospital_uuid;

		$this->expiry_date == null ? $r['expiry_date'] = null : $r['expiry_date'] = date('c', strtotime($this->expiry_date));
		
		$r['opt_status']     = $this->opt_status;

		$this->last_updated == null ? $r['last_updated'] = null : $r['last_updated'] = date('c', strtotime($this->last_updated));
		$this->creation_time == null ? $r['creation_time'] = null : $r['creation_time'] = date('c', strtotime($this->creation_time));
		
		$r['location'] = array();
		$r['location']['street1']        = $this->street1;
		$r['location']['street2']        = $this->street2;
		$r['location']['neighborhood']   = $this->neighborhood;
		$r['location']['city']           = $this->city;
		$r['location']['region']         = $this->region;
		$r['location']['postal_code']    = $this->postal_code;
		$r['location']['country']        = $this->country;

		$r['location']['gps'] = array();
		$r['location']['gps']['latitude']       = $this->latitude;
		$r['location']['gps']['longitude']      = $this->longitude;
		
		$r['birth_date']     = $this->birth_date;
		$r['opt_race']       = $this->opt_race;
		$r['opt_religion']   = $this->opt_religion;
		$r['opt_gender']     = $this->opt_gender;
		$r['years_old']      = $this->years_old;
		$r['min_age']        = $this->minAge;
		$r['max_age']        = $this->maxAge;
		$r['last_seen']      = $this->last_seen;
		$r['last_clothing']  = $this->last_clothing;
		$r['other_comments'] = $this->other_comments;
		$r['rep_uuid']       = $this->rep_uuid;

		$i = array();
		// add primary image first
		foreach($this->images as $image) {
			if($image->principal == 1) {
				$i[] = $image->makeArrayObject();
			}
		}
    
		// add secondary images now
		foreach($this->images as $image) {
			if($image->principal != 1) {
				$i[] = $image->makeArrayObject();
			}
		}
		$r['images'] = $i;

    // add voice notes
		$v = array();
		foreach($this->voice_notes as $voice_note) {
			$v[] = $voice_note->makeArrayObject();
		}
		$r['voice_notes'] = $v;
		
    // add person notes
    $pn = array();
		foreach($this->person_notes as $person_note) {
			$pn[] = $person_note->makeArrayObject();
		}
		$r['person_notes'] = $pn;
    
		return $r;
	}

	
	public function parseXml($reParse = false) {

		global $global;
		require_once($global['approot']."inc/lib_uuid.inc");

		// save xml for debugging?
		if($global['debugAndSaveXmlToFile'] == true) {
			$filename = date("Y_md_H-i-s.".getMicrotimeComponent())."___".mt_rand().".xml"; // 2012_0402_17-50-33.454312___437849328789.xml
			$path = $global['debugAndSaveXmlToFilePath'].$filename;
			file_put_contents($path, $this->theString);
		}

		// is this a supported XML type?
		if(!in_array((string)trim($this->xmlFormat), $global['enumXmlFormats'])) {
			return (int)400;
		}

		/** parse ZONER0 JSON ***********************************************************************************************************/
			
		if($this->xmlFormat == "ZONER0") {

			if(!$reParse) {
				$this->createUUID();
				$this->arrival_triagepic = true;
			}
			
			$r = json_decode($this->theString, true);
			//error_log(print_r($r, true));
			
			$this->given_name = $r['given'];
			$this->family_name = $r['family'];
			$this->creation_time = date('Y-m-d H:i:s', time());
      $this->last_updated  = date('Y-m-d H:i:s', time());
			$this->expiry_date = date('Y-m-d H:i:s', time()+(60*60*24*365));
			$this->opt_status = 'mis';

			if($r['imageData'] != null && trim($r['imageData']) != "") {
				$i = new personImage();
				$i->init();
				$i->p_uuid = $this->p_uuid;
				$i->fileContentBase64 = $r['imageData'];
				$i->decode();
				$i->principal = 1;
				$this->images[] = $i;
			}

			
		} else {

			$aa = new XMLParser();
			$aa->rawXML = $this->theString;
			$aa->parse();

			 // error out for failing to parse xml
			if($aa->getError()) {
				return (int)403;
			}
	
			$a = $aa->getArray();
	
			/** HANDLE REUNITE4 XMLs *************************************************************************************************************************/
	
			if($this->xmlFormat == "REUNITE4") {
	
				if(!$reParse) {
					$this->createUUID();
					$this->arrival_reunite = true;
				}
				$this->given_name     = isset($a['person']['givenName'])  ? $a['person']['givenName']  : null;
				$this->family_name    = isset($a['person']['familyName']) ? $a['person']['familyName'] : null;
				$this->opt_status     = isset($a['person']['status'])     ? $a['person']['status']     : null;
				
				$this->street1        = isset($a['person']['location']['street1'])      ? $a['person']['location']['street1']      : null;
				$this->street2        = isset($a['person']['location']['street2'])      ? $a['person']['location']['street2']      : null;
				$this->neighborhood   = isset($a['person']['location']['neighborhood']) ? $a['person']['location']['neighborhood'] : null;
				$this->city           = isset($a['person']['location']['city'])         ? $a['person']['location']['city']         : null;
				$this->region         = isset($a['person']['location']['region'])       ? $a['person']['location']['region']       : null;
				$this->postal_code    = isset($a['person']['location']['postalCode'])   ? $a['person']['location']['postalCode']   : null;
				$this->country        = isset($a['person']['location']['country'])      ? $a['person']['location']['country']      : null;
				
				// We Kluge the location data into this old field... even after its mno longer used in EAPv2
				$kluge = "";
				$kluge .= isset($a['person']['location']['street1'])      ? $a['person']['location']['street1']."\n"      : "";
				$kluge .= isset($a['person']['location']['street2'])      ? $a['person']['location']['street2']."\n"      : "";
				$kluge .= isset($a['person']['location']['neighborhood']) ? $a['person']['location']['neighborhood']."\n" : "";
				$kluge .= isset($a['person']['location']['city'])         ? $a['person']['location']['city']."\n"         : "";
				$kluge .= isset($a['person']['location']['region'])       ? $a['person']['location']['region']."\n"       : "";
				$kluge .= isset($a['person']['location']['postalCode'])   ? $a['person']['location']['postalCode']."\n"   : "";
				$kluge .= isset($a['person']['location']['country'])      ? $a['person']['location']['country']."\n"      : "";
				if(trim($kluge) != "") {
					$this->last_seen = $kluge;
				}
					
				
				$this->latitude       = isset($a['person']['location']['gps']['lat'])   ? $a['person']['location']['gps']['lat']   : null;
				$this->longitude      = isset($a['person']['location']['gps']['lon'])   ? $a['person']['location']['gps']['lon']   : null;
				
				$this->last_updated   = date('Y-m-d H:i:s');
	
				// only handle creation times on a new report
				if(!$reParse) {
					// sanitize datetime ~ remove all non-latin characters
					$datetime      = isset($a['person']['dateTimeSent']) ? $a['person']['dateTimeSent'] : null;
					if($datetime !== null ) {
						$datetime = preg_replace('/[^\x{0000}-\x{007F}A-Za-z !@#$%^&*()]/u','', $datetime);
					}
					$timezoneUTC   = new DateTimeZone("UTC");
					$timezoneLocal = new DateTimeZone(date_default_timezone_get());
					$datetime2     = new DateTime();
					$datetime2->setTimezone($timezoneUTC);
					$datetime2->setTimestamp(strtotime($datetime));
					$datetime2->setTimezone($timezoneLocal);
					$this->creation_time = $datetime2->format('Y-m-d H:i:s');
					if($this->creation_time == "1969-12-31 19:00:00") {
						$this->creation_time = date('Y-m-d H:i:s', time());
					}
				}
	
				// sanitize datetime ~ remove all non-latin characters
				$datetime3 = isset($a['person']['expiryDate']) ? $a['person']['expiryDate'] : null;
				if($datetime3 !== null ) {
					$datetime3 = preg_replace('/[^\x{0000}-\x{007F}A-Za-z !@#$%^&*()]/u','', $datetime3);
				}
				$timezoneUTC2   = new DateTimeZone("UTC");
				$timezoneLocal2 = new DateTimeZone(date_default_timezone_get());
				$datetime4     = new DateTime();
				$datetime4->setTimezone($timezoneUTC2);
				$datetime4->setTimestamp(strtotime($datetime3));
				$datetime4->setTimezone($timezoneLocal2);
				$this->expiry_date = $datetime4->format('Y-m-d H:i:s');
				if($this->expiry_date == "1969-12-31 19:00:00") {
					$this->expiry_date = date('Y-m-d H:i:s', time()+(60*60*24*365));
				}
	
	
				$this->opt_gender     = isset($a['person']['gender'])       ? $a['person']['gender']       : null;
				$this->years_old      = isset($a['person']['estimatedAge']) ? $a['person']['estimatedAge'] : null;
				$this->minAge         = isset($a['person']['minAge'])       ? $a['person']['minAge']       : null;
				$this->maxAge         = isset($a['person']['maxAge'])       ? $a['person']['maxAge']       : null;
				$this->other_comments = isset($a['person']['note'])         ? $a['person']['note']         : null;
	
				// only update the incident_id if not already set
				if($this->incident_id === null) {
	
					$q = "
						SELECT *
						FROM incident
						WHERE shortname = '".mysql_real_escape_string((string)$a['person']['eventShortname'])."';
					";
					$result = $this->db->Execute($q);
					if($result === false) { daoErrorLog(__FILE__, __LINE__, __METHOD__, __CLASS__, __FUNCTION__, $this->db->ErrorMsg(), "person get incident ((".$q."))"); }
	
					$this->incident_id = $result->fields["incident_id"];
				}
	
				/** HANDLE IMAGE ADDITIONS ********************************************************************************************/
	
				// we allow only 1 primary image, so the first one we find is the principal image
				$foundPrincipal = false;
	
				if(isset($a['person']['photos']['photo']) || isset($a['person']['photos'][0])) {
					foreach($a['person']['photos'] as $photo) {
						if(trim($photo['data']) != "") {
							$i = new personImage();
							$i->init();
							$i->p_uuid = $this->p_uuid;
							$i->fileContentBase64 = $photo['data'];
							$i->decode();
							if(isset($photo['primary']) && (int)$photo['primary'] == 1) {
								$i->principal = 1;
								$foundPrincipal = true;
							} else {
								$i->principal = 0;
							}
	
							if(isset($photo['tags'])) {
								foreach($photo['tags'] as $tag) {
									$t = new personImageTag();
									$t->init();
									$t->p_uuid   = $this->p_uuid;
									$t->image_id = $i->image_id;
									$t->tag_x    = isset($tag['x'])    ? $tag['x']    : null;
									$t->tag_y    = isset($tag['y'])    ? $tag['y']    : null;
									$t->tag_w    = isset($tag['w'])    ? $tag['w']    : null;
									$t->tag_h    = isset($tag['h'])    ? $tag['h']    : null;
									$t->tag_text = isset($tag['text']) ? $tag['text'] : null;
									$i->tags[] = $t;
								}
							}
	
							// if this new image has a sha1 = to an already present image for this person, it is a duplicate
							if(!$this->isImageDuplicate($i->sha1original)) {
	
								
								
								// HUGE HACK!$#%G%%#$%$#%$#%$#@
								// IF REUNITE 3.0.2 reports a new image, make it primary and all the rest 2ndary
								// HACK BEGIN
									
								if(isset($_SERVER['HTTP_USER_AGENT'])) {
									$agent  = explode(" ", $_SERVER['HTTP_USER_AGENT']);
									$appver = explode("/", $agent[0]);
									$app    = "'".mysql_real_escape_string($appver[0])."'";
									$ver    = "'".mysql_real_escape_string($appver[1])."'";
								} else {
									$app = "null";
									$ver = "null";
								}
								
								if($app == "'ReUnite'" && $ver == "'3.0.2'") {
								
									// make the new image primary
									$i->principal = 1;
	
									// mark all other images as secondary
									foreach($this->images as $g) {
										$g->principal = 0;
									}
								}
									
								// HACK END
								
								
								
								$this->images[] = $i;
								// if this is a reParse, log the revision (addition of new image)
								if($reParse) {
									$this->saveRevisionNewImage();
								}
							
							} else {
								$this->ecode = 422;
								$i = null;
							}
						}
					}
				}
	
				/** HANDLE IMAGE DELETIONS ********************************************************************************************/
	
				// only check for deletions on reParse
				if($reParse) {
	
					$delCount = 0;
	
					if(isset($a['person']['deletePhotos']['photo']) || isset($a['person']['deletePhotos'][0])) {
	
						foreach($a['person']['deletePhotos'] as $dtag) {
	
							$x = 0;
							foreach($this->images as $idel) {
	
								if($idel->sha1original == $dtag) {
									$this->images[$x]->delete();
									$this->images[$x] = null;
									$delCount++;
									$this->saveRevisionDeleteImage();
	
								} elseif($idel->url == $dtag) {
									$this->images[$x]->delete();
									$this->images[$x] = null;
									$delCount++;
									$this->saveRevisionDeleteImage();
	
								} elseif($idel->url_thumb == $dtag) {
									$this->images[$x]->delete();
									$this->images[$x] = null;
									$delCount++;
									$this->saveRevisionDeleteImage();
								}
								$x++;
							}
						}
						if($delCount == 0) {
							$this->ecode = 423; // bad request
							
						// ok, we deleted some images, so make sure we still have at least one priary image remaining
						} else {
	
							$pcount = 0;
							$icount = 0;
							foreach($this->images as $g) {
								$icount++;
								if($g != null && $g->principal == 1) {
									$pcount++;
								}
							}
							
							// if we have images remaining and no primary, make the first image the primary
							if(($icount > 0) && ($pcount == 0)) {
								$gcount = 0;
								foreach($this->images as $g) {
									if($g != null && $g->principal == 0 && $gcount == 0) {
										$g->principal = 1;
										$gcount++;
									}
								}
							}
						}
					}
				}
	
	
				/** HANDLE VOICE_NOTE ADDITIONS ********************************************************************************************/
	
				// the case when we have only 1 or more voice note(s)
				if(isset($a['person']['voiceNotes']['voiceNote']) || isset($a['person']['voiceNotes'][0])) {
	
					foreach($a['person']['voiceNotes'] as $voiceNote) {
	
						if(trim($voiceNote['data']) != "") {
	
							$v = new voiceNote();
							$v->init();     // reserves a voicenote id for this note
							$v->p_uuid      = $this->p_uuid;
							$v->dataBase64  = isset($voiceNote['data'])             ? $voiceNote['data']             : null;
							$v->length      = isset($voiceNote['length'])           ? $voiceNote['length']           : null;
							$v->format      = isset($voiceNote['format'])           ? $voiceNote['format']           : null;
							$v->sample_rate = isset($voiceNote['sampleRate'])       ? $voiceNote['sampleRate']       : null;
							$v->channels    = isset($voiceNote['numberOfChannels']) ? $voiceNote['numberOfChannels'] : null;
							$v->speaker     = isset($voiceNote['speaker'])          ? $voiceNote['speaker']          : null;
							$v->decode();
	
							// if this new voicenote has a sha1 = to an already present voicenote for this person, it is a duplicate
							if(!$this->isVoiceNoteDuplicate($v->sha1original)) {
	
								$this->voice_notes[] = $v;
								// if this is a reParse, log the revision (addition of new voiceNote)
								if($reParse) {
									$this->saveRevisionNewVoiceNote();
								}
							} else {
								$this->ecode = 424;
								$i = null;
							}
						}
					}
				}
	
	
				/** HANDLE VOICENOTE DELETIONS ********************************************************************************************/
	
				// only check for deletions on reParse
				if($reParse) {
	
					$delCount = 0;
	
					if(isset($a['person']['deleteVoiceNotes']['voiceNote']) || isset($a['person']['deleteVoiceNotes'][0])) {
	
						foreach($a['person']['deleteVoiceNotes'] as $dtag) {
	
							$x = 0;
							foreach($this->voice_notes as $vdel) {
	
								if($vdel->sha1original == $dtag) {
									$this->voice_notes[$x]->delete();
									$this->voice_notes[$x] = null;
									$delCount++;
									$this->saveRevisionDeleteVoiceNote();
	
								} elseif($idel->url == $dtag) {
									$this->voice_notes[$x]->delete();
									$this->voice_notes[$x] = null;
									$delCount++;
									$this->saveRevisionDeleteVoiceNote();
	
								} elseif($idel->url_thumb == $dtag) {
									$this->voice_notes[$x]->delete();
									$this->voice_notes[$x] = null;
									$delCount++;
									$this->saveRevisionDeleteVoiceNote();
								}
								$x++;
							}
						}
						if($delCount == 0) {
							$this->ecode = 425; // bad request
						}
					}
				}
	
				/** END VOICENOTE DELETION ******************************************************************************************************/
	
				$this->sanitizePrincipalFlags();
	
				// check if reported p_uuid is valid (in range of sequence) ~ 402 error if not
				if(!shn_is_p_uuid_valid($this->p_uuid)) {
					return (int)402;
				}
	
				// check if the event is closed to reporting...
				if(!$this->isEventOpen()) {
					return (int)405;
				}
	
				// no errors
				return (int)$this->ecode;
	
				
			/** HANDLE TRIAGEPIC1 XML's **********************************************************************************************************/
			} elseif($this->xmlFormat == "TRIAGEPIC1") {
	
				// when we have more than 1 contentObject, they are renamed to 0...x
				if(isset($a['EDXLDistribution'][0])) {
					$ix = 0;
	
				// when there is only 1 contentObject, we go by name
				} elseif(isset($a['EDXLDistribution']['contentObject'])) {
					$ix = "contentObject";
	
				// all else, we fail and quit
				} else {
					return (int)403; // error code for failed to parse xml
				}
	
				// during a reparse init a new edxl object 
				if(!$reParse) {
					$this->createUUID();
					$this->arrival_triagepic = true;
					$this->edxl = new personEdxl();
					$this->edxl->init();
					
					// creation time is only parsed on a new report
					// <dateTimeSent>2011-03-28T07:52:17Z</dateTimeSent>
					$datetime      = isset($a['EDXLDistribution']['dateTimeSent']) ? $a['EDXLDistribution']['dateTimeSent'] : date('Y-m-d H:i:s', time());
					$timezoneUTC   = new DateTimeZone("UTC");
					$timezoneLocal = new DateTimeZone(date_default_timezone_get());
					$datetime2     = new DateTime();
					$datetime2->setTimezone($timezoneUTC);
					$datetime2->setTimestamp(strtotime($datetime));
					$datetime2->setTimezone($timezoneLocal);
					$this->creation_time = $datetime2->format('Y-m-d H:i:s');
				}			
				
				$this->family_name = isset($a['EDXLDistribution'][$ix]['xmlContent']['lpfContent']['person']['lastName'])  ? $a['EDXLDistribution'][$ix]['xmlContent']['lpfContent']['person']['lastName']  : null;
				$this->given_name  = isset($a['EDXLDistribution'][$ix]['xmlContent']['lpfContent']['person']['firstName']) ? $a['EDXLDistribution'][$ix]['xmlContent']['lpfContent']['person']['firstName'] : null;
	
				// in the absense of an event name, default to the test event
				$eventName = isset($a['EDXLDistribution'][$ix]['xmlContent']['lpfContent']['person']['eventName']) ? $a['EDXLDistribution'][$ix]['xmlContent']['lpfContent']['person']['eventName'] : 'test';
				$q = "
					SELECT *
					FROM incident
					WHERE shortname = '".mysql_real_escape_string((string)$eventName)."'
					LIMIT 1;
				";
				$result = $this->db->Execute($q);
				if($result === false) { daoErrorLog(__FILE__, __LINE__, __METHOD__, __CLASS__, __FUNCTION__, $this->db->ErrorMsg(), "person get incident ((".$q."))"); }
				$this->incident_id = $result->fields["incident_id"];
				$longName = $result->fields['name'];
	
	
				$b = isset($a['EDXLDistribution'][$ix]['xmlContent']['lpfContent']['person']['triageCategory']) ? $a['EDXLDistribution'][$ix]['xmlContent']['lpfContent']['person']['triageCategory'] : 'Green';
				if(($b == "Green") || ($b == "BH Green")) {
					$this->opt_status = "ali";
				} elseif(($b == "Yellow") || ($b == "Red") || ($b == "Gray")) {
					$this->opt_status = "inj";
				} elseif($b == "Black") {
					$this->opt_status = "dec";
				} else {
					$this->opt_status = "unk";
				}
	
				$this->last_updated = date('Y-m-d H:i:s', time());
				$gendur = isset($a['EDXLDistribution'][$ix]['xmlContent']['lpfContent']['person']['gender']) ? $a['EDXLDistribution'][$ix]['xmlContent']['lpfContent']['person']['gender'] : null;
				$this->opt_gender = $gendur;
				
				$peds = isset($a['EDXLDistribution'][$ix]['xmlContent']['lpfContent']['person']['peds']) ? $a['EDXLDistribution'][$ix]['xmlContent']['lpfContent']['person']['peds'] : null;
				if($peds == "Y") {
					$this->minAge = 0;
					$this->maxAge = 17;
					$peds = 1;
				} elseif($peds == "N") {
					$this->minAge = 18;
					$this->maxAge = 150;
					$peds = 0;
				} elseif($peds == "Y,N") {
					$this->minAge = 0;
					$this->maxAge = 150;
					$peds = 0;				
				} else {
					$this->minAge = 0;
					$this->maxAge = 150;
					$peds = null;
				}
	
				$this->other_comments = isset($a['EDXLDistribution'][$ix]['contentDescription']) ? $a['EDXLDistribution'][$ix]['contentDescription'] : null;

        // append triagepic comments if they exist
        if(isset($a['EDXLDistribution'][$ix]['xmlContent']['lpfContent']['person']['comments'])) {
          $this->other_comments .= "&#10;".$a['EDXLDistribution'][$ix]['xmlContent']['lpfContent']['person']['comments'];
        }

				// figure out hospital to assign person to (default to NLM testing for absent value)
				$orgId  = isset($a['EDXLDistribution'][$ix]['xmlContent']['lpfContent']['person']['organization']['orgId']) ? $a['EDXLDistribution'][$ix]['xmlContent']['lpfContent']['person']['organization']['orgId'] : 3;
				$q = "
					SELECT *
					FROM hospital
					WHERE npi = '".mysql_real_escape_string((string)$orgId)."'
					LIMIT 1;
				";
				$result = $this->db->Execute($q);
				if($result === false) { daoErrorLog(__FILE__, __LINE__, __METHOD__, __CLASS__, __FUNCTION__, $this->db->ErrorMsg(), "person get hospital ((".$q."))"); }
				$this->hospital_uuid = $result->fields["hospital_uuid"];
				$this->last_seen     = $result->fields["name"]." Hospital";
	
				$this->edxl->content_descr   = isset($a['EDXLDistribution'][$ix]['contentDescription'])                               ? $a['EDXLDistribution'][$ix]['contentDescription'] : null;
				$this->edxl->p_uuid          = $this->p_uuid;
				$this->edxl->schema_version  = isset($a['EDXLDistribution'][$ix]['xmlContent']['lpfContent']['version'])              ? $a['EDXLDistribution'][$ix]['xmlContent']['lpfContent']['version'] : null;
				$this->edxl->login_machine   = isset($a['EDXLDistribution'][$ix]['xmlContent']['lpfContent']['login']['machineName']) ? $a['EDXLDistribution'][$ix]['xmlContent']['lpfContent']['login']['machineName'] : "n/a"; // HACK! cant be null
				$this->edxl->login_account   = isset($a['EDXLDistribution'][$ix]['xmlContent']['lpfContent']['login']['userName'])    ? $a['EDXLDistribution'][$ix]['xmlContent']['lpfContent']['login']['userName'] : null;
				$this->edxl->person_id       = isset($a['EDXLDistribution'][$ix]['xmlContent']['lpfContent']['person']['personId'])   ? $a['EDXLDistribution'][$ix]['xmlContent']['lpfContent']['person']['personId'] : null;
				$this->edxl->event_name      = $eventName;
				$this->edxl->event_long_name = isset($a['EDXLDistribution'][$ix]['xmlContent']['lpfContent']['person']['eventLongName'])           ? $a['EDXLDistribution'][$ix]['xmlContent']['lpfContent']['person']['eventLongName'] : $longName;
				$this->edxl->org_name        = isset($a['EDXLDistribution'][$ix]['xmlContent']['lpfContent']['person']['organization']['orgName']) ? $a['EDXLDistribution'][$ix]['xmlContent']['lpfContent']['person']['organization']['orgName'] : null;
				$this->edxl->org_id          = isset($a['EDXLDistribution'][$ix]['xmlContent']['lpfContent']['person']['organization']['orgId'])   ? $a['EDXLDistribution'][$ix]['xmlContent']['lpfContent']['person']['organization']['orgId'] : null;
				$this->edxl->last_name       = $this->family_name;
				$this->edxl->first_name      = $this->given_name;
				$this->edxl->gender          = $gendur;
				$this->edxl->peds            = $peds;
				$this->edxl->triage_category = $b;
				$this->edxl->when_sent       = $this->last_updated;
				$this->edxl->sender_id       = isset($a['EDXLDistribution']['senderID'])                ? $a['EDXLDistribution']['senderID'] : null;
				$this->edxl->distr_id        = isset($a['EDXLDistribution']['distributionID'])          ? $a['EDXLDistribution']['distributionID'] : null;
				$this->edxl->distr_status    = isset($a['EDXLDistribution']['distributionStatus'])      ? $a['EDXLDistribution']['distributionStatus'] : null;
				$this->edxl->distr_type      = isset($a['EDXLDistribution']['distributionType'])        ? $a['EDXLDistribution']['distributionType'] : null;
				$this->edxl->combined_conf   = isset($a['EDXLDistribution']['combinedConfidentiality']) ? $a['EDXLDistribution']['combinedConfidentiality'] : null;
				$this->edxl->language        = null;
				$this->edxl->when_here       = $this->creation_time;
				$this->edxl->inbound         = 1; //null; HACK! cant be null
				$this->edxl->type            = "lpf";
				$this->cleanInput();
	
				/** process all images **********************************************************************************************************/
				
				// walk all the contentObjects
				for($n = 0; $n < sizeof($a['EDXLDistribution']); $n++) {
	
					// images are always present in nonXMLComtent nodes, so ignore all else...
					if(isset($a['EDXLDistribution'][$n]['nonXMLContent']) && $a['EDXLDistribution'][$n]['nonXMLContent'] != null) {
	
						
						/** Parse all image data from the CO ***************************************************/
						
						$thePersonId = $this->edxl->person_id;
						$imageNode = $a['EDXLDistribution'][$n]['nonXMLContent'];
						$cd = $a['EDXLDistribution'][$n]['contentDescription'];
	
						// remove patient id from the string if it still has the prefix
						$cd = str_replace($thePersonId, "", $cd);
	
						// sometimes the patient id prefix is removed, so check for that substring and remove it
						if(strpos($thePersonId, "-") === false) {
							// do nothing
						} else {
							$eee = explode("-", $thePersonId);
							$cd = str_replace($eee[1], "", $cd);
						}
	
						// remove triage category from the string
						$cd = str_replace($this->edxl->triage_category, "", $cd);
	
						// remove preceding and trailing whitespace(s)
						$cd = trim($cd);
	
						// what we should now have left is in the format: "" or "sX" or "sX - caption" or "- caption"
						// optionally, in an update there may be a string like "Msg #x " preceding the above string portion (TriagePic v1.50+)
	
						// check for and ignore or remove the "Msg #x" string portion finding the revision number
						if(strpos($cd, "Msg") === false) {
							// do nothing
						} else {
							// remove the portion
							$cd = str_replace("Msg", "", $cd); // take out "Msg"
							$cd = trim($cd); // remove preceding space from #x
							$substring = explode(" ", $cd);
							$numberX = $substring[0]; // #x
							$cd = str_replace($numberX, "", $cd); // remove #x
							$cd = trim($cd); // trim once again
							$number = (int)str_replace("#", "", $numberX); // this is the revision number (we may use it later)
						}
	
						// primary no caption
						if($cd === "") {
							$primary = true;
							$caption = null;
	
						// secondary no caption
						} elseif(strpos($cd, "-") === false) {
							$primary = false;
							$caption = null;
	
						// has caption
						} else {
							$ecd = explode("-", $cd);
	
							// primary with caption
							if(trim($ecd[0]) == "") {
								$primary = true;
								$caption = $ecd[1];
	
							// secondary with caption
							} else {
								$primary = false;
								$caption = trim($ecd[1]);
							}
						}
	
						// init new image
						
						$i = new personImage();
						$i->init();
						$i->p_uuid = $this->p_uuid;
						
						// look for dupes (non-deletes)
						
						$dupeFound = false;
						$xmlSha1 = $imageNode['digest'];
												
						// on reparse we set a boolean flag on the new image and dupes of this image (so they arent' deleted)
						if($reParse) {
	
							$walk = 0;
							foreach($this->images as $image) {
								if(strcasecmp($image->sha1original, $xmlSha1) == 0) {
									$this->images[$walk]->nonDeleteFlag = true;
									$dupeFound = true;
								}
								$walk++;
							}
							$i->nonDeleteFlag = true;
						}				
						
						// add image
						
						// if there is actual contentData...
						if(!$dupeFound && trim($imageNode['contentData']) != "") {
	
							$i->fileContentBase64 = $imageNode['contentData'];
							$i->decode();
							$realSha1 = sha1($i->fileContent);
	
							// check for SHA1 mismatch in XML with real SHA1 of binary data
							if(strcasecmp($realSha1, $xmlSha1) != 0) {
								$this->ecode = 420;
							} else {
	
								$i->original_filename = $imageNode['uri'];
								if($primary) {
									$i->principal = 1;
								} else {
									$i->principal = 0;
								}
								if($caption != null) {
									$t = new personImageTag();
									$t->init();
									$t->image_id = $i->image_id;
									$t->tag_x    = 0;
									$t->tag_y    = 0;
									$t->tag_w    = 0;
									$t->tag_h    = 0;
									$t->tag_text = $caption;
									$i->tags[] = $t;
								}
		
								// add the image
								$this->images[] = $i;
								
								if($reParse) {
									$this->saveRevisionNewImage();
								}
									
								// add the edxl image
								$this->edxl->mimeTypes[]    = $imageNode['mimeType'];
								$this->edxl->uris[]         = $imageNode['uri'];
								$this->edxl->contentDatas[] = $imageNode['contentData'];
								$this->edxl->image_ids[]    = isset($i->image_id) ? $i->image_id : null;
								$this->edxl->image_sha1[]   = isset($realSha1) ? $realSha1 : null;
								$this->edxl->image_co_ids[] = shn_create_uuid("edxl_co_header");
							}
						}
					}
				}
				/** end process all images *****************************************************************************************************/
				
				// delete all non-duped images on reparse
				
				if($reParse) {
					$iii = 0;
					foreach($this->images as $image) {
						if($image->nonDeleteFlag !== true) {
							$this->images[$iii]->delete();
							$this->images[$iii] = null;
							$this->saveRevisionDeleteImage();
						}
						$iii++;
					}
				}
	
				$this->sanitizePrincipalFlags();
				
				// check if the event is closed to reporting...
				if(!$this->isEventOpen()) {
					return (int)405;
				}
	
				// exit with success
				return (int)$this->ecode;
	
				
				
			/** parse TRIAGEPIC0 XML ***********************************************************************************************************/
				
			} elseif($this->xmlFormat == "TRIAGEPIC0") {
	
				$this->arrival_triagepic = true;
	
				$this->edxl = new personEdxl();
				$this->edxl->init();
	
				$this->createUUID();
				$this->family_name = $a['EDXLDistribution']['contentObject']['xmlContent']['embeddedXMLContent']['lpfContent']['person']['lastName'];
				$this->given_name  = $a['EDXLDistribution']['contentObject']['xmlContent']['embeddedXMLContent']['lpfContent']['person']['firstName'];
	
				$eventName = $a['EDXLDistribution']['contentObject']['xmlContent']['embeddedXMLContent']['lpfContent']['person']['eventName'];
				$q = "
					SELECT *
					FROM incident
					WHERE shortname = '".mysql_real_escape_string((string)$eventName)."';
				";
				$result = $this->db->Execute($q);
				if($result === false) { daoErrorLog(__FILE__, __LINE__, __METHOD__, __CLASS__, __FUNCTION__, $this->db->ErrorMsg(), "person get incident ((".$q."))"); }
	
				$this->incident_id = $result->fields["incident_id"];
	
	
				$b = $a['EDXLDistribution']['contentObject']['xmlContent']['embeddedXMLContent']['lpfContent']['person']['triageCategory'];
				if(($b == "Green") || ($b == "BH Green")) {
					$this->opt_status = "ali";
				} elseif(($b == "Yellow") || ($b == "Red") || ($b == "Gray")) {
					$this->opt_status = "inj";
				} elseif($b == "Black") {
					$this->opt_status = "dec";
				} else {
					$this->opt_status = "unk";
				}
	
				$this->last_updated = date('Y-m-d H:i:s');
	
				// <dateTimeSent>2011-03-28T07:52:17Z</dateTimeSent>
				$datetime      = $a['EDXLDistribution']['dateTimeSent'];
				$timezoneUTC   = new DateTimeZone("UTC");
				$timezoneLocal = new DateTimeZone(date_default_timezone_get());
				$datetime2     = new DateTime();
				$datetime2->setTimezone($timezoneUTC);
				$datetime2->setTimestamp(strtotime($datetime));
				$datetime2->setTimezone($timezoneLocal);
				$this->creation_time = $datetime2->format('Y-m-d H:i:s');
	
				$this->opt_gender = $a['EDXLDistribution']['contentObject']['xmlContent']['embeddedXMLContent']['lpfContent']['person']['gender'];
	
				$peds = $a['EDXLDistribution']['contentObject']['xmlContent']['embeddedXMLContent']['lpfContent']['person']['peds'];
	
				if($peds == "Y") {
					$this->minAge = 0;
					$this->maxAge = 17;
				} elseif($peds == "N") {
					$this->minAge = 18;
					$this->maxAge = 150;
				} elseif($peds == "Y,N") {
					$this->minAge = 0;
					$this->maxAge = 150;
				}
	
				$this->other_comments = $a['EDXLDistribution']['contentObject']['contentDescription'];
	
	
				$orgId  = $a['EDXLDistribution']['contentObject']['xmlContent']['embeddedXMLContent']['lpfContent']['person']['organization']['orgId'];
				$q = "
					SELECT *
					FROM hospital
					WHERE npi = '".mysql_real_escape_string((string)$orgId)."';
				";
				$result = $this->db->Execute($q);
				if($result === false) { daoErrorLog(__FILE__, __LINE__, __METHOD__, __CLASS__, __FUNCTION__, $this->db->ErrorMsg(), "person get hospital ((".$q."))"); }
	
				$this->hospital_uuid = $result->fields["hospital_uuid"];
				$this->last_seen     = $result->fields["name"]." Hospital";
	
				$this->edxl->content_descr   = $a['EDXLDistribution']['contentObject']['contentDescription'];
				$this->edxl->p_uuid          = $this->p_uuid;
				$this->edxl->schema_version  = $a['EDXLDistribution']['contentObject']['xmlContent']['embeddedXMLContent']['lpfContent']['version'];
				$this->edxl->login_machine   = "n/a"; //null; HACK! cant be null
				$this->edxl->login_account   = $a['EDXLDistribution']['contentObject']['xmlContent']['embeddedXMLContent']['lpfContent']['login']['username'];
				$this->edxl->person_id       = $a['EDXLDistribution']['contentObject']['xmlContent']['embeddedXMLContent']['lpfContent']['person']['personId'];
				$this->edxl->event_name      = $a['EDXLDistribution']['contentObject']['xmlContent']['embeddedXMLContent']['lpfContent']['person']['eventName'];
				$this->edxl->event_long_name = $a['EDXLDistribution']['contentObject']['xmlContent']['embeddedXMLContent']['lpfContent']['person']['eventLongName'];
				$this->edxl->org_name        = $a['EDXLDistribution']['contentObject']['xmlContent']['embeddedXMLContent']['lpfContent']['person']['organization']['orgName'];
				$this->edxl->org_id          = $a['EDXLDistribution']['contentObject']['xmlContent']['embeddedXMLContent']['lpfContent']['person']['organization']['orgId'];
				$this->edxl->last_name       = $a['EDXLDistribution']['contentObject']['xmlContent']['embeddedXMLContent']['lpfContent']['person']['lastName'];
				$this->edxl->first_name      = $a['EDXLDistribution']['contentObject']['xmlContent']['embeddedXMLContent']['lpfContent']['person']['firstName'];
				$this->edxl->gender          = $a['EDXLDistribution']['contentObject']['xmlContent']['embeddedXMLContent']['lpfContent']['person']['gender'];
				$this->edxl->peds            = $a['EDXLDistribution']['contentObject']['xmlContent']['embeddedXMLContent']['lpfContent']['person']['peds'];
				$this->edxl->triage_category = $a['EDXLDistribution']['contentObject']['xmlContent']['embeddedXMLContent']['lpfContent']['person']['triageCategory'];
				$this->edxl->when_sent       = $this->last_updated;
				$this->edxl->sender_id       = $a['EDXLDistribution']['senderID'];
				$this->edxl->distr_id        = $a['EDXLDistribution']['distributionID'];
				$this->edxl->distr_status    = $a['EDXLDistribution']['distributionStatus'];
				$this->edxl->distr_type      = $a['EDXLDistribution']['distributionType'];
				$this->edxl->combined_conf   = $a['EDXLDistribution']['combinedConfidentiality'];
				$this->edxl->language        = null;
				$this->edxl->when_here       = $this->creation_time;
				$this->edxl->inbound         = 1; //null; HACK! cant be null
				$this->edxl->type            = "lpf";
				$this->cleanInput();
	
				// check if the event is closed to reporting...
				if(!$this->isEventOpen()) {
					return (int)405;
				}
	
				// exit with success
				return (int)0;
				
			// catch all for all other invalid inumerations
			} else {
				return (int)400;
			}
		}
	}

	
	/** END parseXML() ***********************************************************************************************************************/
	
	
	function saveRevisionNewImage() {

		// note the newly added image in revision history
		$q = "
			INSERT into person_updates (`p_uuid`, `updated_table`, `updated_column`, `old_value`, `new_value`, `updated_by_p_uuid`)
			VALUES ('".$this->p_uuid."', 'image', 'NEW', null, 'New Image Added.', '".$this->updated_by_p_uuid."');
		";
		$result = $this->db->Execute($q);
		if($result === false) { daoErrorLog(__FILE__, __LINE__, __METHOD__, __CLASS__, __FUNCTION__, $this->db->ErrorMsg(), "person saveRevisionNewImage ((".$q."))"); }
	}


	function saveRevisionDeleteImage() {

		// note the newly added image in revision history
		$q = "
			INSERT into person_updates (`p_uuid`, `updated_table`, `updated_column`, `old_value`, `new_value`, `updated_by_p_uuid`)
			VALUES ('".$this->p_uuid."', 'image', 'DELETE', null, 'Image Deleted.', '".$this->updated_by_p_uuid."');
		";
		$result = $this->db->Execute($q);
		if($result === false) { daoErrorLog(__FILE__, __LINE__, __METHOD__, __CLASS__, __FUNCTION__, $this->db->ErrorMsg(), "person saveRevisionNewImage ((".$q."))"); }
	}


	function saveRevisionNewVoiceNote() {

		// note the newly added voicenote in revision history
		$q = "
			INSERT into person_updates (`p_uuid`, `updated_table`, `updated_column`, `old_value`, `new_value`, `updated_by_p_uuid`)
			VALUES ('".$this->p_uuid."', 'voice_note', 'NEW', null, 'New Voice Note Added.', '".$this->updated_by_p_uuid."');
		";
		$result = $this->db->Execute($q);
		if($result === false) { daoErrorLog(__FILE__, __LINE__, __METHOD__, __CLASS__, __FUNCTION__, $this->db->ErrorMsg(), "person saveRevisionNewVoiceNote ((".$q."))"); }
	}


	function saveRevisionDeleteVoiceNote() {

		// note the newly added voicenote in revision history
		$q = "
			INSERT into person_updates (`p_uuid`, `updated_table`, `updated_column`, `old_value`, `new_value`, `updated_by_p_uuid`)
			VALUES ('".$this->p_uuid."', 'voice_note', 'DELETE', null, 'Voice Note Deleted.', '".$this->updated_by_p_uuid."');
		";
		$result = $this->db->Execute($q);
		if($result === false) { daoErrorLog(__FILE__, __LINE__, __METHOD__, __CLASS__, __FUNCTION__, $this->db->ErrorMsg(), "person saveRevisionNewVoiceNote ((".$q."))"); }
	}


	function havePrimaryAlready() {

		$have = false;
		foreach($this->images as $image) {
			if($image->principal == 1) {
				$have = true;
			}
		}
		return $have;
	}


	function isImageDuplicate($sha1) {

		$dupe = false;
		foreach($this->images as $image) {
			if($image->sha1original == $sha1) {
				$dupe = true;
			}
		}
		return $dupe;
	}


	function isVoiceNoteDuplicate($sha1) {

		$dupe = false;
		foreach($this->voice_notes as $voiceNote) {
			if($voiceNote->sha1original == $sha1) {
				$dupe = true;
			}
		}
		return $dupe;
	}
	
	
	
	function sanitizePrincipalFlags() {

		// sanitize principal image flags...
		if(sizeof($this->images) > 0) {

			// reverse walk and see if there is only one principal
			$principalCount = 0;
			for($walk = sizeof($this->images); $walk--; $walk > 0) {
				if($this->images[$walk] != null && $this->images[$walk]->principal == 1) {
					$principalCount++;
				}
			}
			// more than 1? make last image primary reset all others
			if($principalCount > 1) {
				$set = false;
				for($walk = sizeof($this->images); $walk--; $walk > 0) {
					if(!$set) {
						$this->images[$walk] == null ? $this->images[$walk] = null : $this->images[$walk]->principal = 1;
						$set = true;
					} else {
						$this->images[$walk] == null ? $this->images[$walk] = null : $this->images[$walk]->principal = 0;
					}
				}
				
			// no primary? make last image primary
			} elseif($principalCount < 1) {
				$this->images[sizeof($this->images)-1]->principal = 1;
			}
		}		
	}
	
	// end class
}



