<?php
/**
 * staff Registry Service Module
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

<?php if ($act == "staff_save_edit"): ?>
    <script type="text/javascript">
        function confirmDelete() {
            var answer = confirm('Are you sure you want to delete this person? This action cannot be undone.');
            if(answer) {
                window.location = 'index.php?mod=srs&act=staff_delete&uuid=<?php echo $uuid ?>';
            }
        }
    </script>
<?php endif; ?>

<?php if ($act == "staff_save_edit") : ?>
    <h1>Edit <span class="highlight"><?php echo $staff['full_name'] ?></span></h1>
<?php else: ?>
    <h1>Register Staff Form</h1>
<?php endif; ?>
<?php include_once ('staff_menu.php'); ?>

<?php
shn_form_fopen($act, null, $formOpts);

if ($act == "staff_save_edit")
    shn_form_hidden(array('uuid' => $uuid));
?>
<p>
    <br><em>Note: <span class="req">*</span> indicates a required field</em>
</p>
<?php if (isset($staff['opt_status'])): ?>
<h2>Report In Details</h2>
<br>
<table class="mainTable" style="width: 950px !important;">
    <tbody>
        <tr>
            <td class="mainRowOdd">Status</td>
            <td class="mainRowOdd">
                <?php switch ($staff['opt_status']):
                    case "in": ?>
                        <span class="checked_in">Reported In</span>
                        <?php break; ?>
                    <?php case "out": ?>
                        <span class="checked_out">Checked Out</span>
                        <?php break; ?>
                    <?php case "trn": ?>
                        <span class="checked_out">Transfered</span>
                        <?php break; ?>
                <?php endswitch; ?>
            </td>
        </tr>
        <tr>
            <td class="mainRowEven">Reported Date</td>
            <td class="mainRowEven"><?php echo date("F j, Y H:i", strtotime($staff['in_date'])) ?></td>
        </tr>
        <tr>
            <td class="mainRowOdd">Facility Group</td>
            <td class="mainRowOdd"><?php echo $staff['facility_group'] ?></td>
        </tr>
        <tr>
            <td class="mainRowEven">Facility</td>
            <td class="mainRowEven"><?php echo $staff['facility_name'] ?></td>
        </tr>
        <?php if (isset($staff['out_date'])): ?>
            <tr>
                <td class="mainRowOdd">Last Sign Out</td>
                <td class="mainRowOdd"><?php echo date("F j, Y H:i", strtotime($staff['out_date'])) ?></td>
            </tr>
        <?php endif; ?>
    </tbody>
</table>

<?php else: ?>
<?php
    shn_form_fsopen(_t('New Report In Details'));
    shn_form_date(_t("Date"), "checkInDate", array('req'=>true));
    shn_form_time(_t("Time"), "entryHour", "entryMinute", array('req'=>true));

    $opt_status = array();
    $opt_status[""] = "- Select -";

    foreach ($facilities as $facility) {
    $opt_status[$facility['facility_uuid']] = $facility['facility_name'];
    }

    shn_form_select($opt_status, _t('Facility Location'), "facility", null,  array('req' => true));
    shn_form_fsclose();

?>
<?php endif; ?>
<?php

    $opt_orgs = array();
    $opt_orgs[""] = "- Select -";

    foreach ($orgs as $org) {
        $opt_orgs[$org['id']] = $org['name'];
    }

    $opt_status = array();
    $opt_status["select"] = "- Select -";

    foreach ($volStatuses as $volStatus) {
        $opt_status[$volStatus['id']] = $volStatus['description'];
    }

    $opt_type = array();
    $opt_type["select"] = "- Select -";

    foreach ($staffTypes as $staffType) {
        $opt_type[$staffType['id']] = $staffType['description'];
    }

    shn_form_fsopen(_t('Organization Information'));

    shn_form_select($opt_orgs, _t('Name'), "org_id", null,  array('req' => true, 'value' => $staff['org_id']));
    shn_form_select($opt_status, _t('Volunteer Status'), "vol_status", null,  array('req' => true, 'value' => $staff['vol_id']));
    shn_form_select($opt_type, _t('Type'), "staff_type", null,  array( 'req' => true, 'value' => $staff['stafftype_id']));

    shn_form_fsclose();

    shn_form_fsopen(_t('Staff Details'));

    shn_form_text(_t("First Name"),'given_name','size="20"',array('req'=>true, 'help'=>'Given Name', 'value' => $staff['given_name']));
    shn_form_text(_t("Last Name"),'family_name','size="20"',array('req'=>true, 'help'=>'Family Name', 'value' => $staff['family_name']));
    shn_form_text(_t("MI"),'middle_initial','size="5"',array('help'=>'Middle Initials', 'value' => $staff['custom_name']));
    shn_form_date(_t("Date of Birth"), "dob", array('value' => $staff['birth_date']));
    $opt_status = array("" => "- Select -", "Male" => "Male", "Female" => "Female");
    shn_form_select($opt_status, _t('Gender'), "gender", null,  array('req' => true, 'value' => $staff['opt_gender']));

    shn_form_fsclose();

    shn_form_fsopen(_t('Contact Information'));

    shn_form_text(_t("Street Address"),'street_1','size="25"', array('value' => $staff['street_1']));
    shn_form_text(_t("Address Line 2"),'street_2','size="25"', array('value' => $staff['street_2']));
    shn_form_text(_t("City"),'city','size="10"', array('value' => $staff['city']));
    shn_form_text(_t("State / Province / Region"),'state','size="10"', array('value' => $staff['state']));
    shn_form_text(_t("Postal / Zip Code"),'postal','size="10"', array('value' => $staff['postal']));

    $q = "SELECT `option_code`, `option_description` FROM `field_options` WHERE `field_name` = 'opt_country' order by `option_description`;";
    $res = $global['db']->Execute($q);
    $opt_status = array();
    while (!$res->EOF) {
        $opt_status[$res->fields['option_code']] = $res->fields['option_description'];
        $res->MoveNext();
    }

    shn_form_select($opt_status, _t('Country'), "opt_country", null,  array('value'=>'USA'));
    shn_form_text(_t("Work Phone"),'home','size="10"', array('value' => $staff['home_phone']));
    shn_form_fsclose();

    shn_form_fsopen(_t('Demographic Information'));

    shn_form_text(_t("Occupation"),'occupation','size="20"', array('value' => $staff['occupation']));
    shn_form_text(_t("Special Skills"),'skills','size="20"',array('help'=>'Type in comma separated.', 'value' => $skillsList));
    shn_form_text(_t("Languages Spoken"),'altlang1','size="10"',array('help'=>'1st Language', 'value' => $staff['altlang1']));
    shn_form_text("",'altlang2','size="10"',array('help'=>'2nd Language', 'value' => $staff['altlang2']));
    shn_form_text("",'altlang3','size="10"',array('help'=>'3rd Language', 'value' => $staff['altlang3']));

    shn_form_fsclose();

    shn_form_submit("Save", "class=\"styleTehButton\""); echo '&nbsp&nbsp';
    shn_form_submit("Save + Continue", "class=\"styleTehButton\""); echo '&nbsp&nbsp';
    echo "<script>initDate();</script>";

?>

    <?php if ($act == "staff_save_edit"): ?>
            <input type="button" name="Delete" value="Delete" class="styleTehButton" onclick="confirmDelete();" tabindex="<?php echo++$shn_tabindex ?>"/>
    <?php endif; ?>

    <?php shn_form_fclose(); ?>

    <?php if ($act == "staff_save_edit"): ?>

        <div style="margin: 25px 50px; display: block">
            <b>Note:</b> Deleting a shelter staff is a permanent action and cannot be undone.
            In addition to this, deleting a shelter staff will also remove the association of any additional persons
            related to this person.
        </div>

    <?php endif; ?>



