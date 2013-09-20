<?php
/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

global $conf;
global $shn_tabindex;
$shn_tabindex = 1;
?>

    <h1>CSV Bulk Import Facilities</h1>

<?php shn_form_fopen("importcsv", null, array('enctype' => 'enctype="multipart/form-data"')); ?>
<?php shn_form_hidden(array('type' => 'excel', 'orphans' => 'ignore')); ?>

<?php
    shn_form_fsopen("Import Facilities CSV File");
    shn_form_upload("Upload File", "csvImport");
    shn_form_fsclose();
    shn_form_submit("Submit", "class=\"styleTehButton\"");
    shn_form_fclose();
?>