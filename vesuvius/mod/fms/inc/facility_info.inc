<?php
/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */


// Get facility statuses
$statuses = array();
$sql = "SELECT DISTINCT facility_allocation_status FROM fms_facility ORDER BY facility_allocation_status ASC;";

try {
$sql = $global['db']->Prepare($sql);
$rs = $global['db']->Execute($sql);
} catch (exception $e) {
add_error("<pre>" . $e->getMessage() . "</pre>");
}

foreach ($rs as $row) {
array_push($statuses, $row['facility_allocation_status']);
}

// Get facility types
$types = array();
$sql = "SELECT DISTINCT facility_resource_type_abbr FROM fms_facility ORDER BY facility_resource_type_abbr ASC;";

try {
$sql = $global['db']->Prepare($sql);
$rs = $global['db']->Execute($sql);
} catch (exception $e) {
add_error("<pre>" . $e->getMessage() . "</pre>");
}

foreach ($rs as $row) {
array_push($types, $row['facility_resource_type_abbr']);
}

// Get facility groups
$groups = array();
$sql = "SELECT DISTINCT facility_group FROM fms_facility ORDER BY facility_group ASC;";

try {
$sql = $global['db']->Prepare($sql);
$rs = $global['db']->Execute($sql);
} catch (exception $e) {
add_error("<pre>" . $e->getMessage() . "</pre>");
}

foreach ($rs as $row) {
array_push($groups, $row['facility_group']);
}

?>