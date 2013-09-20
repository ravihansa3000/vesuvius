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
array_push($global['xajax_functions'],'crs_test');

function crs_test($name_query, $minAge, $maxAge, $facility_group, $facility){

    global $global;
//    global $conf;
//
//    $values = array();
//    $where_clause = "";
//
//    // Apply filters
//    if (isset($_SESSION['incident']) && $_SESSION['incident'] != '') {
//        $logic = (isset($values[0])) ? "AND" : "WHERE";
//        $values[] = $_SESSION['incident'];
//        $where_clause .= "$logic incident_id = ? \n";
//    }
//
//    if (isset($name_query)) {
//        $logic = (isset($values[0])) ? "AND" : "WHERE";
//        $values[] = $name_query;
//        $where_clause .= "$logic full_name LIKE CONCAT('%',?,'%') \n";
//    }
//
//    if (isset($facility_group) && $facility_group != 'all') {
//        $logic = (isset($values[0])) ? "AND" : "WHERE";
//        $values[] = $facility_group;
//        $where_clause .= "$logic facility_group LIKE CONCAT('%',?,'%') \n";
//    }
//
//    if (isset($facility) && $facility != 'all') {
//        $logic = (isset($values[0])) ? "AND" : "WHERE";
//        $values[] = $facility;
//        $where_clause .= "$logic facility_uuid = ? \n";
//    }
//
//    if (isset($minAge) && $minAge != '') {
//        $logic = (isset($values[0])) ? "AND" : "WHERE";
//        $values[] = $minAge;
//        $where_clause .= "$logic years_old >= ? \n";
//    }
//
//    if (isset($maxAge) && $maxAge != '') {
//        $logic = (isset($values[0])) ? "AND" : "WHERE";
//        $values[] = $maxAge;
//        $where_clause .= "$logic years_old <= ? \n";
//    }
//
//    $query = "SELECT COUNT(p_uuid) AS count FROM crs_client_search \n";
//    $query .= $where_clause;
//
//    // Execute query
//    try {
//        //$result = $global['db']->Execute($query, $values);
//    } catch (exception $e) {
//        print("<pre>" . $e->getMessage() . "</pre>");
//    }
//
//    $count = $result->Fields('count');

    // calculate the total pages for the query
//        if ($count > 0 && $limit > 0) {
//            $total_pages = ceil($count / $limit);
//        } else {
//            $total_pages = 0;
//        }

    // if for some reasons the requested page is greater than the total
    // set the requested page to total page
//        if ($page > $total_pages) {
//            $page = $total_pages;
//        }
//
//        // calculate the starting position of the rows
//        $start = $limit * $page - $limit;

    // if for some reasons start position is negative set it to 0
    // typical case is that the user type 0 for the requested page
//        if ($start < 0) {
//            $start = 0;
//        }

    // Now create the real data query
  //  $query = "SELECT * FROM crs_client_search ";
   // $query .= $where_clause;

    // Sort ordering
//        if ($sidx) {
//            $query .= "ORDER BY $sidx $sord ";
//        }
//
//        // Pagination//                if(!data.page){
//                    $.tgrid.page = 1;
//                } else {
//                    $.tgrid.page = data.page;
//                }
//                $("#pagenum").val($.tgrid.page);
//                $("#lastpage").val(data.total);
//                $("#results").html(addCommas(data.records));
//                $("#totalpages").html(data.total);
//
//                // header
//                var h = 0;
//                var head = "<tr>";
//                head += "<th>#</th>";
//
//                var sortasc;
//                var sortdesc;
//                if(data.records > 0) {
//                    $.each(data.rows[0], function(key, value){
//
//                        if(key == data.sidx && data.sord == 'asc'){
//                            sortasc = 'sortbuttonselected';
//                        } else {
//                            sortasc = 'sortbutton';
//                        }
//
//                        if(key == data.sidx && data.sord == 'desc'){
//                            sortdesc = 'sortbuttonselected';
//                        } else {
//                            sortdesc = 'sortbutton';
//                        }
//
//                        head += '<th id="' + key + '">';
//                        head += '<a href="#">' + data.colnames[h++] + '</a> ';
//                        head += '<span title="ascending" class="'+ sortasc +'">&#x25B2;</span>';
//                        head += '<span title="descending" class="'+ sortdesc +'">&#x25BC;</span>';
//                        head += '</th>';
//                    });
//                } else {
//                    $.each(data.colnames, function(key, value){
//                        head += '<th id="' + key + '">' + value + '</th>';
//                    });
//                }
//
//                head += "</tr>";
//                $(head).appendTo("#userdata thead");
//
//                // rows
//                var row = '';
//                if(data.records > 0) {
//                    $.each(data.rows, function(i, person) {
//                        row += "<tr>";
//                        var rownum = i+1+(($.tgrid.page-1)*data.limit);
//                        row += "<td>"+rownum+"</td>";
//                        $.each(person, function(key, value){
//                            row += "<td>"+value+"</td>";
//                        });
//                        row += "</tr>";
//                    });
//                } else {
//                    row = '<tr><td colspan="'+(data.colnames.length+1)+'" align="center">No Results Found</td></tr>';
//                }
//
//                $(row).appendTo("#userdata tbody")
//
//                $('#userdata thead th').click(function(){
//                    $.tgrid.sidx = $(this).attr('id');
//                    if($.tgrid.sord == 'desc'){
//                        $.tgrid.sord = "asc";
//                    } else {
//                        $.tgrid.sord = "desc";
//                    }
//                    showresults();
//                });
//        if ($start && $limit) {
//            $query .= "LIMIT $start , $limit";
//        } else {
//            $query .= "LIMIT $limit";
//        }
//
//    // Execute query
//    try {
//        $query = $global['db']->Prepare($query);
//        //$results = $global['db']->Execute($query, $values);
//
//        $html = '';
//        $html .= "<tr>";
//        $html .= "<th>#</th>";
//        $html .= "<th>First Name</th>";
//        $html .= "<th>Last Name</th>";
//        $html .= "<th>Gender</th>";
//        $html .= "<th>Shelter</th>";
//        $html .= "</tr>";
//
//        $global['xajax_res']->addAssign('userdata','innerHTML',$html);
//
//        dao_error_log();
//    } catch (exception $e) {
//        add_error("<pre>" . $e->getMessage() . "</pre>");
//    }

    
    $global['xajax_res']->addScript('alert("To do");');
    return $global['xajax_res']->getXML();
}