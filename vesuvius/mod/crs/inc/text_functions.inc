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
function shn_text_crs_age_dist_chart()
{
    // Validate group access
    if (!check_groupAccess(array(1, 2))) {
        add_error("Permission denied.");
        return;
    }

    // Validate group access
    if (!check_validIncident()) {
        add_error("You must select an open event before attempting to perform this action.");
        return;
    }

    include_once 'charts.inc';
    age_dist_chart();
}

function shn_text_crs_top_zipcodes_chart()
{
    // Validate group access
    if (!check_groupAccess(array(1, 2))) {
        add_error("Permission denied.");
        return;
    }

    // Validate group access
    if (!check_validIncident()) {
        add_error("You must select an open event before attempting to perform this action.");
        return;
    }

    include_once 'charts.inc';
    top_zipcodes_chart();
}

function shn_text_crs_top_facilities_chart()
{
    // Validate group access
    if (!check_groupAccess(array(1, 2))) {
        add_error("Permission denied.");
        return;
    }

    // Validate group access
    if (!check_validIncident()) {
        add_error("You must select an open event before attempting to perform this action.");
        return;
    }

    include_once 'charts.inc';
    top_facilities_chart();
}

function shn_text_crs_agetype_chart()
{
    // Validate group access
    if (!check_groupAccess(array(1, 2))) {
        add_error("Permission denied.");
        return;
    }

    // Validate group access
    if (!check_validIncident()) {
        add_error("You must select an open event before attempting to perform this action.");
        return;
    }

    include_once 'charts.inc';

    $f = isset($_REQUEST['f']) ? $_REQUEST['f'] : NULL;

    agetype_pie_chart($f);
}

function shn_text_crs_gender_chart()
{
    // Validate group access
    if (!check_groupAccess(array(1, 2))) {
        add_error("Permission denied.");
        return;
    }

    // Validate group access
    if (!check_validIncident()) {
        add_error("You must select an open event before attempting to perform this action.");
        return;
    }

    include_once 'charts.inc';

    $f = isset($_REQUEST['f']) ? $_REQUEST['f'] : NULL;

    gender_pie_chart($f);
}

function shn_text_crs_group_registration_chart()
{
    // Validate group access
    if (!check_groupAccess(array(1, 2))) {
        add_error("Permission denied.");
        return;
    }

    // Validate group access
    if (!check_validIncident()) {
        add_error("You must select an open event before attempting to perform this action.");
        return;
    }
    global $global;

    include_once 'charts.inc';

    $query = "SELECT 
        facility_group,
        COUNT(*) AS 'count'
      FROM fms_person_to_facility AS ptf
        INNER JOIN fms_facility AS f ON ptf.facility_uuid = f.facility_uuid
        INNER JOIN crs_person_to_group ptg ON ptf.p_uuid = ptg.p_uuid_1
      GROUP BY facility_group";

    // Execute query
    try {
        $query = $global['db']->Prepare($query);
        $rs = $global['db']->Execute($query);
    } catch (exception $e) {
        print("<pre>" . $e->getMessage() . "</pre>");
    }

    foreach ($rs as $row) {
        $data[$row['facility_group']] = $row['count'];
    }

    pie_chart($data, 100);
}

function shn_text_crs_group_capacity_chart()
{
    // Validate group access
    if (!check_groupAccess(array(1, 2))) {
        add_error("Permission denied.");
        return;
    }

    // Validate group access
    if (!check_validIncident()) {
        add_error("You must select an open event before attempting to perform this action.");
        return;
    }

    global $global;

    include_once 'charts.inc';

    $r = isset($_REQUEST['r']) ? $_REQUEST['r'] : NULL;
    $fg = isset($_REQUEST['fg']) ? base64_decode($_REQUEST['fg']) : NULL;

    $query = "SELECT 
          ROUND((SUM(f.facility_capacity) - count(*))/ SUM(f.facility_capacity) * 100, 2) AS pct_free,
          100 - ROUND((SUM(f.facility_capacity) - count(*))/ SUM(f.facility_capacity) * 100, 2) AS pct_full
      FROM fms_person_to_facility AS ptf
          INNER JOIN person_uuid AS p ON p.p_uuid = ptf.p_uuid
          INNER JOIN fms_facility AS f ON ptf.facility_uuid = f.facility_uuid
          INNER JOIN crs_person_to_group ptg ON ptf.p_uuid = ptg.p_uuid_1
          INNER JOIN person_status AS ps ON ps.p_uuid = ptf.p_uuid
      WHERE ps.opt_status = 'in' AND p.incident_id = ? AND f.facility_group = ?";

    // Execute query
    try {
        $rs = $global['db']->Execute($query, array($_SESSION['incident'], $fg));
    } catch (exception $e) {
        print("<pre>" . $e->getMessage() . "</pre>");
    }
    $stats = $rs->FetchRow();

    $data = array('Available' => floor($stats['pct_free']), 'Used' => ceil($stats['pct_full']));

    pie_chart($data, $r);
}

function shn_text_crs_group_age_chart()
{
    // Validate group access
    if (!check_groupAccess(array(1, 2))) {
        add_error("Permission denied.");
        return;
    }

    // Validate group access
    if (!check_validIncident()) {
        add_error("You must select an open event before attempting to perform this action.");
        return;
    }

    global $global;

    include_once 'charts.inc';

    $r = isset($_REQUEST['r']) ? $_REQUEST['r'] : NULL;
    $fg = isset($_REQUEST['fg']) ? base64_decode($_REQUEST['fg']) : NULL;

    $query = "SELECT 
          SUM(IF(years_old BETWEEN 0 AND 3,1,0)) AS infants,
          SUM(IF(years_old BETWEEN 4 AND 17,1,0)) AS children,
          SUM(IF(years_old BETWEEN 18 AND 64,1,0)) AS adults,
          SUM(IF(years_old BETWEEN 65 AND 150,1,0)) AS seniors,
          SUM(IF(years_old IS NULL,1,0)) AS unk
      FROM fms_person_to_facility AS ptf
          INNER JOIN person_uuid AS p ON p.p_uuid = ptf.p_uuid
          INNER JOIN crs_person_to_group ptg ON ptf.p_uuid = ptg.p_uuid_1
          INNER JOIN fms_facility AS f ON ptf.facility_uuid = f.facility_uuid
          INNER JOIN person_details AS pd ON ptf.p_uuid = pd.p_uuid
          INNER JOIN person_status AS ps ON ps.p_uuid = ptf.p_uuid
      WHERE ps.opt_status = 'in' AND ps.isdeleted = 0 AND p.incident_id = ? AND f.facility_group = ?";

    // Execute query
    try {
        $rs = $global['db']->Execute($query, array($_SESSION['incident'], $fg));
    } catch (exception $e) {
        print("<pre>" . $e->getMessage() . "</pre>");
    }
    $stats = $rs->FetchRow();

    $data = array('Adults' => $stats['adults'], 'Children' => $stats['children'], 'Infants' => $stats['infants'], 'Seniors' => $stats['seniors'], 'Unknown' => $stats['unk']);

    pie_chart($data, $r);
}

function shn_text_crs_progress_chart()
{
    // Validate group access
    if (!check_groupAccess(array(1, 2))) {
        add_error("Permission denied.");
        return;
    }

    // Validate group access
    if (!check_validIncident()) {
        add_error("You must select an open event before attempting to perform this action.");
        return;
    }

    global $global;
    include_once 'charts.inc';

    $f = isset($_REQUEST['f']) ? $_REQUEST['f'] : NULL;

    $f = ($f < 1) ? 1 : $f;

    if ($f) {
        progress_chart($f);
    }
}

function shn_text_crs_top_states_chart()
{
    // Validate group access
    if (!check_groupAccess(array(1, 2))) {
        add_error("Permission denied.");
        return;
    }

    // Validate group access
    if (!check_validIncident()) {
        add_error("You must select an open event before attempting to perform this action.");
        return;
    }

    include_once 'charts.inc';

    $f = isset($_REQUEST['f']) ? $_REQUEST['f'] : NULL;

    top_states_chart($f);
}

function shn_text_crs_top_lang_chart()
{
    // Validate group access
    if (!check_groupAccess(array(1, 2))) {
        add_error("Permission denied.");
        return;
    }

    // Validate group access
    if (!check_validIncident()) {
        add_error("You must select an open event before attempting to perform this action.");
        return;
    }

    include_once 'charts.inc';

    $f = isset($_REQUEST['f']) ? $_REQUEST['f'] : NULL;

    top_languages_chart($f);
}

function shn_text_crs_excel_export()
{
    // Validate group access
    if (!check_groupAccess(array(1, 2))) {
        add_error("Permission denied.");
        return;
    }

    // Validate group access
    if (!check_validIncident()) {
        add_error("You must select an open event before attempting to perform this action.");
        return;
    }

    global $global;
    global $conf;
    global $query_person_fields;
    global $query_person_tables;
    global $expr_array;
    global $wb;

    // Variables
    $title = "Sahana Emergency Management System";
    $subtitle = "Shelter Client Registry Search Export";
    $tmpPath = realpath(sys_get_temp_dir());
    $xlsBaseName = "sahana_crs_export_" . date('Ymd_His');
    $zipName = $xlsBaseName . ".zip";
    $maxRows = 20000;

    $col_names = array('First Name', 'Last Name', 'Gender', 'Group Head', 'Solar Sys', 'Shelter');


    include_once "excel2003Export.class.inc";

    // get the search string
    if (isset($_SESSION['name_query']) && $_SESSION['name_query'] != '') {
        $name_query = trim($_SESSION['name_query']);
    } else {
        $name_query = NULL;
    }

    // Facility Group search filter
    if (isset($_SESSION['facility_group'])) {
        $facility_group = $_SESSION['facility_group'];
    } else {
        $facility_group = null;
    }

    // Facility search filter
    if (isset($_SESSION['facility'])) {
        $facility = $_SESSION['facility'];
    } else {
        $facility = null;
    }

    // Min Age search filter
    if (isset($_SESSION['min_age'])) {
        $min_age = $_SESSION['min_age'];
    } else {
        $min_age = null;
    }

    // Max Age search filter
    if (isset($_SESSION['max_age'])) {
        $max_age = $_SESSION['max_age'];
    } else {
        $max_age = null;
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

        if (isset($min_age) && $min_age != '') {
            $logic = (isset($values[0])) ? "AND" : "WHERE";
            $values[] = $min_age;
            $where_clause .= "$logic years_old >= ? \n";
        }

        if (isset($max_age) && $max_age != '') {
            $logic = (isset($values[0])) ? "AND" : "WHERE";
            $values[] = $max_age;
            $where_clause .= "$logic years_old <= ? \n";
        }

        // Now create the real data query
        $query = "SELECT * FROM crs_client_search ";
        $query .= $where_clause;

        // Sort ordering
        $query .= "ORDER BY full_name ASC";

        // Execute query
        try {
            $results = $global['db']->Execute($query, $values);
        } catch (exception $e) {
            error_log($e->getMessage());
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

        // Create some filter queries
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
            $fq->setKey('name');
            $fq->setQuery("full_name:\"$name_query\"");
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

        // Apply the filters
        if (count($filterQueries) > 0) {
            $query->addFilterQueries($filterQueries);
        }

        // Sort ordering
        $query->addSort('full_name', 'asc');

        $query->setRows(999999999);

        // Now query Solr for the full search set
        try {
            $results = $client->select($query);
        } catch (exception $e) {
            print $e;
        }
    }

    // Convert the results object into an array so it can be chunked
    $resultArray = array();
    foreach ($results as $data) {
        $resultArray[] = array(
          'p_uuid' => $data['p_uuid'],
          'facility_uuid' => $data['facility_uuid'],
          'facility_name' => $data['facility_name'],
          'facility_group' => $data['facility_group'],
          'given_name' => $data['given_name'],
          'family_name' => $data['family_name'],
          'group_primary' => $data['group_primary'],
          'client_group_name' => $data['client_group_name'],
          'opt_status' => $data['opt_status'],
          'opt_gender' => $data['opt_gender']
        );
    }

    // Batch to rows so they don't exceed MS Excel row limits
    $chunks = array_chunk($resultArray, $maxRows);

    // Release the query object
    unset($results);

    // Create zip file
    $zipFile = new ZipArchive();
    $zipPath = tempnam($tmpPath, "crs_");

    if ($zipFile->open($zipPath, ZIPARCHIVE::CREATE) !== TRUE) {
        echo("Export: Could not create zip file $zipPath. Check permissions.");
    }

    $i = 0;
    $wbPaths = array();

    foreach ($chunks as $batch) {

        $filename = $xlsBaseName;

        // Name the xls export
        if (count($chunks) > 1) {
            $filename .= "_part-" . ($i + 1);
        }
        $filename .= ".xls";

        // Create xls workbook 
        create_workbook($col_names, $title, $subtitle);


        // Loop through the records adding each row to the worksheet
        foreach ($batch as $row) {

            // Status
            if ($row['opt_status'] == "in") {
                $status = "Checked In";
            } else {
                $status = "Checked Out";
            }

            $wb->home();
            $wb->label($row['given_name']);
            $wb->right();
            $wb->label($row['family_name']);
            $wb->right();
            $wb->label($row['opt_gender']);
            $wb->right();
            $wb->label($row['client_group_name']);
            $wb->right();
            $wb->label($row['facility_group']);
            $wb->right();
            $wb->label($row['facility_name']);
            $wb->right();
            $wb->label($status);
            $wb->down();
        }

        // Save the xls real path and filename to array so it can be added to zip archive
        $wbPaths[$i] = array($tmpPath . "/" . $filename, $filename);
        $wb->save($wbPaths[$i][0]);
        $i++;
    }


    // Add the files to the zip archive
    foreach ($wbPaths as $file) {
        $zipFile->addFile($file[0], $file[1]);
    }

    // Now save the zip archive by closing it
    $zipFile->close();
    unset($zipFile);

    // Cleanup raw xls files
    foreach ($wbPaths as $file) {
        unlink($file[0]);
    }


    // Make sure the browser doesn't try to deliver a chached version
    header("Pragma: public");
    header("Expires: 0");
    header("Cache-Control: must-revalidate, post-check=0, pre-check=0");

    header("Content-Type: application/zip");
    header("Content-Disposition: attachment;filename=$zipName");
    header("Content-Transfer-Encoding: binary");
    header("Content-Length: " . filesize($zipPath));

    readfile($zipPath);
    unlink($zipPath);
}

function shn_text_crs_report_fcontacts_export()
{
    // Validate group access
    if (!check_groupAccess(array(1, 2))) {
        add_error("Permission denied.");
        return;
    }

    // Validate group access
    if (!check_validIncident()) {
        add_error("You must select an open event before attempting to perform this action.");
        return;
    }

    global $global;
    global $query_person_fields;
    global $query_person_tables;
    global $expr_array;
    global $wb;

    // Variables
    $title = "Sahana Emergency Management System";
    $subtitle = "Facility Contact Export";
    $tmpPath = realpath(sys_get_temp_dir());
    $xlsBaseName = "sahana_crs_export_" . date('Ymd_His');
    $zipName = $xlsBaseName . ".zip";
    $maxRows = 20000;

    $col_names = array(
      "Facility Group", "Site Code", "Site Name", "Head Name", "First Name", "Last Name",
      "Age", "Gender", "Address", "Street 2", "City", "State", "Zip Code", "Home Phone",
      "Mobile Phone", "Alternate Phone");


    include_once "excel2003Export.class.inc";


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

    $values = array();
    $where_clause = "";

    // Apply filters

    if (isset($facility_group) && $facility_group != 'all') {
        $logic = (isset($values[0])) ? "AND" : "WHERE";
        $values[] = $facility_group;
        $where_clause .= "$logic facility_group = ? \n";
    }

    if (isset($facility) && $facility != 'all') {
        $logic = (isset($values[0])) ? "AND" : "WHERE";
        $values[] = $facility;
        $where_clause .= "$logic facility_uuid = ? \n";
    }

    // Filter only checked in clients
    $logic = (isset($values[0])) ? "AND" : "WHERE";
    $values[] = 'in';
    $where_clause .= "$logic opt_status = ? \n";

    // Filter only to this incident
    $logic = (isset($values[0])) ? "AND" : "WHERE";
    $values[] = $_SESSION['incident'];
    $where_clause .= "$logic incident_id = ? \n";

    // Get the full row set
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
          alt_phone 
     FROM crs_client_search ";
    $query .= $where_clause;

    // Sort ordering
    $query .= "ORDER BY full_name ASC";


    //$global['db']->SetFetchMode(ADODB_FETCH_ASSOC);
    // Execute query
    try {
        $query = $global['db']->Prepare($query);
        $result = $global['db']->Execute($query, $values);
    } catch (exception $e) {
        print("<pre>" . $e->getMessage() . "</pre>");
    }
    $global['db']->SetFetchMode(ADODB_FETCH_DEFAULT);
    $clients = $result->GetRows();

    // Batch to rows so they don't exceed MS Excel row limits
    $chunks = array_chunk($clients, $maxRows);

    // Release the query object
    //unset($result);
    //unset($clients);
    // Create zip file
    $zipFile = new ZipArchive();
    $zipPath = tempnam($tmpPath, "crs_");

    if ($zipFile->open($zipPath, ZIPARCHIVE::CREATE) !== TRUE) {
        echo("Export: Could not create zip file $zipPath. Check permissions.");
    }

    $i = 0;
    $wbPaths = array();

    foreach ($chunks as $batch) {

        $filename = $xlsBaseName;

        // Name the xls export
        if (count($chunks) > 1) {
            $filename .= "_part-" . ($i + 1);
        }
        $filename .= ".xls";

        // Create xls workbook 
        create_workbook($col_names, $title, $subtitle);

        // Loop through the records adding each row to the worksheet
        foreach ($batch as $row) {

            $wb->home();

            // Can't use a foreach here because array_chunk resorts the keys :(
            $wb->label($row['facility_group']);
            $wb->right();
            $wb->label($row['facility_code']);
            $wb->right();
            $wb->label($row['facility_name']);
            $wb->right();
            $wb->label($row['group_primary']);
            $wb->right();
            $wb->label($row['given_name']);
            $wb->right();
            $wb->label($row['family_name']);
            $wb->right();
            $wb->label($row['years_old']);
            $wb->right();
            $wb->label($row['opt_gender']);
            $wb->right();
            $wb->label($row['street_1']);
            $wb->right();
            $wb->label($row['street_2']);
            $wb->right();
            $wb->label($row['city']);
            $wb->right();
            $wb->label($row['state']);
            $wb->right();
            $wb->label($row['postal']);
            $wb->right();
            $wb->label($row['home_phone']);
            $wb->right();
            $wb->label($row['mobile_phone']);
            $wb->right();
            $wb->label($row['alt_phone']);
            $wb->right();

            $wb->down();
        }

        // Save the xls real path and filename to array so it can be added to zip archive
        $wbPaths[$i] = array($tmpPath . "/" . $filename, $filename);
        $wb->save($wbPaths[$i][0]);
        $i++;
    }


    // Add the files to the zip archive
    foreach ($wbPaths as $file) {
        $zipFile->addFile($file[0], $file[1]);
    }

    // Now generate the zip archive
    $zipFile->close();
    unset($zipFile);

    // Cleanup raw xls files
    foreach ($wbPaths as $file) {
        unlink($file[0]);
    }


    // Make sure the browser doesn't try to deliver a chached version

    header("Pragma: public");
    header("Expires: 0");
    header("Cache-Control: must-revalidate, post-check=0, pre-check=0");

    header("Content-Type: application/zip");
    header("Content-Disposition: attachment;filename=$zipName");
    header("Content-Transfer-Encoding: binary");
    header("Content-Length: " . filesize($zipPath));

    readfile($zipPath);
    unlink($zipPath);
}

function shn_text_crs_report_checkout_export()
{
    // Validate group access
    if (!check_groupAccess(array(1, 2))) {
        add_error("Permission denied.");
        return;
    }

    // Validate group access
    if (!check_validIncident()) {
        add_error("You must select an open event before attempting to perform this action.");
        return;
    }

    global $global;
    global $query_person_fields;
    global $query_person_tables;
    global $expr_array;
    global $wb;

    // Variables
    $title = "Sahana Emergency Management System";
    $subtitle = "Facility Client Checkout Export";
    $tmpPath = realpath(sys_get_temp_dir());
    $xlsBaseName = "sahana_crs_export_" . date('Ymd_His');
    $zipName = $xlsBaseName . ".zip";
    $maxRows = 20000;

    $col_names = array(
      "Facility Group", "Site Code", "Site Name", "Last Checkout", "Head Name", "First Name", "Last Name",
      "Age", "Gender", "Address", "Street 2", "City", "State", "Zip Code", "Home Phone",
      "Mobile Phone", "Alternate Phone");


    include_once "excel2003Export.class.inc";


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

    $values = array();
    $where_clause = "";

    // Apply filters

    if (isset($facility_group) && $facility_group != 'all') {
        $logic = (isset($values[0])) ? "AND" : "WHERE";
        $values[] = $facility_group;
        $where_clause .= "$logic facility_group = ? \n";
    }

    if (isset($facility) && $facility != 'all') {
        $logic = (isset($values[0])) ? "AND" : "WHERE";
        $values[] = $facility;
        $where_clause .= "$logic facility_uuid = ? \n";
    }

    // Filter only checked out clients
    $logic = (isset($values[0])) ? "AND" : "WHERE";
    $values[] = 'out';
    $where_clause .= "$logic opt_status = ? \n";

    // Filter only to this incident
    $logic = (isset($values[0])) ? "AND" : "WHERE";
    $values[] = $_SESSION['incident'];
    $where_clause .= "$logic incident_id = ? \n";

    // Get the full row set
    $query = "SELECT 
          facility_group,
          facility_code,
          facility_name,
          client_group_name,
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
    $query .= "ORDER BY full_name ASC";


    //$global['db']->SetFetchMode(ADODB_FETCH_ASSOC);
    // Execute query
    try {
        $query = $global['db']->Prepare($query);
        $result = $global['db']->Execute($query, $values);
    } catch (exception $e) {
        print("<pre>" . $e->getMessage() . "</pre>");
    }
    $global['db']->SetFetchMode(ADODB_FETCH_DEFAULT);
    $clients = $result->GetRows();

    // Batch to rows so they don't exceed MS Excel row limits
    $chunks = array_chunk($clients, $maxRows);

    // Release the query object
    //unset($result);
    //unset($clients);
    // Create zip file
    $zipFile = new ZipArchive();
    $zipPath = tempnam($tmpPath, "crs_");

    if ($zipFile->open($zipPath, ZIPARCHIVE::CREATE) !== TRUE) {
        echo("Export: Could not create zip file $zipPath. Check permissions.");
    }

    $i = 0;
    $wbPaths = array();

    foreach ($chunks as $batch) {

        $filename = $xlsBaseName;

        // Name the xls export
        if (count($chunks) > 1) {
            $filename .= "_part-" . ($i + 1);
        }
        $filename .= ".xls";

        // Create xls workbook 
        create_workbook($col_names, $title, $subtitle);

        // Loop through the records adding each row to the worksheet
        foreach ($batch as $row) {

            $wb->home();

            // Can't use a foreach here because array_chunk resorts the keys :(
            $wb->label($row['facility_group']);
            $wb->right();
            $wb->label($row['facility_code']);
            $wb->right();
            $wb->label($row['facility_name']);
            $wb->right();
            $wb->label($row['out_date']);
            $wb->right();
            $wb->label($row['client_group_name']);
            $wb->right();
            $wb->label($row['given_name']);
            $wb->right();
            $wb->label($row['family_name']);
            $wb->right();
            $wb->label($row['years_old']);
            $wb->right();
            $wb->label($row['opt_gender']);
            $wb->right();
            $wb->label($row['street_1']);
            $wb->right();
            $wb->label($row['street_2']);
            $wb->right();
            $wb->label($row['city']);
            $wb->right();
            $wb->label($row['state']);
            $wb->right();
            $wb->label($row['postal']);
            $wb->right();
            $wb->label($row['home_phone']);
            $wb->right();
            $wb->label($row['mobile_phone']);
            $wb->right();
            $wb->label($row['alt_phone']);
            $wb->right();

            $wb->down();
        }

        // Save the xls real path and filename to array so it can be added to zip archive
        $wbPaths[$i] = array($tmpPath . "/" . $filename, $filename);
        $wb->save($wbPaths[$i][0]);
        $i++;
    }


    // Add the files to the zip archive
    foreach ($wbPaths as $file) {
        $zipFile->addFile($file[0], $file[1]);
    }

    // Now generate the zip archive
    $zipFile->close();
    unset($zipFile);

    // Cleanup raw xls files
    foreach ($wbPaths as $file) {
        unlink($file[0]);
    }


    // Make sure the browser doesn't try to deliver a chached version

    header("Pragma: public");
    header("Expires: 0");
    header("Cache-Control: must-revalidate, post-check=0, pre-check=0");

    header("Content-Type: application/zip");
    header("Content-Disposition: attachment;filename=$zipName");
    header("Content-Transfer-Encoding: binary");
    header("Content-Length: " . filesize($zipPath));

    readfile($zipPath);
    unlink($zipPath);
}

function create_workbook($col_names, $title, $subtitle)
{

    global $wb;

    // Create excel2003 object
    $wb = new excel2003ExportHelper($title);

    // Write title information
    $wb->label($title);
    $wb->down();
    $wb->home();
    $wb->label($subtitle);
    $wb->down();
    $wb->home();
    $wb->label(date("m-d-Y h:i"));
    $wb->down();

    // Write header row
    $wb->home();
    foreach ($col_names as $col_name) {
        $wb->label($col_name);
        $wb->right();
    }
    $wb->down();
}

?>
