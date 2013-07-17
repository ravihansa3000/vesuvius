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
/*
 * Function creates an chart image of age group distrobution
 *
 * @access public
 * @return void
 */
function age_dist_chart()
{

    global $global;

    // Now the actual query
    $query = "SELECT 
            years_old, 
            COUNT(*) `count` 
        FROM person_details pd
            INNER JOIN person_uuid AS p ON p.p_uuid = pd.p_uuid
            INNER JOIN crs_person_to_group ptg ON pd.p_uuid = ptg.p_uuid_1
            INNER JOIN person_status AS ps ON ps.p_uuid = pd.p_uuid
        WHERE p.incident_id = ? 
            AND ps.opt_status = 'in' AND ps.isdeleted = 0 AND years_old > 0 AND years_old < 120
        GROUP BY years_old
        ORDER BY years_old ASC;";

    // Execute query
    try {
        $results = $global['db']->Execute($query, $_SESSION['incident']);
    } catch (exception $e) {
        add_error("<pre>" . $e->getMessage() . "</pre>");
    }


    $ages = array();
    $counts = array();
    foreach ($results as $row) {
        $ages[] = $row['years_old'];
        $counts[] = $row['count'];
    }

    if (count($ages) < 1) {

        $myPicture = nodata();
    } else {
        /* pChart library inclusions */
        include_once $global['approot'] . "3rd/pchart/class/pData.class.php";
        include_once $global['approot'] . "3rd/pchart/class/pCache.class.php";
        include_once $global['approot'] . "3rd/pchart/class/pDraw.class.php";
        include_once $global['approot'] . "3rd/pchart/class/pImage.class.php";
        include_once $global['approot'] . "3rd/pchart/class/pScatter.class.php";

        // Check for images tmp directory
        $tmp_dir = $global['approot'] . "www/tmp/chartcache";
        if (!file_exists($tmp_dir)) {
            $old = umask(0);
            mkdir($tmp_dir, 0777, TRUE);
            umask($old);
        }

        /* Create your dataset object */
        $myData = new pData();
        $myData->loadPalette($global['approot'] . "3rd/pchart/palettes/nycoem.color", TRUE);

        /* Create the cache object */
        $myCache = new pCache(array("CacheFolder" => $tmp_dir));

        $myData->setAxisName(0, "Ages");
        $myData->setAxisXY(0, AXIS_X);
        $myData->setAxisPosition(0, AXIS_POSITION_BOTTOM);

        /* Create the Y axis and the binded series */
        $myData->addPoints($ages, "Ages");
        $myData->addPoints($counts, "Counts");
        $myData->setSerieOnAxis("Counts", 1);
        $myData->setAxisName(1, "Number of People");
        $myData->setAxisXY(1, AXIS_Y);
        $myData->setAxisPosition(1, AXIS_POSITION_LEFT);

        /* Create the 1st scatter chart binding */
        $myData->setScatterSerie("Ages", "Counts", 0);
        $myData->setScatterSerieDescription(0, "Ages");

        /* Compute the hash linked to the chart data */
        $ChartHash = $myCache->getHash($myData);


        /* Create the pChart object */
        $myPicture = new pImage(300, 300, $myData);

        /* Set the default font */
        $myPicture->setFontProperties(
            array("FontName" => $global['approot'] . "3rd/pchart/fonts/DejaVuSans.ttf",
              "FontSize" => 8)
        );

        /* Set the graph area */
        $myPicture->setGraphArea(40, 10, 295, 250);

        /* Create the Scatter chart object */
        $myScatter = new pScatter($myPicture, $myData);

        /* Draw the scale */
        $scaleSettings = array("XMargin" => 15, "YMargin" => 15, "Floating" => TRUE, "GridR" => 200, "GridG" => 200, "GridB" => 200, "DrawSubTicks" => TRUE, "CycleBackground" => TRUE);
        $myScatter->drawScatterScale($scaleSettings);

        /* Draw the legend */
        $myScatter->drawScatterLegend(230, 10, array("Mode" => LEGEND_HORIZONTAL, "Style" => LEGEND_NOBORDER));

        /* Draw a scatter plot chart */
        $myPicture->Antialias = TRUE;
        $myScatter->drawScatterPlotChart();
    }
    $myPicture->autoOutput();
}

function top_facilities_chart()
{

    global $global;

    $query = "SELECT 
        f.facility_resource_type_abbr,
        f.facility_name, 
        count(*) AS `count`
    FROM crs_latest_ptf AS ptf
        INNER JOIN person_uuid AS p ON p.p_uuid = ptf.p_uuid
        INNER JOIN fms_facility AS f ON ptf.facility_uuid = f.facility_uuid
        INNER JOIN crs_person_to_group ptg ON ptf.p_uuid = ptg.p_uuid_1
        INNER JOIN person_status AS ps ON ps.p_uuid = ptf.p_uuid
      WHERE ps.opt_status = 'in' AND ps.isdeleted = 0 AND p.incident_id = ?
    GROUP BY ptf.facility_uuid
    ORDER BY `count` DESC, f.facility_name
    LIMIT 10";

    // Execute query
    $results = $global['db']->GetArray($query, $_SESSION['incident']);

    $facilities = array();
    $counts = array();
    $nameLength = 0;

    foreach ($results as $row) {
        $facilities[] = $row['facility_name'] . " - " . $row['facility_resource_type_abbr'];
        $counts[] = $row['count'];

        if ($nameLength < strlen($row['facility_name'] . " - " . $row['facility_resource_type_abbr'])) {
            $nameLength = strlen($row['facility_name'] . " - " . $row['facility_resource_type_abbr']);
        }
    }


    if (count($facilities) < 1) {

        $myPicture = nodata();
    } else {

        /* pChart library inclusions */
        include_once $global['approot'] . "3rd/pchart/class/pData.class.php";
        include_once $global['approot'] . "3rd/pchart/class/pCache.class.php";
        include_once $global['approot'] . "3rd/pchart/class/pDraw.class.php";
        include_once $global['approot'] . "3rd/pchart/class/pImage.class.php";
        include_once $global['approot'] . "3rd/pchart/class/pScatter.class.php";

        // Check for images tmp directory
        $tmp_dir = $global['approot'] . "www/tmp/chartcache";
        if (!file_exists($tmp_dir)) {
            $old = umask(0);
            mkdir($tmp_dir, 0777, TRUE);
            umask($old);
        }

        // Autosize
        $width = 0;
        $height = 50 + count($results) * 15;
        $lMargin = $nameLength * 6;

        /* Create your dataset object */
        $myData = new pData();
        $myData->loadPalette($global['approot'] . "3rd/pchart/palettes/nycoem.color", TRUE);

        /* Create the cache object */
        $myCache = new pCache(array("CacheFolder" => $tmp_dir));

        /* Create and populate the pData object */
        $MyData = new pData();
        $MyData->addPoints($counts, "Count");
        $MyData->setAxisName(0, "Client Registration Count");
        $MyData->addPoints($facilities, "Facilities");
        $MyData->setSerieDescription("Facilities", "Facilities");
        $MyData->setAbscissa("Facilities");
        //$MyData->setAbscissaName("Facilities");
        $MyData->setAxisDisplay(0, AXIS_FORMAT_METRIC, 1);


        /* Create the pChart object */
        $myPicture = new pImage(400, $height, $MyData);
        $myPicture->setFontProperties(array("FontName" => $global['approot'] . "3rd/pchart/fonts/DejaVuSans.ttf", "FontSize" => 7));

        /* Draw the chart scale */
        $myPicture->setGraphArea($lMargin, 15, 390, $height - 10);
        $myPicture->drawScale(array("CycleBackground" => TRUE, "DrawSubTicks" => TRUE, "GridR" => 0, "GridG" => 0, "GridB" => 0, "GridAlpha" => 10, "Pos" => SCALE_POS_TOPBOTTOM));

        /* Draw the chart */
        $myPicture->drawBarChart(array("DisplayPos" => LABEL_POS_RIGHT, "DisplayValues" => TRUE, "Gradient" => TRUE, "Surrounding" => 10));
    }

    $myPicture->autoOutput();
}

function progress_chart($percent)
{
    global $global;

    /* pChart library inclusions */
    include_once $global['approot'] . "3rd/pchart/class/pData.class.php";
    include_once $global['approot'] . "3rd/pchart/class/pCache.class.php";
    include_once $global['approot'] . "3rd/pchart/class/pDraw.class.php";
    include_once $global['approot'] . "3rd/pchart/class/pImage.class.php";
    include_once $global['approot'] . "3rd/pchart/class/pScatter.class.php";

    // Check for images tmp directory
    $tmp_dir = $global['approot'] . "www/tmp/chartcache";
    if (!file_exists($tmp_dir)) {
        $old = umask(0);
        mkdir($tmp_dir, 0777, TRUE);
        umask($old);
    }

    /* Create the pChart object */
    $myPicture = new pImage(101, 12);

    /* Set the font & shadow options */
    $myPicture->setFontProperties(array("FontName" => $global['approot'] . "3rd/pchart/fonts/DejaVuSans.ttf", "FontSize" => 8));

    /* Draw a progress bar */
    $progressOptions = array("Width" => 100, "Height" => 10, "R" => 188, "G" => 224, "B" => 46, "Surrounding" => 20, "BoxBorderR" => 0, "BoxBorderG" => 0, "BoxBorderB" => 0, "BoxBackR" => 255, "BoxBackG" => 255, "BoxBackB" => 255, "RFade" => 209, "GFade" => 31, "BFade" => 27);
    $myPicture->drawProgress(0, 0, $percent, $progressOptions);

    /* Render the picture (choose the best way) */
    $myPicture->autoOutput("pictures/example.drawProgress.png");
}

function agetype_pie_chart($facility = null)
{
    global $global;

    /* pChart library inclusions */
    include_once $global['approot'] . "3rd/pchart/class/pData.class.php";
    include_once $global['approot'] . "3rd/pchart/class/pCache.class.php";
    include_once $global['approot'] . "3rd/pchart/class/pDraw.class.php";
    include_once $global['approot'] . "3rd/pchart/class/pImage.class.php";
    include_once $global['approot'] . "3rd/pchart/class/pPie.class.php";

    // Check for images tmp directory
    $tmp_dir = $global['approot'] . "www/tmp/chartcache";
    if (!file_exists($tmp_dir)) {
        $old = umask(0);
        mkdir($tmp_dir, 0777, TRUE);
        umask($old);
    }

    $query = "SELECT 
        SUM(IF(years_old BETWEEN 0 AND 3, 1, 0)) AS infants,
        SUM(IF(years_old BETWEEN 4 AND 17, 1, 0)) AS children,
        SUM(IF(years_old BETWEEN 18 AND 64, 1, 0)) AS adults,
        SUM(IF(years_old BETWEEN 65 AND 150, 1, 0)) AS seniors,
        SUM(IF(years_old IS NULL, 1, 0)) AS unk
    FROM person_details AS pd
        INNER JOIN person_uuid AS p ON p.p_uuid = pd.p_uuid
        INNER JOIN crs_latest_ptf AS ptf ON ptf.p_uuid = pd.p_uuid
        INNER JOIN fms_facility AS f ON ptf.facility_uuid = f.facility_uuid
        INNER JOIN crs_person_to_group ptg ON ptf.p_uuid = ptg.p_uuid_1
        INNER JOIN person_status AS ps ON ps.p_uuid = ptf.p_uuid
        WHERE ps.isdeleted = 0 AND p.incident_id = ?";

    if ($facility) {
        $query .= "\n  AND ps.opt_status = 'in' AND ps.isdeleted = 0 AND ptf.facility_uuid = ?";
    }
    $query .= "\n GROUP BY f.facility_uuid";

    // Execute query age group count
    $ag = $global['db']->GetRow($query, array($_SESSION['incident'], $facility));

    if (empty($ag)) {

        $myPicture = nodata();
    } else {

        // Pie charts don't like zero values
        if ($ag['infants'] > 0) {
            $keys[] = 'Infants';
            $values[] = $ag['infants'];
        }

        if ($ag['children'] > 0) {
            $keys[] = 'Children';
            $values[] = $ag['children'];
        }

        if ($ag['adults'] > 0) {
            $keys[] = 'Adults';
            $values[] = $ag['adults'];
        }

        if ($ag['seniors'] > 0) {
            $keys[] = 'Seniors';
            $values[] = $ag['seniors'];
        }

        if ($ag['unk'] > 0) {
            $keys[] = 'Unknown';
            $values[] = $ag['unk'];
        }


        /* Create and populate the pData object */
        $MyData = new pData();
        $MyData->loadPalette($global['approot'] . "3rd/pchart/palettes/nycoem.color", TRUE);
        $MyData->addPoints($values, "type");
        $MyData->setSerieDescription("type", "Age Type");

        /* Define the absissa serie */
        $MyData->addPoints($keys, "Labels");
        $MyData->setAbscissa("Labels");

        /* Create the pChart object */
        $myPicture = new pImage(300, 230, $MyData);

        /* Set the default font properties */
        $myPicture->setFontProperties(array("FontName" => $global['approot'] . "3rd/pchart/fonts/DejaVuSans.ttf", "FontSize" => 8));

        /* Enable shadow computing */
        //$myPicture->setShadow(TRUE, array("X" => 2, "Y" => 2, "R" => 0, "G" => 0, "B" => 0, "Alpha" => 50));

        /* Create the pPie object */
        $PieChart = new pPie($myPicture, $MyData);
        $settings = array(
          "WriteValues" => true,
          "DataGapAngle" => 10,
          "DataGapRadius" => 6,
          "Border" => true,
          "ValueR" => 77,
          "ValueG" => 77,
          "ValueB" => 77,
          "ValueAlpha" => 100,
          "ValuePosition" => PIE_VALUE_OUTSIDE
        );
        $PieChart->draw3DPie(175, 100, $settings);

        /* Write the legend box */
        $myPicture->setShadow(FALSE);
        $PieChart->drawPieLegend(10, 10, array("Alpha" => 0));
    }
    $myPicture->autoOutput();
}

function gender_pie_chart($facility = null)
{
    global $global;

    /* pChart library inclusions */
    include_once $global['approot'] . "3rd/pchart/class/pData.class.php";
    include_once $global['approot'] . "3rd/pchart/class/pCache.class.php";
    include_once $global['approot'] . "3rd/pchart/class/pDraw.class.php";
    include_once $global['approot'] . "3rd/pchart/class/pImage.class.php";
    include_once $global['approot'] . "3rd/pchart/class/pPie.class.php";

// Check for images tmp directory
    $tmp_dir = $global['approot'] . "www/tmp/chartcache";
    if (!file_exists($tmp_dir)) {
        $old = umask(0);
        mkdir($tmp_dir, 0777, TRUE);
        umask($old);
    }

    $query = "SELECT 
        facility_name,
        SUM(IF(opt_gender='Male',1,0)) AS male,
        SUM(IF(opt_gender='Female',1,0)) AS female
    FROM person_details AS pd
        INNER JOIN person_uuid AS p ON p.p_uuid = pd.p_uuid
        INNER JOIN crs_latest_ptf AS ptf ON ptf.p_uuid = pd.p_uuid
        INNER JOIN fms_facility AS f ON ptf.facility_uuid = f.facility_uuid
        INNER JOIN crs_person_to_group ptg ON ptf.p_uuid = ptg.p_uuid_1
        INNER JOIN person_status AS ps ON ps.p_uuid = pd.p_uuid
        WHERE ps.isdeleted = 0 AND p.incident_id = ?";

    if ($facility) {
        $query .= "\n AND ptf.facility_uuid = ? AND ps.opt_status = 'in'";
    }

    $query .= "\n GROUP BY f.facility_uuid;";

    // Execute query for gender count
    $rs = $global['db']->Execute($query, array($_SESSION['incident'], $facility));

    $gender = $rs->FetchRow();

    if (empty($gender)) {
        $myPicture = nodata();
    } else {

        /* Create and populate the pData object */
        $MyData = new pData();
        //$MyData->loadPalette($global['approot'] . "3rd/pchart/palettes/nycoem.color", TRUE);
        $MyData->addPoints(array($gender['male'], $gender['female']), "type");
        $MyData->setSerieDescription("type", "Age Type");

        /* Define the absissa serie */
        $MyData->addPoints(array("Male", "Female"), "Labels");
        $MyData->setAbscissa("Labels");

        /* Create the pChart object */
        $myPicture = new pImage(300, 230, $MyData);


        /* Set the default font properties */
        $myPicture->setFontProperties(array("FontName" => $global['approot'] . "3rd/pchart/fonts/DejaVuSans.ttf", "FontSize" => 8));

        /* Create the pPie object */
        $PieChart = new pPie($myPicture, $MyData);
        $settings = array(
          "WriteValues" => true,
          "DataGapAngle" => 10,
          "DataGapRadius" => 6,
          "Border" => true,
          "ValueR" => 77,
          "ValueG" => 77,
          "ValueB" => 77,
          "ValueAlpha" => 100,
          "ValuePosition" => PIE_VALUE_OUTSIDE
        );
        $PieChart->draw3DPie(175, 100, $settings);

        /* Write the legend box */
        $myPicture->setShadow(FALSE);
        $PieChart->drawPieLegend(10, 10, array("Alpha" => 0));
    }
    $myPicture->autoOutput();
}

function pie_chart($data, $radius = 80)
{
    global $global;

    /* pChart library inclusions */
    include_once $global['approot'] . "3rd/pchart/class/pData.class.php";
    include_once $global['approot'] . "3rd/pchart/class/pCache.class.php";
    include_once $global['approot'] . "3rd/pchart/class/pDraw.class.php";
    include_once $global['approot'] . "3rd/pchart/class/pImage.class.php";
    include_once $global['approot'] . "3rd/pchart/class/pPie.class.php";

    // Check for images tmp directory
    $tmp_dir = $global['approot'] . "www/tmp/chartcache";
    if (!file_exists($tmp_dir)) {
        $old = umask(0);
        mkdir($tmp_dir, 0777, TRUE);
        umask($old);
    }


    if (empty($data)) {
        $myPicture = nodata();
    } else {

        $points = array_values($data);
        $labels = array_keys($data);

        /* Create and populate the pData object */
        $MyData = new pData();
        $MyData->loadPalette($global['approot'] . "3rd/pchart/palettes/nycoem.color", TRUE);
        $MyData->addPoints($points, "type");

        /* Define the absissa serie */
        $MyData->addPoints($labels, "Labels");
        $MyData->setAbscissa("Labels");

        /* Create the pChart object */
        $myPicture = new pImage((($radius + 50) * 2), (($radius + 25) * 2), $MyData);

        /* Set the default font properties */
        $myPicture->setFontProperties(array("FontName" => $global['approot'] . "3rd/pchart/fonts/DejaVuSans.ttf", "FontSize" => 8));

        /* Create the pPie object */
        $PieChart = new pPie($myPicture, $MyData);
        $settings = array(
          "WriteValues" => true,
          "DataGapAngle" => 10,
          "DataGapRadius" => 6,
          "Border" => true,
          "ValueR" => 77,
          "ValueG" => 77,
          "ValueB" => 77,
          "ValueAlpha" => 100,
          "ValuePosition" => PIE_VALUE_OUTSIDE
        );
        $PieChart->draw3DPie(105, 100, $settings);

        /* Write the legend box */
        $myPicture->setShadow(FALSE);
        $PieChart->drawPieLegend((($radius * 2) + 30), 10, array("Alpha" => 0));
    }
    $myPicture->autoOutput();
}

function top_states_chart($facility = NULL)
{

    global $global;

    $query = "SELECT 
        contact_value AS state,
        COUNT(*) AS 'count'
    FROM person_uuid AS p
        INNER JOIN contact AS c on c.p_uuid = p.p_uuid 
        INNER JOIN crs_latest_ptf AS ptf ON ptf.p_uuid = p.p_uuid
        INNER JOIN fms_facility AS f ON ptf.facility_uuid = f.facility_uuid
        INNER JOIN crs_person_to_group ptg ON ptf.p_uuid = ptg.p_uuid_1
        INNER JOIN person_status AS ps ON ps.p_uuid = p.p_uuid
    WHERE ps.opt_status = 'in' AND ps.isdeleted = 0 AND p.incident_id = ?
        AND c.opt_contact_type = 'state'";

    if ($facility) {
        $query .= "\n AND f.facility_uuid = ?";
    }
    $query .= "GROUP BY contact_value
    ORDER BY count DESC, state ASC
    LIMIT 5;";

    // Execute query
    $rs = $global['db']->Execute($query, array($_SESSION['incident'], $facility));

    $states = array();
    $counts = array();
    foreach ($rs as $row) {
        $states[] = $row['state'];
        $counts[] = $row['count'];
    }

    if (count($states) < 1) {

        $myPicture = nodata();
    } else {

        /* pChart library inclusions */
        include_once $global['approot'] . "3rd/pchart/class/pData.class.php";
        include_once $global['approot'] . "3rd/pchart/class/pCache.class.php";
        include_once $global['approot'] . "3rd/pchart/class/pDraw.class.php";
        include_once $global['approot'] . "3rd/pchart/class/pImage.class.php";
        include_once $global['approot'] . "3rd/pchart/class/pScatter.class.php";

// Check for images tmp directory
        $tmp_dir = $global['approot'] . "www/tmp/chartcache";
        if (!file_exists($tmp_dir)) {
            $old = umask(0);
            mkdir($tmp_dir, 0777, TRUE);
            umask($old);
        }

        /* Create and populate the pData object */
        $MyData = new pData();
        $MyData->loadPalette($global['approot'] . "3rd/pchart/palettes/nycoem.color", TRUE);
        $MyData->addPoints($counts, "Client Counts");
        $MyData->addPoints($states, "States");
        $MyData->setSerieDescription("States", "States");
        $MyData->setAbscissa("States");

        /* Create the pChart object */
        $myPicture = new pImage(300, 230, $MyData);

        /* Set the default font */
        $myPicture->setFontProperties(array("FontName" => $global['approot'] . "3rd/pchart/fonts/DejaVuSans.ttf", "FontSize" => 8));

        /* Define the chart area */
        $myPicture->setGraphArea(60, 25, 290, 200);

        /* Draw the scale */
        $scaleSettings = array("GridR" => 200, "GridG" => 200, "GridB" => 200, "DrawSubTicks" => TRUE, "CycleBackground" => TRUE);
        $myPicture->drawScale($scaleSettings);

        /* Write the chart legend */
        $myPicture->drawLegend(180, 12, array("Style" => LEGEND_NOBORDER, "Mode" => LEGEND_HORIZONTAL));

        /* Turn on shadow computing */
        //$myPicture->setShadow(TRUE, array("X" => 1, "Y" => 1, "R" => 0, "G" => 0, "B" => 0, "Alpha" => 10));

        /* Draw the chart */
        //$myPicture->setShadow(TRUE, array("X" => 1, "Y" => 1, "R" => 0, "G" => 0, "B" => 0, "Alpha" => 10));
        $settings = array("Gradient" => TRUE, "DisplayPos" => LABEL_POS_TOP, "DisplayValues" => TRUE, "DisplayR" => 77, "DisplayG" => 77, "DisplayB" => 77, "DisplayShadow" => TRUE, "Surrounding" => 10);
        $myPicture->drawBarChart($settings);
    }
    $myPicture->autoOutput();
}

function top_zipcodes_chart($facility = NULL)
{

    global $global;

    $query = "SELECT 
            c.contact_value AS postal,
            COUNT(*) AS 'count'
        FROM crs_person_to_group ptg
            INNER JOIN person_uuid AS p ON p.p_uuid = ptg.p_uuid_1
            INNER JOIN contact AS c on c.p_uuid = ptg.p_uuid_1
            INNER JOIN person_status AS ps ON ps.p_uuid = ptg.p_uuid_1
        WHERE ps.opt_status = 'in' AND ps.isdeleted = 0 AND p.incident_id = ?
            AND c.opt_contact_type = 'postal' AND c.contact_value != ' '";

    if ($facility) {
        $query .= "\n AND f.facility_uuid = ?";
    }
    $query .= "GROUP BY contact_value
    ORDER BY count DESC, postal ASC
    LIMIT 10;";

    // Execute query
    $results = $global['db']->GetArray($query, array($_SESSION['incident'], $facility));


    $facilities = array();
    $counts = array();
    $nameLength = 0;
    foreach ($results as $row) {
        $facilities[] = substr($row['postal'], 0, 25);
        $counts[] = $row['count'];

        if ($nameLength < strlen($row['postal'])) {
            $nameLength = strlen($row['postal']);
        }
    }

    if (count($facilities) < 1) {

        $myPicture = nodata();
    } else {


        // Autosize
        $width = 0;
        $height = 50 + count($results) * 15;
        $lMargin = $nameLength * 6;

        /* pChart library inclusions */
        include_once $global['approot'] . "3rd/pchart/class/pData.class.php";
        include_once $global['approot'] . "3rd/pchart/class/pCache.class.php";
        include_once $global['approot'] . "3rd/pchart/class/pDraw.class.php";
        include_once $global['approot'] . "3rd/pchart/class/pImage.class.php";
        include_once $global['approot'] . "3rd/pchart/class/pScatter.class.php";

        // Check for images tmp directory
        $tmp_dir = $global['approot'] . "www/tmp/chartcache";
        if (!file_exists($tmp_dir)) {
            $old = umask(0);
            mkdir($tmp_dir, 0777, TRUE);
            umask($old);
        }

        /* Create your dataset object */
        $myData = new pData();
        $myData->loadPalette($global['approot'] . "3rd/pchart/palettes/nycoem.color", TRUE);

        /* Create the cache object */
        $myCache = new pCache(array("CacheFolder" => $tmp_dir));

        /* Create and populate the pData object */
        $MyData = new pData();
        $MyData->addPoints($counts, "Count");
        $MyData->setAxisName(0, "Client Registration Count");
        $MyData->addPoints($facilities, "Facilities");
        $MyData->setSerieDescription("Facilities", "Facilities");
        $MyData->setAbscissa("Facilities");
        //$MyData->setAbscissaName("Facilities");
        $MyData->setAxisDisplay(0, AXIS_FORMAT_METRIC, 1);


        /* Create the pChart object */
        $myPicture = new pImage(400, $height, $MyData);
        $myPicture->setFontProperties(array("FontName" => $global['approot'] . "3rd/pchart/fonts/DejaVuSans.ttf", "FontSize" => 7));

        /* Draw the chart scale */
        $myPicture->setGraphArea(75, 15, 380, $height - 10);
        $myPicture->drawScale(array("CycleBackground" => TRUE, "DrawSubTicks" => TRUE, "GridR" => 0, "GridG" => 0, "GridB" => 0, "GridAlpha" => 10, "Pos" => SCALE_POS_TOPBOTTOM));

        /* Draw the chart */
        $myPicture->drawBarChart(array("DisplayPos" => LABEL_POS_RIGHT, "DisplayValues" => TRUE, "Gradient" => TRUE, "Surrounding" => 10));
    }
    $myPicture->autoOutput();
}

function top_languages_chart($facility = NULL)
{

    global $global;

    $query = "SELECT 
        l.language,
        COUNT(*) AS 'count'
    FROM person_uuid AS p
        INNER JOIN crs_person_to_language AS ptl ON ptl.p_uuid = p.p_uuid 
        INNER JOIN crs_language AS l ON l.id = ptl.language
        INNER JOIN crs_latest_ptf AS ptf ON ptf.p_uuid = p.p_uuid
        INNER JOIN crs_person_to_group ptg ON ptf.p_uuid = ptg.p_uuid_1
        INNER JOIN person_status AS ps ON ps.p_uuid = p.p_uuid
    WHERE ps.isdeleted = 0 AND p.incident_id = ?";

    if ($facility) {
        $query .= "\n AND ptf.facility_uuid = ?";
    }
    $query .= "\n GROUP BY ptl.language
    ORDER BY count DESC, l.language ASC
    LIMIT 5;";

    // Execute query
    $rs = $global['db']->Execute($query, array($_SESSION['incident'], $facility));

    $languages = array();
    $counts = array();
    foreach ($rs as $row) {
        $languages[] = $row['language'];
        $counts[] = $row['count'];
    }

    if (count($languages) < 1) {

        $myPicture = nodata();
    } else {

        /* pChart library inclusions */
        include_once $global['approot'] . "3rd/pchart/class/pData.class.php";
        include_once $global['approot'] . "3rd/pchart/class/pCache.class.php";
        include_once $global['approot'] . "3rd/pchart/class/pDraw.class.php";
        include_once $global['approot'] . "3rd/pchart/class/pImage.class.php";
        include_once $global['approot'] . "3rd/pchart/class/pScatter.class.php";

        // Check for images tmp directory
        $tmp_dir = $global['approot'] . "www/tmp/chartcache";
        if (!file_exists($tmp_dir)) {
            $old = umask(0);
            mkdir($tmp_dir, 0777, TRUE);
            umask($old);
        }

        /* Create and populate the pData object */
        /* Create your dataset object */
        $MyData = new pData();
        $MyData->loadPalette($global['approot'] . "3rd/pchart/palettes/nycoem.color", TRUE);
        $MyData->addPoints($counts, "Client Counts");
        $MyData->addPoints($languages, "Language");
        $MyData->setSerieDescription("Language", "Language");
        $MyData->setAbscissa("Language");

        /* Create the pChart object */
        $myPicture = new pImage(300, 230, $MyData);

        /* Set the default font */
        $myPicture->setFontProperties(array("FontName" => $global['approot'] . "3rd/pchart/fonts/DejaVuSans.ttf", "FontSize" => 8));

        /* Define the chart area */
        $myPicture->setGraphArea(60, 25, 290, 200);

        /* Draw the scale */
        $scaleSettings = array("GridR" => 200, "GridG" => 200, "GridB" => 200, "DrawSubTicks" => TRUE, "CycleBackground" => TRUE);
        $myPicture->drawScale($scaleSettings);

        /* Write the chart legend */
        $myPicture->drawLegend(180, 12, array("Style" => LEGEND_NOBORDER, "Mode" => LEGEND_HORIZONTAL));

        /* Draw the chart */
        //$myPicture->setShadow(TRUE, array("X" => 1, "Y" => 1, "R" => 0, "G" => 0, "B" => 0, "Alpha" => 10));
        $settings = array("Gradient" => TRUE, "DisplayPos" => LABEL_POS_TOP, "DisplayValues" => TRUE, "DisplayR" => 77, "DisplayG" => 77, "DisplayB" => 77, "DisplayShadow" => TRUE, "Surrounding" => 10);
        $myPicture->drawBarChart($settings);
    }
    $myPicture->autoOutput();
}

function nodata()
{
    global $global;

    /* pChart library inclusions */
    include_once $global['approot'] . "3rd/pchart/class/pDraw.class.php";
    include_once $global['approot'] . "3rd/pchart/class/pImage.class.php";

    // Set dimensions
    $width = 400;
    $height = 200;

    /* Create the pChart object */
    $myPicture = new pImage($width, $height);

    /* Draw the background */
    $Settings = array("R" => 229, "G" => 229, "B" => 229, "Dash" => 1, "DashR" => 240, "DashG" => 240, "DashB" => 240);
    $myPicture->drawFilledRectangle(0, 0, $width, $height, $Settings);

    /* Write some text */
    $myPicture->setFontProperties(array("FontName" => $global['approot'] . "3rd/pchart/fonts/DejaVuSans.ttf", "FontSize" => 10));
    $TextSettings = array("DrawBox" => TRUE, "BoxRounded" => TRUE, "R" => 51, "G" => 103, "B" => 153, "Angle" => 0, "FontSize" => 24);
    $myPicture->drawText($width * 0.15, $height / 2, "Data Not Available", $TextSettings);

    return $myPicture;
}

?>
