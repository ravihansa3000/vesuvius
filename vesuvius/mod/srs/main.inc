<?php

/**
 * Staff Registry Service Module
 *
 * PHP version >= 5.1
 *
 * LICENSE: This source file is subject to LGPL license
 * that is available through the world-wide-web at the following URI:
 * http://www.gnu.org/copyleft/lesser.html
 *
 * @category  Sahana_Emergency_Management_System
 * @package   Module_srs
 * @author    Clayton Kramer <clayton.kramer@mail.cuny.edu>
 * @copyright 2011 CUNY School of Professional Studies
 * @license   http://www.gnu.org/copyleft/lesser.html GNU Lesser General Public License (LGPL)
 *
 */
// Includes
require_once $global['approot'] . "/inc/lib_uuid.inc";
require_once $global['approot'] . "/inc/lib_location.inc";


// srs includes
require_once 'inc/security.inc';
require_once 'inc/functions.inc';
require_once 'inc/json_functions.inc';
require_once 'inc/text_functions.inc';
require_once 'inc/staff_definition.inc';

require_once($global['approot'].'/inc/lib_xajax.inc');
require_once($global['approot'].'/mod/srs/xajax.inc');

shn_xajax_registerFunction('srs_search');

$global['srs_group_menu'] = false;

global $conf;
$conf['enable_solr_for_search'] = false;


/**
 * Default function
 *
 * @access public
 * @return void
 */

function shn_srs_default()
{
    global $global;
    global $conf;

    echo "<script type=\"text/javascript\" src=\"res/js/jquery-1.6.4.min.js\" ></script>
        <script type=\"text/javascript\" src=\"res/js/jquery-ui-autocomplete.min.js\" ></script>
		<script type=\"text/javascript\" src=\"res/js/jquery-ui-datepicker.min.js\" ></script>
		<script type=\"text/javascript\" >".file_get_contents($global['approot']."/mod/srs/srs.js")."</script>";

    $control = "";

    shn_tabmenu_open();
    shn_tabmenu_item("default", _t("Staff-Menu|About"), "srs");
    shn_tabmenu_item("staff_add", _t("Staff-Menu|Register Staff"), "srs");
    shn_tabmenu_item("search", _t("Staff-Menu|Search Staff"), "srs");
    shn_tabmenu_item("default", _t("Staff-Menu|Dashboard"), "srs");
    shn_tabmenu_item("default", _t("Staff-Menu|Reports"), "srs");
    shn_tabmenu_item("settings", _t("Staff-Menu|Settings"), "srs");
    shn_tabmenu_close();


    $incident = isset($_SESSION['incident']) ? $_SESSION['incident'] : null;

    // Check to see if the user has selected an incident
    if (empty($incident)) {

        // Get the current list of open events
        $query = "SELECT * FROM incident i  
            LEFT JOIN sys_user_groups g ON i.private_group = g.group_id
            WHERE i.parent_id is NULL AND i.closed = 0
            ORDER BY date desc";

        try {
            $query = $global['db']->Prepare($query);
            $results = $global['db']->Execute($query);
        } catch (exception $e) {
            error_log($e->getMessage());
        }
        $events = $results->GetArray();

        // Default to the only open event
        if (count($events) == 1) {
            $incident = $events[0]['incident_id'];
            $_SESSION['incident'] = $incident;
        }
    }

    if (isset($_GET['staff_add'])) {
        $control = "shn_srs_staff_add";
    } elseif(isset($_GET['staff_save_new'])) {
        $control = "shn_srs_staff_save_new";
    } elseif(isset($_GET['search'])) {
        $control = "shn_srs_search";
    } elseif(isset($_GET['settings'])) {
        $control = "shn_srs_settings";
    } elseif(isset($_GET['org_list'])) {
        $control = "shn_srs_org_list";
    } elseif(isset($_GET['volstatus_list'])) {
        $control = "shn_srs_volstatus_list";
    } elseif(isset($_GET['staff_type_list'])) {
        $control = "shn_srs_staff_type_list";
    } elseif(isset($_GET['staff_view'])) {
        $control = "shn_srs_staff_view";
    } elseif(isset($_GET['staff_timeline'])) {
        $control = "shn_srs_staff_timeline";
    } elseif(isset($_GET['staff_timeline_edit'])) {
        $control = "shn_srs_staff_timeline_edit";
    } elseif(isset($_GET['staff_timeline_del'])) {
        $control = "shn_srs_staff_timeline_del";
    } elseif(isset($_GET['staff_checkout'])) {
        $control = "shn_srs_staff_checkout";
    } elseif(isset($_GET['staff_checkout_save'])) {
        $control = "shn_srs_staff_checkout_save";
    } elseif(isset($_GET['staff_checkin'])) {
        $control = "shn_srs_staff_checkin";
    }

        if ($control == "") {
        include_once 'templates/default.php';
    }

    $control();
}

/*
 *  Setting menu for staff
 */
function shn_srs_settings(){

    echo "<li><a href=\"srs?org_list&tabid=5"."\"><span>Organization Names</span></a></li>";
    echo "<li><a href=\"srs?volstatus_list&tabid=5"."\"><span>Volunteer Statuses</span></a></li>";
    echo "<li><a href=\"srs?staff_type_list&tabid=5"."\"><span>Staff Types</span></a></li>";

}

/**
 * Search for staffs
 * 
 * @global array $global 
 */
function shn_srs_search()
{
    // Validate group access
    if (!check_groupAccess(array(1, 2))) {
        add_error("You do not have permission to search the shelter registry.");
        return;
    }

    // Validate group access
    if (!check_validIncident()) {
        add_error("You must select an open event before attempting to perform this action.");

        shn_srs_default();
        return;
    }

    global $global;


    // Get facilities
    $facilities = get_facilities();

    // Get Organizations
    $orgs = getOrgs();

    // Get Volunteer Statuses
    $volStatuses = getVolStatuses();

    // Get the list of facility groups
    $sql = "SELECT DISTINCT f.facility_group FROM fms_facility f
        INNER JOIN fms_facility_to_event fti ON fti.facility_uuid = f.facility_uuid
        WHERE fti.incident_id = ?
        ORDER BY facility_group;";

    // Execute query
    try {
        $sql = $global['db']->Prepare($sql);
        $facilityGroups = $global['db']->Execute($sql, $_SESSION['incident']);
    } catch (exception $e) {
        error_log($e->getMessage());
    }

    $minAge = (isset($_SESSION['minAge'])) ? trim($_SESSION['minAge']) : '';
    $maxAge = (isset($_SESSION['maxAge'])) ? trim($_SESSION['maxAge']) : '';

    include_once 'templates/staff_search.php';
}

function shn_srs_staff_checkin()
{
    // Validate group access
    if (!check_groupAccess(array(1, 2))) {
        add_error("You do not have permission to check in a staff to the shelter registry.");
        return;
    }

    // Validate group access
    if (!check_validIncident()) {
        add_error("You must select an open event before attempting to perform this action.");

        shn_srs_default();
        return;
    }

    // Get the UUID
    $uuid = (isset($_SESSION['uuid'])) ? $_SESSION['uuid'] : null;
    $uuid = (isset($_REQUEST['uuid'])) ? $_REQUEST['uuid'] : $uuid;
    $uuid = $uuid;
    $_SESSION['uuid'] = $uuid;

    // Get staff data
    $staff = get_staff($uuid);

    // Get facilities
    $facilities = get_facilities();

    // Set form properties
    $act = 'staff_checkin_save';
    $formOpts = array('enctype' => 'enctype="multipart/form-data"');
    $submitOpts = 'name="submit" class=""';

    include_once 'templates/staff_checkin.php';
}

function shn_srs_staff_checkin_save()
{
    global $global;

    // Get the UUID
    $uuid = (isset($_SESSION['uuid'])) ? $_SESSION['uuid'] : null;
    $uuid = (isset($_REQUEST['uuid'])) ? $_REQUEST['uuid'] : $uuid;
    $uuid = base64_decode($uuid);
    $_SESSION['uuid'] = base64_encode($uuid);

    // entry date
    isset($_REQUEST['entryMonth']) ? $entryMonth = $_REQUEST['entryMonth'] : $entryMonth = null;
    isset($_REQUEST['entryDay']) ? $entryDay = $_REQUEST['entryDay'] : $entryDay = null;
    isset($_REQUEST['entryYear']) ? $entryYear = $_REQUEST['entryYear'] : $entryYear = null;
    isset($_REQUEST['entryHour']) ? $entryHour = $_REQUEST['entryHour'] : $entryHour = null;
    isset($_REQUEST['entryMinute']) ? $entryMinute = $_REQUEST['entryMinute'] : $entryMinute = null;

    // Facility
    isset($_REQUEST['facility']) ? $facility = $_REQUEST['facility'] : $facility = null;



    // Open a database transaction
    $global['db']->StartTrans();

    // Format the entry date in a SQL ASNI format
    $inDate = "$entryYear-$entryMonth-$entryDay $entryHour:$entryMinute:00";

    $table = 'fms_person_to_facility';
    $record = array();
    $record['p_uuid'] = $uuid;
    $record['facility_uuid'] = $facility;
    $record['in_date'] = $inDate;
    $global['db']->AutoExecute($table, $record, 'INSERT');

    // Set person status
    setPersonStatus('in', $uuid);

    // Check for any failed transactions
    $error = $global['db']->HasFailedTrans();

    // Close transaction
    $global['db']->CompleteTrans();

    // Get staff data
    $staff = get_staff($uuid);

    if ($error) {

        add_error("There was an error processing <strong>" . $staff['full_name'] . "</strong> checkout.");
    } else {

        add_confirmation("You have successfully checked in <strong>" .
            $staff['full_name'] . "</strong> to the " . $staff['facility_name']
            . " shelter facility.");
    }

    // Get staff data
    $staff = get_staff($uuid);

    // Get staff skills
    $skillsList = get_staff_skills($uuid);

    include_once 'templates/staff_view.php';
}

function shn_srs_staff_transfer()
{

    // Get the UUID
    $uuid = (isset($_SESSION['uuid'])) ? $_SESSION['uuid'] : null;
    $uuid = (isset($_REQUEST['uuid'])) ? $_REQUEST['uuid'] : $uuid;
    $uuid = base64_decode($uuid);
    $_SESSION['uuid'] = base64_encode($uuid);

    // Get staff data
    $staff = get_staff($uuid);

    // Get facilities
    $facilities = get_facilities();

    // Set form properties
    $act = 'staff_transfer_save';
    $formOpts = array('enctype' => 'enctype="multipart/form-data"');
    $submitOpts = 'name="submit" class=""';

    include_once 'templates/staff_transfer.php';
}

function shn_srs_staff_transfer_save()
{
    global $global;

    // Get the UUID
    $uuid = (isset($_SESSION['uuid'])) ? $_SESSION['uuid'] : null;
    $uuid = (isset($_REQUEST['uuid'])) ? $_REQUEST['uuid'] : $uuid;
    $uuid = base64_decode($uuid);
    $_SESSION['uuid'] = base64_encode($uuid);

    // exit date
    isset($_REQUEST['exitMonth']) ? $exitMonth = $_REQUEST['exitMonth'] : $exitMonth = null;
    isset($_REQUEST['exitDay']) ? $exitDay = $_REQUEST['exitDay'] : $exitDay = null;
    isset($_REQUEST['exitYear']) ? $exitYear = $_REQUEST['exitYear'] : $exitYear = null;
    isset($_REQUEST['exitHour']) ? $exitHour = $_REQUEST['exitHour'] : $exitHour = null;
    isset($_REQUEST['exitMinute']) ? $exitMinute = $_REQUEST['exitMinute'] : $exitMinute = null;

    // Destination Facility
    $destFacility = isset($_REQUEST['destFacility']) ? $_REQUEST['destFacility'] : null;

    // Get staff data
    $staff = get_staff($uuid);

    // Open a database transaction
    $global['db']->StartTrans();

    // Format the exit date in a SQL ASNI format
    $outDate = "$exitYear-$exitMonth-$exitDay $exitHour:$exitMinute:00";

    // Create transfer record
    $query = "INSERT INTO fms_person_to_transfer (user, p_uuid, src_facility, dest_facility, created_at)
        VALUES (?, ?, ?, ?, CURRENT_TIMESTAMP)";
    $values = array(
      $_SESSION['user_id'],
      $uuid,
      $staff['facility_uuid'],
      $destFacility
    );
    $global['db']->Execute($query, $values);


    $transferId = $global['db']->Insert_ID();

    $whereStr = "WHERE p_uuid = in_date = " . shn_db_clean($staff['in_date']);

    // Checkout and mark the transfer flag

    $query = "UPDATE fms_person_to_facility 
        SET out_date = ?, transfer_id = ?
        WHERE p_uuid = ? AND in_date = ?";

    $values = array($outDate, $transferId, $uuid, $staff['in_date']);
    $global['db']->Execute($query, $values);

    // Set person status
    setPersonStatus('trn', $uuid);

    // Check for any failed transactions
    $error = $global['db']->HasFailedTrans();

    // Close transaction
    $global['db']->CompleteTrans();

    if ($error) {

        add_error("There was an error processing <strong>" . $staff['full_name'] . "</strong> checkout.");
    } else {

        add_confirmation("You have successfully transfered <strong>" .
            $staff['full_name'] . "</strong> from the " .
            $staff['facility_name'] . " shelter registry.");
    }

    // Get staff data again
    $staff = get_staff($uuid);

    // Get staff skills
    $skillsList = get_staff_skills($uuid);

    include_once 'templates/staff_view.php';
}

function shn_srs_staff_checkout()
{

    // Get the UUID
    $uuid = (isset($_SESSION['uuid'])) ? $_SESSION['uuid'] : null;
    $uuid = (isset($_REQUEST['uuid'])) ? $_REQUEST['uuid'] : $uuid;
    $uuid = $uuid;
    $_SESSION['uuid'] = $uuid;

    // Get staff data
    $staff = get_staff($uuid);

    // Set form properties
    $act = 'staff_checkout_save';
    $formOpts = array('enctype' => 'enctype="multipart/form-data"');
    $submitOpts = 'name="submit" class=""';

    include_once 'templates/staff_checkout.php';
}

function shn_srs_staff_checkout_save()
{
    global $global;
    $db = $global['db'];

    // Get the UUID
    $uuid = (isset($_SESSION['uuid'])) ? $_SESSION['uuid'] : null;
    $uuid = (isset($_REQUEST['uuid'])) ? $_REQUEST['uuid'] : $uuid;
    $uuid = $uuid;
    $_SESSION['uuid'] = $uuid;

    // exit date
    isset($_REQUEST['checkOutDate']) ? $exitDate = $_REQUEST['checkOutDate'] : $exitDate = null;
    isset($_REQUEST['exitHour']) ? $exitHour = $_REQUEST['exitHour'] : $exitHour = null;
    isset($_REQUEST['exitMinute']) ? $exitMinute = $_REQUEST['exitMinute'] : $exitMinute = null;

    // Get staff data
    $staff = get_staff($uuid);

    // Open a database transaction
    $db->StartTrans();

    // Format the exit date in a SQL ASNI format
    $outDate = "$exitDate $exitHour:$exitMinute:00";

    // Lookup staff facility
    $query = "SELECT MAX(in_date) AS in_date FROM fms_person_to_facility WHERE p_uuid = ? ";

    // get the member's current facility
    $result = $db->Execute($query, $uuid);
    $ptf = $result->FetchRow();
    $whereStr = "in_date = '" . $ptf['in_date'] . "'";

    // Update the table
    $table = 'fms_person_to_facility';
    $record = array();
    $record['out_date'] = $outDate;
    $db->AutoExecute($table, $record, 'UPDATE', $whereStr);

    // Set person status
    setPersonStatus('out', $uuid);

    // Check for any failed transactions
    $error = $db->HasFailedTrans();

    // Close transaction
    $db->CompleteTrans();

    if ($error) {

        add_error("There was an error processing <strong>" . $staff['full_name'] . "</strong> checkout.");
    } else {

        add_confirmation("You have successfully checked out <strong>" .
            $staff['full_name'] . "</strong> from the " .
            $staff['facility_name'] . " shelter registry.");
    }

    // Get staff data again
    $staff = get_staff($uuid);

    // Get staff skills
    $skillsList = get_staff_skills($uuid);

    include_once 'templates/staff_view.php';
}

function shn_srs_staff_timeline()
{

    global $global;

    // Get the UUID
    $uuid = (isset($_SESSION['uuid'])) ? $_SESSION['uuid'] : null;
    $uuid = (isset($_REQUEST['uuid'])) ? $_REQUEST['uuid'] : $uuid;
    $uuid = $uuid;
    $_SESSION['uuid'] = $uuid;

    // Get staff data
    $staff = get_staff($uuid);

    $sql = "SELECT 
            ptf.id,
            f1.facility_name,
            f1.facility_resource_type_abbr,
            ptf.in_date,
            ptf.out_date,
            ptt.dest_facility,
            f3.facility_name AS dest_name,
            f3.facility_resource_type_abbr AS dest_type,
            p.full_name AS user
        FROM fms_person_to_facility ptf
            INNER JOIN fms_facility f1 ON f1.facility_uuid = ptf.facility_uuid
            LEFT JOIN fms_person_to_transfer ptt ON ptt.id = ptf.transfer_id
            LEFT JOIN person_uuid p ON p.p_uuid = ptt.user
            LEFT JOIN fms_facility f2 ON f2.facility_uuid = ptt.src_facility
            LEFT JOIN fms_facility f3 ON f3.facility_uuid = ptt.dest_facility
        WHERE ptf.p_uuid = ? 
        ORDER BY ptf.in_date ASC";

    // Execute query
    try {
        $sql = $global['db']->Prepare($sql);
        $results = $global['db']->Execute($sql, $uuid);
    } catch (exception $e) {
        error_log($e->getMessage());
    }

    include_once 'templates/staff_timeline.php';
}

function shn_srs_staff_timeline_del()
{

    global $global;
    $db = $global['db'];

    $timelineId = isset($_REQUEST['id']) ? $_REQUEST['id'] : 0;

    // Begin a transaction
    $db->StartTrans();

    // See if there is a transfer facility
    $q1 = "SELECT ptt.id
        FROM fms_person_to_facility ptf 
            INNER JOIN fms_person_to_transfer ptt ON ptt.id = ptf.transfer_id
            INNER JOIN person_status ps ON ps.p_uuid = ptt.p_uuid
        WHERE ptf.id = ?";

    $pttId = $db->GetOne($q1, array($timelineId));

    if (isset($pttId)) {
        // Delete the persont_to_transfer record
        $q2 = "DELETE FROM fms_person_to_facility WHERE id = ?";
        $db->Execute($q2, array($pttId));
    }

    $q3 = "DELETE FROM fms_person_to_facility WHERE id = ?";
    $db->Execute($q3, array($timelineId));

    // Check for any failed transactions
    $error = $db->HasFailedTrans();

    // Close transaction
    $db->CompleteTrans();

    if ($error) {
        add_error($db->ErrorMsg());
    } else {
        add_confirmation("Delete of record successful.");
    }

    shn_srs_staff_timeline();
}

function shn_srs_staff_timeline_edit()
{

    global $global;
    $db = $global['db'];

    $timelineId = isset($_REQUEST['id']) ? $_REQUEST['id'] : 0;

    $sql = "SELECT 
            ptf.id,
            p.p_uuid,
            p.full_name,
            ptf.facility_uuid,
            f1.facility_name,
            f1.facility_resource_type_abbr,
            ptf.in_date,
            ptf.out_date,
            ptt.dest_facility,
            f1.facility_uuid AS src_id,
            f1.facility_name AS src_name,
            f1.facility_resource_type_abbr AS src_type,
            f3.facility_uuid AS dest_id,
            f3.facility_name AS dest_name,
            f3.facility_resource_type_abbr AS dest_type,
            p.full_name AS user
        FROM fms_person_to_facility ptf
            INNER JOIN fms_facility f1 ON f1.facility_uuid = ptf.facility_uuid
            LEFT JOIN fms_person_to_transfer ptt ON ptt.id = ptf.transfer_id
            LEFT JOIN person_uuid p ON p.p_uuid = ptf.p_uuid
            LEFT JOIN fms_facility f2 ON f2.facility_uuid = ptt.src_facility
            LEFT JOIN fms_facility f3 ON f3.facility_uuid = ptt.dest_facility
        WHERE ptf.id = ?
        ORDER BY ptf.in_date ASC";

    try {
        $results = $db->Execute($sql, array($timelineId));
    } catch (exception $e) {
        add_error("<pre>" . print_r($e->getMessage(), true) . "</pre>");
    }

    // Check for null results
    if (empty($results)) {
        add_error($db->ErrorMsg());
        return;
    } else {
        $data = $results->FetchRow();
    }

    $uuid = $data['p_uuid'];
    $_SESSION['uuid'] = $uuid;

    // Get staff data
    $staff = get_staff($uuid);

    // Get facilities
    $facilities = get_facilities();

    // Set form properties
    $act = 'staff_timeline_save';
    $formOpts = array('enctype' => 'enctype="multipart/form-data"');
    $submitOpts = 'name="submit" class=""';

    include_once 'templates/staff_timeline_edit.php';
}

function shn_srs_staff_timeline_save()
{
    global $global;
    $db = $global['db'];

    $requiredFields = array(
      'checkInDate' => 'Sign in date',
      'entryHour' => 'Sign in hour',
      'entryMinute' => 'Sign in minute',
      'srcFacility' => 'Assigned facility location'
    );

    $test = null;

    foreach ($requiredFields as $key => $value) {

        $test = $_REQUEST[$key];

        if (empty($_REQUEST[$key])) {
            add_error($value . " not provided.");
            $error = true;
        }
    }

    // Entry date
    $entryDate = isset($_REQUEST['checkInDate']) ? $_REQUEST['checkInDate'] : null;
    $entryHour = isset($_REQUEST['entryHour']) ? $_REQUEST['entryHour'] : null;
    $entryMinute = isset($_REQUEST['entryMinute']) ? $_REQUEST['entryMinute'] : null;

    $inDate = "$entryDate $entryHour:$entryMinute:00";

    // Exit date
    $exitDate = isset($_REQUEST['checkOutDate']) ? $_REQUEST['checkOutDate'] : null;
    $exitHour = isset($_REQUEST['exitHour']) ? $_REQUEST['exitHour'] : null;
    $exitMinute = isset($_REQUEST['exitMinute']) ? $_REQUEST['exitMinute'] : null;

    $inDate = "$entryDate $entryHour:$entryMinute:00";
    $outDate = "$exitDate $exitHour:$exitMinute:00";

    // Facility
    $facility = isset($_REQUEST['srcFacility']) ? $_REQUEST['srcFacility'] : null;
    $destFacility = isset($_REQUEST['destFacility']) ? $_REQUEST['destFacility'] : null;
    $timelineId = isset($_REQUEST['id']) ? $_REQUEST['id'] : null;

    // Open a database transaction
    $db->StartTrans();

    // Update sql
    $values = array($facility, $inDate);
    $sql = "UPDATE fms_person_to_facility
        SET facility_uuid = ?, in_date = ?";

    if (!empty($exitDate)) {
        $sql .= ", out_date = ?";
        $values[] = $outDate;
    }

    // See if there is already a transfer facility
    $q2 = "SELECT ptt.id
        FROM fms_person_to_facility ptf 
            INNER JOIN fms_person_to_transfer ptt ON ptt.id = ptf.transfer_id
        WHERE ptf.id = ?";

    $pttId = $db->GetOne($q2, array($timelineId));

    if (!empty($destFacility)) {

        if (isset($pttId)) {

            // Update current transfer record
            $query = "UPDATE fms_person_to_transfer
                SET src_facility = ?, dest_facility = ?
                WHERE id = ?";

            $vals = array($facility, $destFacility, $pttId);

            try {
                $db->Execute($query, $vals);

                $foo = 1;
            } catch (Exception $e) {
                add_error("<pre>" . print_r($e->getMessage(), true) . "</pre>");
            }
        } else {

            // Create transfer record
            $query = "INSERT INTO fms_person_to_transfer (user, p_uuid, src_facility, dest_facility, created_at)
                VALUES (?, ?, ?, ?, CURRENT_TIMESTAMP)";
            $vals = array(
              $_SESSION['user_id'],
              $_SESSION['uuid'],
              $facility,
              $destFacility
            );

            try {
                $db->Execute($query, $vals);
            } catch (Exception $e) {
                add_error("<pre>" . print_r($e->getMessage(), true) . "</pre>");
            }

            $transferId = $global['db']->Insert_ID();

            $sql .= ", transfer_id = ?";
            $values[] = $transferId;
        }
    } else {

        if ($pttId) {
            // No destFacility provided but we have a pttId so delete it

            $q = "DELETE FROM fms_person_to_transfer WHERE id = ?";

            try {
                $db->Execute($q, array($pttId));
            } catch (Exception $e) {
                add_error("<pre>" . print_r($e->getMessage(), true) . "</pre>");
            }
        }
    }

    $sql .= "
        WHERE id = ?";
    $values[] = $timelineId;

    try {
        $results = $db->Execute($sql, $values);
    } catch (exception $e) {
        add_error("<pre>" . print_r($e->getMessage(), true) . "</pre>");
    }

    // Check for any failed transactions
    $error = $global['db']->HasFailedTrans();

    // Close transaction
    $global['db']->CompleteTrans();

    if ($error) {
        add_error("Updated failed.");
    } else {
        add_confirmation("Update successful.");
    }

    shn_srs_staff_timeline_edit();
}

/**
 * View staff details function
 *
 * @access public
 * @return void
 */
function shn_srs_staff_view()
{

    // Get the UUID
    $uuid = (isset($_SESSION['uuid'])) ? $_SESSION['uuid'] : null;
    $uuid = (isset($_REQUEST['uuid'])) ? $_REQUEST['uuid'] : $uuid;
    $uuid = $uuid;
    $_SESSION['uuid'] = $uuid;

    // Get staff data
    $staff = get_staff($uuid);

    // Get staff skills
    $skillsList = get_staff_skills($uuid);

    include_once 'templates/staff_view.php';
}

/**
 * Add new staffs form
 *
 * @access public
 * @return void
 */
function shn_srs_staff_add()
{
    // Validate group access
    if (!check_groupAccess(array(1, 2))) {
        add_error("You do not have permission to add a staff to the shelter registry.");
        return;
    }

    // Validate group access
    if (!check_validIncident()) {
        add_error("You must select an open event before attempting to add a staff to the registry.");

        shn_srs_default();
        return;
    }

    global $global;

    // Set form properties
    $act = 'staff_save_new';
    $formOpts = array('enctype' => 'enctype="multipart/form-data"');
    $submitOpts = 'name="submit" class=""';

    // Clear session uuid
    $_SESSION['uuid'] = null;

    // Get Organizations
    $orgs = getOrgs();

    // Get Volunteer Statuses
    $volStatuses = getVolStatuses();

    // Get Staff Types
    $staffTypes = getStaffTypes();

    // Get facilities
    $facilities = get_facilities();

    echo "<h1>New Shelter Client Form</h1>";

    shn_form_fopen("staff_save_new", null, array('enctype' => 'enctype="multipart/form-data"', 'req_message' => true));
    shn_form_hidden(array('seq' => 'entry'));

    shn_form_fsopen(_t('New Report In Details'));
    shn_form_date(_t("Date"), "checkInDate", array('req'=>true));
    shn_form_time(_t("Time"), "entryHour", "entryMinute", array('req'=>true));

    $opt_status = array();
    $opt_status[""] = "- Select -";

    foreach ($facilities as $facility) {
        $opt_status[$facility['facility_uuid']] = $facility['facility_name'];
    }

    shn_form_select($opt_status, _t('Facility Location'), "facility", null,  array('req' => true));
    shn_form_fsclose();

    $opt_orgs = array();
    $opt_orgs[""] = "- Select -";

    foreach ($orgs as $org) {
        $opt_orgs[$org['id']] = $org['name'];
    }

    $opt_status = array();
    $opt_status["select"] = "- Select -";

    foreach ($volStatuses as $volStatus) {
        $opt_status[$volStatus['id']] = $volStatus['description'];
    }

    $opt_type = array();
    $opt_type["select"] = "- Select -";

    foreach ($staffTypes as $staffType) {
        $opt_type[$staffType['id']] = $staffType['description'];
    }

    shn_form_fsopen(_t('Organization Information'));

    shn_form_select($opt_orgs, _t('Name'), "org_id", null,  array('req' => true));
    shn_form_select($opt_status, _t('Volunteer Status'), "vol_status", null,  array('req' => true));
    shn_form_select($opt_type, _t('Type'), "staff_type", null,  array( 'req' => true));

    shn_form_fsclose();

    shn_form_fsopen(_t('Staff Details'));

    shn_form_text(_t("First Name"),'given_name','size="20"',array('req'=>true, 'help'=>'Given Name'));
    shn_form_text(_t("Last Name"),'family_name','size="20"',array('req'=>true, 'help'=>'Family Name'));
    shn_form_text(_t("MI"),'middle_initial','size="5"',array('help'=>'Middle Initials'));
    shn_form_date(_t("Date of Birth"), "dob");
    $opt_status = array("" => "- Select -", "Male" => "Male", "Female" => "Female");
    shn_form_select($opt_status, _t('Gender'), "gender", null,  array('req' => true));

    shn_form_fsclose();

    shn_form_fsopen(_t('Contact Information'));

    shn_form_text(_t("Street Address"),'street_1','size="25"');
    shn_form_text(_t("Address Line 2"),'street_2','size="25"');
    shn_form_text(_t("City"),'city','size="10"');
    shn_form_text(_t("State / Province / Region"),'state','size="10"');
    shn_form_text(_t("Postal / Zip Code"),'postal','size="10"');

    $q = "SELECT `option_code`, `option_description` FROM `field_options` WHERE `field_name` = 'opt_country' order by `option_description`;";
    $res = $global['db']->Execute($q);
    $opt_status = array();
    while (!$res->EOF) {
        $opt_status[$res->fields['option_code']] = $res->fields['option_description'];
        $res->MoveNext();
    }

    shn_form_select($opt_status, _t('Country'), "opt_country", null,  array('value'=>'USA'));
    shn_form_text(_t("Work Phone"),'home','size="10"');
    shn_form_fsclose();

    shn_form_fsopen(_t('Demographic Information'));

    shn_form_text(_t("Occupation"),'occupation','size="20"');
    shn_form_text(_t("Special Skills"),'skills','size="20"',array('help'=>'Type in comma separated.'));
    shn_form_text(_t("Languages Spoken"),'altlang1','size="10"',array('help'=>'1st Language'));
    shn_form_text("",'altlang2','size="10"',array('help'=>'2nd Language'));
    shn_form_text("",'altlang3','size="10"',array('help'=>'3rd Language'));

    shn_form_fsclose();


    shn_form_submit("Save", "class=\"styleTehButton\""); echo '&nbsp&nbsp';
    shn_form_submit("Save + Continue", "class=\"styleTehButton\""); echo '&nbsp&nbsp';
    echo "<script>initDate();</script>";
    shn_form_fclose();
    echo "<br><br>";

}

/**
 *  Save existing staff updates
 */
function shn_srs_staff_save_edit()
{
    include_once 'inc/staff_save.inc';

    $mode = isset($_REQUEST['mode']) ? $_REQUEST['mode'] : null;
    $submit = isset($_REQUEST['submit']) ? $_REQUEST['submit'] : null;

    // Get the staff uuid
    isset($_REQUEST['uuid']) ? $uuid = base64_decode($_REQUEST['uuid']) : $uuid = null;
    $error = false;

    $validation = array(
      'given_name' => 'First name not provided.',
      'family_name' => 'Last name not provided.',
      'org_id' => 'Organization not selected.',
      'vol_status' => 'Volunteer status not selected.',
      'staff_type' => 'You must select a staff type.',
      'uuid' => 'UUID error.'
    );

    $test = null;

    foreach ($validation as $key => $value) {

        $test = $_REQUEST[$key];

        if (empty($_REQUEST[$key])) {
            add_error($value);
            $error = true;
        }
    }


    if (isset($uuid) && $error == false) {
        $success = staff_save($uuid);

        if ($success != -1 && $error == false) {
            add_confirmation("Updated.");
        } else {
            add_error("Failed to update.");
        }
        shn_srs_staff_edit();
    }
}

/**
 *  Save new staffs action
 */
function shn_srs_staff_save_new()
{
    include_once 'inc/staff_save.inc';

    $mode = isset($_REQUEST['mode']) ? $_REQUEST['mode'] : null;
    $submit = isset($_REQUEST['submit']) ? $_REQUEST['submit'] : null;
    $error = false;


    $validation = array(
      'given_name' => 'First name not provided.',
      'family_name' => 'Last name not provided.',
      'facility' => "Facility not selected",
      'gender' => "Gender not selected",
      'checkInDate' => "Date not provided.",
      'entryHour' => 'Hour not provided',
      'entryMinute' => 'Minute not provided',
      'org_id' => 'Organization not selected.',
      'vol_status' => 'Volunteer status not selected.',
      'staff_type' => 'Staff Type not selected.'
    );

    $test = null;

    foreach ($validation as $key => $value) {

        $test = $_REQUEST[$key];

        if (empty($_REQUEST[$key])) {
            add_error($value);
            $error = true;
        }
    }

    if ($error == false) {
        // Save the new staff
        $uuid = staff_save();
    }

    if ($error == true || $uuid == "-1") {
        add_error('There was an error saving the staff recored');
    } else {
        // See if the user is entering group members
        add_confirmation("You have successfully saved the staff to the registry information.");
        if ($submit == 'Save + Continue') {

            // Return to the staff add form for next new staff
            shn_srs_staff_add();
        } else {
            shn_srs_search();
        }
    }
}

/**
 * Delete a staff
 * 
 * @global array $global 
 */
function shn_srs_staff_delete()
{

    global $global;

    $uuid = base64_decode($_REQUEST['uuid']);

    // Set the deleted bit
    $sql[] = "UPDATE person_status SET isdeleted = 1 WHERE p_uuid = ?;";

    // Open transaction
    $global['db']->StartTrans();

    foreach ($sql as $q) {
        // Delete person
        try {
            $q = $global['db']->Prepare($q);
            $results = $global['db']->Execute($q, $uuid);
        } catch (exception $e) {
            error_log($e->getMessage());
        }
    }

    // Close transaction
    $global['db']->CompleteTrans();

    // Return to the default STS page
    shn_srs_search();
}

/**
 * Edit staffs function
 *
 * @access public
 * @return void
 */
function shn_srs_staff_edit()
{
    // Validate group access
    if (!check_groupAccess(array(1, 2))) {
        add_error("You do not have permission to add a staff to the shelter registry.");
        return;
    }

    // Validate group access
    if (!check_validIncident()) {
        add_error("You must select an open event before attempting to add a staff to the registry.");

        shn_srs_default();
        return;
    }

    include_once 'inc/functions.inc';

    global $global;

    // Set form properties
    $act = 'staff_save_edit';
    $formOpts = array('enctype' => 'enctype="multipart/form-data"');
    $submitOpts = 'name="submit" class="styleTheButton"';

    // Get Organizations
    $orgs = getOrgs();

    // Get Volunteer Statuses
    $volStatuses = getVolStatuses();

    // Get Staff Types
    $staffTypes = getStaffTypes();

    // Get facilities
    $facilities = get_facilities();

    // Get the UUID
    $uuid = (isset($_SESSION['uuid'])) ? $_SESSION['uuid'] : null;
    $uuid = (isset($_REQUEST['uuid'])) ? $_REQUEST['uuid'] : $uuid;
    $uuid = base64_decode($uuid);
    $_SESSION['uuid'] = base64_encode($uuid);

    // Get facilities
    $facilities = get_facilities();

    // Get staff data
    $staff = get_staff($uuid);

    // Load phone numbers as arrays
    $homePhone = isset($staff["home_phone"]) ? parse_phone($staff["home_phone"]) : '';
    $workPhone = isset($staff["work_phone"]) ? parse_phone($staff["work_phone"]) : '';


    // Get staff's skills
    $skillsList = get_staff_skills($uuid);

    // Set form mode
    $global['srs_form_mode'] = 'edit';
    $global['srs_group_menu'] = true;

    include_once 'templates/staff_edit_form.php';
}

function shn_srs_dashboard_orgs()
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

    global $global;

    include_once "inc/functions.inc";

    // Get the total staff types
    $sql = "SELECT 
        st.description,
        COUNT(*) AS `count`
    FROM srs_person_to_staff_types ptst
        INNER JOIN srs_staff_types st ON st.id = ptst.stafftype_id 
        INNER JOIN srs_person_to_org ptg ON ptg.p_uuid = ptst.p_uuid
        INNER JOIN person_status AS ps ON ps.p_uuid = ptst.p_uuid
        INNER JOIN person_uuid AS p ON p.p_uuid = ptst.p_uuid
    WHERE ps.opt_status = 'in' AND ps.isdeleted = 0 AND p.incident_id = ?
    GROUP BY st.id";

    // Execute query
    try {
        $rs = $global['db']->Execute($sql, $_SESSION['incident']);
    } catch (exception $e) {
        error_log($e->getMessage());
    }
    $totalStaffTypes = $rs->GetArray();
    
    unset($rs);


    // Get the orgs totals
    $sql = "SELECT 
        org.id,
        org.name,
        COUNT(*) AS `count`
    FROM srs_person_to_org ptg
        INNER JOIN srs_organizations org ON org.id = ptg.org_id
        INNER JOIN person_status AS ps ON ps.p_uuid = ptg.p_uuid
        INNER JOIN person_uuid AS p ON p.p_uuid = ptg.p_uuid
    WHERE ps.opt_status = 'in' AND ps.isdeleted = 0 AND p.incident_id = ?
    GROUP BY org.id
    ORDER BY `count` DESC;";

    // Execute query
    try {
        $rs = $global['db']->Execute($sql, $_SESSION['incident']);
    } catch (exception $e) {
        error_log($e->getMessage());
    }

    $orgs = $rs->GetArray();

    include_once 'templates/dashboard_orgs.php';
}

function shn_srs_dashboard_map()
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


    // Get event location
    global $global;
    $eventId = $_SESSION['incident'];

    // Lookup the event's geocodes
    $q = "SELECT name, latitude, longitude 
        FROM incident WHERE incident_id = ?";

    try {
        $loc = $global['db']->GetRow($q, $eventId);
    } catch (exception $e) {
        error_log($e->getMessage());
    }

    if (empty($loc['latitude'])) {
        $zoom = 7;
        $loc['latitude'] = 40.717646687032094;
        $loc['longitude'] = -74.412109375;
    } else {
        $zoom = 10;
    }


    include_once 'templates/dashboard_map.php';
}

function shn_srs_dashboard_staff()
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

    global $global;

    include_once "inc/functions.inc";

    // Get the total staff types
    $sql = "SELECT 
        st.description,
        SUM(IF(ps.opt_status ='in',1,0)) AS reported_in,
        SUM(IF(ps.opt_status ='out',1,0)) AS signed_out,
        SUM(IF(ps.opt_status ='trn',1,0)) AS transfered,
        COUNT(*) AS total
    FROM srs_person_to_staff_types ptst
        INNER JOIN srs_staff_types st ON st.id = ptst.stafftype_id 
        INNER JOIN srs_person_to_org ptg ON ptg.p_uuid = ptst.p_uuid
        INNER JOIN person_status AS ps ON ps.p_uuid = ptst.p_uuid
    GROUP BY st.id
    ORDER BY st.description";

    // Execute query
    try {
        $rs = $global['db']->Execute($sql);
    } catch (exception $e) {
        error_log($e->getMessage());
    }

    $totalStaffTypes = $rs->GetArray();


    include_once 'templates/dashboard_staff.php';
}

function shn_srs_dashboard_facility()
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

    global $global;

    include_once "inc/functions.inc";

    // Get the total staff types
    $sql = "SELECT 
            f.facility_uuid,
            f.facility_name,
            f.facility_resource_type_abbr,
            COUNT(*) AS `count`
        FROM srs_person_to_org ptg
            INNER JOIN person_uuid AS p ON p.p_uuid = ptg.p_uuid
            INNER JOIN crs_latest_ptf  ptf ON ptf.p_uuid = ptg.p_uuid
            INNER JOIN fms_facility f ON f.facility_uuid = ptf.facility_uuid
            INNER JOIN person_status AS ps ON ps.p_uuid = ptg.p_uuid
        WHERE ps.opt_status = 'in' AND p.incident_id = ?
        GROUP BY f.facility_uuid
        ORDER BY f.facility_name";

    // Execute query
    try {
        $rs = $global['db']->Execute($sql, $_SESSION['incident']);
    } catch (exception $e) {
        error_log($e->getMessage());
    }

    $facilityTotals = $rs->GetArray();


    include_once 'templates/dashboard_facility.php';
}

function shn_srs_dashboard_displaymap()
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

    global $global;

    $query = "SELECT 
        f.facility_uuid,
        f.facility_name,
        f.latitude,
        f.longitude,
        f.facility_capacity,
        count(*) AS `count`
    FROM fms_person_to_facility AS ptf
        INNER JOIN person_uuid AS p ON p.p_uuid = ptf.p_uuid
        INNER JOIN fms_facility AS f ON ptf.facility_uuid = f.facility_uuid
    WHERE p.incidient_id = ?
    GROUP BY ptf.facility_uuid";

    try {
        $results = $global['db']->Execute($query, $_SESSION['incident']);
    } catch (exception $e) {
        error_log($e->getMessage());
    }

    $locations = "";
    $lat = 0;
    $lon = 0;
    $i = 0;
    foreach ($results as $row) {

        // Calculate capacity usage 

        $pct_full = 100 - round(($row['facility_capacity'] - $row['count']) / $row['facility_capacity'] * 100, 0);

        // Assign pin color based on percent full
        if ($pct_full > 100) {
            $pin = 'purple';
        } elseif ($pct_full >= 90) {
            $pin = 'red';
        } elseif ($pct_full >= 50) {
            $pin = 'orange';
        } elseif ($pct_full > 0) {
            $pin = 'green';
        } else {
            $pin = 'blue';
        }


        $locations .= "['" . preg_replace("/'/", "\'", $row["facility_name"]) . "', " . $row["latitude"] . ", " . $row["longitude"] . ", '$pin' ],\n";
        $lat += $row["latitude"];
        $lon += $row["longitude"];
        $i++;
    }

    // Trim the last comma
    $locations = substr($locations, 0, -2);

    // Find the average center
    $avg_lat = $lat / $i;
    $avg_lon = $lon / $i;

    include('templates/dashboard_map.php');
}

function shn_srs_report_fcomp()
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

    global $global;

    // Get the sorting and ordering
    $sidx = isset($_REQUEST['sidx']) ? $_REQUEST['sidx'] : 1;
    $sord = isset($_REQUEST['sord']) ? $_REQUEST['sord'] : "asc";

    // Column titles
    $columns = array(
      "Facility Group", "Site Type", "Site Code", "Site Name", "Current Population",
      "Medical Alerts", "Men", "Women", "Infants 0-3", "Children 4-17", "Adults 18-64",
      "Seniors 65+", "% Capacity", "Sheltering Capacity");

    // Get stats on the facility
    $query = "SELECT 
          f.facility_group,
          f.facility_resource_type_abbr,
          f.facility_code,
          f.facility_name,
          count(*) AS population,
          SUM(IF(special_medical IS null,1,0)) AS med_alert,
          SUM(IF(opt_gender ='Male',1,0)) AS males,
          SUM(IF(opt_gender ='Female',1,0)) AS females,
          SUM(IF(years_old BETWEEN 0 AND 3,1,0)) AS infants,
          SUM(IF(years_old BETWEEN 4 AND 17,1,0)) AS children,
          SUM(IF(years_old BETWEEN 18 AND 64,1,0)) AS adults,
          SUM(IF(years_old BETWEEN 65 AND 150,1,0)) AS seniors,
          FLOOR((f.facility_capacity - count(*))/ f.facility_capacity * 100) AS pct_free,
          f.facility_capacity AS capacity
      FROM fms_person_to_facility AS ptf
          INNER JOIN fms_facility AS f ON ptf.facility_uuid = f.facility_uuid
          INNER JOIN srs_med_alert AS ma ON ma.p_uuid = ptf.p_uuid
          INNER JOIN person_details AS pd ON ptf.p_uuid = pd.p_uuid
          INNER JOIN person_status AS ps ON ps.p_uuid = pd.p_uuid
          INNER JOIN person_uuid AS p ON p.p_uuid = ptf.p_uuid
      WHERE ps.opt_status = 'in' AND ps.isdeleted = 0 AND p.incident = ?
      GROUP BY ptf.facility_uuid
      ORDER BY $sidx $sord,  facility_name";

    $global['db']->SetFetchMode(ADODB_FETCH_ASSOC);
    // Execute query
    try {
        $rs = $global['db']->Execute($query, $_SESSION['incident']);
    } catch (exception $e) {
        error_log($e->getMessage());
    }
    $global['db']->SetFetchMode(ADODB_FETCH_DEFAULT);

    include_once 'templates/report_fcomp.php';
}

function shn_srs_report_contacts()
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

    global $global;

    // Redirect posts
    if (isset($_POST['f'])) {
        header("Location: " . $_SERVER['PHP_SELF'] . "?mod=srs&act=report_fcontacts&f=" . $_POST['f'] . "&fg=" . $_POST['fg']);
    }

    // Get facilities
    $facilities = get_facilities();

    // Get Organizations
    $orgs = getOrgs();

    // Get Volunteer Statuses
    $volStatuses = getVolStatuses();

    include_once "inc/functions.inc";

    $facilityId = isset($_REQUEST['f']) ? base64_decode($_REQUEST['f']) : null;
    $facilityGroupId = isset($_REQUEST['fg']) ? base64_decode($_REQUEST['fg']) : null;

    include_once 'templates/report_contacts.php';
}

function shn_text_srs_report_contacts_print()
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

    include_once 'inc/report_contacts_print.inc';
    include_once 'templates/report_contacts_print.php';
}

function shn_srs_report_timeline()
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

    global $global;
    $db = $global['db'];

    $orgId = isset($_REQUEST['id']) ? $_REQUEST['id'] : null;
    $facilityId = isset($_REQUEST['f']) ? base64_decode($_REQUEST['f']) : null;

    include_once "inc/functions.inc";

    // Get Organizations
    $orgs = getOrgs();

    // Get the facility name
    if (isset($facilityId)) {
        $q = "SELECT facility_name FROM fms_facility WHERE facility_uuid = ?";
        $facilityName = $db->GetOne($q, $facilityId);
    } else {
        $facilityName = null;
    }

    // Set form properties
    $act = 'report_timeline';
    $formOpts = array('enctype' => 'enctype="multipart/form-data"');
    $submitName = "Save";
    $submitOpts = 'name="submit" class=""';

    // Filter logic
    $whereClause = "";
    $values = array();
    if (isset($orgId) && $orgId != 'all') {
        $logic = (isset($values[0])) ? "AND" : "WHERE";
        $values[] = $orgId;
        $whereClause .= "$logic org_id = ? \n";
    }

    if (isset($facilityId) && $facilityId != 'all') {
        $logic = (isset($values[0])) ? "AND" : "WHERE";
        $values[] = $facilityId;
        $whereClause .= "$logic facility_uuid = ? \n";
    }

    // Get only this incident
    $logic = (isset($values[0])) ? "AND" : "WHERE";
    $values[] = $_SESSION['incident'];
    $whereClause .= "$logic incident_id = ? \n";

    // Timeline org query
    $query = "SELECT 
            org_name,
            staff_type,
            vol_type,
            p_uuid,
            full_name,
            home_phone,
            work_phone
        FROM srs_staff_search 
            $whereClause
        GROUP BY org_id, p_uuid
        ORDER BY org_name, full_name";

    // Execute query
    try {
        $rs = $db->Execute($query, $values);
    } catch (exception $e) {
        error_log($e->getMessage());
    }
    $data = $rs->GetRows();

    include_once 'templates/report_timeline.php';
}

function shn_srs_report_checkout()
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

    global $global;

    // Redirect posts
    if (isset($_POST['f'])) {
        header("Location: " . $_SERVER['PHP_SELF'] . "?mod=srs&act=report_checkout&f=" . $_POST['f'] . "&fg=" . $_POST['fg']);
    }

    include_once "inc/functions.inc";

    $f = isset($_REQUEST['f']) ? base64_decode($_REQUEST['f']) : null;
    $fg = isset($_REQUEST['fg']) ? base64_decode($_REQUEST['fg']) : null;

    // Get the list of facilities
    $group = ($fg == 'all') ? null : $fg;
    $facilities = get_facilities($group);

    // Get a list of facility groups
    $facilityGroups = get_facility_groups();

    include_once 'templates/report_checkout.php';
}

function shn_text_srs_report_checkout_print()
{

    include_once 'inc/report_checkout_print.inc';
    include_once 'templates/report_checkout_print.php';
}

function shn_srs_org_list()
{
    // Get org list
    $orgs = getOrgs();
    $orgName = '';

    // Set form properties
    $act = 'org_save';
    $formOpts = array('enctype' => 'enctype="multipart/form-data"');
    $submitName = "Add";
    $submitOpts = 'name="submit" class=""';

    include_once 'templates/settings_list_orgs.php';
}

function shn_srs_org_edit()
{
    $orgId = isset($_REQUEST['id']) ? $_REQUEST['id'] : null;

    // Get org list
    $orgs = getOrgs();

    $orgName = getOrgName($orgId);

    // Set form properties
    $act = 'org_save';
    $formOpts = array('enctype' => 'enctype="multipart/form-data"');
    $submitName = "Save";
    $submitOpts = 'name="submit" class=""';

    include_once 'templates/settings_list_orgs.php';
}

function shn_srs_org_save()
{

    global $global;

    $orgName = isset($_REQUEST['org_name']) ? trim($_REQUEST['org_name']) : null;
    $orgId = isset($_REQUEST['id']) ? trim($_REQUEST['id']) : null;
    $action = $_REQUEST['submit'];

    $foo = 'bar';

    switch ($action) {

        // Save new organization
        case "Add":

            $query = "INSERT INTO srs_organizations (name, incident_id, created_at) VALUES (?, ?, ?)";
            $values = array($orgName, $_SESSION['incident'], date('Y-m-d H:i:s'));

            // Execute query
            try {
                $rs = $global['db']->Execute($query, $values);
            } catch (exception $e) {
                error_log($e->getMessage());
            }
            break;

        // Update organization
        case "Save":

            $query = "UPDATE srs_organizations SET name = ? WHERE id = ?";
            $values = array($orgName, $orgId);

            // Execute query
            try {
                $rs = $global['db']->Execute($query, $values);
            } catch (exception $e) {
                error_log($e->getMessage());
            }
            break;
    }

    $orgName = '';
    $orgId = null;
    shn_srs_org_list();
}

function shn_srs_org_del()
{
    global $global;

    $orgId = isset($_REQUEST['id']) ? trim($_REQUEST['id']) : null;

    // Delete organization

    $query = "DELETE FROM srs_organizations WHERE id = ?";
    $values = array($orgId);

    // Execute query
    try {
        $rs = $global['db']->Execute($query, $values);
    } catch (exception $e) {
        error_log($e->getMessage());
    }

    shn_srs_org_list();
}

function shn_srs_import_rmp_orgs()
{

    global $global, $conf;
    $db = $global['db'];
    $error = false;

    $eventId = $_SESSION['incident'];

    // Attempt to connect and retrieve Sahana Resource Management Planning (SRMP) data
    $content = @file_get_contents("{$conf['sems_url']}/webservices/get/{$conf['sems_token']}/organizations.json", true);
    if (strpos($http_response_header[0], "200")) {
        $data = json_decode($content, true);
    } else {
        $error = true;
        add_error("Unable to connect to the Sahana RMP server. Please check the system configuration.");
    }

    if (!$error) {

        foreach ($data as $row) {
            // First check to see if there is already a record for the value
            $q = "SELECT id FROM srs_organizations WHERE name = ? AND incident_id = ?";
            $v = array($row['organization'], $eventId);
            $id = $db->GetOne($q, $v);

            if (empty($id)) {
                // Insert a record
                $q = "INSERT INTO srs_organizations (name, incident_id, created_at)
                    VALUES (?, ?, ?)";
                $v[] = date('Y-m-d H:i:s');
                $db->Execute($q, $v);
                add_confirmation("Imported " . $v[0]);
            } else {
                add_information("<i>" . $v[0] . "</i> already present.");
            }
        }
    }

    shn_srs_org_list();
}

function shn_srs_volstatus_list()
{
    // Get vol status list
    $volStatuses = getVolStatuses();
    $volStatusName = '';

    // Set form properties
    $act = 'volstatus_save';
    $formOpts = array('enctype' => 'enctype="multipart/form-data"');
    $submitName = "Add";
    $submitOpts = 'name="submit" class=""';

    include_once 'templates/settings_volstatus_list.php';
}

function shn_srs_volstatus_edit()
{
    $volStatusId = isset($_REQUEST['id']) ? $_REQUEST['id'] : null;

    // Get vol status list
    $volStatuses = getVolStatuses();

    $volStatusName = getVolStatusName($volStatusId);

    // Set form properties
    $act = 'volstatus_save';
    $formOpts = array('enctype' => 'enctype="multipart/form-data"');
    $submitName = "Save";
    $submitOpts = 'name="submit" class=""';

    include_once 'templates/settings_volstatus_list.php';
}

function shn_srs_volstatus_save()
{

    global $global;

    $volStatusName = isset($_REQUEST['org_name']) ? trim($_REQUEST['org_name']) : null;
    $volStatusId = isset($_REQUEST['id']) ? trim($_REQUEST['id']) : null;
    $action = $_REQUEST['submit'];

    $foo = 'bar';

    switch ($action) {

        // Save new organization
        case "Add":

            $query = "INSERT INTO srs_vol_status (description, incident_id, created_at) VALUES (?, ?, ?)";
            $values = array($volStatusName, $_SESSION['incident'], date('Y-m-d H:i:s'));

            // Execute query
            try {
                $rs = $global['db']->Execute($query, $values);
            } catch (exception $e) {
                error_log($e->getMessage());
            }
            break;

        // Update organization
        case "Save":

            $query = "UPDATE srs_vol_status SET description = ? WHERE id = ?";
            $values = array($volStatusName, $volStatusId);

            // Execute query
            try {
                $rs = $global['db']->Execute($query, $values);
            } catch (exception $e) {
                error_log($e->getMessage());
            }
            break;
    }

    $volStatusName = '';
    $volStatusId = null;
    shn_srs_volstatus_list();
}

function shn_srs_volstatus_del()
{
    global $global;

    $volStatusId = isset($_REQUEST['id']) ? trim($_REQUEST['id']) : null;

    // Delete organization

    $query = "DELETE FROM srs_vol_status WHERE id = ?";
    $values = array($volStatusId);

    // Execute query
    try {
        $rs = $global['db']->Execute($query, $values);
    } catch (exception $e) {
        error_log($e->getMessage());
    }

    shn_srs_volstatus_list();
}

function shn_srs_staff_type_list()
{
    // Get vol status list
    $staffTypes = getStaffTypes();
    $staffTypeName = '';

    // Set form properties
    $act = 'staff_type_save';
    $formOpts = array('enctype' => 'enctype="multipart/form-data"');
    $submitName = "Add";
    $submitOpts = 'name="submit" class="styleTheButton"';

    include_once 'templates/settings_staff_type_list.php';
}

function shn_srs_staff_type_edit()
{
    $staffTypeId = isset($_REQUEST['id']) ? $_REQUEST['id'] : null;

    // Get vol status list
    $staffTypes = getStaffTypes();

    $staffTypeName = getStaffTypeName($staffTypeId);

    // Set form properties
    $act = 'staff_type_save';
    $formOpts = array('enctype' => 'enctype="multipart/form-data"');
    $submitName = "Save";
    $submitOpts = 'name="submit" class="styleTheButton"';

    include_once 'templates/settings_staff_type_list.php';
}

function shn_srs_staff_type_save()
{

    global $global;

    $staffTypeDescription = isset($_REQUEST['org_name']) ? trim($_REQUEST['org_name']) : null;
    $staffTypeId = isset($_REQUEST['id']) ? trim($_REQUEST['id']) : null;
    $action = $_REQUEST['submit'];

    $foo = 'bar';

    switch ($action) {

        // Save new organization
        case "Add":

            $query = "INSERT INTO srs_staff_types (description, incident_id, created_at) VALUES (?, ?, ?)";
            $values = array($staffTypeDescription, $_SESSION['incident'], date('Y-m-d H:i:s'));

            // Execute query
            try {
                $rs = $global['db']->Execute($query, $values);
            } catch (exception $e) {
                error_log($e->getMessage());
            }
            break;

        // Update organization
        case "Save":

            $query = "UPDATE srs_staff_types SET description = ? WHERE id = ?";
            $values = array($staffTypeDescription, $staffTypeId);

            // Execute query
            try {
                $rs = $global['db']->Execute($query, $values);
            } catch (exception $e) {
                error_log($e->getMessage());
            }
            break;
    }

    $staffTypeDescription = '';
    $staffTypeId = null;
    shn_srs_staff_type_list();
}

function shn_srs_staff_type_del()
{
    global $global;

    $staffTypeId = isset($_REQUEST['id']) ? trim($_REQUEST['id']) : null;

    // Delete organization

    $query = "DELETE FROM srs_staff_types WHERE id = ?";
    $values = array($staffTypeId);

    // Execute query
    try {
        $rs = $global['db']->Execute($query, $values);
    } catch (exception $e) {
        error_log($e->getMessage());
    }

    shn_srs_staff_type_list();
}

function shn_srs_import_rmp_stafftypes()
{

    global $global, $conf;
    $db = $global['db'];
    $error = false;

    $eventId = $_SESSION['incident'];

    // Attempt to connect and retrieve Sahana Resource Management Planning (SRMP) data
    $content = @file_get_contents("{$conf['sems_url']}/webservices/getevent/{$conf['sems_token']}/$eventId/eventStaffTypes.json", true);
    if (strpos($http_response_header[0], "200")) {
        $data = json_decode($content, true);
    } else {
        $error = true;
        add_error("Unable to connect to the Sahana RMP server. Please check the system configuration.");
    }

    if (!$error) {

        foreach ($data as $row) {
            // First check to see if there is already a record for the value
            $q = "SELECT id FROM srs_staff_types WHERE description = ? AND incident_id = ?";
            $v = array($row['rt_staff_resource_type'], $eventId);
            $id = $db->GetOne($q, $v);

            if (empty($id)) {
                // Insert a record
                $q = "INSERT INTO srs_staff_types (description, incident_id, created_at)
                    VALUES (?, ?, ?)";
                $v[] = date('Y-m-d H:i:s');
                $db->Execute($q, $v);
                add_confirmation("Imported " . $v[0]);
            } else {
                add_information("<i>" . $v[0] . "</i> already present.");
            }
        }
    }

    shn_srs_staff_type_list();
}

?>