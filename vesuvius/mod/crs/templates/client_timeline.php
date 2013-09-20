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
$i = 1;

?>

<h1>Registration Timeline <span class="highlight"><?php echo $client['given_name']." ".$client['family_name'] ?></span></h1>

<?php include_once ('client_menu.php'); ?>
<span>The timeline below shows the check in and check out dates of a client.</span>
<br>
<span>Current Status:
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
</span>
<br>
<br>
<table class="mainTable" style="width: 950px !important;">
<tbody>
<tr>
    <td>&nbsp;</td>
    <td>Facility</td>
    <td>Checked In</td>
    <td>Checked Out</td>
    <td>Duration</td>
    <td>Transfered To</td>
    <td>Transfered By</td>
</tr>
<?php $j=0;?>
    <?php foreach ($results as $row): ?>
        <?php $class = ($j%2 == 0 ? "mainRowEven" : "mainRowOdd");?>
        <tr>
            <td class="<?php echo $class; ?>"><?php echo $i++ ?>.</td>
            <td class="<?php echo $class; ?>">
                <?php echo $row['facility_name'] ?>
            </td class="<?php echo $class; ?>">
            <td class="<?php echo $class; ?>"><?php echo date("F j, Y H:i", strtotime($row['in_date'])) ?></td>
            <td class="<?php echo $class; ?>"><?php echo!empty($row['out_date']) ? date("F j, Y H:i", strtotime($row['out_date'])) : "" ?></td>
            <td class="<?php echo $class; ?>"><?php echo!empty($row['out_date']) ? date_duration($row['in_date'], $row['out_date'], FALSE) : date_duration($row['in_date'], NULL, FALSE) ?></td>
            <td class="<?php echo $class; ?>"><?php echo $row['dest_name'] ?></td>
            <td class="<?php echo $class; ?>"><?php echo $row['user'] ?></td>
        </tr>
        <?php $j++; ?>
    <?php endforeach; ?>
</tbody>
</table>

