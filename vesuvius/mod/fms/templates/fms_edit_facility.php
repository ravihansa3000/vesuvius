<?php
/**
 * Facility Management System Module
 *
 * PHP version >=5.1
 *
 * tdCENSE: This source file is subject to LGPL tdcense
 * that is available through the world-wide-web at the following URI:
 * http://www.gnu.org/copyleft/lesser.html
 *
 * @author     Clayton Kramer <clayton.kramer@mail.cuny.edu>
 * @package    module FMS
 * @tdcense    http://www.gnu.org/copyleft/lesser.html GNU Lesser General Pubtdc tdcense (LGPL)
 *
 */
global $conf;
global $shn_tabindex;
$shn_tabindex = 0;
?>
<script type="text/javascript"src="https://maps.google.com/maps/api/js?sensor=false"></script>
<?php if ($mode == "edit"): ?>
    <script type="text/javascript">

        function confirmDelete() {
            var answer = confirm('Are you sure you want to delete this facility? This action cannot be undone.');
            if(answer) {
                window.location = 'index.php?mod=fms&act=delete&id=<?php echo base64_encode($facility['facility_uuid']) ?>';
            }
        }

    </script>
<?php endif; ?>
<script type="text/javascript">
    var MYMAP = {
        bounds: null,
        map: null
    }
    MYMAP.init = function(selector, latLng, zoom) {
        var myOptions = {
            zoom: zoom,
            center: latLng,
            mapTypeId: google.maps.MapTypeId.ROADMAP
        }
        this.map = new google.maps.Map($(selector)[0], myOptions);
        this.bounds = new google.maps.LatLngBounds();
    }
                                            
    MYMAP.placeMarker = function(point, name) {
                                        			
        // extend the bounds to include the new point
        MYMAP.bounds.extend(point);
                                        			
        var marker = new google.maps.Marker({
            position: point,
            title: name,
            map: MYMAP.map
        });     
                                        			
        var infoWindow = new google.maps.InfoWindow();
        var html = "value.info";
                                                        
                                                        
        google.maps.event.addListener(marker, 'click', function() {
            infoWindow.setContent(html);
            infoWindow.open(MYMAP.map, marker);
        });
        //MYMAP.map.fitBounds(MYMAP.bounds);
                           
                       
    }

    $(document).ready(function() {
                                                
        $("#map_canvas").css({
            height: 500,
            width: 800
        });
                                                
        $("#geocode").click(function(){ 
                                        
            var address = $('#street_1').val() + " ";
            address += $('#city').val() + ", ";
            address += $('#state').val() + " ";
            address += $('#postal').val() + " ";
                                            
            var geocoder = new google.maps.Geocoder();

            geocoder.geocode( { 'address': address}, function(results, status) {

                if (status == google.maps.GeocoderStatus.OK) {
                    var lat = results[0].geometry.location.lat();
                    var lng = results[0].geometry.location.lng();
                        
                    $('#lat').val(lat);
                    $('#long').val(lng);
                        
                    //showLocation(lat, lng, $('#name').val());
                        
                    var myLatLng = new google.maps.LatLng(lat, lng);
                    MYMAP.init('#map_canvas', myLatLng, 12);
                    MYMAP.placeMarker(myLatLng, name);
                                       
                } else {
                    alert("Error retrieving latitude and longitude.")
                }
            }); 

            showLocation($('#lat').val(), $('#long').val(), $('#name').val())     
        });
                     
        function showLocation(lat, lng, name) {
            var myLatLng = new google.maps.LatLng(lat, lng);
            MYMAP.init('#map_canvas', myLatLng, 12);
            MYMAP.placeMarker(myLatLng, name);
        }
            
<?php if ($mode == "edit"): ?>
            showLocation(<?php echo $facility['latitude'] ?>, <?php echo $facility['longitude'] ?>, '<?php echo $facility['name'] ?>');
<?php endif; ?>
                                                
    });
</script>

<?php if ($mode == "edit"): ?>
    <h1>Edit Facility</h1>
<?php else: ?>
    <h1>Add Facility</h1>
<?php endif; ?>

<span>Use this form to view or edit facility information</span>

<?php
shn_form_fopen("controller", null, array('enctype' => 'enctype="multipart/form-data"'));
?>
<?php shn_form_hidden(array('mode' => $mode)); ?>
<?php if ($mode == "edit"): ?>
    <?php shn_form_hidden(array('id' => base64_encode($facility['facility_uuid']))); ?>
<?php endif; ?>

<?php
shn_form_fopen("controller", null, array('enctype' => 'enctype="multipart/form-data"', 'req_message' => true));

    shn_form_hidden(array('seq' => 'entry'));

    shn_form_fsopen(_t('Basic'));
    shn_form_text(_t("Name"),'name','size="20"',array('req'=>true));
    shn_form_text(_t("Facility Code"),'code','size="20"',array('req'=>true));
    shn_form_text(_t("Capacity"),'capacity','size="20"',array('req'=>true));

    $opt_status = array();
    $opt_status[""] = "- Select -";

    foreach ($types as $type) {
        $opt_status[$type] = $type;
    }

    shn_form_select($opt_status, _t('Resource Abbr'), "type", null,  array('req' => true));

    $opt_status = array();
    $opt_status[""] = "- Select -";

    foreach ($statuses as $status) {
        $opt_status[$status] = $status;
    }

    shn_form_select($opt_status, _t('Status Allocation'), "status", null,  array('req' => true));

    $opt_status = array();
    $opt_status[""] = "- Select -";

    foreach ($groups as $group) {
        $opt_status[$group] = $group;
    }

    shn_form_select($opt_status, _t('Group Name'), "group", null,  array('req' => true));

    shn_form_fsclose();;

    shn_form_fsopen(_t('Contact Information'));

    shn_form_text(_t("Main Phone"),'wp','size="25"', array('help'=>'In ### - ### - #### format.'));
    shn_form_text(_t("Email Address"),'email','size="25"');

    shn_form_fsclose();

    shn_form_fsopen(_t('Location Data'));

    shn_form_text(_t("Street Address"),'street_1','size="25"', array('req' => true));
    shn_form_text(_t("Address Line 2"),'street_2','size="25"', array('help'=>'(Building, Floor, Room etc.)'));
    shn_form_text(_t("City"),'city','size="10"', array('req' => true));
    shn_form_text(_t("State / Province / Region"),'state','size="10"', array('req' => true));
    shn_form_text(_t("Postal / Zip Code"),'postal','size="10"', array('req' => true));

    $q = "SELECT `option_code`, `option_description` FROM `field_options` WHERE `field_name` = 'opt_country' order by `option_description`;";
    $res = $global['db']->Execute($q);
    $opt_status = array();
    while (!$res->EOF) {
        $opt_status[$res->fields['option_code']] = $res->fields['option_description'];
        $res->MoveNext();
    }

    shn_form_select($opt_status, _t('Country'), "opt_country", null,  array('value'=>'USA'));

    shn_form_text(_t("Latitude"),'lat','size="25"');
    shn_form_text(_t("Longitude"),'long','size="25"');

    echo "</br>";

    shn_form_button(_t("Google Geocode Address"), "class=\"styleTehButton\" id=\"geocode\"");

    echo "<div id=\"map_canvas\"></div>";

    shn_form_fsclose();

    shn_form_submit("Save", "class=\"styleTehButton\""); echo '&nbsp&nbsp';

    shn_form_fclose();

?>
