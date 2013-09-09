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

function shn_text_srs_mapmarker()
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
    
    $count = isset($_REQUEST['c']) ? $_REQUEST['c'] : 0;

    include_once 'charts.inc';
    mapmarker($count);
}

function shn_text_srs_top_orgs_chart()
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
    top_orgs_chart();
}

function shn_text_srs_top_facilities_chart()
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

function shn_text_srs_staff_types_chart()
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
            st.description,
            COUNT(*) AS `count`
        FROM srs_person_to_staff_types ptst
            INNER JOIN srs_staff_types st ON st.id = ptst.stafftype_id 
            INNER JOIN srs_person_to_org ptg ON ptg.p_uuid = ptst.p_uuid
            INNER JOIN person_status AS ps ON ps.p_uuid = ptst.p_uuid
        WHERE ps.opt_status = 'in' 
        GROUP BY st.id";

    // Execute query
    try {
        $rs = $global['db']->Execute($query);
    } catch (exception $e) {
        print("<pre>" . $e->getMessage() . "</pre>");
    }

    $data = array();
    foreach ($rs as $row) {
        $data[$row['description']] = $row['count'];
    }

    pie_chart($data, 200);
}

function shn_text_srs_staff_status_chart()
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
            ps.opt_status,
            COUNT(*)
        FROM srs_person_to_staff_types ptst
            INNER JOIN srs_staff_types st ON st.id = ptst.stafftype_id 
            INNER JOIN srs_person_to_org ptg ON ptg.p_uuid = ptst.p_uuid
            INNER JOIN person_status AS ps ON ps.p_uuid = ptst.p_uuid
        GROUP BY ps.opt_status
        ORDER BY st.description";

    $data = array();
    // Execute query
    $rs = $global['db']->GetAssoc($query);

    if (!empty($rs)) {
        $data['Signed Out'] = isset($rs['out']) ? $rs['out'] : 0 ;
        $data['Reported In'] = isset($rs['in']) ? $rs['in'] : 0 ;
        $data['Transfered'] = isset($rs['trn']) ? $rs['trn'] : 0 ;
    }

    pie_chart($data, 200);
}

function shn_text_srs_excel_export()
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
    $xlsBaseName = "sahana_srs_export_" . date('Ymd_His');
    $zipName = $xlsBaseName . ".zip";
    $maxRows = 20000;

    $colNames = array('First Name', 'Last Name', 'Gender', 'Group Head', 'Solar Sys', 'Shelter');


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
        $query = "SELECT * FROM srs_client_search ";
        $query .= $where_clause;

        // Sort ordering
        $query .= "ORDER BY full_name ASC";

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
    if (isset($results)) {
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
    }
    // Batch to rows so they don't exceed MS Excel row limits
    $chunks = array_chunk($resultArray, $maxRows);

    // Release the query object
    unset($results);

    // Create zip file
    $zipFile = new ZipArchive();
    $zipPath = tempnam($tmpPath, "srs_");

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
        create_workbook($colNames, $title, $subtitle);

        // Status
        if ($row['opt_status'] == "in") {
            $status = "Checked In";
        } else {
            $status = "Checked Out";
        }

        // Loop through the records adding each row to the worksheet
        foreach ($batch as $row) {
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

function shn_text_srs_report_contacts_export()
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
    $subtitle = "Staff Contacts Export";
    $tmpPath = realpath(sys_get_temp_dir());
    $xlsBaseName = "sahana_srs_export_" . date('Ymd_His');
    $zipName = $xlsBaseName . ".zip";
    $maxRows = 20000;

    $colNames = array("Organization", "Staff Type", "Volunteer Type", "First", "Last",
      "Home Street 1", "Home Street 2", "Home City", "Home State", "Home Zip", "Home Phone",
      "Work Street 1", "Work Street 2", "Work City", "Work State", "Work Zip", "Work Phone");


    include_once "excel2003Export.class.inc";

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

    // Filter checked for only staff
    $logic = (isset($values[0])) ? "AND" : "WHERE";
    $where_clause .= "$logic vol_id IS NOT NULL \n";

    // Get the full row set
    $query = "SELECT * FROM srs_staff_search ";
    $query .= $where_clause;

    // Sort ordering
    $query .= "ORDER BY org_name, staff_type, full_name";


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
    $zipPath = tempnam($tmpPath, "srs_");

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
        create_workbook($colNames, $title, $subtitle);

        // Loop through the records adding each row to the worksheet
        foreach ($batch as $row) {

            $wb->home();

            // Can't use a foreach here because array_chunk resorts the keys :(
            $wb->label($row['org_name']);
            $wb->right();
            $wb->label($row['staff_type']);
            $wb->right();
            $wb->label($row['vol_type']);
            $wb->right();
            $wb->label($row['given_name']);
            $wb->right();
            $wb->label($row['family_name']);
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
            $wb->label(formatPhone($row['home_phone']));
            $wb->right();
            $wb->label($row['work_street_1']);
            $wb->right();
            $wb->label($row['work_street_2']);
            $wb->right();
            $wb->label($row['work_city']);
            $wb->right();
            $wb->label($row['work_state']);
            $wb->right();
            $wb->label($row['work_postal']);
            $wb->right();
            $wb->label(formatPhone($row['work_phone']));
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

function shn_text_srs_report_timeline_export()
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
    $subtitle = "Staff Timeline Export";
    $tmpPath = realpath(sys_get_temp_dir());
    $xlsBaseName = "sahana_srs_export_" . date('Ymd_His');
    $zipName = $xlsBaseName . ".zip";
    $maxRows = 20000;

    $colNames = array(
      "Organization", "First", "Last", "Staff Type", "Volunteer Type", "Home Phone",
      "Work Phone", "Facility", "Reported In", "Signed Out", "Duration",
      "Transfered To");

    include_once "excel2003Export.class.inc";

    // Org search filter
    $orgId = !empty($_REQUEST['id']) ? $_REQUEST['id'] : 0;

    // Get the full row set
    $query = "SELECT 
            org_name,
            staff_type,
            vol_type,
            p_uuid,
            given_name,
            family_name,
            home_phone,
            work_phone
        FROM srs_staff_search
            WHERE org_id = ?
        GROUP BY org_id, p_uuid
        ORDER BY org_name, full_name";

    // Execute query
    try {
        $query = $global['db']->Prepare($query);
        $result = $global['db']->Execute($query, $orgId);
    } catch (exception $e) {
        print("<pre>" . $e->getMessage() . "</pre>");
    }
    $global['db']->SetFetchMode(ADODB_FETCH_DEFAULT);
    $clients = $result->GetRows();

    // Batch to rows so they don't exceed MS Excel row limits
    $chunks = array_chunk($clients, $maxRows);

    // Create zip file
    $zipFile = new ZipArchive();
    $zipPath = tempnam($tmpPath, "srs_");

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
        create_workbook($colNames, $title, $subtitle);

        // Loop through the records adding each row to the worksheet
        foreach ($batch as $row) {

            // Collect timelines
            $timelines = getStaffTimeline($row['p_uuid']);

            // Write sub header row

            foreach ($timelines as $timeline) {
                $wb->home();

                $wb->label($row['org_name']);
                $wb->right();

                $wb->label($row['given_name']);
                $wb->right();
                $wb->label($row['family_name']);
                $wb->right();

                $wb->label($row['staff_type']);
                $wb->right();
                $wb->label($row['vol_type']);
                $wb->right();

                $wb->label(!empty($row['home_phone']) ? formatPhone($row['home_phone']) : "");
                $wb->right();
                $wb->label(!empty($row['work_phone']) ? formatPhone($row['work_phone']) : "");

                // Begin timelines

                $wb->right();
                $wb->label($timeline['facility_name']);
                $wb->right();
                $wb->label(date("F j, Y H:i", strtotime($timeline['in_date'])));
                $wb->right();
                $wb->label(!empty($timeline['out_date']) ? date("F j, Y H:i", strtotime($timeline['out_date'])) : "Signed-in");
                $wb->right();

                if (!empty($timeline['out_date'])) {

                    $inDate = strtotime($timeline['in_date']);
                    $outDate = strtotime($timeline['out_date']);

                    if ($inDate <= $outDate) {
                        $wb->label(date_duration($timeline['in_date'], $timeline['out_date'], false));
                    } else {
                        $wb->label("Error: in > out time");
                    }
                } else {
                    $wb->label(date_duration($timeline['in_date'], null, false));
                }

                $wb->right();
                $wb->label(!empty($timeline['dest_name']) ? $timeline['dest_name'] : "");
                $wb->down();
            }
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

function create_workbook($colNames, $title, $subtitle)
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
    foreach ($colNames as $col_name) {
        $wb->label($col_name);
        $wb->right();
    }
    $wb->down();
}

?>
