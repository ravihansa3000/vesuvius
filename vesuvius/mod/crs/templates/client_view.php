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
?>
<h1>View Details <span class="highlight"><?php echo $client['full_name'] ?></span></h1>

<?php include_once ('client_menu.php'); ?>
<style>
    h3 {
        color: #34689A;
    }
    .leftcol {
        background-color: #E5EAEF;
    }
</style>
<div class="clientview">
    <h3>Shelter Information</h3>

    <table class="mainTable" style="width: 650px !important;">
        <tbody>
            <tr>
                <td class="leftcol">Status</td>
                <td class="leftcol">
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
                <td class="rightcol">Check In Date</td>
                <td class="rightcol"><?php echo date("F j, Y H:i", strtotime($client['in_date'])) ?></td>
            </tr>
            <tr>
                <td class="leftcol">Check Out Date</td>
                <td class="leftcol"><?php echo isset($client['out_date']) ? date("F j, Y H:i", strtotime($client['out_date'])) : "" ?></td>
            </tr>
            <tr>
                <td class="rightcol">Facility Group</td>
                <td class="rightcol"><?php echo $client['facility_group'] ?></td>
            </tr>
            <tr>
                <td class="leftcol"><?php echo ($client['opt_status'] == "in") ? "Current" : "Last" ?> Facility</td>
                <td class="leftcol"><?php echo $client['facility_name'] ?></td>
            </tr>
            <tr>
                <td class="rightcol">Transfered To Facility</td>
                <td class="rightcol"><?php echo isset($client['dest_facility']) ? $client['dest_facility'] : "" ?></td>
            </tr>
        </tbody>
    </table>

    <h3>Basic Information</h3>
    <table class="mainTable" style="width: 650px !important;">
        <tbody>
            <tr>
                <td class="leftcol">First Name</td>
                <td class="leftcol"><?php echo $client['given_name'] ?></td>
            </tr>
            <tr>
                <td class="rightcol">Middle Initial</td>
                <td class="rightcol"><?php echo $client['custom_name'] ?></td>
            </tr>
            <tr>
                <td class="leftcol">Last Name</td>
                <td class="leftcol"><?php echo $client['family_name'] ?></td>
            </tr>
            <tr>
                <td class="rightcol">Date of Birth</td>
                <td class="rightcol"><?php echo isset($client['birth_date']) ? date('l F jS Y', strtotime($client['birth_date'])) : "" ?></td>
            </tr>
            <tr>
                <td class="leftcol">Age</td>
                <td class="leftcol"><?php echo $client['years_old'] ?></td>
            </tr>
        </tbody>
    </table>



    <h3>Contact Information</h3>
    <table class="mainTable" style="width: 650px !important;">
        <tbody>
            <tr>
                <td class="leftcol">Street Address</td>
                <td class="leftcol"><?php echo $client['street_1'] ?></td>
            </tr>
            <tr>
                <td class="rightcol">Address Line 2</td>
                <td class="rightcol"><?php echo $client['street_2'] ?></td>
            </tr>
            <tr>
                <td class="leftcol">City</td>
                <td class="leftcol"><?php echo $client['city'] ?></td>
            </tr>
            <tr>
                <td class="rightcol">State</td>
                <td class="rightcol"><?php echo $client['state'] ?></td>
            </tr>
            <tr>
                <td class="leftcol">Postal/Zip</td>
                <td class="leftcol"><?php echo $client['postal'] ?></td>
            </tr>
            <tr>
                <td class="rightcol">Country</td>
                <td class="rightcol"><?php echo $client['opt_country'] ?></td>
            </tr>
            <tr>
                <td class="leftcol">Home Phone</td>
                <td class="leftcol"><?php echo!empty($client['home_phone']) ? formatPhone($client['home_phone']) : "" ?></td>
            </tr>
            <tr>
                <td class="rightcol">Mobile Phone</td>
                <td class="rightcol"><?php echo!empty($client['mobile_phone']) ? formatPhone($client['mobile_phone']) : "" ?></td>
            </tr>
            <tr>
                <td class="leftcol">Alternate Phone</td>
                <td class="leftcol"><?php echo!empty($client['alt_phone']) ? formatPhone($client['alt_phone']) : "" ?></td>
            </tr>
            <tr>
                <td class="rightcol">Email</td>
                <td class="rightcol"><?php echo $client['email'] ?></td>
            </tr>
        </tbody>
    </table>

    <h3>Demographic Information</h3>
    <table class="mainTable" style="width: 650px !important;">
        <tbody>
            <tr>
                <td class="leftcol">Occupation</td>
                <td class="leftcol"><?php echo $client['occupation'] ?></td>
            </tr>
            <tr>
                <td class="rightcol">Special Skills</td>
                <td class="rightcol"><?php echo $skillsList ?></td>
            </tr>
            <tr>
                <td class="leftcol">Primary Language</td>
                <td class="leftcol"><?php echo $client['altlang1'] ?></td>
            </tr>
            <tr>
                <td class="rightcol">Second Language</td>
                <td class="rightcol"><?php echo $client['altlang2'] ?></td>
            </tr>
            <tr>
                <td class="leftcol">Third Language</td>
                <td class="leftcol"><?php echo $client['altlang3'] ?></td>
            </tr>
        </tbody>
    </table>

    <h3>Emergency Contact</h3>
    <table class="mainTable" style="width: 650px !important;">
        <tbody>
            <tr>
                <td class="leftcol">Name</td>
                <td class="leftcol"><?php echo $client['ec_name'] ?></td>
            </tr>
            <tr>
                <td class="rightcol">Relationship</td>
                <td class="rightcol"><?php echo $client['ec_relation'] ?></td>
            </tr>
            <tr>
                <td class="leftcol">Phone Number</td>
                <td class="leftcol"><?php echo $client['ec_phone'] ?></td>
            </tr>
        </tbody>
    </table>

    <?php if (isset($client['group_primary']) && $client['group_primary'] != ''): ?>
        <h3>Group Information</h3>
        <table class="mainTable" style="width: 650px !important;">
            <tbody>
                <tr>
                    <td class="leftcol">Group</td>
                    <td class="leftcol"><?php echo $client['client_group_name'] ?></td>
                </tr>
                <tr>
                    <td class="rightcol">Relationship to Group Primary</td>
                    <td class="rightcol"><?php echo $client['relation'] ?></td>
                </tr>
            </tbody>
        </table>
    <?php else: ?>
        <h3>Group Information</h3>
        <table class="mainTable" style="width: 650px !important;">
            <tbody>
                <tr>
                    <td class="leftcol">Infants</td>
                    <td class="leftcol"><?php echo $client['infants'] ?></td>
                </tr>
                <tr>
                    <td class="rightcol">Children</td>
                    <td class="rightcol"><?php echo $client['children'] ?></td>
                </tr>
                <tr>
                    <td class="leftcol">Adults</td>
                    <td class="leftcol"><?php echo $client['adults'] ?></td>
                </tr>
                <tr>
                    <td class="rightcol">Unknown Age</td>
                    <td class="rightcol"><?php echo $client['unknown'] ?></td>
                </tr>
                <tr>
                    <td class="leftcol"><i>Total Members</i></td>
                    <td class="leftcol"><i><?php echo $client['group_count'] ?></i></td>
                </tr>
            </tbody>
        </table>
    <?php endif; ?>

    <h3>Medical Status</h3>
    <table class="mainTable" style="width: 650px !important;">
        <tbody>
            <tr>
                <td class="leftcol">Special Medical Needs</td>
                <td class="leftcol"><?php echo ($client['special_medical']) ? "Yes" : "No" ?></td>
            </tr>
        </tbody>
    </table>
</div>