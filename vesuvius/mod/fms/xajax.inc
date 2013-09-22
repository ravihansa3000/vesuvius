<?php
/**
 * Created by JetBrains PhpStorm.
 * User: kaushika
 * Date: 8/25/13
 * Time: 9:12 AM
 * To change this template use File | Settings | File Templates.
 */

global $global;

// create an array as it does not exist previosuly and will make warnings
$global['xajax_functions'] = array();

// publicly register accessible xajax funtions
array_push($global['xajax_functions'], 'fms_search');

function fms_search($rj)
{
    //$name_query, $minAge, $maxAge, $facility_group, $facility
    global $global;
    global $conf;

    $values = array();
    $where_clause = "";

    $query_object = json_decode($rj, true);
    $name_query     = $query_object['name_query'];
    $sidx           = $query_object['sidx'];
    $sord           = $query_object['sordx'];
    $page           = $query_object['page'];
    $limit          = $query_object['limit'];

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

    $query = "SELECT COUNT(*) AS count FROM fms_facility AS f
    INNER JOIN fms_facility_to_event fti ON f.facility_uuid = fti.facility_uuid \n";
    $query .= $where_clause;

    // Execute query
    try {
        $result = $global['db']->Execute($query, $values);
        if ( !$result ) {
            daoErrorLog(__FILE__,__LINE__,__CLASS__,__METHOD__,__FUNCTION__,$global['db']->ErrorMsg(), "Client Search first Query");
        }
    } catch (exception $e) {
        print("<pre>" . $e->getMessage() . "</pre>");
    }

    $count = $result->Fields('count');

    //calculate the total pages for the query
    if ($count > 0 && $limit > 0) {
        $total_pages = ceil($count / $limit);
    } else {
        $total_pages = 0;
    }

    //if for some reasons the requested page is greater than the total
    // set the requested page to total page
    if ($page > $total_pages) {
        $page = $total_pages;
    }

    // calculate the starting position of the rows
    $start = $limit * $page - $limit;

    //if for some reasons start position is negative set it to 0
    // typical case is that the user type 0 for the requested page
    if ($start < 0) {
        $start = 0;
    }

    //Now create the real data query
    $query = "SELECT * FROM fms_facility AS f
    INNER JOIN fms_facility_to_event fti ON f.facility_uuid = fti.facility_uuid \n";
    $query .= $where_clause;

    //Sort ordering
    if ($sidx) {
        $query .= "ORDER BY $sidx $sord ";
    }
    if ($start && $limit) {
        $query .= "LIMIT $start , $limit";
    } else {
        $query .= "LIMIT $limit";
    }

    // Execute query
    try {
        $query = $global['db']->Prepare($query);
        $results = $global['db']->Execute($query, $values);
        if ( !$results ) {
            daoErrorLog(__FILE__,__LINE__,__METHOD__,__CLASS__,__FUNCTION__, $global['db']->ErrorMsg(), "Client Search");
        }

        $html = '<table class="mainTable" style="width: 950px !important;">';
        $html .= "<tr>";
        $html .= "<td><b>#</b></td>";
        $html .= "<td><b>Facility</b></td>";
        $html .= "<td><b>Contact</b></td>";
        $html .= "<td><b>Type</b></td>";
        $html .= "<td><b>Group</b></td>";
        $html .= "<td><b>Location</b></td>";
        $html .= "</tr>";

        $i = 1;
        foreach( $results as $search_result ) {
            $base_uuid = $search_result['p_uuid'];
            $class = ($i%2 == 0 ? "mainRowOdd" : "mainRowEven");
            $html .= '<tr>';
            $html .= '<td class="' . $class . '">'.$i.'</td>';
            $html .= '<td class="' . $class . '"><a href=index.php?mod=crs&amp;act=client_view&amp;uuid='.$base_uuid.'>'.$search_result['facility_name'].'</td>';
            $html .= '<td class="' . $class . '"><a href=index.php?mod=crs&amp;act=client_view&amp;uuid='.$base_uuid.'>'.$search_result['work_phone'].'</td>';
            $html .= '<td class="' . $class . '">'.$search_result['facility_resource_type_abbr'].'</td>';
            $html .= '<td class="' . $class . '">'.$search_result['facility_group'].'</td>';

            $address = isset($search_result['street_1']) && $search_result['street_1'] != '' ? $search_result['street_1'] . "<br/>" : '';
            $address .= isset($search_result['street_2']) && $search_result['street_2'] != '' ? $search_result['street_2'] . "<br/>" : '';
            $address .= isset($search_result['city']) ? $search_result['city'] . ", " : '';
            $address .= isset($search_result['state']) ? $search_result['state'] . "<br/>" : '';
            $address .= isset($search_result['postal_code']) ? $search_result['postal_code'] . "<br/>" : '';

            $html .= '<td class="' . $class . '">'.$address.'</td>';
            $html .= '</tr>';
            $i++;
        }
        $html .= '</table>';

        $global['xajax_res']->addAssign('userdata', 'innerHTML', $html);

    } catch (exception $e) {
        add_error("<pre>" . $e->getMessage() . "</pre>");
    }

    //$global['xajax_res']->addScript(file_get_contents($global['approot']."/mod/crs/crs_ajax.js"));
    return $global['xajax_res']->getXML();
}