<?php

/**
 * Facility Management Module
 *
 * PHP version >=5
 *
 * LICENSE: This source file is subject to LGPL license
 * that is available through the world-wide-web at the following URI:
 * http://www.gnu.org/copyleft/lesser.html
 *
 * @author     Clayton Kramer <clayton.kramer@mail.cuny.edu>
 * @package    module STS
 * @license    http://www.gnu.org/copyleft/lesser.html GNU Lesser General Public License (LGPL)
 *
 */
global $global;


$expr_array = array();

// get the search string
if (isset($_REQUEST['query'])) {

    $search_str = trim($_REQUEST['query']);
    $_SESSION['fms']['fq'] = $search_str;
} elseif (isset($_SESSION['fms']['query'])) {

    $search_str = trim($_SESSION['fms']['query']);
} else {

    $search_str = null;
    $_SESSION['fms']['query'] = null;
}


// get the page number
if (isset($_REQUEST['page'])) {
    $page = trim($_REQUEST['page']);
    $_SESSION['fms']['page'] = $page;
} elseif (isset($_SESSION['fms']['page'])) {
    $page = trim($_SESSION['fms']['page']);
} else {
    $page = null;
    $_SESSION['fms']['page'] = null;
}

// get how many rows we want to have into the grid - rowNum parameter in the grid
if (isset($_REQUEST['limit'])) {
    $limit = trim($_REQUEST['limit']);
    $_SESSION['fms']['limit'] = $limit;
} elseif (isset($_SESSION['fms']['limit'])) {
    $limit = trim($_SESSION['fms']['limit']);
} else {
    $limit = 10;
    $_SESSION['fms']['limit'] = 10;
};

// get index row - i.e. user click to sort. At first time sortname parameter -
// after that the index from colModel
if (isset($_REQUEST['sidx'])) {
    $sidx = trim($_REQUEST['sidx']);
    $_SESSION['fms']['sidx'] = $sidx;
} elseif (isset($_SESSION['fms']['sidx'])) {
    $sidx = trim($_SESSION['fms']['sidx']);
} else {
    $sidx = null;
    $_SESSION['fms']['sidx'] = 'facility_name';
};

// sorting order - at first time sortorder
if (isset($_REQUEST['sord'])) {
    $sord = trim($_REQUEST['sord']);
    $_SESSION['fms']['sord'] = $sord;
} elseif (isset($_SESSION['fms']['sord'])) {
    $sord = trim($_SESSION['fms']['sord']);
} else {
    $sord = null;
    $_SESSION['fms']['sord'] = null;
};

// Facility Group search filter
if (isset($_REQUEST['facility_group'])) {

    $facility_group = trim($_REQUEST['facility_group']);
    $_SESSION['fms']['facility_group'] = $facility_group;
} elseif (isset($_SESSION['fms']['facility_group'])) {

    $facility_group = $_SESSION['fms']['facility_group'];
} else {

    $facility_group = null;
    $_SESSION['fms']['facility_group'] = null;
}

// Build search expressions
$where_clause = "WHERE (f.facility_name LIKE CONCAT('%',?,'%') 
  OR f.facility_code LIKE CONCAT('%',?,'%')
  OR f.facility_group LIKE CONCAT('%',?,'%')
  OR f.city LIKE CONCAT('%',?,'%') 
  OR f.borough LIKE CONCAT('%',?,'%')
  OR f.postal_code LIKE CONCAT('%',?,'%')
  OR f.work_phone LIKE CONCAT('%',?,'%'))
  AND fti.incident_id = ?";

// @todo This should be made into a function that builds the where clause 
foreach (range(1, 7) as $v) {
    $values[$v] = $search_str;
}
$values[] = $_SESSION['incident'];

$query = "SELECT COUNT(*) AS count FROM fms_facility AS f 
    INNER JOIN fms_facility_to_event fti ON f.facility_uuid = fti.facility_uuid
    $where_clause;";

// Execute query
try {
    $result = $global['db']->Execute($query, $values);
} catch (exception $e) {
    print("<pre>" . $e->getMessage() . "</pre>");
}

$count = $result->Fields('count');

// calculate the total pages for the query
if ($count > 0 && $limit > 0) {
    $total_pages = ceil($count / $limit);
} else {
    $total_pages = 0;
}

// if for some reasons the requested page is greater than the total
// set the requested page to total page
if ($page > $total_pages) {
    $page = $total_pages;
}

// calculate the starting position of the rows
$start = $limit * $page - $limit;

// if for some reasons start position is negative set it to 0
// typical case is that the user type 0 for the requested page
if ($start < 0) {
    $start = 0;
}

// Now the actual query
$query = "SELECT * FROM fms_facility AS f 
    INNER JOIN fms_facility_to_event fti ON f.facility_uuid = fti.facility_uuid
    $where_clause";

// Sort ordering
if ($sidx) {
    $query .= "ORDER BY $sidx $sord ";
}

// Pagination
if ($start && $limit) {
    $query .= "LIMIT $start , $limit";
} else {
    $query .= "LIMIT $limit";
}

// Execute query
try {
    $query = $global['db']->Prepare($query);
    $results = $global['db']->CacheExecute(15, $query, $values);
} catch (exception $e) {
    add_error("<pre>" . $e->getMessage() . "</pre>");
    error_log(":Sahana RP JSON: $e\n\n");
}

// Create JSON responce object
$response->page = $page;
$response->records = $count;
$response->total = $total_pages;
$response->limit = $limit;
$response->sidx = $sidx;
$response->sord = $sord;
$response->colnames = array('Facility', 'Contact', 'Type', 'Group', 'Location');

$i = 0;
foreach ($results as $row) {

    // Add content to the cells
    $cell1 = "<a href=\"index.php?mod=fms&amp;act=edit&amp;id=" . base64_encode($row['facility_uuid']) . "\">";
    $cell1 .= $row['facility_name'] . "<br/>";
    $cell1 .= isset($row['facility_code']) ? $row['facility_code'] . "</a>" : '';

    $cell2 = isset($row['work_phone']) ? formatPhone($row['work_phone']) . "<br/>" : '';
    $cell2 .= isset($row['work_email']) ? $row['work_email'] . "<br/>" : '';

    $cell3 = $row['facility_resource_type_abbr'];
    $cell4 = $row['facility_group'];

    $cell5 = isset($row['street_1']) && $row['street_1'] != '' ? $row['street_1'] . "<br/>" : '';
    $cell5 .= isset($row['street_2']) && $row['street_2'] != '' ? $row['street_2'] . "<br/>" : '';
    $cell5 .= isset($row['city']) ? $row['city'] . ", " : '';
    $cell5 .= isset($row['state']) ? $row['state'] . "<br/>" : '';
    $cell5 .= isset($row['postal_code']) ? $row['postal_code'] . "<br/>" : '';

    // Add cell data to rows with id
    $response->rows[$i]['id'] = $row['facility_uuid'];
    $response->rows[$i]['cells'] = array(
      'facility_name' => $cell1,
      'work_phone' => $cell2,
      'facility_resource_type_abbr' => $cell3,
      'facility_group' => $cell4,
      'postal_code' => $cell5
    );
    $i++;
}

echo json_encode($response, JSON_HEX_TAG | JSON_HEX_APOS | JSON_HEX_QUOT | JSON_HEX_AMP);
?>
