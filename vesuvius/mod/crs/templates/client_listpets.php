<?php
/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
?>

<h1>Pets Registered For <span class="highlight"><?php echo $client['full_name'] ?></span></h1>
<?php include_once ('client_menu.php'); ?>

<style>
    tr td {
        text-align: center !important;
    }

    .head {
        font-weight: bold;
    }

</style>

<span>This table displays a list of pets currently assigned to <?php echo $client['first_name']." ".$client['family_name'] ?>. Client on the name to edit the pet details.</span>
<table class="mainTable" style="width: 650px !important;">
    <tbody>
    <tr class="head" height="30px">
        <td>Name</td>
        <td>Type</td>
        <td>Gender</td>
        <td>Age</td>
        <td>Breed</td>
        <td>Color</td>
        <td>Last Updated</td>
    </tr>
    <?php $i=0;?>
        <?php foreach ($pets as $pet): ?>
            <?php $class = ($i%2 == 0 ? "mainRowEven" : "mainRowOdd");?>
            <tr>
                <td class="<?php echo $class; ?>"><a href="index.php?mod=crs&amp;act=client_addpet&amp;pet_id=<?php echo $pet['id'] ?>"><?php echo $pet['pet_name']?></a></td>
                <td class="<?php echo $class; ?>"><?php echo $pet['type_of_pet']?></td>
                <td class="<?php echo $class; ?>"><?php echo $pet['sex']?></td>
                <td class="<?php echo $class; ?>"><?php echo $pet['age']?></td>
                <td class="<?php echo $class; ?>"><?php echo $pet['breed']?></td>
                <td class="<?php echo $class; ?>"><?php echo $pet['color']?></td>
                <td class="<?php echo $class; ?>"><?php echo $pet['updated_at']?></td>
            </tr>
            <?php $i++; ?>
        <?php endforeach; ?>
    </tbody>
</table>