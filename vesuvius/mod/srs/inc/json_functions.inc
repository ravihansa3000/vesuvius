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

function shn_json_srs_statusmap()
{
    // Validate group access
    if (!check_groupAccess(array(1, 2))) {
        add_error("You do not have permission to search the shelter registry.");
        return;
    }

    // Validate group access
    if (!check_validIncident()) {
        add_error("Permission denied.");
        return;
    }

    include_once 'json/json_statusmap_data.inc';
}

function shn_json_srs_staff_search()
{
    // Validate group access
    if (!check_groupAccess(array(1, 2))) {
        add_error("You do not have permission to search the shelter registry.");
        return;
    }

    // Validate group access
    if (!check_validIncident()) {
        add_error("Permission denied.");
        return;
    }

    include_once 'json/json_staff_search.inc';
}

function shn_json_srs_report_contacts()
{
    // Validate group access
    if (!check_groupAccess(array(1, 2))) {
        add_error("Permission denied.");
        return;
    }

    // Validate group access
    if (!check_validIncident()) {
        add_error("You must select an open event before attempting to perform this action.");
        return;
    }
    include_once 'json/json_report_contacts.inc';
}

function shn_json_srs_report_checkout()
{
    // Validate group access
    if (!check_groupAccess(array(1, 2))) {
        add_error("Permission denied.");
        return;
    }

    // Validate group access
    if (!check_validIncident()) {
        add_error("You must select an open event before attempting to perform this action.");
        return;
    }
    include_once 'json/json_report_checkout.inc';
}

function shn_json_srs_tooltips()
{
    // Validate group access
    if (!check_groupAccess(array(1, 2))) {
        add_error("Permission denied.");
        return;
    }

    global $global;
    $tip = (isset($_REQUEST['tip'])) ? mysql_escape_string(trim($_REQUEST['tip'])) : null;
    $tip = preg_replace('/_help$/', '', $tip);

    $query = "SELECT id, title, tip, defaults FROM srs_tooltips WHERE id = '$tip'";

    // Execute query
    try {
        $query = $global['db']->Prepare($query);
        $results = $global['db']->Execute($query);

         foreach ($results as $row) {
        $response['id'] = $row['id'];
        $response['title'] = $row['title'];
        $response['tip'] = $row['tip'];
        $response['defaults'] = $row['defaults'];
    }

    if (isset($response)) {
        echo json_encode($response, JSON_HEX_TAG | JSON_HEX_APOS | JSON_HEX_QUOT | JSON_HEX_AMP);
    }
    } catch (exception $e) {
        add_error("<pre>" . $e->getMessage() . "</pre>");
    }

   
}

function shn_json_srs_langlist()
{
    // Validate group access
    if (!check_groupAccess(array(1, 2))) {
        add_error("Permission denied.");
        return;
    }

    global $global;
    $term = (isset($_REQUEST['term'])) ? mysql_escape_string(trim($_REQUEST['term'])) : null;

    $query = "SELECT id, language FROM crs_language WHERE language LIKE '%" . $term . "%'";

    // Execute query
    try {
        $query = $global['db']->Prepare($query);
        $results = $global['db']->Execute($query);

        $i = 0;
    foreach ($results as $row) {
        $response[$i]['value'] = $row['language'];
        $response[$i]['label'] = $row['language'];
        $i++;
    }

    if (isset($response)) {
        echo json_encode($response, JSON_HEX_TAG | JSON_HEX_APOS | JSON_HEX_QUOT | JSON_HEX_AMP);
    }
    } catch (exception $e) {
        add_error("<pre>" . $e->getMessage() . "</pre>");
    }

    
}

function shn_json_srs_skills()
{
    // Validate group access
    if (!check_groupAccess(array(1, 2))) {
        add_error("Permission denied.");
        return;
    }

    global $global;
    $term = (isset($_REQUEST['term'])) ? mysql_escape_string(trim($_REQUEST['term'])) : null;

    $query = "SELECT DISTINCT skill FROM crs_person_skills WHERE skill LIKE '%" . $term . "%'";

    // Execute query
    try {
        $query = $global['db']->Prepare($query);
        $results = $global['db']->Execute($query);

         $i = 0;
    foreach ($results as $row) {
        $response[$i]['value'] = $row['skill'];
        $response[$i]['label'] = $row['skill'];
        $i++;
    }

    if (isset($response)) {
        echo json_encode($response, JSON_HEX_TAG | JSON_HEX_APOS | JSON_HEX_QUOT | JSON_HEX_AMP);
    }
    } catch (exception $e) {
        add_error("<pre>" . $e->getMessage() . "</pre>");
    }

   
}

function shn_json_srs_occupations()
{
    // Validate group access
    if (!check_groupAccess(array(1, 2))) {
        add_error("Permission denied.");
        return;
    }

    global $global;
    $term = (isset($_REQUEST['term'])) ? trim($_REQUEST['term']) : null;

    $query = "SELECT DISTINCT occupation FROM person_details WHERE occupation LIKE CONCAT('%',?,'%') UNION SELECT occupation FROM crs_common_occupations WHERE occupation LIKE CONCAT('%',?,'%') ORDER BY occupation ASC";

    // execut query
    try {
        $results = $global['db']->Execute($query, array($term, $term));
    } catch (exception $e) {
        add_error("<pre>" . $e->getMessage() . "</pre>");
    }

    $i = 0;
    foreach ($results as $row) {
        $response[$i]['value'] = $row['occupation'];
        $response[$i]['label'] = $row['occupation'];
        $i++;
    }

    if (isset($response)) {
        echo json_encode($response, JSON_HEX_TAG | JSON_HEX_APOS | JSON_HEX_QUOT | JSON_HEX_AMP);
    }
}


?>
