<?
/**
 * @name         PL User Services
 * @version      29
 * @package      plus
 * @author       Greg Miernicki <g@miernicki.com> <gregory.miernicki@nih.gov>
 * @about        Developed in whole or part by the U.S. National Library of Medicine
 * @link         https://pl.nlm.nih.gov/about
 * @license	 http://www.gnu.org/licenses/lgpl-2.1.html GNU Lesser General Public License (LGPL)
 * @lastModified 2013.0321
 */


global $api_version;
$api_version = 29;

// This function is called by the Document/Literal SOAP server upon initalization to register all the api services from this module's api.
function shn_plus_registerAll($namespace) {

	global $server;
	global $api_version;

	// GENERAL API DECLARATIONS /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// //
	// GENERAL API DECLARATIONS /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// //

	shn_plus_registerOne($server, 'ping',
		array(
			'in' => array(),
			'out' => array(
				'time'   => 'xsd:string'
			)
		),
		$namespace,
		"Used to test if the server is active/up."
	);

	shn_plus_registerOne($server, 'pingWithEcho',
		array(
			'in' => array(
				'pingString' => 'xsd:string',
				'latency'    => 'xsd:integer'
			),
			'out' => array(
				'time'   => 'xsd:string'
			)
		),
			$namespace,
			"Used to test if the server is active/up as well as report latency data. The latency is calculated from the previous ping call, so during the intial call of this service, you would report a latency of -1, 0, or null to signifiy no latency."
	);
	
	shn_plus_registerOne($server, 'getEventList',
		array(
			'in' => array(),
			'out' => array(
				'eventList'    => 'xsd:string',
				'errorCode'    => 'xsd:integer',
				'errorMessage' => 'xsd:string'
			)
		),
		$namespace,
		"Returns an array of (event_id, parent_id, name, shortname, date, type, latitude, longitude, street, group) public disaster events."
	);

	shn_plus_registerOne($server, 'getEventListUser',
		array(
			'in' => array(
				'username'     => 'xsd:string',
				'password'     => 'xsd:string'
			),
			'out' => array(
				'eventList'    => 'xsd:string',
				'errorCode'    => 'xsd:integer',
				'errorMessage' => 'xsd:string'
			)
		),
		$namespace,
		"Returns an array of (event_id, parent_id, name, shortname, date, type, latitude, longitude, street) disaster events accessible by a certain user."
	);

	shn_plus_registerOne($server, 'getGroupList',
		array(
			'in' => array(),
			'out' => array(
				'groupList'    => 'xsd:string',
				'errorCode'    => 'xsd:integer',
				'errorMessage' => 'xsd:string'
			)
		),
		$namespace,
		"Returns an array of (group_id, group_name) listing all groups in the system."
	);

	shn_plus_registerOne($server, 'getHospitalList',
		array(
			'in' => array(),
			'out' => array(
				'hospitalList' => 'xsd:string',
				'errorCode'    => 'xsd:integer',
				'errorMessage' => 'xsd:string'
			)
		),
		$namespace,
		"Returns an array of (hospital_uuid, name, shortname, latitude, longitude)."
	);

	shn_plus_registerOne($server, 'getHospitalData',
		array(
			'in' => array(
				'hospital_uuid' => 'xsd:integer'
			),
			'out' => array(
				'name'          => 'xsd:string',
				'shortname'     => 'xsd:string',
				'street1'       => 'xsd:string',
				'street2'       => 'xsd:string',
				'city'          => 'xsd:string',
				'county'        => 'xsd:string',
				'state'         => 'xsd:string',
				'country'       => 'xsd:string',
				'zip'           => 'xsd:string',
				'phone'         => 'xsd:string',
				'fax'           => 'xsd:string',
				'email'         => 'xsd:string',
				'www'           => 'xsd:string',
				'npi'           => 'xsd:string',
				'latitude'      => 'xsd:string',
				'longitude'     => 'xsd:string',
				'errorCode'     => 'xsd:integer',
				'errorMessage'  => 'xsd:string'
			)
		),
		$namespace,
		"Returns all data relavent to a perticular hospital."
	);

	shn_plus_registerOne($server, 'getHospitalPolicy',
		array(
			'in' => array(
				'hospital_uuid'              => 'xsd:integer'
			),
			'out' => array(
				'patientIdPrefix'            => 'xsd:string',
				'patientIdSuffixVariable'    => 'xsd:boolean',
				'patientIdSuffixFixedLength' => 'xsd:integer',
				'photoRequired'              => 'xsd:boolean',
				'honorNoPhotoRequest'        => 'xsd:boolean',
				'photographerNameRequired'   => 'xsd:boolean',
				'errorCode'                  => 'xsd:integer',
				'errorMessage'               => 'xsd:string'
			)
		),
		$namespace,
		"Returns all policy data relavent to a particular hospital."
	);

	shn_plus_registerOne($server, 'getHospitalLegalese',
		array(
			'in' => array(
				'hospital_uuid' => 'xsd:integer'
			),
			'out' => array(
				'legalese'      => 'xsd:string',
				'errorCode'     => 'xsd:integer',
				'errorMessage'  => 'xsd:string'
			)
		),
		$namespace,
		"Retrieve a particular hospital’s legalese clause."
	);

	shn_plus_registerOne($server, 'getHospitalLegaleseAnon',
		array(
			'in' => array(
				'hospital_uuid' => 'xsd:integer'
			),
			'out' => array(
				'legaleseAnon'  => 'xsd:string',
				'errorCode'     => 'xsd:integer',
				'errorMessage'  => 'xsd:string'
			)
		),
		$namespace,
		"Retrieve a particular hospital’s anonymous legalese clause."
	);

	shn_plus_registerOne($server, 'getHospitalLegaleseTimestamps',
		array(
			'in' => array(
				'hospital_uuid' => 'xsd:integer'
			),
			'out' => array(
				'legaleseTimestamp'     => 'xsd:string',
				'legaleseAnonTimestamp' => 'xsd:string',
				'errorCode'             => 'xsd:integer',
				'errorMessage'          => 'xsd:string'
			)
		),
		$namespace,
		"Retrieves information about when the legalese clauses were last updated for a particular hospital."
	);
	
	shn_plus_registerOne($server, 'appCheck',
			array(
					'in' => array(
							'query_string' => 'xsd:string'
					),
					'out' => array(
							'url'          => 'xsd:string',
							'text'         => 'xsd:string',
							'errorCode'    => 'xsd:integer',
							'errorMessage' => 'xsd:string'
					)
			),
			$namespace,
			"Allows an app to find out if it is using the latest version. There is a 1:1 correlation between the query_string and the response in terms of what is returned in two output parameters."
	);

	// ACCOUNT API DECLARATIONS /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// //
	// ACCOUNT API DECLARATIONS /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// //

	shn_plus_registerOne($server, 'registerUser',
		array(
			'in' => array(
				'username'     => 'xsd:string',
				'emailAddress' => 'xsd:string',
				'password'     => 'xsd:string',
				'givenName'    => 'xsd:string',
				'familyName'   => 'xsd:string'
			),
			'out' => array(
				'registered'   => 'xsd:boolean',
				'errorCode'    => 'xsd:integer',
				'errorMessage' => 'xsd:string'
			)
		),
		$namespace,
		"Creates a new inactive (pending confirmation) user in the system with the supplied characteristics and credentials."
	);
  
  shn_plus_registerOne($server, 'registerUserActive',
		array(
			'in' => array(
				'username'     => 'xsd:string',
				'emailAddress' => 'xsd:string',
				'password'     => 'xsd:string',
				'givenName'    => 'xsd:string',
				'familyName'   => 'xsd:string'
			),
			'out' => array(
				'registered'   => 'xsd:boolean',
				'errorCode'    => 'xsd:integer',
				'errorMessage' => 'xsd:string'
			)
		),
		$namespace,
		"Creates a new active user in the system with the supplied characteristics and credentials."
	);

	shn_plus_registerOne($server, 'changeUserPassword',
		array(
			'in' => array(
				'username'     => 'xsd:string',
				'oldPassword'  => 'xsd:string',
				'newPassword'  => 'xsd:string'
			),
			'out' => array(
				'changed'      => 'xsd:boolean',
				'errorCode'    => 'xsd:integer',
				'errorMessage' => 'xsd:string'
			)
		),
		$namespace,
		"Changes a user password from old to new."
	);

	shn_plus_registerOne($server, 'resetUserPassword',
		array(
			'in' => array(
				'email'        => 'xsd:string'
			),
			'out' => array(
				'sent'         => 'xsd:boolean',
				'errorCode'    => 'xsd:integer',
				'errorMessage' => 'xsd:string'
			)
		),
		$namespace,
		"Initiates the password reset procedure."
	);

	shn_plus_registerOne($server, 'forgotUsername',
		array(
			'in' => array(
				'email'        => 'xsd:string'
			),
			'out' => array(
				'sent'         => 'xsd:boolean',
				'errorCode'    => 'xsd:integer',
				'errorMessage' => 'xsd:string'
			)
		),
		$namespace,
		"Sends an email to the user with their username."
	);

	shn_plus_registerOne($server, 'checkUserAuth',
		array(
			'in' => array(
				'username'     => 'xsd:string',
				'password'     => 'xsd:string'
			),
			'out' => array(
				'valid'        => 'xsd:boolean',
				'errorCode'    => 'xsd:integer',
				'errorMessage' => 'xsd:string'
			)
		),
		$namespace,
		"Used to verify a username / password combination are valid."
	);

	shn_plus_registerOne($server, 'checkUserAuthHospital',
		array(
			'in' => array(
				'username'     => 'xsd:string',
				'password'     => 'xsd:string'
			),
			'out' => array(
				'valid'        => 'xsd:boolean',
				'errorCode'    => 'xsd:integer',
				'errorMessage' => 'xsd:string'
			)
		),
		$namespace,
		"Used to verify a username / password combination are valid for Hospital Staff."
	);

	shn_plus_registerOne($server, 'getUserStatus',
		array(
			'in' => array(
				'username'     => 'xsd:string'
			),
			'out' => array(
				'pending'      => 'xsd:boolean',
				'active'       => 'xsd:boolean',
				'locked'       => 'xsd:boolean',
				'banned'       => 'xsd:boolean',
				'errorCode'    => 'xsd:integer',
				'errorMessage' => 'xsd:string'
			)
		),
		$namespace,
		"Used to determine user account status without authenticating."
	);

	shn_plus_registerOne($server, 'getUserGroup',
		array(
			'in' => array(
				'username'     => 'xsd:string'
			),
			'out' => array(
				'group_id'     => 'xsd:integer',
				'group_name'   => 'xsd:string',
				'errorCode'    => 'xsd:integer',
				'errorMessage' => 'xsd:string'
			)
		),
		$namespace,
		"Shows which group a user is part of without authenticating."
	);


	// SEARCH API DECLARATIONS /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// //
	// SEARCH API DECLARATIONS /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// //

	shn_plus_registerOne($server, 'search',
		array(
			'in' => array(
				'eventShortname'       => 'xsd:string',
				'searchTerm'           => 'xsd:string',
				'filterStatusMissing'  => 'xsd:boolean',
				'filterStatusAlive'    => 'xsd:boolean',
				'filterStatusInjured'  => 'xsd:boolean',
				'filterStatusDeceased' => 'xsd:boolean',
				'filterStatusUnknown'  => 'xsd:boolean',
				'filterStatusFound'    => 'xsd:boolean',
				'filterGenderComplex'  => 'xsd:boolean',
				'filterGenderMale'     => 'xsd:boolean',
				'filterGenderFemale'   => 'xsd:boolean',
				'filterGenderUnknown'  => 'xsd:boolean',
				'filterAgeChild'       => 'xsd:boolean',
				'filterAgeAdult'       => 'xsd:boolean',
				'filterAgeUnknown'     => 'xsd:boolean',
				'filterHospitalSH'     => 'xsd:boolean',
				'filterHospitalWRNMMC' => 'xsd:boolean',
				'filterHospitalOther'  => 'xsd:boolean',
				'pageStart'            => 'xsd:integer',
				'perPage'              => 'xsd:integer',
				'sortBy'               => 'xsd:string'
			),
			'out' => array(
				'resultSet'    => 'xsd:string',
				'recordsFound' => 'xsd:integer',
				'timeElapsed'  => 'xsd:double',
				'errorCode'    => 'xsd:integer',
				'errorMessage' => 'xsd:string'
			)
		),
		$namespace,
		"Performs a search across a specific PUBLIC event with filters, paging, and sort options."
	);

	shn_plus_registerOne($server, 'searchWithAuth',
		array(
			'in' => array(
				'eventShortname'       => 'xsd:string',
				'searchTerm'           => 'xsd:string',
				'filterStatusMissing'  => 'xsd:boolean',
				'filterStatusAlive'    => 'xsd:boolean',
				'filterStatusInjured'  => 'xsd:boolean',
				'filterStatusDeceased' => 'xsd:boolean',
				'filterStatusUnknown'  => 'xsd:boolean',
				'filterStatusFound'    => 'xsd:boolean',
				'filterGenderComplex'  => 'xsd:boolean',
				'filterGenderMale'     => 'xsd:boolean',
				'filterGenderFemale'   => 'xsd:boolean',
				'filterGenderUnknown'  => 'xsd:boolean',
				'filterAgeChild'       => 'xsd:boolean',
				'filterAgeAdult'       => 'xsd:boolean',
				'filterAgeUnknown'     => 'xsd:boolean',
				'filterHospitalSH'     => 'xsd:boolean',
				'filterHospitalWRNMMC' => 'xsd:boolean',
				'filterHospitalOther'  => 'xsd:boolean',
				'pageStart'            => 'xsd:integer',
				'perPage'              => 'xsd:integer',
				'sortBy'               => 'xsd:string',
				'username'             => 'xsd:string',
				'password'             => 'xsd:string'
			),
			'out' => array(
				'resultSet'    => 'xsd:string',
				'recordsFound' => 'xsd:integer',
				'timeElapsed'  => 'xsd:double',
				'errorCode'    => 'xsd:integer',
				'errorMessage' => 'xsd:string'
			)
		),
		$namespace,
		"Performs a search across a specific event with filters, paging, and sort options with authentication."
	);

	shn_plus_registerOne($server, 'searchComplete',
			array(
					'in' => array(
							'eventShortname'       => 'xsd:string',
							'searchTerm'           => 'xsd:string',
							'filterStatusMissing'  => 'xsd:boolean',
							'filterStatusAlive'    => 'xsd:boolean',
							'filterStatusInjured'  => 'xsd:boolean',
							'filterStatusDeceased' => 'xsd:boolean',
							'filterStatusUnknown'  => 'xsd:boolean',
							'filterStatusFound'    => 'xsd:boolean',
							'filterGenderComplex'  => 'xsd:boolean',
							'filterGenderMale'     => 'xsd:boolean',
							'filterGenderFemale'   => 'xsd:boolean',
							'filterGenderUnknown'  => 'xsd:boolean',
							'filterAgeChild'       => 'xsd:boolean',
							'filterAgeAdult'       => 'xsd:boolean',
							'filterAgeUnknown'     => 'xsd:boolean',
							'filterHospitalSH'     => 'xsd:boolean',
							'filterHospitalWRNMMC' => 'xsd:boolean',
							'filterHospitalOther'  => 'xsd:boolean',
							'pageStart'            => 'xsd:integer',
							'perPage'              => 'xsd:integer',
							'sortBy'               => 'xsd:string'
					),
					'out' => array(
							'resultSet'    => 'xsd:string',
							'recordsFound' => 'xsd:integer',
							'timeElapsed'  => 'xsd:double',
							'errorCode'    => 'xsd:integer',
							'errorMessage' => 'xsd:string'
					)
			),
			$namespace,
			"Performs a search across a specific PUBLIC event with filters, paging, and sort options. Returns a complete data set for each person in the results."
	);
	
	shn_plus_registerOne($server, 'searchCompleteWithAuth',
			array(
					'in' => array(
							'eventShortname'       => 'xsd:string',
							'searchTerm'           => 'xsd:string',
							'filterStatusMissing'  => 'xsd:boolean',
							'filterStatusAlive'    => 'xsd:boolean',
							'filterStatusInjured'  => 'xsd:boolean',
							'filterStatusDeceased' => 'xsd:boolean',
							'filterStatusUnknown'  => 'xsd:boolean',
							'filterStatusFound'    => 'xsd:boolean',
							'filterGenderComplex'  => 'xsd:boolean',
							'filterGenderMale'     => 'xsd:boolean',
							'filterGenderFemale'   => 'xsd:boolean',
							'filterGenderUnknown'  => 'xsd:boolean',
							'filterAgeChild'       => 'xsd:boolean',
							'filterAgeAdult'       => 'xsd:boolean',
							'filterAgeUnknown'     => 'xsd:boolean',
							'filterHospitalSH'     => 'xsd:boolean',
							'filterHospitalWRNMMC' => 'xsd:boolean',
							'filterHospitalOther'  => 'xsd:boolean',
							'pageStart'            => 'xsd:integer',
							'perPage'              => 'xsd:integer',
							'sortBy'               => 'xsd:string',
							'username'             => 'xsd:string',
							'password'             => 'xsd:string'
					),
					'out' => array(
							'resultSet'    => 'xsd:string',
							'recordsFound' => 'xsd:integer',
							'timeElapsed'  => 'xsd:double',
							'errorCode'    => 'xsd:integer',
							'errorMessage' => 'xsd:string'
					)
			),
			$namespace,
			"Performs a search across a specific event with filters, paging, and sort options with authentication. Returns a complete data set for each person in the results."
	);
	
	
	
	shn_plus_registerOne($server, 'searchCount',
		array(
			'in' => array(
				'eventShortname'       => 'xsd:string',
				'searchTerm'           => 'xsd:string',
				'filterStatusMissing'  => 'xsd:boolean',
				'filterStatusAlive'    => 'xsd:boolean',
				'filterStatusInjured'  => 'xsd:boolean',
				'filterStatusDeceased' => 'xsd:boolean',
				'filterStatusUnknown'  => 'xsd:boolean',
				'filterStatusFound'    => 'xsd:boolean',
				'filterGenderComplex'  => 'xsd:boolean',
				'filterGenderMale'     => 'xsd:boolean',
				'filterGenderFemale'   => 'xsd:boolean',
				'filterGenderUnknown'  => 'xsd:boolean',
				'filterAgeChild'       => 'xsd:boolean',
				'filterAgeAdult'       => 'xsd:boolean',
				'filterAgeUnknown'     => 'xsd:boolean',
				'filterHospitalSH'     => 'xsd:boolean',
				'filterHospitalWRNMMC' => 'xsd:boolean',
				'filterHospitalOther'  => 'xsd:boolean',
				'pageStart'            => 'xsd:integer',
				'perPage'              => 'xsd:integer',
				'sortBy'               => 'xsd:string'
			),
			'out' => array(
				'recordsFound' => 'xsd:integer',
				'timeElapsed'  => 'xsd:double',
				'errorCode'    => 'xsd:integer',
				'errorMessage' => 'xsd:string'
			)
		),
		$namespace,
		"Performs a search across a specific PUBLIC event with filters, paging, and sort options, but only returns the number of results found."
	);

	shn_plus_registerOne($server, 'searchCountWithAuth',
		array(
			'in' => array(
				'eventShortname'       => 'xsd:string',
				'searchTerm'           => 'xsd:string',
				'filterStatusMissing'  => 'xsd:boolean',
				'filterStatusAlive'    => 'xsd:boolean',
				'filterStatusInjured'  => 'xsd:boolean',
				'filterStatusDeceased' => 'xsd:boolean',
				'filterStatusUnknown'  => 'xsd:boolean',
				'filterStatusFound'    => 'xsd:boolean',
				'filterGenderComplex'  => 'xsd:boolean',
				'filterGenderMale'     => 'xsd:boolean',
				'filterGenderFemale'   => 'xsd:boolean',
				'filterGenderUnknown'  => 'xsd:boolean',
				'filterAgeChild'       => 'xsd:boolean',
				'filterAgeAdult'       => 'xsd:boolean',
				'filterAgeUnknown'     => 'xsd:boolean',
				'filterHospitalSH'     => 'xsd:boolean',
				'filterHospitalWRNMMC' => 'xsd:boolean',
				'filterHospitalOther'  => 'xsd:boolean',
				'pageStart'            => 'xsd:integer',
				'perPage'              => 'xsd:integer',
				'sortBy'               => 'xsd:string',
				'username'             => 'xsd:string',
				'password'             => 'xsd:string'
			),
			'out' => array(
				'recordsFound' => 'xsd:integer',
				'timeElapsed'  => 'xsd:double',
				'errorCode'    => 'xsd:integer',
				'errorMessage' => 'xsd:string'
			)
		),
		$namespace,
		"Performs a search across a specific event with filters, paging, and sort options with authentication, but only returns the number of results found."
	);

	
	
	shn_plus_registerOne($server, 'findMostRecentReportAsFiltered',
		array(
			'in' => array(
				'machine'            => 'xsd:string',
				'login'              => 'xsd:string',
				'username'           => 'xsd:string',
				'hospitalShortName'  => 'xsd:string',
				'eventShortName'     => 'xsd:string',
				'closedEventsIncl'   => 'xsd:string',
				'hospitalEventsIncl' => 'xsd:string',
				'publicEventsIncl'   => 'xsd:string',
				'practiceMode'       => 'xsd:string',
				'expiredIncl'        => 'xsd:string'
			),
			'out' => array(
				'machine'           => 'xsd:string',
				'login'             => 'xsd:string',
				'username'          => 'xsd:string',
				'hospitalShortName' => 'xsd:string',
				'eventShortName'    => 'xsd:string',
				'mcID'              => 'xsd:string',
				'creationDate'      => 'xsd:string'
			)
		),
		$namespace,
		"Assists a fresh install of TriagePic, either de novo or where the local data in the Shared TriagePic Data folder has been wiped."
	);	
				
	
	
	// REPORTING API DECLARATIONS //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// //
	// REPORTING API DECLARATIONS //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// //

	shn_plus_registerOne($server, 'reportPerson',
		array(
			'in' => array(
				'personXML'      => 'xsd:string',
				'eventShortName' => 'xsd:string',
				'xmlFormat'      => 'xsd:string',
				'username'       => 'xsd:string',
				'password'       => 'xsd:string'
			),
			'out' => array(
				'url'            => 'xsd:string',
				'errorCode'      => 'xsd:integer',
				'errorMessage'   => 'xsd:string'
			)
		),
		$namespace,
		"Report a new person using the LPF v1.2 XML format."
	);

	
	// Record Revision API DECLARATIONS /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// //
	// Record Revision API DECLARATIONS /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// //

	shn_plus_registerOne($server, 'reReportPerson',
		array(
			'in' => array(
				'uuid'           => 'xsd:string',
				'personXML'      => 'xsd:string',
				'eventShortname' => 'xsd:string',
				'xmlFormat'      => 'xsd:string',
				'username'       => 'xsd:string',
				'password'       => 'xsd:string'
			),
			'out' => array(
				'errorCode'      => 'xsd:integer',
				'errorMessage'   => 'xsd:string'
			)
		),
		$namespace,
		"Re-report a person using XML which will replace an existing record."
	);

	shn_plus_registerOne($server, 'expirePerson',
		array(
			'in' => array(
				'uuid'           => 'xsd:string',
				'explanation'    => 'xsd:string',
				'username'       => 'xsd:string',
				'password'       => 'xsd:string'
			),
			'out' => array(
				'queued'         => 'xsd:boolean',
				'errorCode'      => 'xsd:integer',
				'errorMessage'   => 'xsd:string'
			)
		),
		$namespace,
		"Owners, admins, and HSA can immediately expire a record, everyone else can submit an expiration request to a moderated queue."
	);

	shn_plus_registerOne($server, 'reportAbuse',
			array(
					'in' => array(
							'uuid'           => 'xsd:string',
							'identifier'     => 'xsd:string'
					),
					'out' => array(
							'errorCode'      => 'xsd:integer',
							'errorMessage'   => 'xsd:string'
					)
			),
			$namespace,
			"A flag is set for a moderator to review a particular record for potential spam or other reason."
	);
	
	shn_plus_registerOne($server, 'getPersonExpiryDate',
		array(
			'in' => array(
				'uuid'           => 'xsd:string',
			),
			'out' => array(
				'expiryDate'     => 'xsd:string',
				'errorCode'      => 'xsd:integer',
				'errorMessage'   => 'xsd:string'
			)
		),
		$namespace,
		"Get the datetime in UTC when a person record will expire."
	);

	shn_plus_registerOne($server, 'setPersonExpiryDate',
		array(
			'in' => array(
				'uuid'           => 'xsd:string',
				'expiryDate'     => 'xsd:string',
				'username'       => 'xsd:string',
				'password'       => 'xsd:string'
			),
			'out' => array(
				'queued'         => 'xsd:boolean',
				'errorCode'      => 'xsd:integer',
				'errorMessage'   => 'xsd:string'
			)
		),
		$namespace,
		"Owners, admins, and HSA can immediately update a records expiry date, everyone else can submit a request to update the expiry date to a moderated queue."
	);

	shn_plus_registerOne($server, 'setPersonExpiryDateOneYear',
		array(
			'in' => array(
				'uuid'           => 'xsd:string',
				'username'       => 'xsd:string',
				'password'       => 'xsd:string'
			),
			'out' => array(
				'queued'         => 'xsd:boolean',
				'errorCode'      => 'xsd:integer',
				'errorMessage'   => 'xsd:string'
			)
		),
		$namespace,
		"Owners, admins, and HSA can immediately update a records expiry date, everyone else can submit a request to update the expiry date to a moderated queue. The expiry date in this case is updated to exactly one year from the time of the request."
	);

	shn_plus_registerOne($server, 'getUuidByMassCasualtyId',
		array(
			'in' => array(
				'mcid'           => 'xsd:string',
				'shortname'      => 'xsd:string',
				'username'       => 'xsd:string',
				'password'       => 'xsd:string'
			),
			'out' => array(
				'uuid'           => 'xsd:string',
				'errorCode'      => 'xsd:integer',
				'errorMessage'   => 'xsd:string'
			)
		),
		$namespace,
		"A simple lookup of the internal uuid of a person based on their mass casualty id (if it exists)."
	);

	shn_plus_registerOne($server, 'changeMassCasualtyId',
		array(
			'in' => array(
				'newMcid'        => 'xsd:string',
				'uuid'           => 'xsd:string',
				'username'       => 'xsd:string',
				'password'       => 'xsd:string'
			),
			'out' => array(
				'errorCode'      => 'xsd:integer',
				'errorMessage'   => 'xsd:string'
			)
		),
		$namespace,
		"Updates the mass casualty id of a person based on their uuid."
	);

	shn_plus_registerOne($server, 'hasRecordBeenRevised',
		array(
			'in' => array(
				'uuid'           => 'xsd:string',
				'username'       => 'xsd:string',
				'password'       => 'xsd:string'
			),
			'out' => array(
				'revisionsMade'  => 'xsd:integer',
				'errorCode'      => 'xsd:integer',
				'errorMessage'   => 'xsd:string'
			)
		),
		$namespace,
		"A simple lookup to determine if a record is still an original or has incurred revisions."
	);

	shn_plus_registerOne($server, 'addComment',
		array(
			'in' => array(
				'uuid'               => 'xsd:string',
				'comment'            => 'xsd:string',
				'suggested_status'   => 'xsd:string',
				'suggested_location' => 'xsd:string',
				'suggested_image'    => 'xsd:string',
				'username'           => 'xsd:string',
				'password'           => 'xsd:string'
			),
			'out' => array(
				'errorCode'      => 'xsd:integer',
				'errorMessage'   => 'xsd:string'
			)
		),
		$namespace,
		"Add a text comment and suggest a status, location, and image for a person."
	);

	shn_plus_registerOne($server, 'getPersonPermissions',
		array(
			'in' => array(
				'uuid'           => 'xsd:string',
				'username'       => 'xsd:string',
				'password'       => 'xsd:string'
			),
			'out' => array(
				'owner'          => 'xsd:string',
				'groupIDs'       => 'xsd:string',
				'errorCode'      => 'xsd:integer',
				'errorMessage'   => 'xsd:string'
			)
		),
		$namespace,
		"Find out who has permission to revise a record."
	);

	// Image Indexing API DECLARATIONS /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// //
	// Image Indexing API DECLARATIONS /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// //

	shn_plus_registerOne($server, 'getImageCountsAndTokens',
		array(
			'in' => array(
				'key'            => 'xsd:string'
			),
			'out' => array(
				'imageCount'     => 'xsd:integer',
				'firstToken'     => 'xsd:integer',
				'lastToken'      => 'xsd:integer',
				'nullTokenCount' => 'xsd:integer',
				'errorCode'      => 'xsd:integer',
				'errorMessage'   => 'xsd:string'
			)
		),
		$namespace,
		"Returns a count of the total number of images and the value of the latest brownie (image index value)."
	);

	shn_plus_registerOne($server, 'getImageList',
		array(
			'in' => array(
				'tokenStart'     => 'xsd:integer',
				'tokenEnd'       => 'xsd:integer',
				'key'            => 'xsd:string'
			),
			'out' => array(
				'imageList'      => 'xsd:string',
				'errorCode'      => 'xsd:integer',
				'errorMessage'   => 'xsd:string'
			)
		),
		$namespace,
		"Returns a list of the images and their associated properties, paginated by a start and end token (index value)."
	);

	shn_plus_registerOne($server, 'getImageListBlock',
		array(
			'in' => array(
				'tokenStart'     => 'xsd:integer',
				'stride'         => 'xsd:integer',
				'key'            => 'xsd:string'
			),
			'out' => array(
				'imageList'      => 'xsd:string',
				'errorCode'      => 'xsd:integer',
				'errorMessage'   => 'xsd:string'
			)
		),
		$namespace,
		"Returns a list of the images and their associated properties, in blocks of stride size beginning with a start token (index value)."
	);

	shn_plus_registerOne($server, 'getNullTokenList',
		array(
			'in' => array(
				'tokenStart'     => 'xsd:integer',
				'tokenEnd'       => 'xsd:integer',
				'key'            => 'xsd:string'
			),
			'out' => array(
				'nullTokenList'  => 'xsd:string',
				'errorCode'      => 'xsd:integer',
				'errorMessage'   => 'xsd:string'
			)
		),
		$namespace,
		"Returns a list of the images that have been removed from the system (either deleted or expired)."
	);
    
    // test exporting API
    shn_plus_registerOne($server, 'exportPerson',
		array(
			'in' => array(
                'incidentId'  => 'xsd:integer',
			),
			'out' => array(
				'export_person' => 'xsd:string',
				'errorCode'    => 'xsd:integer',
				'errorMessage' => 'xsd:string'
			)
		),
		$namespace,
		"test Exporting Person data..."
	);
}

// test exportPFIF data for inicidentId
function exportPerson($incidentId) {
	shn_plus_logAccess("exportPerson");
	global $global;
        
    $sql = "SELECT * from person_uuid where incident_id='".mysql_real_escape_string($incidentId)."'";
	$result = $global['db']->Execute($sql);
        
    $list = array();
	while (!$result == NULL && !$result->EOF) {
            $p = new Person();
            $p->p_uuid = $result->fields['p_uuid'];
            $p->load();
            $p->prepareSerialize();
            $list[] = serialize($p);            
            $result->MoveNext();
	}
	return array(
		'export_person' => (string)json_encode($list),
		'errorCode'    => (int)0,
		'errorMessage' => null
	);
}



