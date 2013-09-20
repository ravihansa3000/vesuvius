<?php
/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
?>

<div id="dialog" title="Confirmation Required">
    Are you sure you want to delete?
</div>
<script type="text/javascript" src="res/js/jquery-1.6.4.min.js" ></script>
<script type="text/javascript" src="res/js/jquery.validate.min.js" ></script>
<script type="text/javascript" src="res/js/jquery-ui-1.8.17.custom.min.js" ></script>
<script type="text/javascript">
    $(document).ready(function(){
        
        // Validate
        $("#form0").validate({errorPlacement: function(error, element) {
                if ( element.is(":radio") )
                    error.insertAfter(element.parent().parent());
                else if ( element.is(":checkbox") )
                    error.insertBefore(element);
                else
                    error.insertAfter(element);
            }
        });

        // Confirmation dialog
        $("#dialog").dialog({
            modal: true,
            bgiframe: true,
            autoOpen: false
        });


        $(".confirmLink").click(function(e) {
            e.preventDefault();
            var url = $(this).attr("href");
            
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

<style>
    tr td {
        text-align: center !important;
    }

    .head {
        font-weight: bold;
    }

</style>

<h1>Volunteer Status Settings</h1>

<?php shn_form_fopen($act, null, $formOpts); ?>

<?php
if ($submitName == 'Save')
    shn_form_hidden(array('id' => $volStatusId));
else
    $volStatusName = "";
?>

<fieldset>
    <legend><?php echo ($submitName == 'Add') ? "Add" : "Edit"; ?> Volunteer Status</legend>
            <span>
                <input type="text" name ="org_name" size="45" class="required field text" value="<?php echo $volStatusName ?>"/>
            </span>
            <span>
                <?php shn_form_submit($submitName, "class=\"styleTehButton\""); ?>
            </span>
</fieldset>

<?php shn_form_fclose(); ?>
<table class="mainTable" style="width: 650px !important;">
    <tbody>
    <tr class="head" height="30px">
        <td width="50px">ID</td>
        <td width="200px">Name</td>
        <td width="200px">Last Updated</td>
        <td width="200px" colspan="2">Actions</td>
    </tr>
        <?php if (is_array($volStatuses)) : ?>
            <?php $i=0;?>
            <?php foreach ($volStatuses as $row): ?>
                <?php $class = ($i%2 == 0 ? "mainRowEven" : "mainRowOdd");?>
                <tr>
                    <td class="<?php echo $class; ?>"><?php echo $row['id'] ?></td>
                    <td class="<?php echo $class; ?>"><?php echo $row['description'] ?></td>
                    <td class="<?php echo $class; ?>"><?php echo $row['updated_at'] ?></td>
                    <td class="<?php echo $class; ?>">
                        <a class="" href="index.php?mod=srs&amp;act=volstatus_edit&amp;id=<?php echo $row['id'] ?>">Edit</a>
                    </td>
                    <td class="<?php echo $class; ?>">
                        <a class="confirmLink" href="index.php?mod=srs&amp;act=volstatus_del&amp;id=<?php echo $row['id'] ?>">Delete</a>
                    </td>
                </tr>
                <?php $i++; ?>
            <?php endforeach; ?>
        <?php endif; ?>
    </tbody>
</table>
