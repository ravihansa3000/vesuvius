<?php

$global['crs_group_menu'] = false;

function shn_crs_default()
{
    global $global;
    global $conf;

    $control = "";

    shn_tabmenu_open();
    shn_tabmenu_item("default", _t("Client-Menu|About"), "crs");
    shn_tabmenu_item("client_add", _t("Client-Menu|Add Person"), "crs");
    shn_tabmenu_item("default", _t("Client-Menu|Registry Search"), "crs");
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
    if(isset($_GET['client_add'])) {
        $control = "shn_admin_client_add";
    }

    if($control == "") {
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

    // Clear session uuid
    $_SESSION['uuid'] = null;

    // Get facilities
    $facilities = get_facilities();

    include_once 'templates/client_edit_form.php';
}
