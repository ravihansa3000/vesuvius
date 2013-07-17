<?php

/**
 * Shelteree Tracking Module
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
global $query_person_fields;
global $query_person_tables;
global $expr_array;

$expr_array = array();

// get the search string
if (isset($_REQUEST['uuid'])) {
  $uuid = trim($_REQUEST['uuid']);
} else {
  $uuid = null;
}

// get the search string
if (isset($_REQUEST['query'])) {
  $search_str = trim($_REQUEST['query']);
} else {
  $search_str = null;
}

// get the page number
if (isset($_REQUEST['page'])) {
  $page = trim($_REQUEST['page']);
} else {
  $page = null;
}

// get how many rows we want to have into the grid - rowNum parameter in the grid
if (isset($_REQUEST['limit'])) {
  $limit = trim($_REQUEST['limit']);
} else {
  $limit = 5;
};

// Get the city query
if (isset($_REQUEST['city'])) {
  $city = trim($_REQUEST['city']);
} else {
  $city = null;
};

// Get the state query
if (isset($_REQUEST['state'])) {
  $state = trim($_REQUEST['state']);
} else {
  $state = null;
};

// Get the postal query
if (isset($_REQUEST['postal'])) {
  $postal = trim($_REQUEST['postal']);
} else {
  $postal = null;
}

// Get the postal query
if (isset($_REQUEST['phone'])) {
  $phone = trim($_REQUEST['phone']);
} else {
  $phone = null;
}


$values = array();

// Build search expressions
$values[] = $_SESSION['incident'];
$values[] = $search_str;
$where_clause = "WHERE incident_id = ? AND full_name LIKE CONCAT('%',?,'%') 
  AND client_group_name = '' \n";

// Don't display the current user as an available choice
$values[] = base64_decode($_SESSION['uuid']);
$where_clause .= "AND p_uuid != ? \n";

if ($city) {
  $values[] = $city;
  $where_clause .= "AND c7.contact_value LIKE CONCAT('%',?,'%') \n";
}

if ($state) {
  $values[] = $state;
  $where_clause .= "AND c8.contact_value LIKE CONCAT('%',?,'%') \n";
}

if ($postal) {
  $values[] = $postal;
  $where_clause .= "AND c9.contact_value LIKE CONCAT('%',?,'%') \n";
}

if ($phone) {
  $values[] = $phone;
  $values[] = $phone;
  $values[] = $phone;
  $where_clause .= "AND (
      c1.contact_value LIKE CONCAT('%',?,'%') 
   OR c2.contact_value LIKE CONCAT('%',?,'%') 
   OR c3.contact_value LIKE CONCAT('%',?,'%'))\n";
}

$query = "SELECT COUNT(p_uuid) AS count FROM crs_client_search \n" . $where_clause;


// execute query
try {
  $result = $global['db']->Execute($query, $values);
} catch (exception $e) {
  print($e);
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
$query = "SELECT * FROM crs_client_search \n" . $where_clause;

// Sort ordering
$query .= "ORDER BY full_name ";

// Pagination
if ($start && $limit) {
  $query .= "LIMIT $start , $limit";
} else {
  $query .= "LIMIT $limit";
}

// Execute full query
try {
  $results = $global['db']->Execute($query, $values);
} catch (exception $e) {
  error_log(":Sahana RP JSON: $e\n\n");
}

// Create JSON responce object
$response->page = $page;
$response->records = $count;
$response->total = $total_pages;
$response->limit = $limit;
$response->colnames = array('Representative', 'Contanct Info', 'Shelter Location', 'Group Members');

$i = 0;
foreach ($results as $row) {

  $uuid = base64_encode($uuid);
  $groupId = base64_encode($row['p_uuid']);

  // Group representative
  $representative = $row['full_name'] . "<br/>"
      . $row['years_old'] . " " . $row['opt_gender'];

  // Representative's contact info
  $contact_info = "";

  $contact_info .= isset($row['street_1']) && $row['street_1'] != '' ? $row['street_1'] . "<br/>" : '';
  $contact_info .= isset($row['street_2']) && $row['street_2'] != '' ? $row['street_2'] . "<br/>" : '';
  $contact_info .= isset($row['city']) ? $row['city'] . ", " : '';
  $contact_info .= isset($row['state']) ? $row['state'] . "<br/>" : '';
  $contact_info .= isset($row['home_phone']) ? formatPhone($row['home_phone']) . "<br/>" : '';
  $contact_info .= isset($row['mobile_phone']) ? formatPhone($row['mobile_phone']) . "<br/>" : '';
  $contact_info .= isset($row['alt_phone']) ? formatPhone($row['alt_phone']) . "<br/>" : '';

  // Facility location
  if (isset($row['facility_name'])) {
    $facility = $row['facility_name'];
    $facility .= "<br/>Entered: " . $row['in_date'];
  } else {
    $facility = "&nbsp;";
  }

  // Get group members
  $members = get_group_list($row['p_uuid']);

  $group_info = '';
  foreach ($members as $member) {
    if ($member['p_uuid'] != $row['p_uuid']) {
      $group_info .= $member['full_name'] . "<br/>";
    }
  }

  // Append object details
  $response->rows[$i]['id'] = $groupId;
  $response->rows[$i]['cells'] = array(
    'representative' => $representative,
    'contact_info' => $contact_info,
    'facility_info' => $facility,
    'group_info' => $group_info
  );
  $i++;
}

echo json_encode($response, JSON_HEX_TAG | JSON_HEX_APOS | JSON_HEX_QUOT | JSON_HEX_AMP);
?>
