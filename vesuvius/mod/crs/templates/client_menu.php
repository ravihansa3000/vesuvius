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
global $global;

// Only dispaly these client menue tab items for existing uuid
if (isset($uuid)) {
    shn_tabmenu_open();
    shn_tabmenu_item("client_view", "View", "crs");
    shn_tabmenu_item("client_edit", "Edit", "crs");
    shn_tabmenu_item("client_listpets", "List Pets", "crs");
    shn_tabmenu_item("client_addpet", "Add Pet", "crs");
    shn_tabmenu_item("client_timeline", "Timeline", "crs");

    if (isset($client['group_primary']) && $client['group_primary'] == NULL) {
        // Primary client
        if ($client['opt_status'] == "in") {
            // See if the client is checked in
            shn_tabmenu_item("group_new_client", "Add Members", "crs");
        }

        if ($client['group_count'] > 1) {
            // Group head
            shn_tabmenu_item("group_list", "List Group", "crs");

            if ($client['opt_status'] == "in") {
                // See if the client is checked in
                shn_tabmenu_item("client_checkout", "Check Out", "crs");
                shn_tabmenu_item("group_checkout", "Check Out Group", "crs");
                shn_tabmenu_item("group_transfer", "Transfer Group", "crs");
            } else {
                shn_tabmenu_item("client_checkin", "Check In", "crs");
            }
        } else {
            // Individual

            if ($client['opt_status'] == "in") {
                // See if the client is checked in
                shn_tabmenu_item("group_join", "Move to Group", "crs");
                shn_tabmenu_item("client_checkout", "Check Out", "crs");
                shn_tabmenu_item("client_transfer", "Transfer", "crs");
            } else {
                shn_tabmenu_item("client_checkin", "Check In", "crs");
            }
        }
    } elseif ($client['group_primary'] != NULL) {
        // Group member
        shn_tabmenu_item("group_remove_client", "Remove From Group", "crs");
        if ($client['opt_status'] == "in") {
            // See if the client is checked in
            shn_tabmenu_item("client_checkout", "Check Out", "crs");
            shn_tabmenu_item("client_transfer", "Transfer", "crs");
        } else {
            shn_tabmenu_item("client_checkin", "Check In", "crs");
        }
    }

    shn_tabmenu_close();
}
?>
