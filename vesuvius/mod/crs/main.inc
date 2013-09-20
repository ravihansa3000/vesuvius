<?php

$global['crs_group_menu'] = false;

require_once 'inc/security.inc';
require_once 'inc/functions.inc';
require_once 'inc/client_definition.inc';

require_once($global['approot'].'/inc/lib_xajax.inc');
require_once($global['approot'].'/mod/crs/xajax.inc');

shn_xajax_registerFunction('crs_test');

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
    } elseif(isset($_GET['search'])) {
        $control = "shn_crs_search";
    } elseif(isset($_GET['client_view'])) {
        $control = "shn_crs_client_view";
    } elseif(isset($_GET['client_edit'])) {
        $control = "shn_crs_client_edit";
    } elseif(isset($_GET['client_addpet'])) {
        $control = "shn_crs_client_addpet";
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

    global $global;

    // Set form properties
    $act = 'client_save_new';
    $formOpts = array('enctype' => 'enctype="multipart/form-data"');
    $submitOpts = 'name="submit" class="styleTheButton"';

    // Get facilities
    $facilities = get_facilities();

    // Clear session uuid
    $_SESSION['uuid'] = null;

    echo "<h1>New Shelter Client Form</h1>";

    shn_form_fopen("client_save_new", null, array('enctype' => 'enctype="multipart/form-data"', 'req_message' => true));
    shn_form_hidden(array('seq' => 'entry'));

    shn_form_fsopen(_t('Check In Information'));
    shn_form_date(_t("Date"), "checkInDate", array('req'=>true));
    shn_form_time(_t("Time"), "entryHour", "entryMinute", array('req'=>true));

    $opt_status = array();
    $opt_status["select"] = "- Select -";

    foreach ($facilities as $facility) {
        $opt_status[$facility['facility_uuid']] = $facility['facility_name'];
    }

    shn_form_select($opt_status, _t('Facility Location'), "facility", null,  array('value'=>'select', 'req' => true));
    shn_form_fsclose();

    shn_form_fsopen(_t('Group Representative or Individual Information'));

    shn_form_text(_t("First Name"),'given_name','size="20"',array('req'=>true, 'help'=>'Given Name'));
    shn_form_text(_t("Last Name"),'family_name','size="20"',array('req'=>true, 'help'=>'Family Name'));
    shn_form_text(_t("MI"),'middle_initial','size="5"',array('help'=>'Middle Initials'));
    shn_form_text(_t("Age"),'years_old','size="5"',array('help'=>'Age'));
    shn_form_date(_t("Date of Birth"), "dob");
    $opt_status = array("" => "- Select -", "Male" => "Male", "Female" => "Female");
    shn_form_select($opt_status, _t('Gender'), "gender", null,  array('value'=>'select', 'req' => true));

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
    shn_form_text(_t("Home Phone"),'home','size="10"');
    shn_form_text(_t("Mobile Phone"),'mobile','size="10"');
    shn_form_text(_t("Alternate Phone"),'alter','size="10"');
    shn_form_fsclose();

    shn_form_fsopen(_t('Demographic Information'));

    shn_form_text(_t("Occupation"),'occupation','size="20"');
    shn_form_text(_t("Special Skills"),'skills','size="20"',array('help'=>'Type in comma separated.'));
    shn_form_text(_t("Languages Spoken"),'altlang1','size="10"',array('help'=>'1st Language'));
    shn_form_text("",'altlang2','size="10"',array('help'=>'2nd Language'));
    shn_form_text("",'altlang3','size="10"',array('help'=>'3rd Language'));

    shn_form_fsclose();

    shn_form_fsopen(_t('Emergency Contact'));

    shn_form_text(_t("Name"),'ec_name','size="20"');
    shn_form_text(_t("Relationship"),'ec_relation','size="20"');
    shn_form_text(_t("Phone Number"),'ec_phone','size="10"',array('help'=>'In ### - ### - #### format.'));

    shn_form_fsclose();

    shn_form_fsopen(_t('Medical Alerts'));

    shn_form_checkbox(_t("Special Medical Need"),'special_medical','size="20"');

    shn_form_fsclose();

    shn_form_submit("Save", "class=\"styleTehButton\""); echo '&nbsp&nbsp';
    shn_form_submit("Save + Continue", "class=\"styleTehButton\""); echo '&nbsp&nbsp';
    shn_form_submit("Save + Add Group Member", "class=\"styleTehButton\"");
    echo "<script>initDate();</script>";
    shn_form_fclose();
    echo "<br><br>";
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
    $uuid = base64_decode($uuid);
    $_SESSION['uuid'] = base64_encode($uuid);

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
