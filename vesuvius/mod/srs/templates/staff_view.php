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
?>
<h1>View Details <span class="highlight"><?php echo $staff['full_name'] ?></span></h1>

<?php include_once ('staff_menu.php'); ?>
<style>
    h3 {
        color: #34689A;
    }
    .leftcol {
        background-color: #E5EAEF;
    }
</style>
<div class="clientview">
    <h2>Shelter Information</h2>

    <table class="mainTable" style="width: 650px !important;">
        <tbody>
            <tr>
                <td class="leftcol">Current Status</td>
                <td class="leftcol">
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
                </td>
            </tr>
            <tr>
                <td class="rightcol">Last Check In Date</td>
                <td class="rightcol"><?php echo date("F j, Y H:i", strtotime($staff['in_date'])) ?></td>
            </tr>
            <tr>
                <td class="leftcol">Last Check Out Date</td>
                <td class="leftcol"><?php echo isset($staff['out_date']) ? date("F j, Y H:i", strtotime($staff['out_date'])) : "" ?></td>
            </tr>
            <tr>
                <td class="rightcol">Facility Group</td>
                <td class="rightcol"><?php echo $staff['facility_group'] ?></td>
            </tr>
            <tr>
                <td class="leftcol"><?php echo ($staff['opt_status'] == "in") ? "Current" : "Last" ?> Facility</td>
                <td class="leftcol"><?php echo $staff['facility_name'] ?></td>
            </tr>
            <tr>
                <td class="rightcol">Transfered To Facility</td>
                <td class="rightcol"><?php echo isset($staff['dest_facility']) ? $staff['dest_facility'] : "" ?></td>
            </tr>
        </tbody>
    </table>

    <h2>Basic Information</h2>
    <table class="mainTable" style="width: 650px !important;">
        <tbody>
            <tr>
                <td class="leftcol">First Name</td>
                <td class="leftcol"><?php echo $staff['given_name'] ?></td>
            </tr>
            <tr>
                <td class="rightcol">Middle Initial</td>
                <td class="rightcol"><?php echo $staff['custom_name'] ?></td>
            </tr>
            <tr>
                <td class="leftcol">Last Name</td>
                <td class="leftcol"><?php echo $staff['family_name'] ?></td>
            </tr>
            <tr>
                <td class="rightcol">Date of Birth</td>
                <td class="rightcol"><?php echo isset($staff['birth_date']) ? date('l F jS Y', strtotime($staff['birth_date'])) : "" ?></td>
            </tr>
            <tr>
                <td class="leftcol">Age</td>
                <td class="leftcol"><?php echo $staff['years_old'] ?></td>
            </tr>
        </tbody>
    </table>

    <h2>Contact Information</h2>
    
    <h3>Home</h3>
    <table class="mainTable" style="width: 650px !important;">
        <tbody>
            <tr>
                <td class="leftcol">Street Address</td>
                <td class="leftcol"><?php echo $staff['street_1'] ?></td>
            </tr>
            <tr>
                <td class="rightcol">Address Line 2</td>
                <td class="rightcol"><?php echo $staff['street_2'] ?></td>
            </tr>
            <tr>
                <td class="leftcol">City</td>
                <td class="leftcol"><?php echo $staff['city'] ?></td>
            </tr>
            <tr>
                <td class="rightcol">State</td>
                <td class="rightcol"><?php echo $staff['state'] ?></td>
            </tr>
            <tr>
                <td class="leftcol">Postal/Zip</td>
                <td class="leftcol"><?php echo $staff['postal'] ?></td>
            </tr>
            <tr>
                <td class="rightcol">Country</td>
                <td class="rightcol"><?php echo $staff['opt_country'] ?></td>
            </tr>
            <tr>
                <td class="leftcol">Home Phone</td>
                <td class="leftcol"><?php echo!empty($staff['home_phone']) ? formatPhone($staff['home_phone']) : "" ?></td>
            </tr>
            <tr>
                <td class="rightcol">Email</td>
                <td class="rightcol"><?php echo $staff['email'] ?></td>
            </tr>
        </tbody>
    </table>
    
    <h3>Work</h3>
    <table class="mainTable" style="width: 650px !important;">
        <tbody>
            <tr>
                <td class="leftcol">Street Address</td>
                <td class="leftcol"><?php echo $staff['work_street_1'] ?></td>
            </tr>
            <tr>
                <td class="rightcol">Address Line 2</td>
                <td class="rightcol"><?php echo $staff['work_street_2'] ?></td>
            </tr>
            <tr>
                <td class="leftcol">City</td>
                <td class="leftcol"><?php echo $staff['work_city'] ?></td>
            </tr>
            <tr>
                <td class="rightcol">State</td>
                <td class="rightcol"><?php echo $staff['work_state'] ?></td>
            </tr>
            <tr>
                <td class="leftcol">Postal/Zip</td>
                <td class="leftcol"><?php echo $staff['work_postal'] ?></td>
            </tr>
            <tr>
                <td class="rightcol">Home Phone</td>
                <td class="rightcol"><?php echo!empty($staff['work_phone']) ? formatPhone($staff['work_phone']) : "" ?></td>
            </tr>
            <tr>
                <td class="leftcol">Email</td>
                <td class="leftcol"><?php echo $staff['work_email'] ?></td>
            </tr>
        </tbody>
    </table class="mainTable" style="width: 650px !important;">

    <h2>Demographic Information</h2>
    <table class="mainTable" style="width: 650px !important;">
        <tbody>
            <tr>
                <td class="leftcol">Occupation</td>
                <td class="leftcol"><?php echo $staff['occupation'] ?></td>
            </tr>
            <tr>
                <td class="rightcol">Special Skills</td>
                <td class="rightcol"><?php echo $skillsList ?></td>
            </tr>
            <tr>
                <td class="leftcol">Primary Language</td>
                <td class="leftcol"><?php echo $staff['altlang1'] ?></td>
            </tr>
            <tr>
                <td class="rightcol">Second Language</td>
                <td class="rightcol"><?php echo $staff['altlang2'] ?></td>
            </tr>
            <tr>
                <td class="leftcol">Third Language</td>
                <td class="leftcol"><?php echo $staff['altlang3'] ?></td>
            </tr>
        </tbody>
    </table>

    
</div>