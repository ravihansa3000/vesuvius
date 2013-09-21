<?php

/**
 * Facility Administration Module
 *
 * PHP version >=5
 *
 * LICENSE: This source file is subject to LGPL license
 * that is available through the world-wide-web at the following URI:
 * http://www.gnu.org/copyleft/lesser.html
 *
 * @author     Clayton Kramer <clayton.kramer@mail.cuny.edu>
 * @package    module fms
 * @version    0.4
 * @license    http://www.gnu.org/copyleft/lesser.html GNU Lesser General Public License (LGPL)
 *
 */
include_once 'inc/security.inc';
include_once 'inc/functions.inc';

/**
 * Default Admin function
 *
 * @access public
 * @return void
 */
function shn_fms_default()
{
    echo "<script type=\"text/javascript\" src=\"res/js/jquery-1.6.4.min.js\" ></script>";

    $control = "";

    shn_tabmenu_open();
    shn_tabmenu_item("search", _t("Facility-Menu|Registry Search"), "fms");
    shn_tabmenu_item("add", _t("Facility-Menu|Add Facility"), "fms");
    shn_tabmenu_item("displaymap", _t("Facility-Menu|Display Map"), "fms");
    shn_tabmenu_item("import", _t("Facility-Menu|Import"), "fms");
    shn_tabmenu_close();

    if (isset($_GET['search'])) {
        shn_fms_search();
    } elseif(isset($_GET['add'])) {
        $control = "shn_fms_add";
    } elseif(isset($_GET['displaymap'])) {
        $control = "shn_fms_displaymap";
    } elseif(isset($_GET['import'])) {
        $control = "shn_fms_import";
    } elseif(isset($_GET['facility_geo'])) {
        $control = "shn_fms_import";

    }

    if ($control == "") {
        shn_fms_search();
    }

    $control();
}

/**
 * This is the Primary Controller
 * @access public
 * @return void
 */
function shn_fms_controller()
{
    global $global;

    //Controller
    $mode = isset($_REQUEST['mode']) ? $_REQUEST['mode'] : null;
    switch ($mode) {
        case 'add' :

            shn_fms_saveNew();

            break;

        case 'edit' :
            shn_fms_save();
            shn_fms_edit();
            break;


        default :
            //shn_sts_default();
            break;
    }
}

/**
 * This is the CSV Import Controller
 * @access public
 * @return void
 */
function shn_fms_importcsv()
{
    global $global;
    $db = $global['db'];

    // Get the incident ID
    $incident = $_SESSION['incident'];

    //add_information("<pre>" . print_r($_REQUEST, true) . "</pre>");
    add_information("<pre>" . print_r($_SESSION, true) . "</pre>");

    $uploadDir = $global['approot'] . 'www/tmp/';

    $uploadedFile = $_FILES["csvImport"];
    move_uploaded_file($uploadedFile["tmp_name"], $uploadDir . $uploadedFile["name"]);

    $importFile = $uploadDir . $uploadedFile["name"];

    $row = 0;
    if (($handle = fopen($importFile, "r")) !== FALSE) {
        while (($data = fgetcsv($handle, 1000, ",", '"')) !== FALSE) {

            if ($row > 0) {
                $num = count($data);
                for ($c = 0; $c < $num; $c++) {
                    $facilities[$row][$c] = $data[$c];
                }
            }
            $row++;
        }
        fclose($handle);

        //print("<pre>" . print_r($facilities, true) . "</pre>");
    }

    foreach ($facilities as $row) {

        $values = array(
          $row[0],
          $row[1],
          $row[2],
          $row[3],
          $row[4],
          $row[5],
          $row[6],
          $row[7],
          $row[8],
          $row[11],
          $row[12],
          $row[13],
          $row[14],
          $row[15],
          $row[16],
          $row[17],
          $row[18],
          $row[19],
          $row[21],
          $row[20]
        );

        // Check to see if there is already a facility with this name for this incident

        $q = "SELECT facility_uuid FROM fms_facility WHERE facility_name = ? AND facility_resource_type_abbr = ?";
        $facilityId = $db->GetOne($q, array($row[0], $row[2]));

        if ($facilityId) {
            // Update record

            $values['facility_uuid'] = $facilityId;

            $q = "UPDATE fms_facility 
                    SET 
                      facility_name = ?,
                      facility_code = ?,
                      facility_resource_type_abbr = ?,
                      facility_resource_status = ?,
                      facility_capacity  = ?,
                      facility_activation_sequence = ?,
                      facility_allocation_status = ?,
                      facility_group = ?,
                      facility_group_type = ?,
                      work_email = ?,
                      work_phone = ?,
                      street_1 = ?,
                      street_2 = ?,
                      city  = ?,
                      state  = ?,
                      postal_code  = ?,
                      borough  = ?,
                      country  = ?,
                      longitude  = ?,
                      latitude  = ?
                  WHERE facility_uuid = ?";
            $db->Execute($q, $values);
        } else {
            // Insert new records

            $q = "INSERT INTO fms_facility 
                   (
                    facility_name,
                    facility_code,
                    facility_resource_type_abbr,
                    facility_resource_status,
                    facility_capacity,
                    facility_activation_sequence,
                    facility_allocation_status,
                    facility_group,
                    facility_group_type,
                    work_email,
                    work_phone,
                    street_1,
                    street_2,
                    city,
                    state,
                    postal_code,
                    borough,
                    country,
                    longitude,
                    latitude)
                   VALUES (?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)";
            $db->Execute($q, $values);

            $facilityId = $db->Insert_ID();
        }

        // Check to see if there is already a facility to this event
        $q = "SELECT COUNT(*) FROM fms_facility_to_event 
            WHERE facility_uuid = ? AND incident_id = ?";
        $values = array($facilityId, $incident);
        $result = $db->GetOne($q, $values);

        if ($result < 1) {

            // Add a record to the fms_facility_to_event
            $q = "INSERT INTO fms_facility_to_event (facility_uuid, incident_id)
                VALUES (?, ?)";

            $db->Execute($q, $values);
        }
    }
}


/**
 * Upload facilities from MS Excel 2003.
 *
 * @access public
 * @return void
 */
function shn_fms_import()
{
    global $global;

    // Display
    include_once 'templates/fms_upload_import.php';
}

/**
 * Import facilities from MS Excel 2003.
 *
 * @access public
 * @return void
 */
function shn_fms_importXLS()
{
    // Load the import class
    include_once ('inc/facility_import.inc');
    global $global;

    $uploadDir = $global['approot'] . 'www/tmp/';

    $uploadedFile = $_FILES["xlsImport"];
    move_uploaded_file($uploadedFile["tmp_name"], $uploadDir . $uploadedFile["name"]);

    $importFile = $uploadDir . $uploadedFile["name"];

    $import = new facilityImportXLS();
    $import->processExcel($importFile);
    $events = $import->events;

    if ($import->totalImported > 0) {
        add_confirmation("<b>New:</b> $import->totalNew  <b>Updated:</b> $import->totalUpdated
        <b>Total:</b> $import->totalImported <b>Errors:</b> $import->totalErrors");
    } else {
        add_error("No facility records added or updated. Please review the import events above for help.");
        // Provide some import events to the administrator
        foreach ($events as $event) {
            switch ($event["type"]) {
                case "OK":
                    add_confirmation($event["message"]);
                    break;
                case "INFO":
                    add_information($event["message"]);
                    break;
                case "WARN":
                    add_warning($event["message"]);
                    break;
                case "ERROR":
                    add_error($event["message"]);
                    break;
                default:
                    add_information($event["message"]);
            }
        }
    }
}

function shn_fms_sems_import()
{
    global $conf;
    $error = false;

    $content = @file_get_contents("{$conf['sems_url']}/webservices/get/{$conf['sems_token']}/events.json", true);
    if (strpos($http_response_header[0], "200")) {
        $events = json_decode($content, true);
    } else {
        $error = true;
        add_error("Unable to connect to the SEMS servers. Please check the system configuration.");
    }

    // Set form properties
    $act = 'sems_import_save';
    $formOpts = array('enctype' => 'enctype="multipart/form-data"');
    $submitOpts = 'name="submit" class="styleTheButton"';

    include_once 'templates/fms_json_import.php';
}

function shn_fms_sems_import_save()
{
    // Load the import class
    include_once ('inc/facility_import_json.inc');

    global $global;
    global $conf;

    $createUsers = (isset($_REQUEST['create_users']) && $_REQUEST['create_users'] == 1) ? true : false;
    $genPassword = (isset($_REQUEST['genpasswd']) && $_REQUEST['genpasswd'] == 1) ? true : false;
    $manPassword = (isset($_REQUEST['manpasswd']) && $_REQUEST['manpasswd'] == 1) ? true : false;
    $overwrite = (isset($_REQUEST['overwrite']) && $_REQUEST['overwrite'] == 1) ? true : false;
    $globalPasswd = (isset($_REQUEST['passwd'])) ? trim($_REQUEST['passwd']) : null;
    $geocode = (isset($_REQUEST['geocode'])) ? true : false;


    if ($manPassword) {
        // Check password meets requirements
        if (shn_check_password_matches_policy($globalPasswd, "asdfasdfasdf") != 1) {
            add_error("Your entered password does not meet the system's policy requirements.");
            return;
        }
    }


    // Set table name
    $table = 'incident';

    // Get the events from sems server
    $json = file_get_contents("{$conf['sems_url']}/webservices/get/{$conf['sems_token']}/events.json", true);
    $events = json_decode($json, true);

    foreach ($events as $event) {

        // Check for existing record
        $sql = "SELECT incident_id FROM $table WHERE incident_id = ?";
        $rs = $global['db']->Execute($sql, $event['a_id']);
        $mode = ($rs->RowCount() > 0) ? 'UPDATE' : 'INSERT';

        // Assign record values
        $record = array();

        $record['name'] = $event['a_event_name'];
        $record['shortname'] = substr(str_replace(" ", "", ucwords(strtolower($event['a_event_name']))), 0, 13) . $event['a_id'];
        $record['type'] = "REAL";
        $record['date'] = date('Y-m-d', $event['a_zero_hour']);
        $record['closed'] = ($event['est_event_status_type'] == "Open") ? 0 : 1;

        if ($mode == 'UPDATE') {
            // Update existing record
            $global['db']->AutoExecute($table, $record, $mode, "incident_id = '" . $event['a_id'] . "'");
            $type = "Update";
        } else {
            // Insert new record
            $record['incident_id'] = $event['a_id'];
            $global['db']->AutoExecute($table, $record, $mode);
            $type = "New";
        }

        // Only import facilities for open events
        if ($event['est_event_status_type'] == "Open") {
            $json = file_get_contents("{$conf['sems_url']}/webservices/getevent/{$conf['sems_token']}/{$event['a_id']}/eventFacilities.json", true);
            $facilities = json_decode($json, true);

            $import = new facilityImportJSON();

            $import->setProperties($createUsers, $genPassword, $manPassword, $globalPasswd, $overwrite);

            if (isset($conf['sems_geocode_enabled']) && $conf['sems_geocode_enabled']) {
                $import->geocode = true;
            }

            $import->saveImportData($facilities, $event['a_id']);
        }

        // Metrics
        $stats[] = array(
          'incident_name' => $event['a_event_name'],
          'type' => $type,
          'inserts' => isset($import->totalNew) ? $import->totalNew : 0,
          'updates' => isset($import->totalNew) ? $import->totalUpdated : 0,
          'total' => isset($import->totalNew) ? $import->totalImported : 0,
          'errors' => isset($import->totalNew) ? $import->totalErrors : 0,
          'log' => isset($import->totalNew) ? $import->eventLog : null
        );


        if (isset($import->userAccount)) {

            foreach ($import->userAccount as $creds) {
                $creds['event_name'] = $event['a_event_name'];
                $accounts[] = $creds;
            }
        }

        // Release the object
        unset($import);
    }

    include_once 'templates/sems_import_results.php';
}

/**
 * Show all Facility registrations.
 *
 * @access public
 * @return void
 */
function shn_fms_search()
{

    include_once 'templates/fms_search.php';
}

/**
 * Provides json search results
 *
 * @access public
 * @return void
 */
function shn_json_fms_facility_search()
{

    include_once 'inc/json_facility_search.inc';
}

/**
 * Edit a Facility.
 *
 * @access public
 * @return void
 */
function shn_fms_edit()
{
    include_once 'inc/functions.inc';
    global $global;

    include_once 'inc/facility_info.inc';

    $facility_uuid = base64_decode($_REQUEST['id']);

    $query = "SELECT
            facility_uuid,
            facility_name AS name,
            facility_code AS code,
            facility_resource_type_abbr AS type,
            facility_allocation_status AS status,
            facility_capacity AS capacity,
            facility_group AS `group`,
            work_email,
            work_phone,
            street_1,
            street_2,
            city,
            state,
            postal_code,
            borough,
            country,
            longitude,
            latitude,
            created_at,
            updated_at
            FROM fms_facility 
            WHERE facility_uuid = ?";

    try {
        $result = $global['db']->Execute($query, $facility_uuid);
        $facility = $result->FetchRow();
    } catch (exception $e) {
        add_error("<pre>" . $e->getMessage() . "</pre>");
    }

    // Load phone numbers as arrays
    $work_phone = parsePhone($facility["work_phone"]);

    // Display
    $mode = 'edit';
    include_once 'templates/fms_edit_facility.php';
}

/**
 * Deletes a Registration
 */
function shn_fms_delete()
{
    global $global;
    $f_uuid = base64_decode($_REQUEST['id']);
    $query = "DELETE FROM fms_facility WHERE facility_uuid = ?;";

    try {
        $result = $global['db']->Execute($query, $f_uuid);
        add_confirmation("Facility deleted.");
    } catch (exception $e) {
        add_error("Unable to delete facility. Please contact the system administrator.");
        error_log($e->getMessage());
    }

    shn_fms_default();
}

/**
 * Save any changes to the Facility
 */
function shn_fms_save()
{
    global $global;

    // Error condition flag
    $error = false;

    // Manditory data
    $requiredFields = array(
        "name" => "Name",
        "code" => "Facility Code",
        "capacity" => "Capacity",
        "type" => "Resource Abbr",
        "status"  => "Status Allocation",
        "group" => "Group Name",
        "street_1" => "Street Address",
        "city" => "City",
        "state" => "State",
        "postal" => "Zip Code"
    );

    foreach ($requiredFields as $key => $value) {

        $test = $_REQUEST[$key];

        if (!isset($_REQUEST[$key]) || empty($_REQUEST[$key])) {
            add_error($value . " not provided.");
            $error = true;
        }
    }

    // Facility Phone
    isset($_REQUEST['wp1']) ? $wp1 = $_REQUEST['wp1'] : $wp1 = null;
    isset($_REQUEST['wp2']) ? $wp2 = $_REQUEST['wp2'] : $wp2 = null;
    isset($_REQUEST['wp3']) ? $wp3 = $_REQUEST['wp3'] : $wp3 = null;

    if ($wp1 && $wp2 && $wp3) {
        $work_phone = $wp1 . $wp2 . $wp3;
    } else {
        $work_phone = null;
    }

    // Setup parameter values
    $values = array();
    $values[] = isset($_REQUEST['name']) ? trim($_REQUEST['name']) : '';
    $values[] = isset($_REQUEST['code']) ? trim($_REQUEST['code']) : '';
    $values[] = isset($_REQUEST['type']) ? trim($_REQUEST['type']) : '';
    $values[] = isset($_REQUEST['status']) ? trim($_REQUEST['status']) : '';
    $values[] = isset($_REQUEST['capacity']) ? trim($_REQUEST['capacity']) : '';
    $values[] = isset($_REQUEST['group']) ? trim($_REQUEST['group']) : '';
    $values[] = isset($_REQUEST['email']) ? trim($_REQUEST['email']) : '';
    $values[] = isset($work_phone) ? trim($work_phone) : '';
    $values[] = isset($_REQUEST['street_1']) ? trim($_REQUEST['street_1']) : '';
    $values[] = isset($_REQUEST['street_2']) ? trim($_REQUEST['street_2']) : '';
    $values[] = isset($_REQUEST['city']) ? trim($_REQUEST['city']) : '';
    $values[] = isset($_REQUEST['state']) ? trim($_REQUEST['state']) : '';
    $values[] = isset($_REQUEST['postal']) ? trim($_REQUEST['postal']) : '';
    $values[] = isset($_REQUEST['longitude']) ? trim($_REQUEST['longitude']) : '';
    $values[] = isset($_REQUEST['latitude']) ? trim($_REQUEST['latitude']) : '';
    $values[] = isset($_REQUEST['id']) ? trim(base64_decode($_REQUEST['id'])) : '';


    if (!$error) {

        $query = "UPDATE fms_facility
            SET
            facility_name = ?,
            facility_code = ?,
            facility_resource_type_abbr = ?,
            facility_allocation_status = ?,
            facility_capacity = ?,
            facility_group = ?,
            work_email = ?,
            work_phone = ?,
            street_1 = ?,
            street_2 = ?,
            city = ?,
            state = ?,
            postal_code = ?,
            longitude = ?,
            latitude = ?
            WHERE facility_uuid = ?;";

        // Update the facility record
        try {
            //add_information("<pre>" . $query . "</pre>");
            $result = $global['db']->Execute($query, $values);
        } catch (exception $e) {
            add_error("<pre>" . $e->getMessage() . "</pre>");
            $error = true;
        }

        // Provide event status to user
        if ($error) {
            add_error("Failed to update facility record.");
        } else {
            add_confirmation("Saved <b>" . $_REQUEST['name'] . "</b> information.");
        }
    } else {
        shn_fms_edit($_REQUEST['facility_uuid']);
    }
}

/**
 * Add a new Facility.
 *
 * @access public
 * @return void
 */
function shn_fms_add()
{
    global $global;
    include_once 'inc/functions.inc';

    $mode = "add";

    include_once 'inc/facility_info.inc';

    // Display
    include_once 'templates/fms_edit_facility.php';
}

/**
 * Save a new Facility
 */
function shn_fms_saveNew()
{

    // Error condition flag
    $error = false;

    // Manditory data
    // Manditory data
    $requiredFields = array(
        "name" => "Name",
        "code" => "Facility Code",
        "capacity" => "Capacity",
        "type" => "Resource Abbr",
        "status"  => "Status Allocation",
        "group" => "Group Name",
        "street_1" => "Street Address",
        "city" => "City",
        "state" => "State",
        "postal" => "Zip Code"
    );

    foreach ($requiredFields as $key => $value) {

        $test = $_REQUEST[$key];

        if (!isset($_REQUEST[$key]) || empty($_REQUEST[$key])) {
            add_error("<b>$value</b>  not provided.");
            $error = true;
        }
    }

    // Facility Phone
    isset($_REQUEST['wp']) ? $wp = $_REQUEST['wp'] : $wp = null;

    if ($wp) {
        $work_phone = $wp;
    } else {
        $work_phone = null;
    }

    if (!$error) {
        global $global;
        $db = $global['db'];

        $values = array(
          $_POST['name'],
          $_POST['code'],
          $_POST['type'],
          $_POST['status'],
          $_POST['capacity'],
          $_POST['group'],
          $_POST['email'],
          $work_phone,
          $_POST['street_1'],
          $_POST['street_2'],
          $_POST['city'],
          $_POST['state'],
          $_POST['postal'],
          empty($_POST['long']) ? 0.0 : $_POST['long'],
          empty($_POST['lat']) ? 0.0 : $_POST['lat']
        );

        $query = "INSERT INTO fms_facility (
                facility_name,
                facility_code,
                facility_resource_type_abbr,
                facility_allocation_status,
                facility_capacity,
                facility_group,
                work_email,
                work_phone,
                street_1,
                street_2,
                city,
                state,
                postal_code,
                longitude,
                latitude)
                VALUES (?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)";


        // Insert the facility record
        try {
            $result = $global['db']->Execute($query, $values);
        } catch (exception $e) {
            add_error("<pre>" . $e->getMessage() . "</pre>");
            $error = true;
        }

        // Add incident reference
        if ($error == false) {
            $facility_uuid = $global['db']->Insert_ID();
            $q = "INSERT INTO fms_facility_to_event VALUES (?,?, CURRENT_TIMESTAMP, CURRENT_TIMESTAMP)";
            try {
                $result = $global['db']->Execute($q, array($facility_uuid, $_SESSION['incident']));
            } catch (exception $e) {
                add_error("<pre>" . $e->getMessage() . "</pre>");
                $error = true;
            }
        }

        // Provide event status to user
        if ($error) {
            add_error("Failed to add facility.");
        } else {
            add_confirmation("Saved facility information.");
        }
    } else {

        global $global;
        include_once 'inc/functions.inc';

        $mode = "add";

        include_once 'inc/facility_info.inc';
        
        $facility = array(
          'name' => $_POST['name'],
          'code' => $_POST['code'],
          'type' => $_POST['type'],
          'status' => $_POST['status'],
          'capacity' => $_POST['capacity'],
          'group' => $_POST['group'],
          'email' => $_POST['email'],
          'street_1' => $_POST['street_1'],
          'street_2' => $_POST['street_2'],
          'city' => $_POST['city'],
          'state' => $_POST['state'],
          'postal' => $_POST['postal'],
          'longitude' => $_POST['long'],
          'latitude' => $_POST['lat']
        );
        
        $work_phone = array(
          'areacode' => $_POST['wp1'],
          'prefix' => $_POST['wp2'],
          'suffix' => $_POST['wp3']
        );

        include_once 'templates/fms_edit_facility.php';
    }
}

function add_search_expr($alias, $expression, $col, $operator, $value, $group = 0, $group_expr = 'AND')
{

    global $expr_array;

    $expr_array[$group]['group_expr'] = $group_expr;
    $expr_array[$group]['clause'][] = array('alias' => $alias, 'expr' => $expression, 'col' => $col, 'op' => $operator, 'val' => $value);
}

function shn_fms_querybuilder($base_query, $expr_array)
{
    // WHERE string
    $where = "";

    array_multisort($expr_array);


    for ($g = 0; $g < count($expr_array); $g++) {

        // Only add the group's expression if it's not the first
        if ($g > 0) {
            $where .= $expr_array[$g]['group_expr'] . " (";
        } else {
            $where .= "WHERE (";
        }

        $clause = $expr_array[$g]['clause'];
        for ($c = 0; $c < count($clause); $c++) {

            if ($c > 0) {
                $where .= $clause[$c]['expr'];
            }

            $str = " %s.%s %s '%s' ";
            $where .= sprintf($str, $clause[$c]['alias'], $clause[$c]["col"], $clause[$c]["op"], $clause[$c]["val"]);
        }

        // Close group
        $where .= ")\n";
    }

    //add_information("<pre>$where</pre>");
    return $base_query . $where;
}

function shn_fms_displaymap()
{
    global $global;

    $incident = (isset($_SESSION['incident'])) ? $_SESSION['incident'] : -1;

    $query = "SELECT f.facility_name, f.latitude, f.longitude 
        FROM fms_facility f
        INNER JOIN fms_facility_to_event fti ON fti.facility_uuid = f.facility_uuid
        WHERE fti.incident_id = ?";

    try {
        $results = $global['db']->Execute($query, $incident);
    } catch (exception $e) {
        add_error("<pre>" . $e->getMessage() . "</pre>");
        $error = true;
    }

    $locations = "";
    $lat = 0;
    $lon = 0;
    $i = 0;
    foreach ($results as $row) {
        $locations .= "['" . preg_replace("/'/", "\'", $row["facility_name"]) . "', " . $row["latitude"] . ", " . $row["longitude"] . "],\n";
        $lat += $row["latitude"];
        $lon += $row["longitude"];
        $i++;
    }

    // trim the last comma
    $locations = substr($locations, 0, -2);
    $avg_lat = $lat / $i;
    $avg_lon = $lon / $i;

    include_once 'templates/fms_map.php';
}

function shn_json_fms_facility_geo()
{

    global $global;
    $db = $global['db'];

    $incident = (isset($_SESSION['incident'])) ? $_SESSION['incident'] : -1;

    // Collect the facility data
    $query = "SELECT *
        FROM fms_facility f
        INNER JOIN fms_facility_to_event fti ON fti.facility_uuid = f.facility_uuid
        WHERE fti.incident_id = ?";

    $db->SetFetchMode(ADODB_FETCH_ASSOC);
    $results = $db->GetArray($query, $incident);

    // Construct response

    $i = 0;
    foreach ($results as $row) {

        // Make information box
        $info = "<h3>" . $row['facility_name'] . "</h3>";

        $info .= "<p>" . $row['street_1'] . "<br/>";
        if (!empty($row['street_2'])) {
            $info .= $row['street_2'] . "<br />";
        }

        $info .= $row['city'] . ", " . $row['state'] . " " . $row['postal_code'] . "<br />";
        $info .= formatPhone($row['work_phone']) . "</p>";

        $info .= "<p>Facility type: " . $row['facility_resource_type_abbr'] . "<br />";
        $info .= "Capacity: " . $row['facility_capacity'] . "</p>";

        $info .= "<p><a href=\"index.php?mod=fms&amp;act=edit&amp;id=" . base64_encode($row['facility_uuid']) . "\">Edit</a></p>";


        $response[$i]['id'] = base64_encode($row['facility_uuid']);
        $response[$i]['name'] = $row['facility_name'];
        $response[$i]['lat'] = $row['latitude'];
        $response[$i]['lon'] = $row['longitude'];
        $response[$i]['info'] = $info;
        $response[$i]['address'] = array(
          'street_1' => $row['street_1'],
          'street_2' => $row['street_2'],
          'city' => $row['city'],
          'state' => $row['state'],
          'postal' => $row['postal_code']
        );

        $i++;
    }

    echo json_encode($response, JSON_HEX_TAG | JSON_HEX_APOS | JSON_HEX_QUOT | JSON_HEX_AMP);
}

function shn_text_fms_set_incident()
{

    $_SESSION['incident'] = $_REQUEST['incident'];

    echo 0;
}

?>