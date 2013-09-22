<?php

/**
 * Client Registry Service Module
 *
 * PHP version >= 5.1
 *
 * LICENSE: This source file is subject to LGPL license
 * that is available through the world-wide-web at the following URI:
 * http://www.gnu.org/copyleft/lesser.html
 *
 * @author     Clayton Kramer <clayton.kramer@mail.cuny.edu>
 * @package    module CRS
 * @license    http://www.gnu.org/copyleft/lesser.html GNU Lesser General Public License (LGPL)
 *
 */
// Query for the person's data
$GLOBALS["query_person_fields"] = "
          p.p_uuid,
          p.given_name,
          p.custom_name,
          p.family_name,
          p.full_name,
          pd.birth_date,
          pd.years_old,
          pd.opt_country,
          pd.occupation,
          pd.opt_gender,
          ps.opt_status,
          ps.last_updated,
          ps.creation_time,
          c1.contact_value AS home_phone,
          c2.contact_value AS mobile_phone,
          c3.contact_value AS alt_phone,
          c4.contact_value AS email,
          c5.contact_value AS street_1,
          c6.contact_value AS street_2,
          c7.contact_value AS city,
          c8.contact_value AS state,
          c9.contact_value AS postal,
          ptf.in_date,
          ptf.out_date,
          f.facility_uuid,
          f.facility_name,
          f.facility_group,
          sl1.language AS altlang1,
          sl2.language AS altlang2,
          sl3.language AS altlang3,
          ec.name AS ec_name,
          ec.phone AS ec_phone,
          ec.relationship AS ec_relation,
          ma.injury,
          ma.special_medical,
          ma.special_mental,
          IFNULL(gd.adults,1) AS adults,
          IFNULL(gd.children,0) AS children,
          IFNULL(gd.infants,0) AS infants,
          IFNULL(gd.unknown,0) AS unknown,
          IFNULL(ptg.p_uuid_2, '') AS group_primary,
          (gd.adults + gd.children + gd.infants) AS group_count,
          cr.relation,
          IFNULL(p2.full_name, '') AS client_group_name
          ";

$GLOBALS["query_person_tables"] = "FROM person_uuid p
          INNER JOIN crs_person_to_group ptg ON p.p_uuid = ptg.p_uuid_1
          LEFT JOIN crs_common_relations cr ON ptg.relation = cr.id
          LEFT JOIN person_uuid p2 ON ptg.p_uuid_2 = p2.p_uuid
          LEFT JOIN person_details pd ON p.p_uuid = pd.p_uuid
          LEFT JOIN person_status ps ON p.p_uuid = ps.p_uuid
          LEFT JOIN contact c1 ON (c1.p_uuid = p.p_uuid AND c1.opt_contact_type = 'curr')
          LEFT JOIN contact c2 ON (c2.p_uuid = p.p_uuid AND c2.opt_contact_type = 'cmob')
          LEFT JOIN contact c3 ON (c3.p_uuid = p.p_uuid AND c3.opt_contact_type = 'calt')
          LEFT JOIN contact c4 ON (c4.p_uuid = p.p_uuid AND c4.opt_contact_type = 'email')
          LEFT JOIN contact c5 ON (c5.p_uuid = p.p_uuid AND c5.opt_contact_type = 'street_1')
          LEFT JOIN contact c6 ON (c6.p_uuid = p.p_uuid AND c6.opt_contact_type = 'street_2')
          LEFT JOIN contact c7 ON (c7.p_uuid = p.p_uuid AND c7.opt_contact_type = 'city')
          LEFT JOIN contact c8 ON (c8.p_uuid = p.p_uuid AND c8.opt_contact_type = 'state')
          LEFT JOIN contact c9 ON (c9.p_uuid = p.p_uuid AND c9.opt_contact_type = 'postal')
          LEFT JOIN fms_person_to_facility ptf ON p.p_uuid = ptf.p_uuid
          LEFT JOIN fms_facility f ON f.facility_uuid = ptf.facility_uuid
          LEFT JOIN crs_person_to_language pl1 ON p.p_uuid = pl1.p_uuid AND pl1.ordinal = 1
          LEFT JOIN crs_language sl1 ON pl1.language = sl1.id
          LEFT JOIN crs_person_to_language pl2 ON p.p_uuid = pl2.p_uuid AND pl2.ordinal = 2
          LEFT JOIN crs_language sl2 ON pl2.language = sl2.id
          LEFT JOIN crs_person_to_language pl3 ON p.p_uuid = pl3.p_uuid AND pl3.ordinal = 3
          LEFT JOIN crs_language sl3 ON pl3.language = sl3.id
          LEFT JOIN crs_emerg_cont ec ON p.p_uuid = ec.p_uuid
          LEFT JOIN crs_med_alert ma ON p.p_uuid = ma.p_uuid
          LEFT JOIN crs_group_details gd ON p.p_uuid = gd.p_uuid
          ";

$GLOBALS['expr_array'] = array();

/*
 * Helper function returns a record set of client data
 *
 * @access public
 * @return object
 */

function get_client($uuid)
{
    global $global;

    // Now the actual query
    $sql = "SELECT * FROM crs_client_search WHERE p_uuid = ?";

    // Execute client query
    try {
        $sql = $global['db']->Prepare($sql);
        $rs = $global['db']->Execute($sql, $uuid);

    } catch (exception $e) {
        add_error("<pre>" . $e->getMessage() . "</pre>");
    }

    $client = $rs->fetchRow();
    return $client;
}

/*
 * Helper function for returning a client's skills
 * 
 * @access public
 * @return string
 */

function get_client_skills($uuid)
{
    global $global;

    // Get client's skills
    $sql = "SELECT skill FROM crs_person_skills WHERE p_uuid = ?";

    // Execute query
    try {
        $sql = $global['db']->Prepare($sql);
        $skills = $global['db']->Execute($sql, $uuid);
    } catch (exception $e) {
        add_error("<pre>" . $e->getMessage() . "</pre>");
    }

    $skillsList = "";
    foreach ($skills as $skill) {
        $skillsList .= $skill["skill"] . ", ";
    }

    if (trim($skillsList) == ",") {
        $skillsList = "";
    }

    return $skillsList;
}

/**
 *  This function is a helper for saving client languages
 * @access public
 * @return void
 */
function save_language_info($language, $ordinal, $uuid, $mode)
{
    global $global;
    $language = trim($language);
    if (isset($language) && $language != '') {

        // Check to see if the language already exists
        $sql = "SELECT id FROM crs_language WHERE language = ?";
        $rs = $global['db']->Execute($sql, $language);

        if ($rs->RowCount() > 0) {
            $row = $rs->FetchRow();
            $id = $row['id'];
        } else {
            $record = array();
            $record['language'] = $language;
            $global['db']->AutoExecute('crs_language', $record, 'INSERT');
            $id = $global['db']->Insert_ID();
        }

        // Prepare person to language
        $table = 'crs_person_to_language';

        $record = array();
        $record['ordinal'] = $ordinal;
        $record['language'] = $id;

        if ($mode == 'UPDATE') {
            $global['db']->AutoExecute($table, $record, $mode, "p_uuid = '$uuid' AND ordinal = '$ordinal'");
        } else {
            $record['p_uuid'] = $uuid;
            $global['db']->AutoExecute($table, $record, $mode);
        }
    } else {
        $sql = "DELETE FROM crs_person_to_language WHERE ordinal = ? AND p_uuid = ?";
        $global['db']->Execute($sql, array($ordinal, $uuid));
    }
}

/**
 * Helper for saving client skills 
 * 
 * @param string $skills
 * @param int $uuid
 * @access public
 * @return void
 */
function save_skills($skills, $uuid)
{
    global $global;

    // Trim any trailing commas
    $skills = preg_replace('/, ?$/', '', $skills);
    $skills_split = preg_split('/,/', $skills);

    // Dedup the list using keys method, It is faster than array_unique()
    $skills_list = array();
    foreach ($skills_split as $key => $val) {
        $skillsList[trim($val)] = true;
    }
    $skillsList = array_keys($skillsList);

    // Purge any previous skills
    $sql = "DELETE FROM crs_person_skills WHERE p_uuid = ?";
    $global['db']->Execute($sql, $uuid);

    foreach ($skillsList as $skill) {
        $table = 'crs_person_skills';

        $record = array();
        $record['p_uuid'] = $uuid;
        $record['skill'] = $skill;

        $global['db']->AutoExecute($table, $record, 'INSERT');
    }
}

/**
 *  This function acts as a helper for updating contact information
 * @access public
 * @return void
 */
function save_contact_info($type, $value, $uuid)
{

    global $global;

    // Set table name
    $table = 'contact';

    if ($value) {

        // Check for existing record
        $sql = "SELECT p_uuid FROM $table WHERE p_uuid = ? AND opt_contact_type = ?";
        $rs = $global['db']->Execute($sql, array($uuid, $type));
        $mode = ($rs->RowCount() > 0) ? 'UPDATE' : 'INSERT';

        // Assign record values
        $record = array();
        $record['p_uuid'] = $uuid;
        $record['opt_contact_type'] = $type;
        $record['contact_value'] = $value;

        if ($mode == 'UPDATE') {
            // Update existing record
            $global['db']->AutoExecute($table, $record, $mode, "p_uuid = '$uuid' AND opt_contact_type = '$type'");
        } else {
            // Insert new record
            $global['db']->AutoExecute($table, $record, $mode);
        }
    } else {
        // Delete contact record value
        $sql = "DELETE FROM $table WHERE p_uuid = ? AND opt_contact_type = ?;";
        $global['db']->Execute($sql, array($uuid, $type));
    }
}

/**
 *  Helper for updating person status table
 *
 * @global type $global
 * @param type $status
 * @param type $uuid 
 */
function setPersonStatus($status, $uuid)
{

    global $global;
    global $conf;

    // Set table name
    $table = 'person_status';

    if ($status) {

        // Check for existing record
        $sql = "SELECT p_uuid FROM $table WHERE p_uuid = ?";
        $rs = $global['db']->Execute($sql, $uuid);
        $mode = ($rs->RowCount() > 0) ? 'UPDATE' : 'INSERT';

        // Assign record values
        $record = array();
        $record['p_uuid'] = $uuid;
        $record['opt_status'] = $status;
        $record['last_updated'] = date('Y-m-d H:i:s');

        if ($mode == 'UPDATE') {
            // Update existing record
            $global['db']->AutoExecute($table, $record, $mode, "p_uuid = '$uuid'");
        } else {
            $record['creation_time'] = date('Y-m-d H:i:s');
            // Insert new record
            $global['db']->AutoExecute($table, $record, $mode);
        }
    } else {
        // Delete contact record value
        $sql = "DELETE FROM $table WHERE p_uuid = ?;";
        $global['db']->Execute($sql, array($uuid, $type));
    }

    // Request Solr delta-import
    if ($conf['enable_solr_for_search'] == true) {
        $serialized_results = file_get_contents("http://" . $conf['SOLR_host'] . ":" . $conf['SOLR_port'] . "/solr/dataimport?command=delta-import");
    }
}

/**
 * Function returns the person status
 * 
 * @global object $global
 * @global array $conf
 * @param string $uuid
 * @return string  
 */
function getPersonStatus($uuid)
{
    global $global;
    global $conf;

    // Set table name
    $table = 'person_status';

    $query = "SELECT p_uuid FROM $table WHERE p_uuid = ? LIMIT 1";
    $result = $global['db']->Execute($query, $uuid);
    $row = $result->FetchRow();
    
    return $row['opt_status'];
}

/**
 * Helper function reconciles the crs_group_details
 * @access public
 * @param string $uuid
 * @return void
 */
function reconcile_group_details($uuid)
{

    global $global;

    // Get the group list
    $group = get_group_list($uuid);
    $success = false;
    $record = array('infants' => 0, 'children' => 0, 'adults' => 0, 'unknown' => 0);

    // Calculate
    foreach ($group as $member) {
        //print("<pre>" . print_r($member, true) . "</pre>");
        if (isset($member['years_old'])) {
            if ($member['years_old'] >= 0 && $member['years_old'] <= 3) {
                $record['infants']++;
            } else if ($member['years_old'] > 3 && $member['years_old'] <= 17) {
                $record['children']++;
            } else {
                $record['adults']++;
            }
        } else {
            $record['unknown']++;
        }
    }

    $table = 'crs_group_details';

    // Quick check for an existing record. 
    $sql = "SELECT p_uuid FROM $table WHERE p_uuid = ?";
    $rs = $global['db']->Execute($sql, $uuid);

    if ($rs->RowCount() > 0) {
        // Update
        try {
            $global['db']->AutoExecute($table, $record, 'UPDATE', "p_uuid = '$uuid'");
            $success = true;
        } catch (exception $e) {
            add_error("<pre>" . $e->getMessage() . "</pre>");
            $success = false;
        }
    } else {
        // Insert
        $record['p_uuid'] = $uuid;
        try {
            $global['db']->AutoExecute($table, $record, 'INSERT');
            $success = true;
        } catch (exception $e) {
            add_error("<pre>" . $e->getMessage() . "</pre>");
            $success = false;
        }
    }

    return $success;
}

/**
 *  Helper for listing groups
 * @access public
 * @param string $uuid
 * @return object
 */
function get_group_list($uuid)
{

    global $global;

    // Get the group's member names
    $sql = "SELECT 
        p_uuid,
        full_name,
        IF(group_primary = '', 'Group Representative', relation) AS relation,
        IF(group_primary = ?, 0,  1) AS is_primary,
        facility_name,
        facility_uuid,
        opt_gender,
        years_old,
/*        occupation, */
        altlang1 AS lang1,
        altlang2 AS lang2,
        altlang3 AS lang3
    FROM crs_client_search 
    WHERE incident_id = ? AND (p_uuid = ? OR group_primary = ?)
    ORDER BY full_name";

    // Execute query
    try {
        $sql = $global['db']->Prepare($sql);
        $rs = $global['db']->Execute($sql, array($uuid, $_SESSION['incident'], $uuid, $uuid));
    } catch (exception $e) {
        add_error("<pre>" . $e->getMessage() . "</pre>");
    }

    return $rs;
}

/**
 * This function is a helper for getting client relationship id's
 * @access public
 * @param string $relation
 * @return int
 */
function get_relation_id($relation)
{
    global $global;
    $relation = trim($relation);
    
    if(empty($relation)) {
        $relation = "NA";
    }

    // Look up relationship id
    $sql = "SELECT id FROM crs_common_relations WHERE relation = ?";
    $rs = $global['db']->Execute($sql, $relation);

    if ($rs->RowCount() > 0) {
        $row = $rs->FetchRow();
        $id = $row['id'];
    } else {
        $record = array();
        $record['relation'] = $relation;
        $global['db']->AutoExecute('crs_common_relations', $record, 'INSERT');
        $id = $global['db']->Insert_ID();
    }

    return $id;
}


function getPets($uuid) {
    
    global $global;
    
    $query = "SELECT * FROM crs_pets WHERE p_uuid = ?";
    $result = $global['db']->Execute($query, $uuid);
    
    return $result->GetAssoc();
}

?>
