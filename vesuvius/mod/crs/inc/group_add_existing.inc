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
function group_add_existing()
{
  global $global;
  global $conf;
  $error = false;

  if (!check_groupAccess()) {
    add_error("You must first login to add a shelteree to the registry.");
    return;
  }

  //add_information("<pre>" . print_r($_POST, true) . "</pre>");
  // Collect form submission
  if (isset($_REQUEST['Submit'])) {
    
    // Group
    isset($_REQUEST['uuid']) ? $uuid = base64_decode($_REQUEST['uuid']) : $uuid = null;
    isset($_REQUEST['g_uuid']) ? $groupId = base64_decode($_REQUEST['g_uuid']) : $groupId = null;
    isset($_REQUEST['group_relation']) ? $relation = trim($_REQUEST['group_relation']) : $relation = null;
  }
  
  // Query incrementer
  $qi = 0;

  // Add to the group
  $q[$qi]['query'] = "UPDATE crs_person_to_group SET p_uuid_2 = ?, relation = ? WHERE p_uuid_1 = ?";
  $q[$qi++]['values'] = array($groupId, $relation, $uuid);


  // Open a transaction and prepare to save to database
  $global['db']->StartTrans();

  // Walk through the sql insert array
  foreach ($q as $sql) {
    
    print("<pre>". print_r($sql, true)."</pre>");

    try {
      $sql = $global['db']->Prepare($sql);
      $res = $global['db']->Execute($sql['query'], $sql['values']);
    } catch (exception $e) {
      add_error("<pre>" . $e->getMessage() . "</pre>");
      $error = true;
    }
  }

  // Close transaction
  $results = $global['db']->CompleteTrans();

  // Provide success || fail status to user
  if ($error) {
    add_error("Failed to add <b>" . $given_name . " " . $family_name . "</b> into the registry.");
  } else {
    add_confirmation("You have successfully registered <b>" . $given_name . " " . $family_name . "</b> into the registry.");
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
