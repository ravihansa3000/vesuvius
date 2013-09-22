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
$med_alert_icon = "";

// Medical Alert Icon
if ($act == "client_save_edit" && ($client["injury"] || $client["special_medical"] || $client["special_mental"])) {
    $style = "medalert";
    $src = "theme/{$conf['theme']}/img/medic_logo.png";
    $med_alert_icon = "style =\"background:#FFFFFF url('$src') no-repeat right top;\"";
} else {
    $style = "restricted";
}
?>
<?php if ($act == "client_save_edit"): ?>
    <script type="text/javascript">
        function confirmDelete() {
            var answer = confirm('Are you sure you want to delete this person? Anyone assigned to this client will be removed from the group. Any pet records associated with this person will be deleted. This action cannot be undone.');
            if(answer) {
                window.location = 'index.php?mod=crs&act=client_delete&uuid=<?php echo $uuid ?>';
            }
        }
    </script>
<?php endif; ?>

<?php if ($act == "client_save_edit") : ?>
    <h1>Edit <span class="highlight"><?php echo $client['full_name'] ?></span></h1>
<?php else: ?>
    <h1>New Shelter Client Form</h1>
<?php endif; ?>
<?php include_once ('client_menu.php'); ?>

<?php
shn_form_fopen($act, null, $formOpts);

if ($act == "client_save_edit")
    shn_form_hidden(array('uuid' => $uuid));
?>
<p>
    <br><em>Note: <span class="req">*</span> indicates a required field</em>
</p>
<?php if (isset($client['opt_status'])): ?>
<h2>Check In Information</h2>
<br>
<table class="mainTable" style="width: 950px !important;">
    <tbody>
        <tr>
            <td class="mainRowOdd">Status</td>
            <td class="mainRowOdd">
                <?php switch ($client['opt_status']):
                    case "in": ?>
                        <span class="checked_in">Checked In</span>
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
            <td class="mainRowEven">Check In Date</td>
            <td class="mainRowEven"><?php echo date("F j, Y H:i", strtotime($client['in_date'])) ?></td>
        </tr>
        <tr>
            <td class="mainRowOdd">Out In Date</td>
            <td class="mainRowOdd"><?php echo isset($client['out_date']) ? date("F j, Y H:i", strtotime($client['out_date'])) : "" ?></td>
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
<?php else: ?>
<?php
    shn_form_fsopen(_t('Check In Information'));
        shn_form_date(_t("Date"), "checkInDate", array('req'=>true));
        shn_form_time(_t("Time"), "entryHour", "entryMinute", array('req'=>true));

        $opt_status = array();
        $opt_status["select"] = "- Select -";

        foreach ($facilities as $facility) {
        $opt_status[$facility['facility_uuid']] = $facility['facility_name'];
        }

        shn_form_select($opt_status, _t('Facility Location'), "facility", null,  array('value'=>'select', 'req' => true));
    shn_form_fsclose();
?>
<?php endif; ?>

<?php
    shn_form_fsopen(_t('Group Representative or Individual Information'));

        shn_form_text(_t("First Name"),'given_name','size="20"',array('req'=>true, 'help'=>'Given Name', 'value' => $client["given_name"]));
        shn_form_text(_t("Last Name"),'family_name','size="20"',array('req'=>true, 'help'=>'Family Name', 'value' => $client["family_name"]));
        shn_form_text(_t("MI"),'middle_initial','size="5"',array('help'=>'Middle Initials', 'value' => $client["custom_name"]));
        shn_form_text(_t("Age"),'years_old','size="5"',array('help'=>'Age', 'value' => $client["given_name"]));
        shn_form_date(_t("Date of Birth"), "dob", array('value' => $client["years_old"]));
        $opt_status = array("" => "- Select -", "Male" => "Male", "Female" => "Female");
        shn_form_select($opt_status, _t('Gender'), "gender", null,  array('value' => $client["opt_gender"], 'req' => true));

    shn_form_fsclose();

?>
<?php if (isset($client['group_primary']) && $client['group_primary'] != ''): ?>
    <div class="formdiv">
        <span class="heading">Group Member</span>
        <ul>
            <li>
                <span>Primary Representative:
                    <a href="index.php?mod=crs&amp;act=client_view&amp;uuid=<?php echo base64_encode($client['group_primary']) ?>">
                        <?php echo $client['client_group_name'] ?></a>
                </span>
            </li>
            <li>
                <label class="desc">Relationship to Primary <?php shn_tooltip_show("group_relation"); ?></label>
                <input id="group_relation" name="group_relation" type="text" class="field text medium" value="<?php if ($act == "client_save_edit" && isset($client['relation']))
                        echo $client['relation'] ?>" maxlength="255" tabindex="<?php echo++$shn_tabindex ?>" />
            </li>
        </ul>
    </div>
<?php endif; ?>
<?php
    shn_form_fsopen(_t('Contact Information'));

    shn_form_text(_t("Street Address"),'street_1','size="25"', array('value' => $client["street_1"]));
    shn_form_text(_t("Address Line 2"),'street_2','size="25"', array('value' => $client["street_2"]));
    shn_form_text(_t("City"),'city','size="10"', array('value' => $client["city"]));
    shn_form_text(_t("State / Province / Region"),'state','size="10"', array('value' => $client["state"]));
    shn_form_text(_t("Postal / Zip Code"),'postal','size="10"', array('value' => $client["postal"]));

    $q = "SELECT `option_code`, `option_description` FROM `field_options` WHERE `field_name` = 'opt_country' order by `option_description`;";
    $res = $global['db']->Execute($q);
    $opt_status = array();
    while (!$res->EOF) {
    $opt_status[$res->fields['option_code']] = $res->fields['option_description'];
    $res->MoveNext();
    }

    shn_form_select($opt_status, _t('Country'), "opt_country", null,  array('value'=>'USA'));
    shn_form_text(_t("Home Phone"),'home','size="10"', array('value' => $client["home_phone"]));
    shn_form_text(_t("Mobile Phone"),'mobile','size="10"', array('value' => $client["mobile_phone"]));
    shn_form_text(_t("Alternate Phone"),'alter','size="10"', array('value' => $client["alt_phone"]));
    shn_form_fsclose();

    shn_form_fsopen(_t('Demographic Information'));

    shn_form_text(_t("Occupation"),'occupation','size="20"', array('value' => $client['occupation']));
    shn_form_text(_t("Special Skills"),'skills','size="20"',array('help'=>'Type in comma separated.', 'value' => $skillsList));
    shn_form_text(_t("Languages Spoken"),'altlang1','size="10"',array('help'=>'1st Language', 'value' => $client["altlang1"]));
    shn_form_text("",'altlang2','size="10"',array('help'=>'2nd Language', 'value' => $client["altlang2"]));
    shn_form_text("",'altlang3','size="10"',array('help'=>'3rd Language', 'value' => $client["altlang3"]));

    shn_form_fsclose();

    shn_form_fsopen(_t('Emergency Contact'));

    shn_form_text(_t("Name"),'ec_name','size="20"', array('value' => $client['ec_name']));
    shn_form_text(_t("Relationship"),'ec_relation','size="20"', array('value' => $client["ec_relation"]));
    shn_form_text(_t("Phone Number"),'ec_phone','size="10"',array('help'=>'In ### - ### - #### format.', array('value' => $ecPhone['areacode'])));

    shn_form_fsclose();

    shn_form_fsopen(_t('Medical Alerts'));

    shn_form_checkbox(_t("Special Medical Need"),'special_medical','size="20"', array('value' => $client['special_medical']));

    shn_form_fsclose();


    shn_form_submit("Save", "class=\"styleTehButton\""); echo '&nbsp&nbsp';
    shn_form_submit("Save + Continue", "class=\"styleTehButton\""); echo '&nbsp&nbsp';
    shn_form_submit("Save + Add Group Member", "class=\"styleTehButton\""); echo '&nbsp';
    echo "<script>initDate();</script>";


?>

    <?php if ($act == "client_save_edit"): ?>
        <span>
            <input type="button" name="Delete" value="Delete" class="styleTehButton" onclick="confirmDelete();" tabindex="<?php echo++$shn_tabindex ?>"/>
        </span>
    <?php endif; ?>
<?php shn_form_fclose(); ?>
    <?php if ($act == "client_save_edit"): ?>

        <div>
            <br>
            <b>Note:</b> Deleting a shelter client is a permanent action and cannot be undone.
            In addition to this, deleting a shelter client will also remove the association of any additional persons
            related to this person.
        </div>

    <?php endif; echo "<br>";?>



