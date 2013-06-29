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
<h1>Client Registry Module</h1>

<p>The Client Registry Module (CRM) is a central online repository where information on all
    shelter client individuals, families and other group relationships are stored. 
    You will be transcribing information directly
    from the Shelter Registration form into this Client Registry Module.
</p>
<p>Information from the Shelter Registration form includes:</p>
<ul>
    <li>Client name</li>
    <li>Hosting facility</li>
    <li>Date and time of entry or exit</li>
    <li>Contact information (address, phone, etc.)</li>
    <li>Demographic information</li>
    <li>Group relationships (family member, student, care giver, etc.)</li>
    <li>Pets</li>
</ul>
<p>Please ensure that both sides of the Registration form are accurately entered into the Client Registry Module.</p>

<?php //print ("<pre>" . print_r($_SESSION, true) . "</pre>") ?>

<?php if (empty($incident)): ?>
    <?php shn_form_fsopen("Select Incident") ?>
    <p>You must select an event before using the CRS.</p>
    <label>Active Events</label>
    <select id="incident_select" name="event">
        <option value="-1"></option>
        <?php foreach ($events as $event): ?>
            <option value="<?php echo $event['incident_id'] ?>"><?php echo $event['name'] ?></option>
        <?php endforeach; ?>
    </select>
    <?php shn_form_fsclose() ?>
<?php endif; ?>

