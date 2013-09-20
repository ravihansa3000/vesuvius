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
?>
<script type="text/javascript">
    $(document).ready(function(){
    
        // Validate
        $("#form0").validate({errorPlacement: function(error, element) {
                if ( element.is(":radio") )
                    error.insertBefore(element);
                else if ( element.is(":checkbox") )
                    error.insertBefore(element);
                else
                    error.insertAfter(element);
            }
        });
    });
</script>
<?php include_once ('client_menu.php'); ?>
<?php if ($act == "client_petsave_new"): ?>
    <h1>Add Pet To <span class="highlight"><?php echo $client['given_name']." ".$client['family_name'] ?></span></h1>
<?php else: ?>
    <h1>Edit Pet <span class="highlight"><?php echo $client['given_name']." ".$client['family_name'] ?></span></h1>
<?php endif; ?>

<?php
shn_form_fopen($act, null, $formOpts);

if ($act == "client_petsave_edit")

    shn_form_hidden(array('uuid' => $uuid, 'pet_id' => $petId));

    shn_form_fsopen(_t('Details'));

        shn_form_text(_t("Name"),'pet_name','size="20"', array('req'=>true, 'help' => 'Pet Name'));
        shn_form_text(_t("Type of Pet"),'pet_type','size="20"', array('req'=>true, 'help' => 'Pet Type'));
        shn_form_text(_t("Gender"),'pet_gender','size="20"', array('req'=>true, 'help' => 'Pet Gender'));
        shn_form_text(_t("Age"),'pet_age','size="20"', array('help' => 'Pet Age'));
        shn_form_text(_t("Breed"),'pet_breed','size="20"', array( 'help' => 'Pet Breed'));
        shn_form_text(_t("Color"),'pet_color','size="20"', array('help' => 'Pet Color'));

    shn_form_fsclose();

    shn_form_submit("Save", "class=\"styleTehButton\"");;

shn_form_fclose();
?>
