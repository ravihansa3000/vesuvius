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
global $shn_tabindex;
$shn_tabindex = 0;
$helpid = 0;
?>
<script type="text/javascript">
  $(document).ready(function(){

    $('.tooltip').each(function(index, obj) {
      var url = "index.php";
      var dialog = $('<div></div>');
      

      var link = $(this).one('click', function() {
        var id = $(this).attr('id');
        $.getJSON(
        url, 
        {stream: "json", mod: "srs", act: "tooltips", tip: id },
        function(data) {
          var content = "Sorry, no tooltip help found for <i>" + id + "</i>";
          var title = '';
          if(data != null) {
            title = data.title
            content = data.tip;

            if(data.defaults) {
              content += "<p><i>Default value: "+ data.defaults +"</i></p>";
            }
          }
          dialog
          .html('<div>'+ content +'</div>')
          .dialog({
            title: "(" + (index+1) +") " + title,
            position: {
              my: 'left',
              at: 'right',
              of: obj,
              offset: "20 65"
            }
          });
        }
      )
          
        link.click(function() {
          dialog.dialog('open');
          return false;
        });
        return false;
      });
    });

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

    $('#entryDatepicker').datepicker({
      beforeShow: readentryDate, onSelect: updateentryDate,
      minDate: new Date(<?php echo date('Y') - 110; ?>, 1 - 1, 1), maxDate: new Date(<?php echo date('Y'); ?>, 12 - 1, 31),
      showOn: 'both', buttonImageOnly: true, buttonImage: 'theme/<?php echo $conf['theme']; ?>/img/icon-calendar.gif'});

    // Prepare to show a date picker linked to three select controls
    function readentryDate() {
      $('#entryDatepicker').val($('#entryMonth').val() + '/' +
        $('#entryDay').val() + '/' + $('#entryYear').val());
      return {};
    }

    // Update three select controls to match a date picker selection
    function updateentryDate(date) {
      $('#entryMonth').val(date.substring(0, 2));
      $('#entryDay').val(date.substring(3, 5));
      $('#entryYear').val(date.substring(6, 10));
    }


  });
</script>

<h1>Check In Client <span class="highlight"><?php echo $staff['full_name'] ?></span></h1>

<?php include_once ('staff_menu.php'); ?>

<?php
shn_form_fopen($act, null, $formOpts);
shn_form_hidden(array('uuid' => base64_encode($uuid)));
?>

<fieldset>
  <legend>Check In Form</legend>
  <ul>
    <?php if ($staff['opt_status'] == "out"): ?>
      <li>
        <div class="message warning">
          <p><?php echo $staff['full_name'] ?> checked out of the <em><?php echo $staff['facility_name'] ?></em> facility at <em><?php echo date("F j, Y H:i", strtotime($staff['out_date'])) ?></em>.
            The staff's previous facility has been selected by default. You can change this selection as required.</p>
        </div>
      </li>
    <?php endif; ?>
    <li class="complex ">
      <label class="desc">Date and Time</label>
      <span>
        <select name="entryMonth" id="entryMonth" tabindex="<?php echo++$shn_tabindex ?>">
          <?php
          foreach (range(1, 12) as $M):
            $month = date('n');
            $selected = (date('n') == $M) ? " selected" : "";
            $value = str_pad($M, 2, "0", STR_PAD_LEFT);
            $display = date('F', mktime(0, 0, 0, $M, 1, 2000));
            ?>
            <option value ="<?php echo $value ?>"<?php echo $selected ?>><?php echo $display ?></option>
          <?php endforeach; ?>
        </select>
        <label for="entryMonth">Month <?php shn_tooltip_show("entryMonth"); ?></label>
      </span>
      <span>
        <select name="entryDay" id="entryDay" tabindex="<?php echo++$shn_tabindex ?>">
          <?php
          foreach (range(1, 31) as $d):

            $selected = (date('d') == $d) ? " selected" : "";
            $value = str_pad($d, 2, "0", STR_PAD_LEFT);
            $display = date('d', mktime(0, 0, 0, 1, $d, 2000));
            ?>
            <option value ="<?php echo $value ?>"<?php echo $selected ?>><?php echo $display ?></option>
          <?php endforeach; ?>
        </select>
        <label for="entryDay">Day <?php shn_tooltip_show("entryDay"); ?></label>
      </span>
      <span>
        <select name="entryYear" id="entryYear" tabindex="<?php echo++$shn_tabindex ?>">
          <?php
          for ($y = (date('Y') - 1); $y <= date('Y'); $y++):

            $selected = (date('Y') == $y) ? " selected" : "";
            $value = str_pad($y, 2, "0", STR_PAD_LEFT);
            $display = date('Y', mktime(0, 0, 0, 1, 1, $y));
            ?>
            <option value ="<?php echo $value ?>"<?php echo $selected ?>><?php echo $display ?></option>
          <?php endfor; ?>
        </select>     
        <label for="entryYear">Year <?php shn_tooltip_show("entryYear"); ?></label>
      </span>
      <span>
        <input type="hidden" disabled="disabled" id="entryDatepicker" size="10">
      </span>
      <span>
        <select name="entryHour" id="entryHour" tabindex="<?php echo++$shn_tabindex ?>">
          <?php
          foreach (range(0, 23) as $h):

            $selected = (date('G') == $h) ? " selected" : "";
            $value = str_pad($h, 2, "0", STR_PAD_LEFT);
            $display = $value;
            ?>
            <option value ="<?php echo $value ?>"<?php echo $selected ?>><?php echo $display ?></option>
          <?php endforeach; ?>
        </select>
        <label for="entryHour">Hour <?php shn_tooltip_show("entryHour"); ?></label>
      </span>
      <span>:</span>
      <span>
        <select name="entryMinute" id="entryMinute" tabindex="<?php echo++$shn_tabindex ?>">

          <?php
          foreach (range(0, 59) as $m):

            $selected = (date('i') == $m) ? " selected" : "";

            $value = str_pad($m, 2, "0", STR_PAD_LEFT);
            $display = $value;
            ?>
            <option value ="<?php echo $value ?>"<?php echo $selected ?>><?php echo $display ?></option>
          <?php endforeach; ?>
        </select>
        <label for="entryMinute">Minute <?php shn_tooltip_show("entryMinute"); ?></label>
      </span>
    </li>
    <li>
      <label class="desc">Facility Location <span class="req">*</span></label>
      <span>
        <select class="required" name="facility" id="facility" tabindex="<?php echo++$shn_tabindex ?>">
          <option value=""></option>
          <?php foreach ($facilities as $facility): ?>
            <?php if ($facility["facility_uuid"] == $staff["facility_uuid"] && $staff['opt_status'] == "out"): ?>
              <option value="<?php echo $facility["facility_uuid"]; ?>" selected><?php echo $facility["facility_name"]; ?></option>
            <?php else: ?>
              <option value="<?php echo $facility["facility_uuid"]; ?>"><?php echo $facility["facility_name"]; ?></option>
            <?php endif; ?>
          <?php endforeach; ?>
        </select>
        <?php shn_tooltip_show("facility"); ?>
      </span>
    </li>
  </ul>
</fieldset>
<fieldset>
  <legend>User Action</legend>
  <ul>
    <li>
      <span>
        <?php shn_form_submit("Check In", $submitOpts); ?>
      </span>
    </li>
  </ul>
</fieldset>


</ul>
<?php shn_form_fsclose(); ?>
<?php shn_form_fclose(); ?>
