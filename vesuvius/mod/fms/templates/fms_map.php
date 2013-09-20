<?php
/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

global $conf;
?>
<script type="text/javascript" src="https://maps.google.com/maps/api/js?sensor=false"></script>

<script type="text/javascript">
    var MYMAP = {
        bounds: null,
        map: null
    }
    MYMAP.init = function(selector, latLng, zoom) {
        var myOptions = {
            zoom:zoom,
            center: latLng,
            mapTypeId: google.maps.MapTypeId.ROADMAP
        }
        this.map = new google.maps.Map($(selector)[0], myOptions);
        this.bounds = new google.maps.LatLngBounds();
    }
    
    MYMAP.placeMarkers = function(url) {
        $.getJSON(url, function(data){
            $.each(data, (function(key, value){
			
                // create a new LatLng point for the marker
                var lat = value.lat;
                var lon = value.lon;
                var point = new google.maps.LatLng(parseFloat(lat),parseFloat(lon));
			
                // extend the bounds to include the new point
                MYMAP.bounds.extend(point);
			
                var marker = new google.maps.Marker({
                    position: point,
                    title: value.name,
                    icon: 'theme/<?php echo $conf['theme']; ?>/img/map-push-pin-green.png',
                    map: MYMAP.map
                });     
			
                var infoWindow = new google.maps.InfoWindow();
                var html = value.info;
                
                
                google.maps.event.addListener(marker, 'click', function() {
                    infoWindow.setContent(html);
                    infoWindow.open(MYMAP.map, marker);
                });
                MYMAP.map.fitBounds(MYMAP.bounds);
            }));
        });
    }

    $(document).ready(function() {
        
        $("#map_canvas").css({
            height: 500,
            width: 800
        });
        
        var myLatLng = new google.maps.LatLng(<?php echo $avg_lat ?>, <?php echo $avg_lon ?>);
        MYMAP.init('#map_canvas', myLatLng, 11);
        MYMAP.placeMarkers('index.php?stream=json&mod=fms&act=facility_geo');
        
    });
</script>

<h1>Facility Location Map</h1>

<div id="map_canvas"></div>