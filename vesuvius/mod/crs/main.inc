<?php

$global['crs_group_menu'] = false;

require_once 'inc/security.inc';
require_once 'inc/functions.inc';
require_once 'inc/client_definition.inc';

require_once($global['approot'].'/inc/lib_xajax.inc');
require_once($global['approot'].'/mod/crs/xajax.inc');

shn_xajax_registerFunction('crs_search');

function shn_crs_default()
{
    global $global;
    global $conf;

    echo "<script type=\"text/javascript\" src=\"res/js/jquery-1.6.4.min.js\" ></script>
        <script type=\"text/javascript\" src=\"res/js/jquery-ui-autocomplete.min.js\" ></script>
		<script type=\"text/javascript\" src=\"res/js/jquery-ui-datepicker.min.js\" ></script>
		<script type=\"text/javascript\" >".file_get_contents($global['approot']."/mod/crs/crs.js")."</script>";

    $control = "";

    shn_tabmenu_open();
    shn_tabmenu_item("default", _t("Client-Menu|About"), "crs");
    shn_tabmenu_item("client_add", _t("Client-Menu|Add Person"), "crs");
    shn_tabmenu_item("search", _t("Client-Menu|Registry Search"), "crs");
    shn_tabmenu_item("default", _t("Client-Menu|Dashboard"), "crs");
    shn_tabmenu_item("default", _t("Client-Menu|Reports"), "crs");
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
            print("<pre>" . $e->getMessage() . "</pre>");
        }
        $events = $results->GetArray();

        // Default to the only open incident
        if (count($events) == 1) {
            $incident = $events[0]['incident_id'];
            $_SESSION['incident'] = $incident;
        }
    }
    if (isset($_GET['client_add'])) {
        $control = "shn_crs_client_add";
    } elseif(isset($_GET['client_save_new'])) {
        $control = "shn_crs_client_save_new";
    } elseif(isset($_GET['client_edit'])) {
        $control = "shn_crs_client_edit";
    } elseif(isset($_GET['client_save_edit'])) {
        $control = "shn_crs_client_save_edit";
    } elseif(isset($_GET['search'])) {
        $control = "shn_crs_search";
    } elseif(isset($_GET['client_view'])) {
        $control = "shn_crs_client_view";
    } elseif(isset($_GET['client_edit'])) {
        $control = "shn_crs_client_edit";
    } elseif(isset($_GET['client_addpet'])) {
        $control = "shn_crs_client_addpet";
    } elseif(isset($_GET['client_petsave_new'])) {
        $control = "shn_crs_client_addpet";
    } elseif(isset($_GET['client_listpets'])) {
        $control = "shn_crs_client_listpets";
    } elseif(isset($_GET['client_petdelete'])) {
        $control = "shn_crs_client_petdelete";
    } elseif(isset($_GET['client_petsave_edit'])) {
        $control = "shn_crs_client_petdelete";
    } elseif(isset($_GET['client_timeline'])) {
        $control = "shn_crs_client_timeline";
    } elseif(isset($_GET['client_checkout'])) {
        $control = "shn_crs_client_checkout";
    } elseif(isset($_GET['client_checkout_save'])) {
        $control = "shn_crs_client_checkout_save";
    } elseif(isset($_GET['client_checkin'])) {
        $control = "shn_crs_client_checkin";
    } elseif(isset($_GET['client_checkin_save'])) {
        $control = "shn_crs_client_checkin_save";
    } elseif(isset($_GET['client_transfer'])) {
        $control = "shn_crs_client_transfer";
    } elseif(isset($_GET['client_transfer_save'])) {
        $control = "shn_crs_client_transfer_save";
    } elseif(isset($_GET['client_delete'])) {
        $control = "shn_crs_client_delete";
    }

    if ($control == "") {
        include_once 'templates/default.php';
    }

    $control();

}

/**
 * Add new clients form
 *
 * @access public
 * @return void
 */
function shn_crs_client_add()
{
    // Validate group accessshn_crs_client_edit
    if (!check_groupAccess(array(1, 2))) {
        add_error("You do not have permission to add a client to the shelter registry.");
        return;
    }

    // Validate group access
    if (!check_validIncident()) {
        add_error("You must select an open event before attempting to add a client to the registry.");

        shn_crs_default();
        return;
    }

    global $global;

    // Set form properties
    $act = 'client_save_new';
    $formOpts = array('enctype' => 'enctype="multipart/form-data"');
    $submitOpts = 'name="submit" class="styleTheButton"';

    // Clear session uuid
    $_SESSION['uuid'] = null;

    // Get facilities
    $facilities = get_facilities();

    include_once 'templates/client_edit_form.php';
}

/**
 *  Save new clients action
 */
function shn_crs_client_save_new()
{
    include_once 'inc/client_save.inc';

    $mode = isset($_REQUEST['mode']) ? $_REQUEST['mode'] : null;
    $submit = isset($_REQUEST['submit']) ? $_REQUEST['submit'] : null;
    $error = false;

    $requiredFields = array(
        'checkInDate' => 'Check In Date',
        'entryHour' => 'Check in hour',
        'entryMinute' => 'Check in minute',
        'facility' => 'Assigned facility location',
        'family_name' => 'Last name',
        'given_name' => 'First name',
        'gender' => 'Gender'
    );

    foreach ($requiredFields as $key => $value) {

        $test = $_REQUEST[$key];

        if (!isset($_REQUEST[$key]) || empty($_REQUEST[$key])) {
            add_error($value . " not provided.");
            $error = true;
        }
    }

    if (!$error) {
        // Save the new client
        $uuid = client_save();

        // See if the user is entering group members
        if ($submit == 'Save + Add Group Member') {

            // Move to the add new client to group form
            //header("Location: " . $_SERVER['PHP_SELF'] . "?mod=crs&act=group_new_client&uuid=" . base64_encode($uuid));
            shn_crs_group_new_client($uuid);

            //shn_crs_group_new_client($uuid);
        } elseif ($submit == 'Save + Continue') {

            // Return to the client add form for next new client
            shn_crs_client_add();
        } else {
            // Return to the search page
            shn_crs_search();
        }
    } else {
        add_error("Unable to save new client do to errors.");
        shn_crs_client_add();
    }
}

function shn_crs_search()
{
    // Validate group access
    if (!check_groupAccess(array(1, 2))) {
        add_error("You do not have permission to search the shelter registry.");
        return;
    }

    // Validate group access
    if (!check_validIncident()) {
        add_error("You must select an open event before attempting to perform this action.");

        shn_crs_default();
        return;
    }

    global $global;

    // Get facilities
    $facilities = get_facilities();

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
        add_error("<pre>" . $e->getMessage() . "</pre>");
    }

    $minAge = (isset($_SESSION['minAge'])) ? trim($_SESSION['minAge']) : '';
    $maxAge = (isset($_SESSION['maxAge'])) ? trim($_SESSION['maxAge']) : '';
    include_once 'templates/client_search.php';
}

/**
 * View client details function
 *
 * @access public
 * @return void
 */
function shn_crs_client_view()
{

    // Get the UUID
    $uuid = (isset($_SESSION['uuid'])) ? $_SESSION['uuid'] : null;
    $uuid = (isset($_REQUEST['uuid'])) ? $_REQUEST['uuid'] : $uuid;
    //$uuid = base64_decode($uuid);
    //$_SESSION['uuid'] = base64_encode($uuid);
    $_SESSION['uuid'] = $uuid;

    // Get client data
    $client = get_client($uuid);

    // Get client skills
    $skillsList = get_client_skills($uuid);

    include_once 'templates/client_view.php';
}

/**
 * Edit clients function
 *
 * @access public
 * @return void
 */
function shn_crs_client_edit()
{
    // Validate group access
    if (!check_groupAccess(array(1, 2))) {
        add_error("You do not have permission to add a client to the shelter registry.");
        return;
    }

    // Validate group access
    if (!check_validIncident()) {
        add_error("You must select an open event before attempting to add a client to the registry.");

        shn_crs_default();
        return;
    }

    include_once 'inc/functions.inc';

    global $global;

    // Set form properties
    $act = 'client_save_edit';
    $formOpts = array('enctype' => 'enctype="multipart/form-data"');
    $submitOpts = 'name="submit" class="styleTheButton"';

    // Get facilities
    $facilities = get_facilities();

    // Get the UUID
    $uuid = (isset($_SESSION['uuid'])) ? $_SESSION['uuid'] : null;
    $uuid = (isset($_REQUEST['uuid'])) ? $_REQUEST['uuid'] : $uuid;
    $uuid = $uuid;
    $_SESSION['uuid'] = $uuid;

    // Get facilities
    $facilities = get_facilities();

    // Get client data
    $client = get_client($uuid);

    // Load phone numbers as arrays
    $homePhone = isset($client["home_phone"]) ? parse_phone($client["home_phone"]) : '';
    $mobilePhone = isset($client["mobile_phone"]) ? parse_phone($client["mobile_phone"]) : '';
    $altPhone = isset($client["alt_phone"]) ? parse_phone($client["alt_phone"]) : '';
    $ecPhone = isset($client["ec_phone"]) ? parse_phone($client["ec_phone"]) : '';

    // Get client's skills
    $skillsList = get_client_skills($uuid);

    // Set form mode
    $global['crs_form_mode'] = 'edit';
    $global['crs_group_menu'] = true;

    include_once 'templates/client_edit_form.php';
}

function shn_crs_client_addpet()
{

    // Validate group access
    if (!check_groupAccess(array(1, 2))) {
        add_error("You do not have permission to check in a client to the shelter registry.");
        return;
    }

    // Validate group access
    if (!check_validIncident()) {
        add_error("You must select an open event before attempting to perform this action.");

        shn_crs_default();
        return;
    }

    global $global;

    // Get the UUID
    $uuid = (isset($_SESSION['uuid'])) ? $_SESSION['uuid'] : null;
    $uuid = (isset($_REQUEST['uuid'])) ? $_REQUEST['uuid'] : $uuid;
    $uuid = $uuid;
    $_SESSION['uuid'] = $uuid;

    $petId = isset($_REQUEST['pet_id']) ? $_REQUEST['pet_id'] : null;

    if (isset($petId)) {

        // Get the pet details
        $query = "SELECT * FROM crs_pets WHERE id = ?";
        $result = $global['db']->Execute($query, $petId);
        $pet = $result->FetchRow();

        $act = 'client_petsave_edit';
    } else {
        $act = 'client_petsave_new';
    }

    // Get client data
    $client = get_client($uuid);

    // Set form properties

    $formOpts = array('enctype' => 'enctype="multipart/form-data"');
    $submitOpts = 'name="submit" class=""';

    // Load form
    include_once 'templates/client_addpet.php';
}

function shn_crs_client_petsave_new()
{
    // Validate group access
    if (!check_groupAccess(array(1, 2))) {
        add_error("You do not have permission to check in a client to the shelter registry.");
        return;
    }

    global $global;

    // Get the UUID
    $uuid = (isset($_SESSION['uuid'])) ? $_SESSION['uuid'] : null;
    $uuid = (isset($_REQUEST['uuid'])) ? $_REQUEST['uuid'] : $uuid;
    $uuid = $uuid;
    $_SESSION['uuid'] = $uuid;

    // Get client data
    $client = get_client($uuid);

    $petName = isset($_REQUEST['pet_name']) ? $_REQUEST['pet_name'] : null;
    $petType = isset($_REQUEST['pet_type']) ? $_REQUEST['pet_type'] : null;
    $petGender = isset($_REQUEST['pet_gender']) ? $_REQUEST['pet_gender'] : null;
    $petAge = isset($_REQUEST['pet_age']) ? $_REQUEST['pet_age'] : null;
    $petBreed = isset($_REQUEST['pet_breed']) ? $_REQUEST['pet_breed'] : null;
    $petColor = isset($_REQUEST['pet_color']) ? $_REQUEST['pet_color'] : null;

    // Open a database transaction
    $global['db']->StartTrans();

    $table = 'crs_pets';
    $record = array();
    $record['p_uuid'] = $uuid;
    $record['pet_name'] = $petName;
    $record['type_of_pet'] = $petType;
    $record['sex'] = $petGender;
    $record['age'] = $petAge;
    $record['breed'] = $petBreed;
    $record['color'] = $petColor;
    $record['created_at'] = date("Y-m-d H:i:s");
    $global['db']->AutoExecute($table, $record, 'INSERT');

    // Check for any failed transactions
    $error = $global['db']->HasFailedTrans();

    // Close transaction
    $global['db']->CompleteTrans();

    if ($error) {

        add_error("There was an error saving the pet to <b>" . $client['full_name'] . "</b>.");
    } else {

        add_confirmation("You have successfully saved $petName to <b>{$client['given_name']}'s</b> registry informaiton.");
    }
}

function shn_crs_client_listpets()
{
    // Validate group access
    if (!check_groupAccess(array(1, 2))) {
        add_error("You do not have permission to check in a client to the shelter registry.");
        return;
    }

    global $global;

    // Get the UUID
    $uuid = (isset($_SESSION['uuid'])) ? $_SESSION['uuid'] : null;
    $uuid = (isset($_REQUEST['uuid'])) ? $_REQUEST['uuid'] : $uuid;
    $uuid = $uuid;
    $_SESSION['uuid'] = $uuid;

    // Get client data
    $client = get_client($uuid);

    // Get pets
    $pets = getPets($uuid);

    // Show list
    include_once 'templates/client_listpets.php';
}


function shn_crs_client_petdelete()
{
    // Validate group access
    if (!check_groupAccess(array(1, 2))) {
        add_error("You do not have permission to check in a client to the shelter registry.");
        return;
    }

    global $global;

    // Get the UUID
    $uuid = (isset($_SESSION['uuid'])) ? $_SESSION['uuid'] : null;
    $uuid = (isset($_REQUEST['uuid'])) ? $_REQUEST['uuid'] : $uuid;
    $uuid = $uuid;
    $_SESSION['uuid'] = $uuid;

    $petId = isset($_REQUEST['pet_id']) ? $_REQUEST['pet_id'] : null;

    // Get the pet details
    $query = "SELECT * FROM crs_pets WHERE id = ?";
    $result = $global['db']->Execute($query, $petId);
    $pet = $result->FetchRow();

    // Get client data
    $client = get_client($uuid);

    // Open a database transaction
    $global['db']->StartTrans();

    // Setup query
    $query = "DELETE FROM crs_pets WHERE id = ?";
    $global['db']->Execute($query, $petId);

    // Check for any failed transactions
    $error = $global['db']->HasFailedTrans();

    // Close transaction
    $global['db']->CompleteTrans();

    if ($error) {

        add_error("There was an error deleting the pet.");
    } else {

        add_confirmation("You have successfully deleted {$pet['pet_name']} from <b>{$client['given_name']}'s</b> registry informaiton.");
    }

    // list the pets
    shn_crs_client_listpets();
}

function shn_crs_client_petsave_edit()
{
    // Validate group access
    if (!check_groupAccess(array(1, 2))) {
        add_error("You do not have permission to check in a client to the shelter registry.");
        return;
    }

    global $global;

    // Get the UUID
    $uuid = (isset($_SESSION['uuid'])) ? $_SESSION['uuid'] : null;
    $uuid = (isset($_REQUEST['uuid'])) ? $_REQUEST['uuid'] : $uuid;
    $uuid = $uuid;
    $_SESSION['uuid'] = $uuid;

    $petId = isset($_REQUEST['pet_id']) ? shn_db_clean($_REQUEST['pet_id']) : null;

    // Get client data
    $client = get_client($uuid);

    $petName = isset($_REQUEST['pet_name']) ? $_REQUEST['pet_name'] : null;
    $petType = isset($_REQUEST['pet_type']) ? $_REQUEST['pet_type'] : null;
    $petGender = isset($_REQUEST['pet_gender']) ? $_REQUEST['pet_gender'] : null;
    $petAge = isset($_REQUEST['pet_age']) ? $_REQUEST['pet_age'] : null;
    $petBreed = isset($_REQUEST['pet_breed']) ? $_REQUEST['pet_breed'] : null;
    $petColor = isset($_REQUEST['pet_color']) ? $_REQUEST['pet_color'] : null;

    // Open a database transaction
    $global['db']->StartTrans();

    $table = 'crs_pets';
    $record = array();
    $record['pet_name'] = $petName;
    $record['type_of_pet'] = $petType;
    $record['sex'] = $petGender;
    $record['age'] = $petAge;
    $record['breed'] = $petBreed;
    $record['color'] = $petColor;
    $global['db']->AutoExecute($table, $record, 'UPDATE', "id = $petId");

    // Check for any failed transactions
    $error = $global['db']->HasFailedTrans();

    // Close transaction
    $global['db']->CompleteTrans();

    if ($error) {

        add_error("There was an error saving the pet to <b>" . $client['full_name'] . "</b>.");
    } else {

        add_confirmation("You have successfully saved $petName to <b>{$client['full_name']}'s</b> registry information.");
    }

    shn_crs_client_listpets();
}

function shn_crs_client_timeline()
{

    global $global;

    // Get the UUID
    $uuid = (isset($_SESSION['uuid'])) ? $_SESSION['uuid'] : null;
    $uuid = (isset($_REQUEST['uuid'])) ? $_REQUEST['uuid'] : $uuid;
    $uuid = $uuid;
    $_SESSION['uuid'] = $uuid;

    // Get client data
    $client = get_client($uuid);

    $sql = "SELECT
            f1.facility_name,
            ptf.in_date,
            ptf.out_date,
            ptt.dest_facility,
            f3.facility_name AS dest_name,
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
        add_error("<pre>" . $e->getMessage() . "</pre>");
    }


    include_once 'templates/client_timeline.php';
}

function shn_crs_client_checkout()
{

    // Get the UUID
    $uuid = (isset($_SESSION['uuid'])) ? $_SESSION['uuid'] : null;
    $uuid = (isset($_REQUEST['uuid'])) ? $_REQUEST['uuid'] : $uuid;
    $uuid = $uuid;
    $_SESSION['uuid'] = $uuid;

    // Get client data
    $client = get_client($uuid);

    // Set form properties
    $act = 'client_checkout_save';
    $formOpts = array('enctype' => 'enctype="multipart/form-data"');
    $submitOpts = 'name="submit" class=""';

    include_once 'templates/client_checkout.php';
}

function shn_crs_client_checkout_save()
{
    global $global;

    // Get the UUID
    $uuid = (isset($_SESSION['uuid'])) ? $_SESSION['uuid'] : null;
    $uuid = (isset($_REQUEST['uuid'])) ? $_REQUEST['uuid'] : $uuid;
    $uuid = $uuid;
    $_SESSION['uuid'] = $uuid;

    // exit date
    isset($_REQUEST['checkOutDate']) ? $outDate = $_REQUEST['checkOutDate'] : $outDate = null;
    isset($_REQUEST['exitHour']) ? $exitHour = $_REQUEST['exitHour'] : $exitHour = null;
    isset($_REQUEST['exitMinute']) ? $exitMinute = $_REQUEST['exitMinute'] : $exitMinute = null;

    // Get client data
    $client = get_client($uuid);

    // Open a database transaction
    $global['db']->StartTrans();

    // Format the exit date in a SQL ASNI format
    $outDate = "$outDate $exitHour:$exitMinute:00";

    // Lookup client facility
    $query = "SELECT MAX(in_date) AS in_date FROM fms_person_to_facility WHERE p_uuid = ? ";
    $sql = $global['db']->Prepare($query);

    // get the member's current facility
    $result = $global['db']->Execute($sql, $uuid);
    $ptf = $result->FetchRow();
    $whereStr = "in_date = " . shn_db_clean($ptf['in_date']);

    // Update the table
    $table = 'fms_person_to_facility';
    $record = array();
    $record['out_date'] = $outDate;
    $global['db']->AutoExecute($table, $record, 'UPDATE', $whereStr);

    // Set person status
    setPersonStatus('out', $uuid);

    // Check for any failed transactions
    $error = $global['db']->HasFailedTrans();

    // Close transaction
    $global['db']->CompleteTrans();

    if ($error) {

        add_error("There was an error processing <strong>" . $client['given_name']. $client['family_name'] . "</strong> checkout.");
    } else {

        add_confirmation("You have successfully checked out <strong>" .
        $client['given_name']. $client['family_name'] . "</strong> from the " .
        $client['facility_name'] . " shelter registry.");
    }

    // Get client data again
    $client = get_client($uuid);

    // Get client skills
    $skillsList = get_client_skills($uuid);

    include_once 'templates/client_view.php';
}

function shn_crs_client_checkin()
{
    // Validate group access
    if (!check_groupAccess(array(1, 2))) {
        add_error("You do not have permission to check in a client to the shelter registry.");
        return;
    }

    // Validate group access
    if (!check_validIncident()) {
        add_error("You must select an open event before attempting to perform this action.");

        shn_crs_default();
        return;
    }

    // Get the UUID
    $uuid = (isset($_SESSION['uuid'])) ? $_SESSION['uuid'] : null;
    $uuid = (isset($_REQUEST['uuid'])) ? $_REQUEST['uuid'] : $uuid;
    $uuid = $uuid;
    $_SESSION['uuid'] = $uuid;

    // Get client data
    $client = get_client($uuid);

    // Get facilities
    $facilities = get_facilities();

    // Set form properties
    $act = 'client_checkin_save';
    $formOpts = array('enctype' => 'enctype="multipart/form-data"');
    $submitOpts = 'name="submit" class=""';

    include_once 'templates/client_checkin.php';
}

function shn_crs_client_checkin_save()
{
    global $global;

    // Get the UUID
    $uuid = (isset($_SESSION['uuid'])) ? $_SESSION['uuid'] : null;
    $uuid = (isset($_REQUEST['uuid'])) ? $_REQUEST['uuid'] : $uuid;
    $uuid = $uuid;
    $_SESSION['uuid'] = $uuid;

    // entry date
    isset($_REQUEST['checkInDate']) ? $inDate = $_REQUEST['checkInDate'] : $inDate = null;
    isset($_REQUEST['entryHour']) ? $entryHour = $_REQUEST['entryHour'] : $entryHour = null;
    isset($_REQUEST['entryMinute']) ? $entryMinute = $_REQUEST['entryMinute'] : $entryMinute = null;

    // Facility
    isset($_REQUEST['facility']) ? $facility = $_REQUEST['facility'] : $facility = null;

    // Get client data
    $client = get_client($uuid);

    // Open a database transaction
    $global['db']->StartTrans();

    // Format the entry date in a SQL ASNI format
    $inDate = "$inDate $entryHour:$entryMinute:00";

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

    if ($error) {

        add_error("There was an error processing <strong>" . $client['given_name']." ".$client['family_name'] . "</strong> checkout.");
    } else {

        add_confirmation("You have successfully checked in <strong>" .
        $client['given_name']." ".$client['family_name'] . "</strong> to the " . $client['facility_name']
        . " shelter facility.");
    }

    // Get client data
    $client = get_client($uuid);

    // Get client skills
    $skillsList = get_client_skills($uuid);

    include_once 'templates/client_view.php';
}

function shn_crs_client_transfer()
{

    // Get the UUID
    $uuid = (isset($_SESSION['uuid'])) ? $_SESSION['uuid'] : null;
    $uuid = (isset($_REQUEST['uuid'])) ? $_REQUEST['uuid'] : $uuid;
    $uuid = $uuid;
    $_SESSION['uuid'] = $uuid;

    // Get client data
    $client = get_client($uuid);

    // Get facilities
    $facilities = get_facilities();

    // Set form properties
    $act = 'client_transfer_save';
    $formOpts = array('enctype' => 'enctype="multipart/form-data"');
    $submitOpts = 'name="submit" class=""';

    include_once 'templates/client_transfer.php';
}

function shn_crs_client_transfer_save()
{
    global $global;

    // Get the UUID
    $uuid = (isset($_SESSION['uuid'])) ? $_SESSION['uuid'] : null;
    $uuid = (isset($_REQUEST['uuid'])) ? $_REQUEST['uuid'] : $uuid;
    $uuid = $uuid;
    $_SESSION['uuid'] = $uuid;

    // exit date
    isset($_REQUEST['checkOutDate']) ? $outDate = $_REQUEST['checkOutDate'] : $outDate = null;
    isset($_REQUEST['exitHour']) ? $exitHour = $_REQUEST['exitHour'] : $exitHour = null;
    isset($_REQUEST['exitMinute']) ? $exitMinute = $_REQUEST['exitMinute'] : $exitMinute = null;

    // Destination Facility
    isset($_REQUEST['destFacility']) ? $destFacility = $_REQUEST['destFacility'] : $destFacility = null;

    // Get client data
    $client = get_client($uuid);

    // Open a database transaction
    $global['db']->StartTrans();

    // Format the exit date in a SQL ASNI format
    $outDate = "$outDate $exitHour:$exitMinute:00";

    // Create transfer record
    $query = "INSERT INTO fms_person_to_transfer (user, p_uuid, src_facility, dest_facility, created_at)
        VALUES (?, ?, ?, ?, CURRENT_TIMESTAMP)";
    $values = array(
        $_SESSION['logged_in'],
        $uuid,
        $client['facility_uuid'],
        $destFacility
    );
    $global['db']->Execute($query, $values);


    $transferId = $global['db']->Insert_ID();

    // Lookup person's current facility
    $query = "SELECT MAX(in_date) AS in_date FROM fms_person_to_facility WHERE p_uuid = ? ";
    $sql = $global['db']->Prepare($query);

    // get the member's current facility
    $result = $global['db']->Execute($sql, $uuid);
    $ptf = $result->FetchRow();
    $whereStr = "in_date = " . shn_db_clean($ptf['in_date']);

    // Checkout and mark the transfer flag
    $table = 'fms_person_to_facility';
    $record = array();
    $record['out_date'] = $outDate;
    $record['transfer_id'] = $transferId;
    $global['db']->AutoExecute($table, $record, 'UPDATE', $whereStr);

    // Set person status
    setPersonStatus('trn', $uuid);

    // Check for any failed transactions
    $error = $global['db']->HasFailedTrans();

    // Close transaction
    $global['db']->CompleteTrans();

    if ($error) {

        add_error("There was an error processing <strong>" . $client['given_name']." ".$client['family_name'] . "</strong> checkout.");
    } else {

        add_confirmation("You have successfully transfered <strong>" .
        $client['given_name']." ".$client['family_name'] . "</strong> from the " .
        $client['facility_name'] . " shelter registry.");
    }

    // Get client data again
    $client = get_client($uuid);

    // Get client skills
    $skillsList = get_client_skills($uuid);

    include_once 'templates/client_view.php';
}

/**
 *  Save existing client updates
 */
function shn_crs_client_save_edit()
{
    include_once 'inc/client_save.inc';

    $mode = isset($_REQUEST['mode']) ? $_REQUEST['mode'] : null;
    $submit = isset($_REQUEST['submit']) ? $_REQUEST['submit'] : null;

    // Get the client uuid
    isset($_REQUEST['uuid']) ? $uuid = $_REQUEST['uuid'] : $uuid = null;

    if (isset($uuid)) {
        client_save($uuid);

        // See if the user is entering group members
        if ($submit == 'Save + Add Group Member') {

            // Move to the add new client to group form
            shn_crs_group_new_client($uuid);

            //shn_crs_group_new_client($uuid);
        } elseif ($submit == 'Save + Continue') {

            // Return to the client add form for next new client
            shn_crs_client_add();
        } else {
            shn_crs_client_edit();
        }
    }
}

/**
 * Delete a client
 *
 * @global array $global
 */
function shn_crs_client_delete()
{

    global $global;

    $uuid = $_REQUEST['uuid'];

    // Set the deleted bit
    $sql[] = "UPDATE person_status SET isdeleted = 1 WHERE p_uuid = ?;";

    // Remove anyone from this person's group
    $sql[] = "UPDATE crs_person_to_group SET p_uuid_2 = NULL WHERE p_uuid_2 = ? ";

    // Delete any pets associated with this person
    $sql[] = "DELETE FROM crs_pets WHERE p_uuid = ?";

    // Open transaction
    $global['db']->StartTrans();

    foreach ($sql as $q) {
        // Delete person
        try {
            $q = $global['db']->Prepare($q);
            $results = $global['db']->Execute($q, $uuid);
        } catch (exception $e) {
            add_error("<pre>" . $e->getMessage() . "</pre>");
        }
    }

    // Close transaction
    $global['db']->CompleteTrans();

    // Return to the default STS page
    shn_crs_search();
}