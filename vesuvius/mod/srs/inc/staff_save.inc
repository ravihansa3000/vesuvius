<?php

/**
 * Staff Registration System Module
 *
 * PHP version >= 5.1
 *
 * LICENSE: This source file is subject to LGPL license
 * that is available through the world-wide-web at the following URI:
 * http://www.gnu.org/copyleft/lesser.html
 *
 * @author     Clayton Kramer <clayton.kramer@mail.cuny.edu>
 * @package    module srs
 * @license    http://www.gnu.org/copyleft/lesser.html GNU Lesser General Public License (LGPL)
 *
 */

/**
 * This function updates the primary shelteree's data
 * @access public
 * @return void
 */
function staff_save($uuid = null)
{
    global $global;
    global $conf;
    $mode = 'INSERT';
    $error = false;


    // Determine if we need to create a new staff or update an inidiviudal's information
    if ($uuid == null) {

        // Generate a uuid if this is a new staff member
        $uuid = shn_create_uuid();
        $mode = 'INSERT';
    } else {

        // Get the staff's information
        $staff = get_staff($uuid);

        $mode = 'UPDATE';
    }

    // Collect form submission
    // Basics
    isset($_REQUEST['given_name']) ? $given_name = $_REQUEST['given_name'] : $given_name = null;
    isset($_REQUEST['middle_initial']) ? $middle_initial = $_REQUEST['middle_initial'] : $middle_initial = null;
    isset($_REQUEST['family_name']) ? $family_name = $_REQUEST['family_name'] : $family_name = null;

    // Home Address
    isset($_REQUEST['street_1']) ? $street_1 = $_REQUEST['street_1'] : $street_1 = null;
    isset($_REQUEST['street_2']) ? $street_2 = $_REQUEST['street_2'] : $street_2 = null;
    isset($_REQUEST['city']) ? $city = $_REQUEST['city'] : $city = null;
    isset($_REQUEST['state']) ? $state = $_REQUEST['state'] : $state = null;
    isset($_REQUEST['postal']) ? $postal = $_REQUEST['postal'] : $postal = null;
    isset($_REQUEST['opt_country']) ? $opt_country = $_REQUEST['opt_country'] : $opt_country = null;


    // Work Phone
    isset($_REQUEST['home']) ? $wp1 = $_REQUEST['home'] : $wp1 = null;

    // Demographics
    isset($_REQUEST['gender']) ? $gender = trim($_REQUEST['gender']) : $gender = null;
    isset($_REQUEST['occupation']) ? $occupation = $_REQUEST['occupation'] : $occupation = null;
    isset($_REQUEST['skills']) ? $skills = $_REQUEST['skills'] : $skills = null;

    // Languages
    isset($_REQUEST['altlang1']) ? $lang1 = $_REQUEST['altlang1'] : $lang1 = null;
    isset($_REQUEST['altlang2']) ? $lang2 = $_REQUEST['altlang2'] : $lang2 = null;
    isset($_REQUEST['altlang3']) ? $lang3 = $_REQUEST['altlang3'] : $lang3 = null;

    // Date of birth
    isset($_REQUEST['dob']) ? $dob = $_REQUEST['dob'] : $dob = null;

    // Facility
    isset($_REQUEST['facility']) ? $facility = $_REQUEST['facility'] : $facility = null;

    // Entry date
    isset($_REQUEST['checkInDate']) ? $entryDate = $_REQUEST['checkInDate'] : $entryDate = null;
    isset($_REQUEST['entryHour']) ? $entryHour = $_REQUEST['entryHour'] : $entryHour = null;
    isset($_REQUEST['entryMinute']) ? $entryMinute = $_REQUEST['entryMinute'] : $entryMinute = null;

    // Org Info
    $orgId = isset($_REQUEST['org_id']) ? $_REQUEST['org_id'] : null;
    $volStatusId = isset($_REQUEST['volStatus']) ? $_REQUEST['volStatus'] : null;
    $staffId = isset($_REQUEST['staffType']) ? $_REQUEST['staffType'] : null;

    $incidentId = $_SESSION['incident'];

    if ($wp1) {
        $workPhone = $wp1;
    } else {
        $workPhone = null;
    }

    // Birth day
    if ($dob) {
        $dob = "$dob";
    } else {
        $dob = null;
    }

    // Open a database transaction
    $global['db']->StartTrans();

    // Person person
    $table = 'person_uuid';

    $record = array();

    $record['full_name'] = "$given_name $family_name";
    $record['family_name'] = $family_name;
    $record['given_name'] = $given_name;
    $record['custom_name'] = $middle_initial;
    $record['incident_id'] = $incidentId;

    if ($mode == "UPDATE") {
        $global['db']->AutoExecute($table, $record, 'UPDATE', "p_uuid = '$uuid'");
    } else {
        $record['p_uuid'] = $uuid;
        $global['db']->AutoExecute($table, $record, 'INSERT');
    }

    // Facility entry information for new staff
    if ($mode == "INSERT") {

        // Format the entry date in a SQL ASNI format
        $now = "$entryDate-$entryHour:$entryMinute:00";

        $table = 'fms_person_to_facility';
        $record = array();

        $record['facility_uuid'] = $facility;
        $record['in_date'] = $now;
        $record['p_uuid'] = $uuid;
        $global['db']->AutoExecute($table, $record, 'INSERT');
    }

    // Organization
    $table = 'srs_person_to_org';

    $record = array();
    $record['org_id'] = $orgId;

    if ($mode == "UPDATE") {
        $global['db']->AutoExecute($table, $record, 'UPDATE', "p_uuid = '$uuid' AND incident_id = $incidentId");
    } else {
        $record['p_uuid'] = $uuid;
        $record['incident_id'] = $incidentId;
        $record['created_at'] = date("Y-m-d H:i:s");
        $global['db']->AutoExecute($table, $record, 'INSERT');
    }

    // Volunteer Status
    $table = 'srs_person_to_vol_status';

    $record = array();
    $record['volstatus_id'] = $volStatusId;

    if ($mode == "UPDATE") {
        $global['db']->AutoExecute($table, $record, 'UPDATE', "p_uuid = '$uuid' AND incident_id = $incidentId");
    } else {
        $record['p_uuid'] = $uuid;
        $record['incident_id'] = $incidentId;
        $record['created_at'] = date("Y-m-d H:i:s");
        $global['db']->AutoExecute($table, $record, 'INSERT');
    }

    // Staff Type
    $table = 'srs_person_to_staff_types';

    $record = array();
    $record['stafftype_id'] = $staffId;

    if ($mode == "UPDATE") {
        $global['db']->AutoExecute($table, $record, 'UPDATE', "p_uuid = '$uuid' AND incident_id = $incidentId");
    } else {
        $record['p_uuid'] = $uuid;
        $record['incident_id'] = $incidentId;
        $record['created_at'] = date("Y-m-d H:i:s");
        $global['db']->AutoExecute($table, $record, 'INSERT');
    }

    // Person details
    $table = 'person_details';

    $record = array();
    $record['p_uuid'] = $uuid;
    $record['birth_date'] = $dob;
    $record['opt_country'] = $opt_country;
    $record['occuptaion'] = $occupation;
    $record['opt_gender'] = $gender;

    $global['db']->AutoExecute($table, $record, $mode, "p_uuid = '$uuid'");


    // Contact Information
    save_contact_info("street_1", $street_1, $uuid);
    save_contact_info("street_2", $street_2, $uuid);
    save_contact_info("city", $city, $uuid);
    save_contact_info("state", $state, $uuid);
    save_contact_info("postal", $postal, $uuid);
    save_contact_info("curr", $workPhone, $uuid);

    // staff languages
    save_language_info($lang1, 1, $uuid, $mode);
    save_language_info($lang2, 2, $uuid, $mode);
    save_language_info($lang3, 3, $uuid, $mode);

    //Skills
    save_skills($skills, $uuid);


    // Set new staff  status
    if ($mode == "INSERT") {
        setPersonStatus('in', $uuid);
    }

    // Check for any failed transactions
    //$error =
        $global['db']->HasFailedTrans();

    // Close transaction
    $global['db']->CompleteTrans();

    // Provide success || fail status to user
    if ($error) {
        $uuid = -1;
    } else {
        // Request Solr delta-import
        if ($conf['enable_solr_for_search'] == true) {
            $serialized_results = file_get_contents("http://" . $conf['SOLR_host'] . ":" . $conf['SOLR_port'] . "/solr/dataimport?command=delta-import");
        }
    }

    return $uuid;
}

/**
 * This function calculates a person's age based on their date of birth
 * @access public
 * @return int
 */
function years_old($dob)
{
    $dob = str_replace("'", "", $dob);
    list($year, $month, $day) = explode("-", $dob);

    $year_diff = date("Y") - $year;
    $month_diff = date("m") - $month;
    $day_diff = date("d") - $day;

    if ($month_diff < 0)
        $year_diff--;
    elseif (($month_diff == 0) && ($day_diff < 0))
        $year_diff--;
    return $year_diff;
}

/**
 * This function converts mm/dd/yyyy into SQL friendly yyyy-mm-dd
 * @access public
 * @return void
 */
function convert_date_to_sql($date)
{
    list($month, $day, $year) = explode("/", $date);
    $time = mktime(0, 0, 0, $month, $day, $year);
    return date('Y-m-d', $time);
}

?>
