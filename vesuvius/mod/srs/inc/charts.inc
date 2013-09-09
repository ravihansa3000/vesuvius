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
/*
 * Function creates an chart image of age group distrobution
 *
 * @access public
 * @return void
 */

function mapmarker($count = 0)
{
    global $global;

    /* pChart library inclusions */
    include_once $global['approot'] . "3rd/pchart/class/pDraw.class.php";
    include_once $global['approot'] . "3rd/pchart/class/pImage.class.php";
    include_once $global['approot'] . "3rd/pchart/class/pData.class.php";
    include_once $global['approot'] . "3rd/pchart/class/pCache.class.php";

    // Check for markers cache directory
    $cacheDir = $global['approot'] . "www/tmp/markercache";
    if (!file_exists($cacheDir)) {
        $old = umask(0);
        mkdir($cacheDir, 0777, true);
        umask($old);
    }

    $myData = new pData();
    $myData->addPoints(array($count));
    $myCache = new pCache(array("CacheFolder" => $cacheDir));
    $ChartHash = $myCache->getHash($myData);

    if ($myCache->isInCache($ChartHash)) {
        /* If we have it, get the picture from the cache! */
        $myCache->autoOutput($ChartHash, "$cacheDir/$count-marker.png");
    } else {
        /* Create the pChart object */
        $myPicture = new pImage(25, 30, null, true);

        /* Draw a customized filled circles */
        $CircleSettings = array("R" => 0, "G" => 204, "B" => 0, "Alpha" => 100, "BorderR" => 0, "BorderG" => 0, "BorderB" => 0);
        $myPicture->drawFilledCircle(12.5, 12.5, 10, $CircleSettings);

        $myPicture->drawLine(12.5, 22.5, 12.5, 30, array("R" => 0, "G" => 0, "B" => 0, "Ticks" => 4, "Weight" => 1));

        /* Write the picture title */
        $myPicture->setFontProperties(array("FontName" => $global['approot'] . "3rd/pchart/fonts/DejaVuSans.ttf", "FontSize" => 7));
        $myPicture->drawText(12.5, 12.5, $count, array("R" => 33, "G" => 33, "B" => 33, "Align" => TEXT_ALIGN_MIDDLEMIDDLE));

        /* Render the picture (choose the best way) */
        $myPicture->autoOutput("$cacheDir/$count-marker.png");
    }
}

function top_orgs_chart()
{

    global $global;

    $query = "SELECT 
            org.name,
            COUNT(*) AS `count`
        FROM srs_person_to_org ptg
            INNER JOIN person_uuid AS p ON p.p_uuid = ptg.p_uuid
            INNER JOIN srs_organizations org ON org.id = ptg.org_id
            INNER JOIN person_status AS ps ON ps.p_uuid = ptg.p_uuid
        WHERE ps.opt_status = 'in' AND ps.isdeleted = 0 and p.incident_id = ?
        GROUP BY org.id
        ORDER BY `count` DESC";

    // Execute query
    $results = $global['db']->GetArray($query, $_SESSION['incident']);

    // Check for empty data
    if (empty($results)) {

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
    } else {

        $facilities = array();
        $counts = array();
        $nameLength = 0;

        foreach ($results as $row) {
            $facilities[] = substr($row['name'], 0, 25);
            $counts[] = $row['count'];

            if ($nameLength < strlen($row['name'])) {
                $nameLength = strlen($row['name']);
            }
        }
        $facilities[0] = (!isset($facilities[0])) ? 0 : $facilities[0];
        $counts[0] = (!isset($counts[0])) ? 0 : $counts[0];

        // Autosize
        $width = 0;
        $height = 50 + count($results) * 12;
        $lMargin = $nameLength * 4;

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
            mkdir($tmp_dir, 0777, true);
            umask($old);
        }

        /* Create your dataset object */
        $myData = new pData();
        $myData->loadPalette($global['approot'] . "3rd/pchart/palettes/nycoem.color", true);

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
        $myPicture->drawGradientArea(
            0, 0, 500, 500, DIRECTION_VERTICAL, array("StartR" => 255, "StartG" => 255, "StartB" => 255, "EndR" => 255, "EndG" => 255, "EndB" => 255, "Alpha" => 100));
        $myPicture->drawGradientArea(0, 0, 400, 300, DIRECTION_HORIZONTAL, array("StartR" => 250, "StartG" => 250, "StartB" => 250, "EndR" => 255, "EndG" => 255, "EndB" => 255, "Alpha" => 20));
        $myPicture->setFontProperties(array("FontName" => $global['approot'] . "3rd/pchart/fonts/DejaVuSans.ttf", "FontSize" => 7));

        /* Draw the chart scale */
        $myPicture->setGraphArea($lMargin, 15, 380, $height - 10);
        $myPicture->drawScale(array("CycleBackground" => true, "DrawSubTicks" => true, "GridR" => 0, "GridG" => 0, "GridB" => 0, "GridAlpha" => 10, "Pos" => SCALE_POS_TOPBOTTOM));

        /* Turn on shadow computing */
        $myPicture->setShadow(true, array("X" => 1, "Y" => 1, "R" => 0, "G" => 0, "B" => 0, "Alpha" => 10));

        /* Draw the chart */
        $myPicture->drawBarChart(array("DisplayPos" => LABEL_POS_RIGHT, "DisplayValues" => true, "Gradient" => true, "Surrounding" => 10));
    }

    $myPicture->autoOutput();
}

function top_facilities_chart()
{

    global $global;

    $query = "SELECT 
            f.facility_name,
            f.facility_resource_type_abbr,
            COUNT(*) AS `count`
        FROM srs_person_to_org ptg
            INNER JOIN person_uuid AS p ON p.p_uuid = ptg.p_uuid
            INNER JOIN crs_latest_ptf ptf ON ptf.p_uuid = ptg.p_uuid
            INNER JOIN fms_facility f ON f.facility_uuid = ptf.facility_uuid
            INNER JOIN person_status AS ps ON ps.p_uuid = ptg.p_uuid
        WHERE ps.opt_status = 'in' AND ps.isdeleted = 0 AND p.incident_id = ?
        GROUP BY f.facility_uuid
        ORDER BY `count` DESC;";

    // Execute query
    $results = $global['db']->GetArray($query, $_SESSION['incident']);

    // Check for empty data
    if (empty($results)) {

        $myPicture = nodata();
    } else {


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
        $facilities[0] = (!isset($facilities[0])) ? 0 : $facilities[0];
        $counts[0] = (!isset($counts[0])) ? 0 : $counts[0];

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
            mkdir($tmp_dir, 0777, true);
            umask($old);
        }

        /* Create your dataset object */
        $myData = new pData();
        $myData->loadPalette($global['approot'] . "3rd/pchart/palettes/nycoem.color", true);

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

        // Autosize
        $width = 0;
        $height = 50 + count($results) * 12;
        $lMargin = $nameLength * 6;

        /* Create the pChart object */
        $myPicture = new pImage(400, $height, $MyData);
        $myPicture->drawGradientArea(
            0, 0, 500, 500, DIRECTION_VERTICAL, array("StartR" => 255, "StartG" => 255, "StartB" => 255, "EndR" => 255, "EndG" => 255, "EndB" => 255, "Alpha" => 100));
        $myPicture->drawGradientArea(0, 0, 400, 300, DIRECTION_HORIZONTAL, array("StartR" => 250, "StartG" => 250, "StartB" => 250, "EndR" => 255, "EndG" => 255, "EndB" => 255, "Alpha" => 20));
        $myPicture->setFontProperties(array("FontName" => $global['approot'] . "3rd/pchart/fonts/DejaVuSans.ttf", "FontSize" => 7));

        /* Draw the chart scale */
        $myPicture->setGraphArea($lMargin, 15, 390, $height - 10);
        $myPicture->drawScale(array("CycleBackground" => true, "DrawSubTicks" => true, "GridR" => 0, "GridG" => 0, "GridB" => 0, "GridAlpha" => 10, "Pos" => SCALE_POS_TOPBOTTOM));

        /* Turn on shadow computing */
        $myPicture->setShadow(true, array("X" => 1, "Y" => 1, "R" => 0, "G" => 0, "B" => 0, "Alpha" => 10));

        /* Draw the chart */
        $myPicture->drawBarChart(array("DisplayPos" => LABEL_POS_RIGHT, "DisplayValues" => true, "Gradient" => true, "Surrounding" => 10));
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
        mkdir($tmp_dir, 0777, true);
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
        mkdir($tmp_dir, 0777, true);
        umask($old);
    }

    $query = "SELECT 
        facility_name,
        SUM(IF(opt_gender='Male',1,0)) AS male,
        SUM(IF(opt_gender='Female',1,0)) AS female
    FROM person_details AS pd
        INNER JOIN crs_latest_ptf AS ptf ON ptf.p_uuid = pd.p_uuid
        INNER JOIN fms_facility AS f ON ptf.facility_uuid = f.facility_uuid";

    if ($facility) {
        $query .= "\n WHERE ptf.facility_uuid = ?";
    }

    $query .= "\n GROUP BY f.facility_uuid;";

    // Execute query for gender count
    try {
        $query = $global['db']->Prepare($query);
        $rs = $global['db']->Execute($query, $facility);
    } catch (exception $e) {
        add_error("<pre>" . $e->getMessage() . "</pre>");
    }
    $gender = $rs->FetchRow();

    /* Create and populate the pData object */
    $MyData = new pData();
    $MyData->loadPalette($global['approot'] . "3rd/pchart/palettes/nycoem.color", true);
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

    $PieChart->draw2DPie(180, 100, array("Gradient" => true, "WriteValues" => PIE_VALUE_PERCENTAGE, "ValueR" => 77, "ValueG" => 77, "ValueB" => 77, "LabelR" => 77, "LabelG" => 77, "LabelB" => 77, "Radius" => 80, "DrawLabels" => FALSE, "DataGapAngle" => 5, "DataGapRadius" => 6, "Border" => FALSE));

    /* Write the legend box */
    $myPicture->setShadow(FALSE);
    $PieChart->drawPieLegend(10, 10, array("Alpha" => 0));

    /* Render the picture (choose the best way) */
    $myPicture->autoOutput();
}

function pie_chart($data, $radius = 80)
{
    global $global;

    // Check for empty data
    if (empty($data)) {

        /* pChart library inclusions */
        include_once $global['approot'] . "3rd/pchart/class/pDraw.class.php";
        include_once $global['approot'] . "3rd/pchart/class/pImage.class.php";

        // Set dimensions
        $width = $radius * 2;
        $height = $radius;

        /* Create the pChart object */
        $myPicture = new pImage($width, $height);

        /* Draw the background */
        $Settings = array("R" => 229, "G" => 229, "B" => 229, "Dash" => 1, "DashR" => 240, "DashG" => 240, "DashB" => 240);
        $myPicture->drawFilledRectangle(0, 0, $width, $height, $Settings);

        /* Write some text */
        $myPicture->setFontProperties(array("FontName" => $global['approot'] . "3rd/pchart/fonts/DejaVuSans.ttf", "FontSize" => 10));
        $TextSettings = array("DrawBox" => TRUE, "BoxRounded" => TRUE, "R" => 51, "G" => 103, "B" => 153, "Angle" => 0, "FontSize" => 24);
        $myPicture->drawText($width * 0.15, $height / 2, "Data Not Available", $TextSettings);
    } else {

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
            mkdir($tmp_dir, 0777, true);
            umask($old);
        }

        $points = array_values($data);
        $labels = array_keys($data);
        $maxLabel = 0;

        foreach ($labels as $label) {

            if ($maxLabel < strlen($label)) {
                $maxLabel = strlen($label);
            }
        }

        // Auto size
        $width = $radius + ($radius * 0.75);
        $height = $radius + 25;

        /* Create and populate the pData object */
        $MyData = new pData();
        $MyData->loadPalette($global['approot'] . "3rd/pchart/palettes/nycoem.color", true);
        $MyData->addPoints($points, "type");

        /* Define the absissa serie */
        $MyData->addPoints($labels, "Labels");
        $MyData->setAbscissa("Labels");

        /* Create the pChart object */
        $myPicture = new pImage($width, $height, $MyData);

        /* Set the default font properties */
        $myPicture->setFontProperties(array("FontName" => $global['approot'] . "3rd/pchart/fonts/DejaVuSans.ttf", "FontSize" => 10));

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
          "Radius" => $width * 0.25,
          "ValuePosition" => PIE_VALUE_OUTSIDE);

        $PieChart->draw3DPie(($width * 0.05) + $radius / 2, ($height / 2), $settings);

        /* Write the legend box */
        $myPicture->setShadow(FALSE);
        $PieChart->drawPieLegend(($width - ($maxLabel * 10)), 10, array("Alpha" => 0));
    }

    /* Render the picture (choose the best way) */
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
