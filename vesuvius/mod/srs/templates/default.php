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
<h1>Staff Registration Module</h1>

<p>The Staff Registration Module (SRM) registers the individuals who will staff the shelters and logistics centers.</p>
<ul>
    <li>The SRM tracks staff members working at facilities and includes information such as work location and job description.</li>
    <li>The SRM allows the user to locate, edit, and sort records, and to query lists by agency or name</li>
    <li>The SRM provides basic reports on the number of staff registered – subtotaled by agency and by classification </li>
</ul>

<?php //print ("<pre>" . print_r($_SESSION, true) . "</pre>") ?>

<?php if (empty($incident)): ?>
    <?php shn_form_fsopen("Select Incident") ?>
    <p>You must select an event before using the srs.</p>
    <label>Active Events</label>
    <select id="incident_select" name="event">
        <option value="-1"></option>
        <?php foreach ($events as $event): ?>
            <option value="<?php echo $event['incident_id'] ?>"><?php echo $event['name'] ?></option>
        <?php endforeach; ?>
    </select>
    <?php shn_form_fsclose() ?>
<?php endif; ?>

