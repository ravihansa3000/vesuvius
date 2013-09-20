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
global $conf;
global $shn_tabindex;
$shn_tabindex = 0;
$helpid = 0;
?>
<script type="text/javascript">

    $('#dobDatepicker').datepicker({
      beforeShow: readBirthday, onSelect: updateBirthday,
      minDate: new Date(<?php echo date('Y') - 110; ?>, 1 - 1, 1), maxDate: new Date(<?php echo date('Y'); ?>, 12 - 1, 31),
      showOn: 'both', buttonImageOnly: true, buttonImage: 'theme/<?php echo $conf['theme']; ?>/img/icon-calendar.gif'});

    // Prepare to show a date picker linked to three select controls
    function readBirthday() {
      $('#dobDatepicker').val($('#dobMonth').val() + '/' +
        $('#dobDay').val() + '/' + $('#dobYear').val());
      return {};
    }

    // Update three select controls to match a date picker selection
    function updateBirthday(date) {
      $('#dobMonth').val(date.substring(0, 2));
      $('#dobDay').val(date.substring(3, 5));
      $('#dobYear').val(date.substring(6, 10));
    }

    $('#exitDatepicker').datepicker({
      beforeShow: readexitDate, onSelect: updateexitDate,
      minDate: new Date(<?php echo date('Y') - 110; ?>, 1 - 1, 1), maxDate: new Date(<?php echo date('Y'); ?>, 12 - 1, 31),
      showOn: 'both', buttonImageOnly: true, buttonImage: 'theme/<?php echo $conf['theme']; ?>/img/icon-calendar.gif'});

    // Prepare to show a date picker linked to three select controls
    function readexitDate() {
      $('#exitDatepicker').val($('#exitMonth').val() + '/' +
        $('#exitDay').val() + '/' + $('#exitYear').val());
      return {};
    }

    // Update three select controls to match a date picker selection
    function updateexitDate(date) {
      $('#exitMonth').val(date.substring(0, 2));
      $('#exitDay').val(date.substring(3, 5));
      $('#exitYear').val(date.substring(6, 10));
    }

</script>
<style>
    tr td {
        text-align: center !important;
    }

    .head {
        font-weight: bold;
    }

</style>

<h1>Checkout Client <span class="highlight"><?php echo $client['given_name']." ".$client['family_name'] ?></span></h1>

<?php include_once ('client_menu.php'); ?>

<?php
shn_form_fopen($act, null, $formOpts);

if ($act == "client_save_edit")
  shn_form_hidden(array('uuid' => $uuid));
?>

<h3>Registration Information</h3>
<table class="mainTable" style="width: 650px !important;">
    <tbody>
      <tr>
        <td class="mainRowOdd">Check In Date</td>
        <td class="mainRowOdd"><?php echo date("F j, Y H:i", strtotime($client['in_date'])) ?></td>
      </tr>
      <tr>
        <td class="mainRowEven">Facility Group</td>
        <td class="mainRowEven"><?php echo $client['facility_group'] ?></td>
      </tr>
      <tr>
        <td class="mainRowOdd">Facility</td>
        <td class="mainRowOdd"><?php echo $client['facility_name'] ?></td>
      </tr>
    </tbody>
  </table>

<?php
shn_form_fopen("client_checkout_save", null, array('enctype' => 'enctype="multipart/form-data"', 'req_message' => true));

shn_form_fsopen(_t('Checkout Form'));
    shn_form_date(_t("Date"), "checkOutDate", array('req'=>true));
    shn_form_time(_t("Time"), "exitHour", "exitMinute", array('req'=>true));
shn_form_fsclose();

    shn_form_submit("Save", "class=\"styleTehButton\""); echo '&nbsp&nbsp';

echo "<script>initDate();</script>";
shn_form_fclose();

?>
</ul>
<?php shn_form_fsclose(); ?>
<?php shn_form_fclose(); ?>
