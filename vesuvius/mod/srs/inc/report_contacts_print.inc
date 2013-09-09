<?php

/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */


global $global;
global $conf;

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
    $_SESSION['sidx'] = 'org_name';
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

// Org search filter
if (isset($_REQUEST['org'])) {

    $org = trim($_REQUEST['org']);
    $_SESSION['org'] = $org;
} elseif (isset($_SESSION['org'])) {

    $org = $_SESSION['org'];
} else {

    $org = null;
    $_SESSION['org'] = null;
}

// Volunteer search filter
if (isset($_REQUEST['vol'])) {

    $vol = trim($_REQUEST['vol']);
    $_SESSION['vol'] = $vol;
} elseif (isset($_SESSION['vol'])) {

    $vol = $_SESSION['vol'];
} else {

    $vol = null;
    $_SESSION['vol'] = null;
}

$conf['enable_solr_for_search'] = false;

// See if we should use Solr Server for search
if ($conf['enable_solr_for_search'] == false) {

    $values = array();
    $where_clause = "";

    // Apply filters

    if (isset($org) && $org != 'all') {
        $logic = (isset($values[0])) ? "AND" : "WHERE";
        $values[] = $org;
        $where_clause .= "$logic org_id = ? \n";
    }

    if (isset($vol) && $vol != 'all') {
        $logic = (isset($values[0])) ? "AND" : "WHERE";
        $values[] = $vol;
        $where_clause .= "$logic vol_id = ? \n";
    }

    if (isset($staffType) && $staffType != 'All') {
        $logic = (isset($values[0])) ? "AND" : "WHERE";
        $values[] = $max_age;
        $where_clause .= "$logic stafftype_id = ? \n";
    }

    // Filter checked out clients
    $logic = (isset($values[0])) ? "AND" : "WHERE";
    $where_clause .= "$logic vol_id IS NOT NULL \n";


    $query = "SELECT COUNT(p_uuid) AS count FROM srs_staff_search ";
    $query .= $where_clause;

    // Execute query
    try {

        $query = $global['db']->Prepare($query);
        $result = $global['db']->Execute($query, $values);
    } catch (exception $e) {
        print("<pre>" . $e->getMessage() . "</pre>");
    }

    $count = $result->Fields('count');

    // calculate the total pages for the query
    if ($count > 0 && $limit > 0) {
        $totalPages = ceil($count / $limit);
    } else {
        $totalPages = 0;
    }

    // if for some reasons the requested page is greater than the total
    // set the requested page to total page
    if ($page > $totalPages) {
        $page = $totalPages;
    }

    // calculate the starting position of the rows
    $start = $limit * $page - $limit;

    // if for some reasons start position is negative set it to 0
    // typical case is that the user type 0 for the requested page
    if ($start < 0) {
        $start = 0;
    }

    // Now create the real data query
    $query = "SELECT * FROM srs_staff_search ";
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

    // Filter checked out clients
    $fq = $query->createFilterQuery();
    $fq->setKey('status');
    $fq->setQuery("opt_status:in");
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
        $totalPages = ceil($count / $limit);
    } else {
        $totalPages = 0;
    }

    // if for some reasons the requested page is greater than the total
    // set the requested page to total page
    if ($page > $totalPages) {
        $page = $totalPages;
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
$colNames = array("Organization", "Staff Type", "First", "Last", "Volunteer Type", "Home Address", "Home Phone", "Work Address", "Work Phone");

$i = 0;
foreach ($results as $row) {

    $homeAddress = isset($row['street_1']) ? $row['street_1'] : "";
    $homeAddress .= isset($row['street_2']) ? "<br/>" . $row['street_2'] : "";
    $homeAddress .= isset($row['city']) ? "<br/>" . $row['city'] : "";
    $homeAddress .= isset($row['state']) ? ", " . $row['state'] : "";
    $homeAddress .= isset($row['postal']) ? " " . $row['postal'] : "";

    $workAddress = isset($row['work_street_1']) ? $row['work_street_1'] : "";
    $workAddress .= isset($row['work_street_2']) ? "<br/>" . $row['work_street_2'] : "";
    $workAddress .= isset($row['work_city']) ? "<br/>" . $row['work_city'] : "";
    $workAddress .= isset($row['work_state']) ? ", " . $row['work_state'] : "";
    $workAddress .= isset($row['work_postal']) ? " " . $row['work_postal'] : "";

    $data[$i++] = array(
      'org_name' => isset($row['org_name']) ? $row['org_name'] : "",
      'staff_type' => $row['staff_type'],
      'given_name' => $row['given_name'],
      'family_name' => $row['family_name'],
      'vol_type' => isset($row['vol_type']) ? $row['vol_type'] : "",
      'street_1' => $homeAddress,
      'home_phone' => isset($row['home_phone']) ? formatPhone($row['home_phone']) : "",
      'work_street_1' => $workAddress,
      'work_phone' => isset($row['work_phone']) ? formatPhone($row['work_phone']) : ""
    );
}
?>
