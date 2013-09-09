<?php
/* 
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */


/**
 * Check Credz
 *
 * @access public
 * @return void
 */
function shn_fms_checkCredz()
{
  $good = false;
  if (isset($_SESSION['logged_in']) && $_SESSION['logged_in'] == 1) {
    if (isset($_SESSION['group_id']) && $_SESSION['group_id'] != 3) { // anonymous
      $good = true; // allowed to report
    }
  }

  // check if EM (Event Manager) is installed, if so, verify this event allows reporting
  return $good;
}

/**
 * Shows a messgage informing the user that the current event is closed.
 *
 * @access public
 * @return void
 */
function shn_fms_showClosedMessage()
{

  add_error("This event is closed and is no longer accepting new person reports.");
  shn_fms_default();
}

/**
 * Event CLOSED Check
 *
 * @access public
 * @return void
 */
function shn_fms_checkClosed()
{
  global $global;
  $q = "SELECT closed
		FROM incident
		WHERE shortname = " . shn_db_clean($_GET['shortname']) . ";
	";
  $res = $global['db']->Execute($q);
  $row = $res->FetchRow();
  $closed = $row['closed'];
  if ($closed != null) {
    return true; // event closed
  } else {
    return false; // event open
  }
}
?>
