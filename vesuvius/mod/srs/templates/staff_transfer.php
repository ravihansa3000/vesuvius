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
global $conf;
global $global;
global $shn_tabindex;
$shn_tabindex = 0;
$helpid = 0;
?>
<style>
    tr td {
        text-align: center !important;
    }

    .head {
        font-weight: bold;
    }

</style>

<h1>Transfer Staff <span class="highlight"><?php echo $staff['full_name'] ?></span></h1>

<?php include_once ($global['approot'] . 'mod/srs/templates/staff_menu.php'); ?>

<pre><?php print_r($_SESSION, true) ?></pre>

<?php
shn_form_fopen($act, null, $formOpts);
?>

<h2>Registration Information</h2>
<table class="mainTable" style="width: 650px !important;">
    <tbody>
        <tr>
            <td class="mainRowOdd">Check In Date</td>
            <td class="mainRowOdd"><?php echo date("F j, Y H:i", strtotime($staff['in_date'])) ?></td>
        </tr>
        <tr>
            <td class="mainRowEven">Facility Group</td>
            <td class="mainRowEven"><?php echo $staff['facility_group'] ?></td>
        </tr>
        <tr>
            <td class="mainRowOdd">Facility</td>
            <td class="mainRowOdd"><?php echo $staff['facility_name'] ?></td>
        </tr>
    </tbody>
</table>

<?php
shn_form_fopen("staff_transfer_save", null, array('enctype' => 'enctype="multipart/form-data"', 'req_message' => true));

    shn_form_fsopen(_t('Transfer Form'));
        shn_form_date(_t("Date"), "checkOutDate", array('req'=>true));
        shn_form_time(_t("Time"), "exitHour", "exitMinute", array('req'=>true));

        $opt_status = array();
        $opt_status["select"] = "- Select -";

        foreach ($facilities as $facility) {
            $opt_status[$facility['facility_uuid']] = $facility['facility_name'];
        }

        shn_form_select($opt_status, _t('Destination Facility Location'), "destFacility", null,  array('value'=>'select', 'req' => true));

        shn_form_fsclose();

        shn_form_submit("Transfer", "class=\"styleTehButton\""); echo '&nbsp&nbsp';

        echo "<script>initDate();</script>";

?>
<?php shn_form_fsclose(); ?>
<?php shn_form_fclose(); ?>
