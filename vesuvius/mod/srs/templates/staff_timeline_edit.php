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
 * @author     Kaushika
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

<div id="dialog" title="Confirmation Required">
    Are you sure you want to delete?
</div>
<script type="text/javascript" src="res/js/jquery-1.6.4.min.js" ></script>
<script type="text/javascript" src="res/js/jquery.validate.min.js" ></script>
<script type="text/javascript" src="res/js/jquery-ui-1.8.17.custom.min.js" ></script>
<?php echo "<script type=\"text/javascript\" >".file_get_contents($global['approot']."/mod/srs/srs.js")."</script>" ?>
<script type="text/javascript">
    $(document).ready(function(){

        // Confirmation dialog
        $("#dialog").dialog({
            modal: true,
            bgiframe: true,
            autoOpen: false
        });

        $("#confirmDelete").click(function() {

            var url = "index.php?mod=srs&act=staff_timeline_del&id=<?php echo $data['id'] ?>";

            $("#dialog").dialog('option', 'buttons', {
                "Confirm" : function() {
                    window.location.href = url;
                },
                "Cancel" : function() {
                    $(this).dialog("close");
                }
            });

            $("#dialog").dialog("open");

        });
    });
</script>

<h1>Edit Timeline Data : <span class="highlight"><?php echo $staff['full_name'] ?></span></h1>

<?php include_once ($global['approot'] . 'mod/srs/templates/staff_menu.php'); ?>

<pre><?php print_r($_SESSION, true) ?></pre>

<?php
    shn_form_fopen($act, null, $formOpts);
    shn_form_hidden(array('id' => $data['id']));


?>
    <h2>Current Timeline Data</h2>
    <table id="timeline-results" class="mainTable" style="width: 950px !important;">
        <tr>
            <td class="mainRowEven"><span class="left"></span>Facility Type:</td>
            <td class="mainRowEven">
                <span class="left">
                    <?php echo $data['src_name'] ?>
                </span>
            </td>
        </tr>
        <tr>
            <td class="col1"><span class="left"></span>Facility Type:</td>
            <td class="col2">
                <span class="left">
                    <?php echo $data['src_type'] ?>
                </span>
            </td>
        </tr>
        <tr>
            <td class="mainRowEven"><span class="left"></span>Reported In:</td>
            <td class="mainRowEven">
                <span class="left">
                    <?php echo date("F j, Y H:i", strtotime($data['in_date'])) ?>
                </span>
            </td>
        </tr>
        <tr>
            <td class="col1"><span class="left"></span>Signed Out:</td>
            <td class="col2">
                <span class="left">
                    <?php echo!empty($data['out_date']) ? date("F j, Y H:i", strtotime($data['out_date'])) : "" ?>
                </span>
            </td>
        </tr>
        <tr>
            <td class="mainRowEven"><span class="left"></span>Duration:</td>
            <td class="mainRowEven">
                <span class="left">
                    <?php
                    if (!empty($data['out_date'])) {

                        $inDate = strtotime($data['in_date']);
                        $outDate = strtotime($data['out_date']);

                        if ($inDate <= $outDate) {
                            echo date_duration($data['in_date'], $data['out_date'], false);
                        } else {
                            echo "<span class=\"red\">Error: in > out time</span>";
                        }
                    } else {
                        echo date_duration($data['in_date'], null, false);
                    }
                    ?>
                </span>
            </td>
        </tr>
        <tr>
            <td class="col1"><span class="left"></span>Transfered To Facility:</td>
            <td class="col2">
                <span class="left">
                    <?php echo $data['dest_name'] . " - " . $data['dest_type'] ?>
                </span>
            </td>
        </tr>
    </table>


<?php

shn_form_fopen("client_checkin_save", null, array('enctype' => 'enctype="multipart/form-data"', 'req_message' => true));

    shn_form_fsopen(_t('Facility Report In'));
        shn_form_date(_t("Date"), "checkInDate", array('req'=>true, 'value' => date('Y-m-d', strtotime($data['in_date']))));
        shn_form_time(_t("Time"), "entryHour", "entryMinute", array('req'=>true, 'value' => $data['in_date']));

        $opt_status = array();
        $opt_status["select"] = "- Select -";

        foreach ($facilities as $facility) {
            $opt_status[$facility['facility_uuid']] = $facility['facility_name'];
        }

        shn_form_select($opt_status, _t('Assigned Facility Location'), "srcFacility", null,  array('value'=>$data["src_id"], 'req' => true));

    shn_form_fsclose();

    shn_form_fsopen(_t('Facility Sign Out'));
        shn_form_date(_t("Date"), "checkOutDate", array('req'=>true));
        shn_form_time(_t("Time"), "exitHour", "exitMinute", array('req'=>true));

        $opt_status = array();
        $opt_status["select"] = "- Select -";

        foreach ($facilities as $facility) {
            $opt_status[$facility['facility_uuid']] = $facility['facility_name'];
        }

        shn_form_select($opt_status, _t('Transferred To Facility'), "destFacility", null,  array('value'=> 'select', 'req' => true));

    shn_form_fsclose();

        shn_form_submit("Update", "class=\"styleTehButton\""); echo '&nbsp&nbsp';
        shn_form_button("Delete", "class=\"styleTehButton\" id=\"confirmDelete\""); echo '&nbsp&nbsp';

        echo "<script>initDate();</script>";
    shn_form_fclose();

?>
<?php shn_form_fsclose(); ?>
<?php shn_form_fclose(); ?>

