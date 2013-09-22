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
<h1>Check In Client <span class="highlight"><?php echo $staff['full_name'] ?></span></h1>

<?php include_once ('staff_menu.php'); ?>

<?php
shn_form_fopen($act, null, $formOpts);
shn_form_hidden(array('uuid' => $uuid));
?>


    <?php if ($staff['opt_status'] == "out"): ?>
        <div class="message warning">
          <p><?php echo $staff['full_name'] ?> checked out of the <em><?php echo $staff['facility_name'] ?></em> facility at <em><?php echo date("F j, Y H:i", strtotime($staff['out_date'])) ?></em>.
            The staff's previous facility has been selected by default. You can change this selection as required.</p>
        </div>
    <?php endif; ?>
<?php
    shn_form_fsopen(_t('Check In Form'));
        shn_form_date(_t("Date"), "checkInDate", array('req'=>true));
        shn_form_time(_t("Time"), "entryHour", "entryMinute", array('req'=>true));

        $opt_status = array();
        $opt_status["select"] = "- Select -";

        foreach ($facilities as $facility) {
            $opt_status[$facility['facility_uuid']] = $facility['facility_name'];
        }

        shn_form_select($opt_status, _t('Facility Location'), "facility", null,  array('value'=>'select', 'req' => true));

        shn_form_fsclose();

        shn_form_submit("Check In", "class=\"styleTehButton\""); echo '&nbsp&nbsp';

        echo "<script>initDate();</script>";
    shn_form_fclose();


?>
<?php shn_form_fsclose(); ?>
<?php shn_form_fclose(); ?>
