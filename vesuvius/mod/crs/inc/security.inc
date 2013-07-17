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
 * Check group credentials
 *
 * @access public
 * @return void
 */
function check_groupAccess($groups)
{
    $access = false;
    if (isset($_SESSION['logged_in']) && $_SESSION['logged_in'] == 1) {

        foreach ($groups as $groupId) {

            if (isset($_SESSION['group_id']) && $_SESSION['group_id'] == $groupId) {
                $access = true;
            }
        }
    }

    return $access;
}

function check_validIncident()
{
    global $global;
    global $conf;
    $valid = false;
    if (isset($_SESSION['incident'])) {

        // Get the current list of open events
        $query = "SELECT incident_id FROM incident i  
            WHERE incident_id = ? AND i.closed = 0";

        try {
            $results = $global['db']->Execute($query, $_SESSION['incident']);
        } catch (exception $e) {
            print("<pre>" . $e->getMessage() . "</pre>");
        }
        $valid = ($results->RowCount() > 0) ? true : false;
    }

    return $valid;
}

/**
 * Shows a messgage informing the user that the current event is closed.
 *
 * @access public
 * @return void
 */
function shn_crs_showClosedMessage()
{

    add_error("This event is closed and is no longer accepting new person reports.");
    shn_crs_default();
}

/**
 * Event CLOSED Check
 *
 * @access public
 * @return bool
 */
function shn_crs_checkClosed()
{
    // Get the shortname from session
    $shortname = (isset($_SESSION['shortname'])) ? $_SESSION['shortname'] : -1;

    global $global;
    $sql = "SELECT closed FROM incident WHERE shortname = ?";

    try {
        $rs = $global['db']->Execute($sql, $shortname);
    } catch (exception $e) {
        print($e->getMessage());
    }
    $row = $rs->FetchRow();
    if ($row['closed'] != null) {
        // event closed
        $closed = true;
    } else {
        // event open
        $closed = false;
    }
    return $closed;
}

?>
