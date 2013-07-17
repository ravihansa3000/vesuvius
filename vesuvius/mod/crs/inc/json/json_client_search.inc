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

// get the search string
if (isset($_REQUEST['name_query']) && $_REQUEST['name_query'] != '') {
    $name_query = trim($_REQUEST['name_query']);
    $_SESSION['name_query'] = $name_query;
} elseif (isset($_SESSION['name_query']) && $_SESSION['name_query'] != '' && $_REQUEST['name_query'] != '') {
    $name_query = trim($_SESSION['name_query']);
} else {
    $name_query = NULL;
    $_SESSION['name_query'] = NULL;
}

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

// Min Age search filter
if (isset($_REQUEST['minAge'])) {

    $minAge = trim($_REQUEST['minAge']);
    $_SESSION['minAge'] = $minAge;
} elseif (isset($_SESSION['minAge'])) {

    $minAge = $_SESSION['minAge'];
} else {

    $minAge = null;
    $_SESSION['minAge'] = null;
}

// Max Age search filter
if (isset($_REQUEST['maxAge'])) {

    $maxAge = trim($_REQUEST['maxAge']);
    $_SESSION['maxAge'] = $maxAge;
} elseif (isset($_SESSION['maxAge'])) {

    $maxAge = $_SESSION['maxAge'];
} else {

    $maxAge = null;
    $_SESSION['maxAge'] = null;
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

    if (isset($name_query)) {
        $logic = (isset($values[0])) ? "AND" : "WHERE";
        $values[] = $name_query;
        $where_clause .= "$logic full_name LIKE CONCAT('%',?,'%') \n";
    }

    if (isset($facility_group) && $facility_group != 'all') {
        $logic = (isset($values[0])) ? "AND" : "WHERE";
        $values[] = $facility_group;
        $where_clause .= "$logic facility_group LIKE CONCAT('%',?,'%') \n";
    }

    if (isset($facility) && $facility != 'all') {
        $logic = (isset($values[0])) ? "AND" : "WHERE";
        $values[] = $facility;
        $where_clause .= "$logic facility_uuid = ? \n";
    }

    if (isset($minAge) && $minAge != '') {
        $logic = (isset($values[0])) ? "AND" : "WHERE";
        $values[] = $minAge;
        $where_clause .= "$logic years_old >= ? \n";
    }

    if (isset($maxAge) && $maxAge != '') {
        $logic = (isset($values[0])) ? "AND" : "WHERE";
        $values[] = $maxAge;
        $where_clause .= "$logic years_old <= ? \n";
    }

    $query = "SELECT COUNT(p_uuid) AS count FROM crs_client_search \n";
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
    $query = "SELECT * FROM crs_client_search ";
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
    try {
        $query = $global['db']->Prepare($query);
        $results = $global['db']->Execute($query, $values);
    } catch (exception $e) {
        add_error("<pre>" . $e->getMessage() . "</pre>");
    }
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

    // Incident
    $fq = $query->createFilterQuery();
    $fq->setKey('incident');
    $fq->setQuery("incident_id:{$_SESSION['incident']}");
    array_push($filterQueries, $fq);
    unset($fq);


    // Name
    if (isset($name_query) && $name_query != "") {
        $fq = $query->createFilterQuery();
        $fq->setKey('text');
        $fq->setQuery("text:\"$name_query\"");
        array_push($filterQueries, $fq);
        unset($fq);
    }

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
    if (isset($minAge) && $minAge != '') {
        $fq = $query->createFilterQuery();
        $fq->setKey('minAge');
        $fq->setQuery("years_old:[$minAge TO *]");
        array_push($filterQueries, $fq);
        unset($fq);
    }

    // Max Ages
    if (isset($maxAge) && $maxAge != '') {
        $fq = $query->createFilterQuery();
        $fq->setKey('maxAge');
        $fq->setQuery("years_old:[* TO $maxAge]");
        array_push($filterQueries, $fq);
        unset($fq);
    }

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
$response->incident = $_SESSION['incident'];
$response->page = $page;
$response->records = $count;
$response->total = $total_pages;
$response->limit = $limit;
$response->sidx = $sidx;
$response->sord = $sord;
$response->colnames = array('First Name', 'Last Name', 'Gender', 'Group Primary', 'Facility Group', 'Shelter', 'Status');

$i = 0;

if (isset($results)) {
    foreach ($results as $row) {

        $base_uuid = base64_encode($row['p_uuid']);
        $base_fuuid = base64_encode($row['facility_uuid']);
        $given_name = (!empty($row['given_name'])) ? "<a href=\"index.php?mod=crs&amp;act=client_view&amp;uuid=$base_uuid\">" . $row['given_name'] . "</a> " : "&nbsp;";
        $family_name = (!empty($row['family_name'])) ? "<a href=\"index.php?mod=crs&amp;act=client_view&amp;uuid=$base_uuid\">" . $row['family_name'] . "</a> " : "&nbsp;";

        // Group Primary Name
        if (isset($row['group_primary']) && $row['group_primary'] != '') {
            $client_group_name = "<a href=\"index.php?mod=crs&amp;act=client_view&amp;uuid="
                . base64_encode($row['group_primary']) . "\">" . $row['client_group_name'] . "</a>";
        } else {
            $client_group_name = "&nbsp;";
        }

        // Status
        switch ($row['opt_status']) {
            case "in":
                $class = "checked_in";
                $text = "Checked In";
                $act = "client_checkout";
                break;
            case "out":
                $class = "checked_out";
                $text = "Checked Out";
                $act = "client_checkin";
                break;
            case "trn" :
                $class = "checked_out";
                $text = "Transfered";
                $act = "client_checkin";
        }
        $status = "<a href=\"index.php?mod=crs&amp;act=$act&amp;uuid=$base_uuid\"><span class=\"$class\">$text</span></a>";

        // Gender
        $gender = isset($row['opt_gender']) ? $row['opt_gender'] : "&nbsp;";

        // Facility location
        if (isset($row['facility_name'])) {
            $facility = "<a href=\"index.php?mod=fms&amp;act=edit&amp;id=$base_fuuid\">" . $row['facility_name'] . "</a>";
        } else {
            $facility = "&nbsp;";
        }

        $response->rows[$i] = array(
          'given_name' => $given_name,
          'family_name' => $family_name,
          'opt_gender' => $gender,
          'client_group_name' => $client_group_name,
          'facility_group' => $row['facility_group'],
          'facility_name' => $facility,
          'opt_status' => $status
        );
        $i++;
    }
}
echo json_encode($response, JSON_HEX_TAG | JSON_HEX_APOS | JSON_HEX_QUOT | JSON_HEX_AMP);
?>
