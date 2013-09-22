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
$i = 1;
?>

<h1>Registration Timeline <span class="highlight"><?php echo $staff['full_name'] ?></span></h1>
<style>
    .mainTable {
        margin-bottom: 0px !important;
    }
</style>
<?php include_once ('staff_menu.php'); ?>
<span>The timeline below shows the reported in and sign out dates and time of a staff.</span><br><br>
<span>Current Status:
    <?php switch ($staff['opt_status']):
        case "in": ?>
            <span class="checked_in">Reported In</span>
            <?php break; ?>
        <?php case "out": ?>
            <span class="checked_out">Signed Out</span>
            <?php break; ?>
        <?php case "trn": ?>
            <span class="checked_out">Transfered</span>
            <?php break; ?>
    <?php endswitch; ?>
</span></br>
<br>


<?php foreach ($results as $row): ?>
    <table id="timeline-results" class="mainTable" style="width: 950px !important;">
        <tr>
            <td class="mainRowEven">
                <span class="rownumber">
                    <?php echo $i++ ?>.
                </span>
            </td>
            <td class="mainRowEven">
                <?php echo $row['facility_name'] ?>
            </td>
            <td class="mainRowEven">
                <span class="editlink right">
                    <a href="index.php?mod=srs&amp;act=staff_timeline_edit&amp;id=<?php echo $row['id'] ?>">Edit</a>
                </span>
            </td>
        </tr>
    </table>

    <table id="timeline-results" class="mainTable" style="width: 950px !important;">
        <tr>
            <td class="mainRowOdd"><span class="left"></span>Facility Type:</td>
            <td class="mainRowOdd">
                <span class="left">
                    <?php echo $row['facility_resource_type_abbr'] ?>
                </span>
            </td>
        </tr>
        <tr>
            <td class="mainRowEven"><span class="left"></span>Reported In:</td>
            <td class="mainRowEven">
                <span class="left">
                    <?php echo date("F j, Y H:i", strtotime($row['in_date'])) ?>
                </span>
            </td>
        </tr>
        <tr>
            <td class="mainRowOdd"><span class="left"></span>Signed Out:</td>
            <td class="mainRowOdd">
                <span class="left">
                    <?php echo!empty($row['out_date']) ? date("F j, Y H:i", strtotime($row['out_date'])) : "" ?>
                </span>
            </td>
        </tr>
        <tr>
            <td class="mainRowEven"><span class="left"></span>Duration:</td>
            <td class="mainRowEven">
                <span class="left">
                    <?php
                    if (!empty($row['out_date'])) {

                        $inDate = strtotime($row['in_date']);
                        $outDate = strtotime($row['out_date']);

                        if ($inDate <= $outDate) {
                            echo date_duration($row['in_date'], $row['out_date'], false);
                        } else {
                            echo "<span class=\"red\">Error: in > out time</span>";
                        }
                    } else {
                        echo date_duration($row['in_date'], null, false);
                    }
                    ?>
                </span>
            </td>
        </tr>
        <tr>
            <td class="mainRowOdd"><span class="left"></span>Transfered To Facility:</td>
            <td class="mainRowOdd">
                <span class="left">
                    <?php echo $row['dest_name'] . " - " . $row['dest_type'] ?>
                </span>
            </td>
        </tr>
        <tr>
            <td class="mainRowEven"><span class="left"></span>User Initiating Transfer:</td>
            <td class="mainRowEven">
                <span class="left">
                    <?php echo $row['user'] ?>
                </span>
            </td>
        </tr>
    </table>
<?php endforeach; ?>



