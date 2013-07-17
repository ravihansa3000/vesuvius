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

/**
 * This function updates the primary shelteree's data
 * @access public
 * @return void
 */
function client_save($uuid = null)
{
    global $global;
    global $conf;
    $mode = 'INSERT';
    $error = false;


    // Determine if we need to create a new client or update an inidiviudal's information
    if ($uuid == null) {

        // Generate a uuid if this is a new client member
        $uuid = shn_create_uuid();
        $mode = 'INSERT';
    } else {

        // Get the client's information
        $client = get_client($uuid);

        $mode = 'UPDATE';
    }

    // Collect form submission
    // Basics
    isset($_REQUEST['given_name']) ? $given_name = $_REQUEST['given_name'] : $given_name = null;
    isset($_REQUEST['middle_initial']) ? $middle_initial = $_REQUEST['middle_initial'] : $middle_initial = null;
    isset($_REQUEST['family_name']) ? $family_name = $_REQUEST['family_name'] : $family_name = null;

    isset($_REQUEST['street_1']) ? $street_1 = $_REQUEST['street_1'] : $street_1 = null;
    isset($_REQUEST['street_2']) ? $street_2 = $_REQUEST['street_2'] : $street_2 = null;
    isset($_REQUEST['city']) ? $city = $_REQUEST['city'] : $city = null;
    isset($_REQUEST['state']) ? $state = $_REQUEST['state'] : $state = null;
    isset($_REQUEST['postal']) ? $postal = $_REQUEST['postal'] : $postal = null;
    isset($_REQUEST['opt_country']) ? $opt_country = $_REQUEST['opt_country'] : $opt_country = null;

    // Family composition
    isset($_REQUEST['num_adults']) ? $num_adults = trim($_REQUEST['num_adults']) : $num_adults = null;
    isset($_REQUEST['num_children']) ? $num_children = trim($_REQUEST['num_children']) : $num_children = null;
    isset($_REQUEST['num_infants']) ? $num_infants = trim($_REQUEST['num_infants']) : $num_infants = null;

    // Home Phone
    isset($_REQUEST['home']) ? $home = $_REQUEST['home'] : $home = null;

    // Mobile Phone
    isset($_REQUEST['mobile']) ? $mobile = $_REQUEST['mobile'] : $mobile = null;

    // Alt Phone
    isset($_REQUEST['alter']) ? $alter = $_REQUEST['alter'] : $alter = null;

    // Email
    isset($_REQUEST['email']) ? $email = $_REQUEST['email'] : $email = null;

    // Demographics
    isset($_REQUEST['gender']) ? $gender = trim($_REQUEST['gender']) : $gender = null;
    isset($_REQUEST['occupation']) ? $occupation = $_REQUEST['occupation'] : $occupation = null;
    isset($_REQUEST['skills']) ? $skills = $_REQUEST['skills'] : $skills = null;

    // Languages
    isset($_REQUEST['altlang1']) ? $lang1 = $_REQUEST['altlang1'] : $lang1 = null;
    isset($_REQUEST['altlang2']) ? $lang2 = $_REQUEST['altlang2'] : $lang2 = null;
    isset($_REQUEST['altlang3']) ? $lang3 = $_REQUEST['altlang3'] : $lang3 = null;

    // Age
    isset($_REQUEST['years_old']) ? $yearsOld = $_REQUEST['years_old'] : $yearsOld = null;

    // Date of birth
    isset($_REQUEST['dob']) ? $dob = $_REQUEST['dob'] : $dob = null;

    // Facility
    isset($_REQUEST['facility']) ? $facility = $_REQUEST['facility'] : $facility = null;

    // Entry date
    isset($_REQUEST['checkInDate']) ? $checkInDate = $_REQUEST['checkInDate'] : $checkInDate = null;
    isset($_REQUEST['entryHour']) ? $entryHour = $_REQUEST['entryHour'] : $entryHour = null;
    isset($_REQUEST['entryMinute']) ? $entryMinute = $_REQUEST['entryMinute'] : $entryMinute = null;

    // Emergency contact
    isset($_REQUEST['ec_name']) ? $ec_name = $_REQUEST['ec_name'] : $ec_name = null;
    isset($_REQUEST['ec_relation']) ? $ec_relation = $_REQUEST['ec_relation'] : $ec_relation = null;
    isset($_REQUEST['ec_phone']) ? $ec_phone = $_REQUEST['ec_phone'] : $ec_phone = null;

    // Emergency contact
    isset($_REQUEST['injured']) ? $injured = 1 : $injured = 0;
    isset($_REQUEST['special_medical']) ? $special_medical = 1 : $special_medical = 0;
    isset($_REQUEST['special_mental']) ? $special_mental = 1 : $special_mental = 0;

    // Group relation
    isset($_REQUEST['group_relation']) ? $groupRelation = $_REQUEST['group_relation'] : $groupRelation = null;


    // Phones
    if ($home) {
        $homePhone = $home;
    } else {
        $homePhone = null;
    }
    if ($mobile) {
        $mobilePhone = $mobile;
    } else {
        $mobilePhone = null;
    }
    if ($alter) {
        $altPhone = $alter;
    } else {
        $altPhone = null;
    }

    if ($ec_phone) {
        $ecPhone = $ec_phone;
    } else {
        $ecPhone = null;
    }

    // Birth day
    if ($dob) {
        $dob = "$dob";
    } else {
        $dob = null;
    }

    // Determin age
    // TODO this needs better validation
    isset($dob) ? $age = years_old($dob) : $age = $yearsOld;

    // Open a database transaction
    $global['db']->StartTrans();

    // Person person
    $table = 'person_uuid';

    $record = array();

    $record['full_name'] = "$given_name $family_name";
    $record['family_name'] = $family_name;
    $record['given_name'] = $given_name;
    $record['custom_name'] = $middle_initial;
    $record['incident_id'] = $_SESSION['incident'];

    if ($mode == "UPDATE") {
        $global['db']->AutoExecute($table, $record, 'UPDATE', "p_uuid = '$uuid'");
    } else {
        $record['p_uuid'] = $uuid;
        $global['db']->AutoExecute($table, $record, 'INSERT');
    }

    // Facility entry information for new clients
    if ($mode == "INSERT") {

        // Format the entry date in a SQL ASNI format
        $now = "$checkInDate $entryHour:$entryMinute:00";

        $table = 'fms_person_to_facility';
        $record = array();

        $record['facility_uuid'] = $facility;
        $record['in_date'] = $now;
        $record['p_uuid'] = $uuid;
        $global['db']->AutoExecute($table, $record, 'INSERT');
    }

    /*
     *  Create default group for individuals
     * This way they can be related to other individuals or groups
     */
    if ($mode == 'INSERT') {
        $table = 'crs_person_to_group';
        $record = array();
        $record['p_uuid_1'] = $uuid;
        $global['db']->AutoExecute($table, $record, 'INSERT');
    } else {
        $table = 'crs_person_to_group';
        $record = array();
        $record['relation'] = get_relation_id($groupRelation);
        $global['db']->AutoExecute($table, $record, 'UPDATE', "p_uuid_1 = '$uuid'");
        
    }

    // Person details
    $table = 'person_details';

    $record = array();
    $record['p_uuid'] = $uuid;
    $record['birth_date'] = $dob;
    $record['opt_country'] = $opt_country;
    $record['occuptaion'] = $occupation;
    $record['years_old'] = $age;
    $record['opt_gender'] = $gender;

    $global['db']->AutoExecute($table, $record, $mode, "p_uuid = '$uuid'");


    // Contact Information
    save_contact_info("street_1", $street_1, $uuid);
    save_contact_info("street_2", $street_2, $uuid);
    save_contact_info("city", $city, $uuid);
    save_contact_info("state", $state, $uuid);
    save_contact_info("postal", $postal, $uuid);
    save_contact_info("curr", $homePhone, $uuid);
    save_contact_info("cmob", $mobilePhone, $uuid);
    save_contact_info("calt", $altPhone, $uuid);

    // Client languages
    save_language_info($lang1, 1, $uuid, $mode);
    save_language_info($lang2, 2, $uuid, $mode);
    save_language_info($lang3, 3, $uuid, $mode);

    //Skills
    save_skills($skills, $uuid);


    // Emergency contact
    if (isset($ec_name) && $ec_name != '') {

        $table = 'crs_emerg_cont';

        $record = array();
        $record['p_uuid'] = $uuid;
        $record['name'] = $ec_name;
        $record['phone'] = $ecPhone;
        $record['relationship'] = $ec_relation;

        $global['db']->AutoExecute($table, $record, $mode, "p_uuid = '$uuid'");
    } else {
        $sql = "DELETE FROM crs_emerg_cont WHERE p_uuid = ?";
        $global['db']->Execute($sql, $uuid);
    }

    // Special medical needs
    if (isset($injured) || isset($special_medical) || isset($special_mental)) {

        $table = 'crs_med_alert';

        $record = array();
        $record['p_uuid'] = $uuid;
        $record['injury'] = $injured;
        $record['special_medical'] = $special_medical;
        $record['sepcial_mental'] = $special_mental;

        $global['db']->AutoExecute($table, $record, $mode, "p_uuid = '$uuid'");
    } else {
        $sql = "DELETE FROM crs_med_alert WHERE p_uuid = ?";
        $global['db']->Execute($sql, $uuid);
    }

    // Update group details 
    reconcile_group_details($uuid);

    // Set person status
    setPersonStatus('in', $uuid);

    // Check for any failed transactions
    $error = $global['db']->HasFailedTrans();

    // Close transaction
    $global['db']->CompleteTrans();

    // Provide success || fail status to user
    if ($error) {
        add_error("Failed to update <b>$given_name $family_name</b> information.");
    } else {

        if ($mode == "INSERT") {
            add_confirmation("You have successfully registered <b>$given_name $family_name</b> into the client shelter registry.");
        } else {
            add_confirmation("You have successfully updated the information for <b>$given_name $family_name</b>.");
        }

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
