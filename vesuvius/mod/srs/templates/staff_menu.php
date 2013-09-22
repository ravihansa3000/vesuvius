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
global $global;

// Only dispaly these staff menue tab items for existing uuid
if (isset($uuid)) {
    shn_tabmenu_open();
    shn_tabmenu_item("staff_view", "View", "srs");
    shn_tabmenu_item("staff_edit", "Edit", "srs");
    shn_tabmenu_item("staff_timeline", "Timeline", "srs");

    if ($staff['opt_status'] == "in") {
        // See if the staff is checked in
        shn_tabmenu_item("staff_checkout", "Sign Out", "srs");
        shn_tabmenu_item("staff_transfer", "Transfer", "srs");
    } else {
        shn_tabmenu_item("staff_checkin", "Sign In", "srs");
    }

    shn_tabmenu_close();
}
?>
