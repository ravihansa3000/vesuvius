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

<h1>Checkout Client <span class="highlight"><?php echo $staff['full_name'] ?></span></h1>

<?php include_once ('staff_menu.php'); ?>

<?php
shn_form_fopen($act, null, $formOpts);

if ($act == "staff_save_edit")
  shn_form_hidden(array('uuid' => base64_encode($uuid)));
?>


<h3>Registration Information</h3>
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
    shn_form_fopen("staff_checkout_save", null, array('enctype' => 'enctype="multipart/form-data"', 'req_message' => true));

        shn_form_fsopen(_t('Checkout Form'));
        shn_form_date(_t("Date"), "checkOutDate", array('req'=>true));
        shn_form_time(_t("Time"), "exitHour", "exitMinute", array('req'=>true));
        shn_form_fsclose();

        shn_form_submit("Save", "class=\"styleTehButton\""); echo '&nbsp&nbsp';

    echo "<script>initDate();</script>";
    shn_form_fclose();

?>

<?php shn_form_fsclose(); ?>
<?php shn_form_fclose(); ?>
