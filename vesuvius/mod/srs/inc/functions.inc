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

/**
 *  Helper function for supplying facilities
 *  
 * @access public
 * @return array
 */
function get_facilities($fg = null)
{
    global $global;

    // Get the list of active facilities
    // Yes, for now we allow people to be assigned to facility regardless of its activation status
    $query = "SELECT * FROM fms_facility f
      INNER JOIN fms_facility_to_event fti ON fti.facility_uuid = f.facility_uuid
      WHERE fti.incident_id = ?";

    if (isset($fg)) {
        $query .= "\n AND facility_group = ?";
    }
    $query .= "\n ORDER BY facility_name;";

    // Execute query
    try {
        $query = $global['db']->Prepare($query);
        $facilities = $global['db']->Execute($query, array($_SESSION['incident'], $fg));
    } catch (exception $e) {
        add_error("<pre>" . $e->getMessage() . "</pre>");
    }

    return $facilities->getArray();
}

/**
 *  Helper function for supplying facility groups
 *  
 * @access public
 * @return array
 */
function get_facility_groups()
{
    global $global;

    // Get the list of active facilities
    // Yes, for now we allow people to be assigned to facility regardless of its activation status
    $query = "SELECT DISTINCT f.facility_group FROM fms_facility f
      INNER JOIN fms_facility_to_event fti ON fti.facility_uuid = f.facility_uuid
      WHERE fti.incident_id = ?
      ORDER BY facility_group;";

    // Execute query
    try {
        $query = $global['db']->Prepare($query);
        $facilities = $global['db']->Execute($query, $_SESSION['incident']);
    } catch (exception $e) {
        add_error("<pre>" . $e->getMessage() . "</pre>");
    }

    return $facilities->getArray();
}

/**
 * create a select field based on field options
 *
 * @param mixed $opt_field
 * @param mixed $label
 * @param string $select_opts
 * @param mixed $extra_opts
 * @access public
 * @return void
 */
function srs_form_opt_select($opt_field, $select_opts = "", $extra_opts = null)
{
    global $global;
    global $shn_tabindex;

    $sort = isset($extra_opts['sort']) ? $extra_opts['sort'] : null;
    $name = isset($extra_opts['name']) ? $extra_opts['name'] : $opt_field;
    $value = isset($_POST[$opt_field]) ? $_POST[$opt_field] : $extra_opts['value'];
    $class = isset($extra_opts['class']) ? $extra_opts['class'] : null;

    // Echo the select and label tags
    echo "<select name=\"" . $name . "\" " . $select_opts . " tabindex=\"" . ++$shn_tabindex . "\" " . $class . " \>\n";


    if ($sort == null) {
        $q = "select option_code, option_description from field_options where field_name='{$opt_field}'";
    } else {
        $q = "select option_code, option_description from field_options where field_name='{$opt_field}' order by $sort ";
    }

    try {
        $res = $global['db']->Execute($q);
    } catch (exception $e) {
        add_error($e->getMessage());
    }

    if (isset($extra_opts['select']) && $extra_opts['select'] == true) {
        echo "<option value='select'>- Select -</option>";
    }
    while (!$res == NULL && !$res->EOF) {
        $sel = ($res->fields['option_code'] == $value) ? 'selected="selected"' : null;
        echo "<option value=\"" . $res->fields['option_code'] . "\" " . $sel . ">" . _lc($res->fields['option_description']) . "</option> ";
        $res->MoveNext();
    }
    if (isset($extra_opts['all']) && $extra_opts['all'] == true) {
        $sel = (strtolower($value) == 'all') ? "selected=\"selected\"" : "";
        echo "<option value='all' $sel>[ALL]</option>";
    }
    echo "</select>";
}

/*
 * Function parses phone number string returning
 * three field U.S. formated array
 *
 * @param string $phone
 * @access public
 * @return array
 */

function parse_phone($phone)
{

    $phone_number = array('areacode' => '', 'prefix' => '', 'suffix' => '');
    $phone_expr = '/\(?(\d{3})\W?\s?(\d{3})\W?(\d{4})/';

    preg_match_all($phone_expr, $phone, $matches);

    if (isset($matches[0][0])) {
        $phone_number["areacode"] = $matches[1][0];
        $phone_number["prefix"] = $matches[2][0];
        $phone_number["suffix"] = $matches[3][0];
    }

    return $phone_number;
}

function formatPhone($phone)
{

    $p = parse_phone($phone);

    $format = "(%d) %d - %d";
    return sprintf($format, $p['areacode'], $p['prefix'], $p['suffix']);
}

/*
 * Function buids a WHERE clause from an array of expressions
 * 
 * @param string $base_query
 * @param array $expre_array
 * @access public
 * @return string 
 */

function shn_srs_querybuilder($base_query, $expr_array)
{
    // WHERE string
    $where = "";

    array_multisort($expr_array);

    //add_information("<pre>" . print_r($expr_array, true) . "</pre>");
    //
  // Build the WHERE clause expressions
    //$where .= "WHERE";

    for ($g = 0; $g < count($expr_array); $g++) {

        // Only add the group's expression if it's not the first
        if ($g > 0) {
            $where .= $expr_array[$g]['group_expr'] . " (";
        } else {
            $where .= "WHERE (";
        }

        $clause = $expr_array[$g]['clause'];
        for ($c = 0; $c < count($clause); $c++) {

            if ($c > 0) {
                $where .= $clause[$c]['expr'];
            }

            $str = " %s.%s %s %s ";
            $where .= sprintf($str, $clause[$c]['alias'], $clause[$c]["col"], $clause[$c]["op"], $clause[$c]["val"]);
        }

        // Close group
        $where .= ")\n";
    }

    //add_information("<pre>$where</pre>");
    return $base_query . $where;
}

/*
 * Function adds a search expression to the an array of expressions
 * 
 * @param string $alias
 * @param string $expression
 * @param string $col
 * @param string $operator
 * @param string $value
 * @param int $group
 * @access public
 * @return void 
 */

function add_search_expr($alias, $expression, $col, $operator, $value, $group = 0, $group_expr = 'AND')
{

    global $expr_array;

    $expr_array[$group]['group_expr'] = $group_expr;
    $expr_array[$group]['clause'][] = array('alias' => $alias, 'expr' => $expression, 'col' => $col, 'op' => $operator, 'val' => $value);
}

/*
 * Function determins the age group of a client based
 * on U.S. FEMA standards
 *
 * @param int $years_old
 * @access public
 * @return string
 */

function age_group($years_old)
{

    $age_group = "";

    // Age group
    if ($years_old >= 0 && $years_old <= 3) {
        $age_group = "Infant";
    } else if ($years_old > 3 && $years_old <= 17) {
        $age_group = "Child";
    } else {
        $age_group = "Adult";
    }

    return $age_group;
}

/*
 * Function (deprecated) provides a image link based on client's age group
 *
 * @access public
 * @return string
 */

function person_icon($url = null, $url_thumb = null, $gender = null, $years_old = null, $minAge = null)
{
    // Predefine default person images
    // TODO: These need to be made properties in the database
    $boy = "res/img/s0boy.png";
    $girl = "res/img/s1girl.png";
    $man = "res/img/s2man.png";
    $woman = "res/img/s3woman.png";
    $unknown = "res/img/s4unknown.png";

    // figure out if the person has any images ...
    if (!empty($url)) {
        $imageUrl = $url;
        $thumbUrl = $url_thumb;
    } else {

        // Supply some default silhouettes for people without images
        switch ($gender) {

            case ("mal"):

                // Adult male or child
                if ($years_old > 0 && $years_old <= 17) {
                    $imageUrl = $boy;
                    $thumbUrl = $boy;
                } elseif ($years_old > 0 || $minAge <= 17) {
                    $imageUrl = $boy;
                    $thumbUrl = $boy;
                } else {
                    $imageUrl = $man;
                    $thumbUrl = $man;
                }
                break;

            case ("fml"):

                // Adult female or child
                if ($years_old > 0 && $years_old > 17) {
                    $imageUrl = $girl;
                    $thumbUrl = $girl;
                } elseif ($years_old > 0 || $minAge > 17) {
                    $imageUrl = $girl;
                    $thumbUrl = $girl;
                } else {
                    $imageUrl = $woman;
                    $thumbUrl = $woman;
                }
                break;

            default:
                $imageUrl = $unknown;
                $thumbUrl = $unknown;
                break;
        }
    }

    // Return thumb url
    return makeBaseUrl() . $thumbUrl;
}

/**
 * Returns the date difference in human readable form 
 * @param type $start
 * @param type $end
 * @return string 
 */
function date_duration($start, $end = NULL, $seconds = TRUE)
{
    $sdate = strtotime($start);
    $edate = empty($end) ? time() : strtotime($end);

    $time = $edate - $sdate;
    if ($time >= 0 && $time <= 59) {
        // Seconds
        $timeshift = $time . ' seconds ';
    } elseif ($time >= 60 && $time <= 3599) {
        // Minutes + Seconds
        $pmin = ($edate - $sdate) / 60;
        $premin = explode('.', $pmin);

        $timeshift = $premin[0] . ' min ';

        if ($seconds) {
            $presec = $pmin - $premin[0];
            $sec = $presec * 60;
            $timeshift .= ' ' . round($sec, 0) . ' sec ';
        }
    } elseif ($time >= 3600 && $time <= 86399) {
        // Hours + Minutes
        $phour = ($edate - $sdate) / 3600;
        $prehour = explode('.', $phour);

        $premin = $phour - $prehour[0];
        $min = explode('.', $premin * 60);

        $timeshift = $prehour[0] . ' hrs ' . $min[0] . ' min ';

        if ($seconds) {
            $presec = '0.' . $min[1];
            $sec = $presec * 60;
            $timeshift .= ' ' . round($sec, 0) . ' sec ';
        }
    } elseif ($time >= 86400) {
        // Days + Hours + Minutes
        $pday = ($edate - $sdate) / 86400;
        $preday = explode('.', $pday);

        $phour = $pday - $preday[0];
        $prehour = explode('.', $phour * 24);

        $premin = ($phour * 24) - $prehour[0];
        $min = explode('.', $premin * 60);

        $timeshift = $preday[0] . ' days ' . $prehour[0] . ' hrs ' . $min[0] . ' min';

        if ($seconds) {
            $min[1] = empty($min[1]) ? 0 : $min[1];
            $presec = '0.' . $min[1];
            $sec = $presec * 60;
            $timeshift .= ' ' . round($sec, 0) . ' sec ';
        }
    }
    return $timeshift;
}

function getOrgs()
{
    global $global;

    // Now the actual query
    $sql = "SELECT * FROM srs_organizations WHERE incident_id = ? ORDER BY name";

    // Execute staff query
    try {
        $rs = $global['db']->Execute($sql, $_SESSION['incident']);
    } catch (exception $e) {
        add_error("<pre>" . $e->getMessage() . "</pre>");
    }

    $orgs = $rs->GetAssoc();
    return $orgs;
}

function getOrgName($orgId)
{
    global $global;

    // Now the actual query
    $sql = "SELECT * FROM srs_organizations WHERE id = ?";

    // Execute staff query
    try {
        $rs = $global['db']->Execute($sql, $orgId);
    } catch (exception $e) {
        add_error("<pre>" . $e->getMessage() . "</pre>");
    }

    $org = $rs->Fetchrow();

    return $org['name'];
}

function getStaffTimeline($uuid)
{
    global $global;

    // Get staff data
    $staff = get_staff($uuid);

    $sql = "SELECT 
            ptf.id,
            f1.facility_name,
            ptf.in_date,
            ptf.out_date,
            ptt.dest_facility,
            f3.facility_name AS dest_name,
            p.full_name AS user
        FROM fms_person_to_facility ptf
        INNER JOIN fms_facility f1 ON f1.facility_uuid = ptf.facility_uuid
        LEFT JOIN fms_person_to_transfer ptt ON ptt.id = ptf.transfer_id
        LEFT JOIN person_uuid p ON p.p_uuid = ptt.user
        LEFT JOIN fms_facility f2 ON f2.facility_uuid = ptt.src_facility
        LEFT JOIN fms_facility f3 ON f3.facility_uuid = ptt.dest_facility
        WHERE ptf.p_uuid = ?
        ORDER BY ptf.in_date ASC";

    // Execute query
    try {
        $results = $global['db']->Execute($sql, $uuid);
    } catch (exception $e) {
        add_error("<pre>" . $e->getMessage() . "</pre>");
    }

    return $results->GetRows();
}

function getVolStatuses()
{
    global $global;

    // Now the actual query
    $sql = "SELECT * FROM srs_vol_status WHERE incident_id = ? ORDER BY description";

    // Execute query
    try {
        $rs = $global['db']->Execute($sql, $_SESSION['incident']);
    } catch (exception $e) {
        add_error("<pre>" . $e->getMessage() . "</pre>");
    }

    return $rs->GetAssoc();
}

function getVolStatusName($volStatusId)
{
    global $global;

    // Now the actual query
    $sql = "SELECT * FROM srs_vol_status WHERE id = ?";

    // Execute query
    try {
        $rs = $global['db']->Execute($sql, $volStatusId);
    } catch (exception $e) {
        add_error("<pre>" . $e->getMessage() . "</pre>");
    }

    $row = $rs->Fetchrow();

    return $row['description'];
}

function getStaffTypes()
{
    global $global;

    // Now the actual query
    $sql = "SELECT * FROM srs_staff_types WHERE incident_id = ? ORDER BY description";

    // Execute query
    try {
        $rs = $global['db']->Execute($sql, $_SESSION['incident']);
    } catch (exception $e) {
        add_error("<pre>" . $e->getMessage() . "</pre>");
    }

    return $rs->GetAssoc();
}

function getStaffTypeName($staffTypeId)
{
    global $global;

    // Now the actual query
    $sql = "SELECT * FROM srs_staff_types WHERE id = ?";

    // Execute query
    try {
        $rs = $global['db']->Execute($sql, $staffTypeId);
    } catch (exception $e) {
        add_error("<pre>" . $e->getMessage() . "</pre>");
    }

    $row = $rs->Fetchrow();

    return $row['description'];
}

function getOrgStaffLevels($org)
{
    global $global;

    // Now the actual query
    $sql = "SELECT 
        st.description,
        COUNT(*) AS `count`
    FROM srs_person_to_staff_types ptst
        INNER JOIN srs_staff_types st ON st.id = ptst.stafftype_id 
        INNER JOIN srs_person_to_org ptg ON ptg.p_uuid = ptst.p_uuid
        INNER JOIN person_status AS ps ON ps.p_uuid = ptst.p_uuid
    WHERE ps.opt_status = 'in' AND ptg.org_id = ?
    GROUP BY st.description";

    // Execute query
    try {
        $rs = $global['db']->Execute($sql, array($org));
    } catch (exception $e) {
        add_error("<pre>" . $e->getMessage() . "</pre>");
    }

    return $rs->GetArray();
}

function getFacilityStaffLevels($fac)
{
    global $global;

    // Now the actual query
    $sql = "SELECT 
        st.description,
        COUNT(*) AS `count`
    FROM srs_person_to_staff_types ptst
        INNER JOIN srs_staff_types st ON st.id = ptst.stafftype_id 
        INNER JOIN crs_latest_ptf  ptf ON ptf.p_uuid = ptst.p_uuid
        INNER JOIN fms_facility f ON f.facility_uuid = ptf.facility_uuid
        INNER JOIN person_status AS ps ON ps.p_uuid = ptst.p_uuid
    WHERE ps.opt_status = 'in' AND ptf.facility_uuid = ?
    GROUP BY st.description";

    // Execute query
    try {
        $rs = $global['db']->Execute($sql, array($fac));
    } catch (exception $e) {
        add_error("<pre>" . $e->getMessage() . "</pre>");
    }

    return $rs->GetArray();
}

?>
