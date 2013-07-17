<?php

/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */


global $global;
global $conf;
global $query_person_fields;
global $query_person_tables;
global $expr_array;

//$conf['enable_solr_for_search'] = false;

$expr_array = array();

// get the page number
if (isset($_REQUEST['page'])) {
    $page = trim($_REQUEST['page']);
    $_SESSION['page'] = $page;
} elseif (isset($_SESSION['page'])) {
    $page = trim($_SESSION['page']);
} else {
    $page = null;
    $_SESSION['page'] = null;
}

// get how many rows we want to have into the grid - rowNum parameter in the grid
if (isset($_REQUEST['limit'])) {
    $limit = trim($_REQUEST['limit']);
    $_SESSION['limit'] = $limit;
} elseif (isset($_SESSION['limit'])) {
    $limit = trim($_SESSION['limit']);
} else {
    $limit = 10;
    $_SESSION['limit'] = 10;
};

// get index row - i.e. user click to sort. At first time sortname parameter -
// after that the index from colModel
if (isset($_REQUEST['sidx'])) {
    $sidx = trim($_REQUEST['sidx']);
    $_SESSION['sidx'] = $sidx;
} elseif (isset($_SESSION['sidx'])) {
    $sidx = trim($_SESSION['sidx']);
} else {
    $sidx = null;
    $_SESSION['sidx'] = 'full_name';
};

// sorting order - at first time sortorder
if (isset($_REQUEST['sord'])) {
    $sord = trim($_REQUEST['sord']);
    $_SESSION['sord'] = $sord;
} elseif (isset($_SESSION['sord'])) {
    $sord = trim($_SESSION['sord']);
} else {
    $sord = NULL;
    $_SESSION['sord'] = NULL;
};


// Facility Group search filter
if (isset($_REQUEST['facility_group'])) {

    $facility_group = trim($_REQUEST['facility_group']);
    $_SESSION['facility_group'] = $facility_group;
} elseif (isset($_SESSION['facility_group'])) {

    $facility_group = $_SESSION['facility_group'];
} else {

    $facility_group = null;
    $_SESSION['facility_group'] = null;
}

// Facility search filter
if (isset($_REQUEST['facility'])) {

    $facility = trim($_REQUEST['facility']);
    $_SESSION['facility'] = $facility;
} elseif (isset($_SESSION['facility'])) {

    $facility = $_SESSION['facility'];
} else {

    $facility = null;
    $_SESSION['facility'] = null;
}


// See if we should use Solr Server for search
if ($conf['enable_solr_for_search'] == false) {


    $values = array();
    $where_clause = "";

    // Apply filters
    
    if (isset($_SESSION['incident']) && $_SESSION['incident'] != '') {
        $logic = (isset($values[0])) ? "AND" : "WHERE";
        $values[] = $_SESSION['incident'];
        $where_clause .= "$logic incident_id = ? \n";
    }

    if (isset($facility_group) && $facility_group != 'all') {
        $logic = (isset($values[0])) ? "AND" : "WHERE";
        $values[] = $facility_group;
        $where_clause .= "$logic facility_group LIKE CONCAT('%',?,'%') \n";
    }

    if (!empty($facility) && $facility != 'all') {
        $logic = (isset($values[0])) ? "AND" : "WHERE";
        $values[] = $facility;
        $where_clause .= "$logic facility_uuid = ? \n";
    }

    // Filter only checked out clients
    $logic = (isset($values[0])) ? "AND" : "WHERE";
    $values[] = 'out';
    $where_clause .= "$logic opt_status = ? \n";


    $query = "SELECT COUNT(p_uuid) AS count FROM crs_client_search ";
    $query .= $where_clause;

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

    // Now create the real data query
    $query = "SELECT 
          facility_group,
          facility_code,
          facility_name,
          group_primary,
          given_name,
          family_name,
          years_old,
          opt_gender,
          street_1,
          street_2,
          city,
          state,
          postal,
          home_phone,
          mobile_phone,
          alt_phone,
          in_date,
          out_date
    FROM crs_client_search ";
    $query .= $where_clause;

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
    $global['db']->SetFetchMode(ADODB_FETCH_ASSOC);
    try {
        $query = $global['db']->Prepare($query);
        $results = $global['db']->Execute($query, $values);
    } catch (exception $e) {
        add_error("<pre>" . $e->getMessage() . "</pre>");
    }
    $global['db']->SetFetchMode(ADODB_FETCH_DEFAULT);
} else {

    // Going Solr prepare for coronal ejection

    include_once $global['approot'] . "3rd/Solarium/library/Solarium/Autoloader.php";
    Solarium_Autoloader::register();

    // create a client instance
    $client = new Solarium_Client();
    $client->getAdapter()->setHost($conf['SOLR_host'])->setPort($conf["SOLR_port"]);

    // get a select query instance
    $query = $client->createSelect();


    /*
     *  Create some filter queries
     */
    $filterQueries = array();


    // Facility Groups
    if (isset($facility_group) && $facility_group != "all") {
        $fq = $query->createFilterQuery();
        $fq->setKey('groups');
        $fq->setQuery("facility_group:\"$facility_group\"");
        array_push($filterQueries, $fq);
        unset($fq);
    }

    // Facility Name
    if (isset($facility) && $facility != "all") {
        $fq = $query->createFilterQuery();
        $fq->setKey('facility');
        $fq->setQuery("facility_uuid:\"$facility\"");
        array_push($filterQueries, $fq);
        unset($fq);
    }

    // Min Ages
    if (isset($min_age) && $min_age != '') {
        $fq = $query->createFilterQuery();
        $fq->setKey('min_age');
        $fq->setQuery("years_old:[$min_age TO *]");
        array_push($filterQueries, $fq);
        unset($fq);
    }

    // Max Ages
    if (isset($max_age) && $max_age != '') {
        $fq = $query->createFilterQuery();
        $fq->setKey('max_age');
        $fq->setQuery("years_old:[* TO $max_age]");
        array_push($filterQueries, $fq);
        unset($fq);
    }

    // Filter for only checked out clients
    $fq = $query->createFilterQuery();
    $fq->setKey('status');
    $fq->setQuery("opt_status:out");
    array_push($filterQueries, $fq);
    unset($fq);


    // Apply the filters
    if (count($filterQueries) > 0) {
        $query->addFilterQueries($filterQueries);
    }

    // First query for count
    $rs = $client->select($query);
    $count = $rs->getNumFound();
    unset($rs);

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

    // Sort ordering
    $sord = (isset($sord)) ? $sord : 'asc';
    if ($sidx) {
        $query->addSort($sidx, $sord);
    } else {
        $query->addSort('full_name', 'asc');
    }

    // Pagination
    if ($start && $limit) {
        $query->setStart($start);
    }
    $query->setRows($limit);


    // Now query Solr for the paginated and sorted set
    try {
        $results = $client->select($query);
    } catch (exception $e) {
        print $e;
    }
}

// Create JSON responce object
$response->page = $page;
$response->records = $count;
$response->total = $total_pages;
$response->limit = $limit;
$response->sidx = $sidx;
$response->sord = $sord;
$response->colnames = array(
  "Facility Group", "Site Code", "Site Name", "Last Checkout", "Head Name", "First Name", "Last Name",
  "Age", "Gender", "Address", "Street 2", "City", "State", "Zip Code", "Home Phone",
  "Mobile Phone", "Alternate Phone");

$i = 0;
foreach ($results as $row) {

    $response->rows[$i] = array(
      'facility_group' => isset($row['facility_group']) ? $row['facility_group'] : "",
      'facility_code' => isset($row['facility_code']) ? $row['facility_code'] : "",
      'facility_name' => isset($row['facility_name']) ? $row['facility_name'] : "",
      'out_date' => isset($row['out_date']) ? $row['out_date'] : "",
      'client_group_name' => isset($row['client_group_name']) ? $row['client_group_name'] : "",
      'given_name' => $row['given_name'],
      'family_name' => $row['family_name'],
      'years_old' => isset($row['years_old']) ? $row['years_old'] : "",
      'opt_gender' => $row['opt_gender'],
      'street_1' => isset($row['street_1']) ? $row['street_1'] : "",
      'street_2' => isset($row['street_2']) ? $row['street_2'] : "",
      'city' => isset($row['city']) ? ucwords(strtolower($row['city'])) : "",
      'state' => isset($row['state']) ? $row['state'] : "",
      'postal' => isset($row['postal']) ? $row['postal'] : "",
      'home_phone' => isset($row['home_phone']) ? formatPhone($row['home_phone']) : "",
      'mobile_phone' => isset($row['mobile_phone']) ? formatPhone($row['mobile_phone']) : "",
      'alt_phone' => isset($row['alt_phone']) ? formatPhone($row['alt_phone']) : ""
    );


    $i++;
}
echo json_encode($response, JSON_HEX_TAG | JSON_HEX_APOS | JSON_HEX_QUOT | JSON_HEX_AMP);
?>
