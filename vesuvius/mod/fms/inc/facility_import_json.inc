<?php

/**
 * Description of facilityImportXLS
 *
 * @author ckramer
 */
class facilityImportJSON
{

    public $importFacilitySpec = array(
      'id' => array('type' => 'integer', 'autoincrement' => true, 'primary' => true),
      'facility_name' => array('type' => "string", 'length' => 64),
      'facility_code' => array('type' => "string", 'length' => 10),
      'facility_resource_type_abbr' => array('type' => "string", 'length' => 10),
      'facility_resource_status' => array('type' => "string", 'length' => 40),
      'facility_capacity' => array('type' => "integer"),
      'facility_activation_sequence' => array('type' => "integer"),
      'facility_allocation_status' => array('type' => "string", 'length' => 30),
      'facility_group' => array('type' => "string", 'length' => 64),
      'facility_group_type' => array('type' => "string", 'length' => 30),
      'facility_group_allocation_status' => array('type' => "string", 'length' => 30),
      'work_email' => array('type' => "string", 'length' => 255),
      'work_phone' => array('type' => "string", 'length' => 32),
      'street_1' => array('type' => "string", 'length' => 255),
      'street_2' => array('type' => "string", 'length' => 255),
      'city' => array('type' => "string", 'length' => 255),
      'state' => array('type' => "string", 'length' => 255),
      'postal_code' => array('type' => "string", 'length' => 30),
      'borough' => array('type' => "string", 'length' => 30),
      'country' => array('type' => "string", 'length' => 10),
      'longitude' => array('type' => "decimal"),
      'latitude' => array('type' => "decimal")
    );
    // Errors go here
    public $eventLog = array();
    public $totalImported = 0;
    public $totalUpdated = 0;
    public $totalNew = 0;
    public $totalErrors = 0;
    public $userAccount = array();
    public $geocode = false;
    private $createUsers;
    private $genPassword;
    private $manPassword;
    private $defaultPassword;
    private $overWrite;

    /**
     * saveImportData()
     *
     * Writes import array to temp table
     *
     * @param $importData
     */
    public function saveImportData($importData, $incident)
    {
        global $global;

        if (empty($importData)) {
            $this->eventLog[] = array("type" => "ERROR", "message" => "No facility data found for this incident. Please make sure this event is properly setup on the SEMS server.");
            $this->totalErrors++;
        } else {

            // Open a database transaction
            $global['db']->StartTrans();

            // Create an INSERT query for each facility
            foreach ($importData as $row) {



                // Re-map the keys
                if (isset($row['capacity'])) {
                    $row['facility_capacity'] = $row['capacity'];
                    unset($row['capacity']);
                }
                if (isset($row['facility_resource_activation_sequence'])) {
                    $row['facility_activation_sequence'] = $row['facility_resource_activation_sequence'];
                    unset($row['facility_resource_activation_sequence']);
                }
                if (isset($row['facility_resource_allocation_status'])) {
                    $row['facility_allocation_status'] = $row['facility_resource_allocation_status'];
                    unset($row['facility_resource_allocation_status']);
                }

                if (isset($row['line_1'])) {
                    $row['street_1'] = $row['line_1'];
                    unset($row['line_1']);
                } else {
                    $row['street_1'] = null;
                }
                if (isset($row['line_2'])) {
                    $row['street_2'] = $row['line_2'];
                    unset($row['line_2']);
                } else {
                    $row['street_2'] = null;
                }


                if (isset($row['email'])) {
                    $row['work_email'] = $row['email'];
                    unset($row['email']);
                } else {
                    $row['work_email'] = null;
                }
                if (isset($row['phone'])) {
                    $row['work_phone'] = $row['phone'];
                    unset($row['phone']);
                } else {
                    $row['work_phone'] = null;
                }

                if (isset($row['zip5'])) {
                    $row['postal_code'] = $row['zip5'];
                    unset($row['zip5']);
                } else {
                    $row['postal_code'] = null;
                }
                if (empty($row['city'])) {
                    $row['city'] = null;
                }
                if (empty($row['state'])) {
                    $row['state'] = null;
                }
                if (empty($row['borough'])) {
                    $row['borough'] = null;
                }
                if (empty($row['country'])) {
                    $row['country'] = null;
                }
                if (empty($row['latitude'])) {
                    $row['latitude'] = null;
                }
                if (empty($row['longitude'])) {
                    $row['longitude'] = null;
                }

                if ($this->geocode) {

                    $address = $row['street_1'] . " ";
                    $address .= $row['city'] . " ";
                    $address .= $row['state'] . ", ";
                    $address .= $row['postal_code'];

                    $latLon = $this->getGeoCoordinates($address);

                    $row['latitude'] = $latLon['lat'];
                    $row['longitude'] = $latLon['lon'];
                }

                $facility_uuid = $this->checkExisting($row);
                if ($facility_uuid) {
                    // Update record
                    $query = $this->makeUpdateStatment($row);
                    $this->totalUpdated++;
                } else {
                    // Make an INSERT
                    $query = $this->makeInsertStatment($row);
                    $this->totalNew++;
                }

                // Make facility table insert or update
                try {
                    $global['db']->Execute($query);
                } catch (exception $e) {
                    $this->eventLog[] = array("type" => "ERROR", "message" => $e->getMessage());
                    $this->totalErrors++;
                }

                // Get the id of the last insert
                if (!$facility_uuid) {
                    $facility_uuid = $global['db']->Insert_ID();
                }

                // Update faciltiy to incident
                $this->updateFacilityIncident($facility_uuid, $row['facility_resource_type_abbr'], $incident);

                // Create user account
                $username = strtolower($row['facility_resource_type_abbr'] . $row['facility_code'] . $incident);

                // Create user property
                if ($this->createUsers) {

                    // Password property
                    if ($this->manPassword) {
                        $password = $this->defaultPassword;
                    } else {
                        // Genderate password
                        $valid = false;
                        while ($valid != 1) {
                            $password = $this->generatePassword(array('length' => 10));
                            $valid = shn_check_password_matches_policy($password, $username);
                        }
                    }

                    // Get the p_uuid for the username
                    $uuid = $this->getPuuid($username, $incident);

                    // Get the user_id
                    $userId = $this->getUserId($uuid);

                    // See if we need to make a user account
                    if (empty($userId)) {
                        $this->addUserAccount($uuid, $username, $password);
                        $status = 'new';
                    } elseif ($this->overWrite) {
                        // Overwrite the user account passwords
                        $this->updateUserAccount($uuid, $username, $password);
                        $status = 'updated';
                    } else {
                        // Don't show the new generated password because the old one wasn't overwritten. 
                        $password = '';
                        $status = 'unchanged';
                    }

                    $this->userAccount[] = array(
                      'incident' => $incident,
                      'facility' => $row['facility_name'],
                      'p_uuid' => $uuid,
                      'username' => $username,
                      'password' => $password,
                      'status' => $status
                    );
                }
            }

            // Check for any failed transactions
            $error = $global['db']->HasFailedTrans();

            // Close transaction
            $global['db']->CompleteTrans();

            $this->totalImported = $this->totalNew + $this->totalUpdated;
        }
    }

    private function addUserAccount($uuid, $username, $password)
    {

        global $global;

        // Create the user account
        $salt1 = _shn_generateSalt();
        $salt2 = _shn_generateSalt();
        $salt = $salt1 . $salt2;
        $user_password = substr($password, 0, 4) . $salt . substr($password, 4);
        $stored_password = md5(trim($user_password));
        $time = time();

        $query = "INSERT INTO users (p_uuid, user_name, password, salt, changed_timestamp, status, created_at, updated_at)
            VALUES (?, ?, ?, ?, ?, 'active', CURRENT_TIMESTAMP, CURRENT_TIMESTAMP)";
        $result = $global['db']->Execute($query, array($uuid, $username, $stored_password, $salt, $time));

        // Add to the registred users group
        $query = "INSERT INTO sys_user_to_group VALUES (?, ?, CURRENT_TIMESTAMP, CURRENT_TIMESTAMP)";
        $result = $global['db']->Execute($query, array(2, $uuid));
    }

    private function updateUserAccount($uuid, $username, $password)
    {

        global $global;

        // Create the user account
        $salt1 = _shn_generateSalt();
        $salt2 = _shn_generateSalt();
        $salt = $salt1 . $salt2;
        $user_password = substr($password, 0, 4) . $salt . substr($password, 4);
        $stored_password = md5(trim($user_password));
        $time = time();

        $query = "UPDATE users SET password = ?, salt = ?, changed_timestamp = ?
            WHERE p_uuid = ?";
        $result = $global['db']->Execute($query, array($stored_password, $salt, $time, $uuid));
    }

    private function getPuuid($username, $incidentId)
    {
        global $global;

        include_once $global['approot'] . "/inc/lib_uuid.inc";
        include_once $global['approot'] . "/inc/lib_location.inc";

        // Check for existing p_uuid
        $query = "SELECT p_uuid FROM person_uuid WHERE full_name = ? AND incident_id = ?";
        $result = $global['db']->Execute($query, array($username, $incidentId));
        $uuid = $result->fields[0];

        if (empty($uuid)) {
            $uuid = shn_create_uuid('person');

            $query = "INSERT INTO person_uuid (p_uuid, full_name, incident_id) VALUES (?, ?, ?)";
            $global['db']->Execute($query, array($uuid, $username, $incidentId));
        }

        return $uuid;
    }

    private function getUserId($uuid)
    {
        global $global;

        // Check to see if the user already has a user_id
        $query = "SELECT user_id FROM users WHERE p_uuid = ?";
        $result = $global['db']->Execute($query, $uuid);
        $userId = $result->fields[0];

        return $userId;
    }

    public function getUserGroupId($incidentName)
    {
        global $global;

        // Check to see if the user group already exists
        $query = "SELECT group_id FROM sys_user_groups WHERE group_name = ?";
        $results = $global['db']->Execute($query, $incidentName);
        $groupId = $results->fields[0];

        if (empty($groupId)) {

            // Get the max group_id because the table doesn't AI :(
            $query = "SELECT MAX(group_id) FROM sys_user_groups";
            $result = $global['db']->Execute($query);
            $groupId = $result->fields[0] + 1;

            // Create a new group
            $query = "INSERT INTO sys_user_groups VALUES (?, ?, CURRENT_TIMESTAMP, CURRENT_TIMESTAMP)";
            $result = $global['db']->Execute($query, array($groupId, $incidentName));
        }
    }

    private function checkExisting($row)
    {

        global $global;

        $query = "SELECT facility_uuid FROM fms_facility
              WHERE facility_name = ? AND facility_code = ? AND facility_resource_type_abbr = ?";

        $values = array($row["facility_name"], $row["facility_code"], $row["facility_resource_type_abbr"]);

        try {
            $query = $global['db']->Prepare($query);
            $rs = $global['db']->Execute($query, $values);
        } catch (exception $e) {
            $this->eventLog[] = array("type" => "ERROR", "message" => "<pre>" . $e->getMessage() . "</pre>");
        }

        $result = $rs->FetchRow();

        return isset($result['facility_uuid']) ? $result['facility_uuid'] : false;
    }

    private function makeUpdateStatment($row)
    {

        // Prepare the INSERT SQL template
        $query = "UPDATE fms_facility 
            SET %s 
            WHERE facility_name = %s 
            AND facility_code=%s
            AND facility_resource_type_abbr=%s;";

        $updates = "";
        $val = "";

        foreach (array_keys($this->importFacilitySpec) as $column) {

            // Ignore id key name because it is the auto
            if ($column != "id") {

                // Handle null values with sane defaults
                switch ($this->importFacilitySpec[$column]["type"]) {
                    case "integer":
                        if ($row[$column] == "") {
                            $val = 0;
                        } else {
                            $val = $row[$column];
                        };
                        break;
                    case "decimal":
                        if ($row[$column] == "") {
                            $val = 0.0;
                        } else {
                            $val = $row[$column];
                        }
                        break;
                    default:
                        $val = trim($row[$column]);
                }

                $update = sprintf("\n\t`%s` = %s,", $column, shn_db_clean($val));
                $updates .= $update;
            }
        }

        // Chop off the trailing comma
        $updates = substr($updates, 0, -1);

        $query = sprintf($query, $updates, shn_db_clean($row["facility_name"]), shn_db_clean($row["facility_code"]), shn_db_clean($row["facility_resource_type_abbr"]));

        return $query;
    }

    private function makeInsertStatment($row)
    {

        // Prepare the INSERT SQL template
        $query = "INSERT INTO fms_facility (%s) \nVALUES (%s\n);";
        $cols = "";
        $vals = "";

        foreach (array_keys($this->importFacilitySpec) as $column) {

            // Ignore id key name because it is the auto
            if ($column != "id") {
                $col = sprintf("\n\t`%s`,", $column);
                $cols = $cols . $col;

                // Handle null values with sane defaults
                switch ($this->importFacilitySpec[$column]["type"]) {
                    case "integer":
                        if ($row[$column] == "") {
                            $val = 0;
                        } else {
                            $val = $row[$column];
                        };
                        break;
                    case "decimal":
                        if ($row[$column] == "") {
                            $val = 0.0;
                        } else {
                            $val = $row[$column];
                        }
                        break;
                    default:
                        $val = trim($row[$column]);
                }

                // Safe the value
                $val = shn_db_clean($val);

                $val = sprintf("\n\t%s,", $val);
                $vals = $vals . "$val";
            }
        }

        // Append creation timestamp
        $cols .= " `created_at`";
        $vals .= " CURRENT_TIMESTAMP";

        $query = sprintf($query, $cols, $vals);

        return $query;
    }

    private function updateFacilityIncident($facility, $type, $incident)
    {

        global $global;

        $table = 'fms_facility_to_event';

        // Check for existing record
        $sql = "SELECT facility_uuid FROM $table WHERE facility_uuid = ? AND incident_id = ?";
        $facilityId = $global['db']->GetOne($sql, array($facility, $incident));
        $mode = (isset($facilityId)) ? 'UPDATE' : 'INSERT';

        // Assign record values
        $record = array();
        $record['facility_uuid'] = $facility;
        $record['incident_id'] = $incident;

        if ($mode == 'UPDATE') {
            // Update existing record
            $global['db']->AutoExecute($table, $record, $mode, "facility_uuid = '$facility' AND incident_id = '$incident'");
        } else {
            // Insert new record
            $global['db']->AutoExecute($table, $record, $mode);
        }
    }

    public function generatePassword($params = array())
    {
        $password = "";

        $length = (!array_key_exists('length', $params)) ? 15 : $params['length'];
        $use_lower = (!array_key_exists('use_lower', $params)) ? TRUE : $params['use_lower'];
        $use_upper = (!array_key_exists('use_upper', $params)) ? TRUE : $params['use_upper'];
        $use_number = (!array_key_exists('use_number', $params)) ? TRUE : $params['use_number'];
        $use_special = (!array_key_exists('use_special', $params)) ? TRUE : $params['use_special'];
        $use_custom = (!array_key_exists('use_custom', $params)) ? '-_()' : $params['use_custom'];

        $upper = "ABCDEFGHIJKLMNPQRSTUVWXYZ";
        $lower = "abcdefghijklmnpqrstuvwxyz";
        $number = "0123456789";
        $special = "!@#$%^&*()-+=.;";

        $seed_length = 0;
        $seed = '';

        if ($use_upper === TRUE) {
            $seed_length += strlen($upper);
            $seed .= $upper;
        }
        if ($use_lower === TRUE) {
            $seed_length += strlen($lower);
            $seed .= $lower;
        }
        if ($use_number === TRUE) {
            $seed_length += strlen($number);
            $seed .= $number;
        }
        if ($use_special === TRUE) {
            $seed_length += strlen($special);
            $seed .= $special;
        }
        if (!empty($use_custom)) {
            $seed_length +=strlen($use_custom);
            $seed .= $use_custom;
        }
        for ($i = 1; $i <= $length; $i++) {
            $password .= $seed{rand(0, $seed_length - 1)};
        }
        return $password;
    }

    public function setProperties($createUsers = true, $genPassword = true, $manPassword = false, $defaultPassword = null, $overWrite = false)
    {
        $this->createUsers = $createUsers;
        $this->genPassword = $genPassword;
        $this->manPassword = $manPassword;
        $this->defaultPassword = $defaultPassword;
        $this->overWrite = $overWrite;
    }

    private function getGeoCoordinates($address)
    {
        $geocode = file_get_contents('http://maps.google.com/maps/api/geocode/json?address=' . rawurlencode($address) . '&sensor=false');
        $output = json_decode($geocode);
        $lat = $output->results[0]->geometry->location->lat;
        $long = $output->results[0]->geometry->location->lng;

        return array('lat' => $lat, 'lon' => $long);
    }

}

?>
