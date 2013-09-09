<?php

global $global, $conf;

// Get the user's event id from the session
$eventId = $_SESSION['incident'];

// Query for the client locations
$query = "SELECT 
            f.facility_uuid,
            f.facility_name,
            f.facility_resource_type_abbr,
            f.work_phone,
            f.street_1,
            f.street_2,
            f.city,
            f.state,
            f.postal_code,
            f.latitude,
            f.longitude,
            f.facility_capacity,
            count(*) AS `count`
        FROM fms_person_to_facility AS ptf
            INNER JOIN person_uuid AS p ON ptf.p_uuid = p.p_uuid
            INNER JOIN fms_facility AS f ON ptf.facility_uuid = f.facility_uuid
            INNER JOIN crs_person_to_group ptg ON ptf.p_uuid = ptg.p_uuid_1
            INNER JOIN person_status AS ps ON ps.p_uuid = ptf.p_uuid
        WHERE ps.opt_status = 'in'
            AND p.incident_id = ?
        GROUP BY ptf.facility_uuid";

try {
    $results = $global['db']->Execute($query, $eventId);
} catch (exception $e) {
    add_error("<pre>" . $e->getMessage() . "</pre>");
}

$response = array();
$i = 0;
foreach ($results as $row) {

    // Calculate capacity usage 

    $pct_full = 100 - round(($row['facility_capacity'] - $row['count']) / $row['facility_capacity'] * 100, 0);

    // Assign pin color based on percent full
    if ($pct_full > 100) {
        $icon = 'theme/' . $conf['theme'] . '/img/map-push-pin-purple.png';
    } elseif ($pct_full >= 90) {
        $icon = 'theme/' . $conf['theme'] . '/img/map-push-pin-red.png';
    } elseif ($pct_full >= 50) {
        $icon = 'theme/' . $conf['theme'] . '/img/map-push-pin-orange.png';
    } elseif ($pct_full > 0) {
        $icon = 'theme/' . $conf['theme'] . '/img/map-push-pin-green.png';
    } else {
        $icon = 'theme/' . $conf['theme'] . '/img/map-push-pin-blue.png';
    }

    // Make information box
    $info = "<h3>" . $row['facility_name'] . "</h3>";

    $info .= "<p>" . $row['street_1'] . "<br/>";
    if (!empty($row['street_2'])) {
        $info .= $row['street_2'] . "<br />";
    }

    $info .= $row['city'] . ", " . $row['state'] . " " . $row['postal_code'] . "<br />";
    $info .= formatPhone($row['work_phone']) . "</p>";

    $info .= "<p>Facility type: " . $row['facility_resource_type_abbr'] . "<br />";
    $info .= "Capacity: " . $row['facility_capacity'] . " people<br />";
    $info .= "Percent Used: $pct_full% </p>";
    $info .= "<div>0% <img src=\"index.php?stream=text&amp;mod=crs&amp;act=progress_chart&amp;f=$pct_full\" /> 100%</div>";

    $info .= "<p><h4>Reports</h4><a class=\"styleTheButton\" href=\"index.php?mod=crs&amp;act=dashboard_facility&amp;f=" . base64_encode($row['facility_uuid']) . "\">Dashboard</a> ";
    $info .= "<a class=\"styleTheButton\" href=\"index.php?mod=crs&amp;act=report_fcontacts&amp;f=" . base64_encode($row['facility_uuid']) . "\">Checked In</a> ";
    $info .= "<a class=\"styleTheButton\" href=\"index.php?mod=crs&amp;act=report_checkout&amp;f=" . base64_encode($row['facility_uuid']) . "\">Checked Out</a></p>";


    $response[$i]['name'] = $row["facility_name"];
    $response[$i]['lat'] = $row["latitude"];
    $response[$i]['lon'] = $row["longitude"];
    $response[$i]['icon'] = $icon;
    $response[$i]['info'] = $info;

    $i++;
}

echo json_encode($response, JSON_HEX_TAG | JSON_HEX_APOS | JSON_HEX_QUOT | JSON_HEX_AMP);
?>