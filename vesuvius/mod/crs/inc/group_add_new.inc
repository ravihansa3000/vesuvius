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
 * This function commits the given primary shelteree's data to the databsae.
 * @access public
 * @return group total
 */
function group_add_new()
{
  global $global;
  global $conf;
  $error = false;

  include_once $global['approot'] . 'inc/lib_uuid.inc';

  // Collect form submission
  if (isset($_REQUEST['submit'])) {

    // Basics
    isset($_REQUEST['given_name']) ? $given_name = trim($_REQUEST['given_name']) : $given_name = null;
    isset($_REQUEST['middle_initial']) ? $middle_initial = trim($_REQUEST['middle_initial']) : $middle_initial = null;
    isset($_REQUEST['family_name']) ? $family_name = trim($_REQUEST['family_name']) : $family_name = null;

    // Demographics
    isset($_REQUEST['gender']) ? $gender = trim($_REQUEST['gender']) : $gender = null;
    isset($_REQUEST['occupation']) ? $occupation = trim($_REQUEST['occupation']) : $occupation = null;

    // Languages
    isset($_REQUEST['lang1']) ? $lang1 = trim($_REQUEST['lang1']) : $lang1 = null;
    isset($_REQUEST['lang2']) ? $lang2 = trim($_REQUEST['lang2']) : $lang2 = null;
    isset($_REQUEST['lang3']) ? $lang3 = trim($_REQUEST['lang3']) : $lang3 = null;

    // Age
    isset($_REQUEST['years_old']) ? $yearsOld = $_REQUEST['years_old'] : $yearsOld = null;

    // Date of birth
    isset($_REQUEST['dobMonth']) ? $dobMonth = $_REQUEST['dobMonth'] : $dobMonth = null;
    isset($_REQUEST['dobDay']) ? $dobDay = $_REQUEST['dobDay'] : $dobDay = null;
    isset($_REQUEST['dobYear']) ? $dobYear = $_REQUEST['dobYear'] : $dobYear = null;
    isset($_REQUEST['dobHour']) ? $dobHour = $_REQUEST['dobHour'] : $dobHour = null;
    isset($_REQUEST['dobMinute']) ? $dobMinute = $_REQUEST['dobMinute'] : $dobMinute = null;

    // Facility
    isset($_REQUEST['facility']) ? $facility = $_REQUEST['facility'] : $facility = null;

    // Entry date
    isset($_REQUEST['entryMonth']) ? $entryMonth = $_REQUEST['entryMonth'] : $entryMonth = null;
    isset($_REQUEST['entryDay']) ? $entryDay = $_REQUEST['entryDay'] : $entryDay = null;
    isset($_REQUEST['entryYear']) ? $entryYear = $_REQUEST['entryYear'] : $entryYear = null;
    isset($_REQUEST['entryHour']) ? $entryHour = $_REQUEST['entryHour'] : $entryHour = null;
    isset($_REQUEST['entryMinute']) ? $entryMinute = $_REQUEST['entryMinute'] : $entryMinute = null;

    // Medical
    isset($_REQUEST['injured']) ? $injured = 1 : $injured = 0;
    isset($_REQUEST['special_medical']) ? $special_medical = 1 : $special_medical = 0;
    isset($_REQUEST['special_mental']) ? $special_mental = 1 : $special_mental = 0;

    // Group
    isset($_REQUEST['group_uuid']) ? $groupId = base64_decode($_REQUEST['group_uuid']) : $groupId = null;
    isset($_REQUEST['group_relation']) && !empty($_REQUEST['group_relation']) ? $relation = $_REQUEST['group_relation'] : $relation = null;
  }
  
  // Birth day
  if (isset($dobDay) && $dobDay != '' && isset($dobMonth) && $dobMonth != '' && isset($dobYear)) {
    $dob = "$dobYear-$dobMonth-$dobDay";
  } else {
    $dob = null;
  }

  // Determin age
  $age = isset($dob) ? years_old($dob) : $yearsOld;

  // Get primary group representative's information
  $group = get_client($groupId);

  // Begin transaction 
  $global['db']->StartTrans();

  // Generate a uuid if this is a new client member
  $uuid = shn_create_uuid();
  $mode = 'INSERT';

  // Person_uuid info

  $table = 'person_uuid';
  $record = array();
  $record['p_uuid'] = $uuid;
  $record['full_name'] = "$given_name $family_name";
  $record['given_name'] = $given_name;
  $record['family_name'] = $family_name;
  $record['custom_name'] = $middle_initial;
  $record['incident_id'] = $_SESSION['incident'];

  $global['db']->AutoExecute($table, $record, $mode);

  // Facility entry information
  if ($facility && $entryDay && $entryMonth && $entryYear && $entryHour && $entryMinute) {

    $table = 'fms_person_to_facility';
    $record = array();
    $record['p_uuid'] = $uuid;
    $record['facility_uuid'] = $facility;
    $record['in_date'] = "$entryYear-$entryMonth-$entryDay $entryHour:$entryMinute:00";

    $global['db']->AutoExecute($table, $record, $mode);
  }

  // Person details
  $table = 'person_details';
  $record = array();
  $record['p_uuid'] = $uuid;
  $record['birth_date'] = $dob;
  $record['opt_country'] = $group['opt_country'];
  $record['years_old'] = $age;
  $record['opt_gender'] = $gender;

  $global['db']->AutoExecute($table, $record, $mode);

  // Add to the group
  $table = 'crs_person_to_group';
  $record = array();
  $record['p_uuid_1'] = $uuid;
  $record['p_uuid_2'] = $groupId;
  if ($relation) {
    $record['relation'] = get_relation_id($relation);
  }

  $global['db']->AutoExecute($table, $record, $mode);


  // Special medical needs
  if (isset($injured) || isset($special_medical) || isset($special_mental)) {

    $table = 'crs_med_alert';

    $record = array();
    $record['p_uuid'] = $uuid;
    $record['injury'] = $injured;
    $record['special_medical'] = $special_medical;
    $record['sepcial_mental'] = $special_mental;

    $global['db']->AutoExecute($table, $record, $mode);
  } else {
    $sql = "DELETE FROM crs_med_alert WHERE p_uuid = ?";
    $global['db']->Execute($sql, $uuid);
  }

  // Contact Information
  save_contact_info("street_1", $group['street_1'], $uuid, $mode);
  save_contact_info("street_2", $group['street_2'], $uuid, $mode);
  save_contact_info("city", $group['city'], $uuid, $mode);
  save_contact_info("state", $group['state'], $uuid, $mode);
  save_contact_info("postal", $group['postal'], $uuid, $mode);
  save_contact_info("curr", $group['home_phone'], $uuid, $mode);
  save_contact_info("cmob", $group['mobile_phone'], $uuid, $mode);
  save_contact_info("calt", $group['alt_phone'], $uuid, $mode);

  // Client languages
  save_language_info($lang1, 1, $uuid, $mode);
  save_language_info($lang2, 2, $uuid, $mode);
  save_language_info($lang3, 3, $uuid, $mode);

  // Update group details 
  reconcile_group_details($groupId);

  // Set person status
  setPersonStatus('in', $uuid);

  // Check for any failed transactions
  $error = $global['db']->HasFailedTrans();

  // Close transaction
  $global['db']->CompleteTrans();

  // Provide success || fail status to user
  if ($error) {
    add_error("Failed to add <b>" . $given_name . " " . $family_name . "</b> into the registry.");
  } else {
    add_confirmation("You have successfully registered <b>" . $given_name . " " . $family_name . "</b> into the registry.");

    // Request Solr delta-import
    if ($conf['enable_solr_for_search'] == true) {
      $serialized_results = file_get_contents("http://" . $conf['SOLR_host'] . ":" . $conf['SOLR_port'] . "/solr/dataimport?command=delta-import");
    }
  }

  return ($uuid);
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
