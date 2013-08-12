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
?>
<script type="text/javascript">
    $(document).ready(function(){

        $.tgrid = {};
    
        showresults();

        $("#searchbutton").click(function(){
            showresults();
        });

        $("#searchbox").keyup(function(event){
            if(event.keyCode == "13")
            {
                showresults();
            }
        });

        $("#maxlimit").change(function(){
            showresults();
        });

        $("#pagenum").change(function(){
            showresults();
        });
    
        $("#first").click(function(){
            $("#pagenum").val(1);
            showresults();
        });

        $("#prev").click(function(){
            $("#pagenum").val(parseInt($("#pagenum").val())-1);
            showresults();
        });

        $("#next").click(function(){
            $("#pagenum").val(parseInt($("#pagenum").val())+1);
            showresults();
        });
    
        $("#last").click(function(){
            $("#pagenum").val(parseInt($("#lastpage").val()));
            showresults();
        });

        function addCommas(nStr)
        {
            nStr += '';
            x = nStr.split('.');
            x1 = x[0];
            x2 = x.length > 1 ? '.' + x[1] : '';
            var rgx = /(\d+)(\d{3})/;
            while (rgx.test(x1)) {
                x1 = x1.replace(rgx, '$1' + ',' + '$2');
            }
            return x1 + x2;
        };

        function showresults() {

            var url = "index.php";
            var page = $("#pagenum").val();

            if(page < 1) {
                page = 1;
            }

            // Clear table
            $("#userdata thead").html("");
            $("#userdata tbody").html("");

            $.getJSON(url, {
                stream:"json", 
                mod: "crs",
                act: "client_search",
                name_query: $("#searchbox").val(),
                minAge: $("#minAge").val(),
                maxAge: $("#maxAge").val(),
                facility_group: $("#facility_group").val(),
                facility: $("#facility").val(),
                limit: $("#maxlimit").val(),
                page: page,
                sidx: $.tgrid.sidx,
                sord: $.tgrid.sord
            }, 
            function(data) {

                // paginator
                if(!data.page){
                    $.tgrid.page = 1;
                } else {
                    $.tgrid.page = data.page;
                }
                $("#pagenum").val($.tgrid.page);
                $("#lastpage").val(data.total);
                $("#results").html(addCommas(data.records));
                $("#totalpages").html(data.total);

                // header
                var h = 0;
                var head = "<tr>";
                head += "<th>#</th>";

                var sortasc;
                var sortdesc;
                if(data.records > 0) {
                    $.each(data.rows[0], function(key, value){

                        if(key == data.sidx && data.sord == 'asc'){
                            sortasc = 'sortbuttonselected';
                        } else {
                            sortasc = 'sortbutton';
                        }

                        if(key == data.sidx && data.sord == 'desc'){
                            sortdesc = 'sortbuttonselected';
                        } else {
                            sortdesc = 'sortbutton';
                        }

                        head += '<th id="' + key + '">';
                        head += '<a href="#">' + data.colnames[h++] + '</a> ';
                        head += '<span title="ascending" class="'+ sortasc +'">&#x25B2;</span>';
                        head += '<span title="descending" class="'+ sortdesc +'">&#x25BC;</span>';
                        head += '</th>';
                    });
                } else {
                    $.each(data.colnames, function(key, value){
                        head += '<th id="' + key + '">' + value + '</th>';
                    });
                }
        
                head += "</tr>";
                $(head).appendTo("#userdata thead");

                // rows
                var row = '';
                if(data.records > 0) {
                    $.each(data.rows, function(i, person) {
                        row += "<tr>";
                        var rownum = i+1+(($.tgrid.page-1)*data.limit);
                        row += "<td>"+rownum+"</td>";
                        $.each(person, function(key, value){
                            row += "<td>"+value+"</td>";
                        });
                        row += "</tr>";
                    });
                } else {
                    row = '<tr><td colspan="'+(data.colnames.length+1)+'" align="center">No Results Found</td></tr>';
                }
        
                $(row).appendTo("#userdata tbody")

                $('#userdata thead th').click(function(){
                    $.tgrid.sidx = $(this).attr('id');
                    if($.tgrid.sord == 'desc'){
                        $.tgrid.sord = "asc";
                    } else {
                        $.tgrid.sord = "desc";
                    }
                    showresults();
                });

            });
        };
    });
</script>
<style type="text/css">

    fieldset {
        background-color: #FCFCFC;
        border: 1px solid #E5EAEF;
        border-radius: 5px 5px 5px 5px;
        box-shadow: 0 0 5px rgba(191, 191, 191, 0.4);
        margin: 0 0 20px;
        padding: 30px;
    }

    legend {
        color: #666666;
        font-weight: bold;
    }

    ul {
        list-style: none;
        display: block;
    }

    li{
        clear: both;
        display: inline;
        vertical-align: middle;
        margin: 0 5px;
    }

</style>
<input type="hidden" id="lastpage"  value=""/>

<h1>Search Shelter Clients</h1>

<div class="search-container">
    <fieldset id="search-fieldset">
        <legend>Name Search</legend>
        <ul>
            <li>
                <span>
                    <input class="required field text" type="text" size="27" id="searchbox" name="searchbox" value="<?php echo (isset($_SESSION['name_query'])) ? $_SESSION['name_query'] : "" ?>"/>
                </span>
            </li>
            <li>
                <span>
                    <input type="button" id="searchbutton" class="styleTehButton" name="submit" value="Search">
                </span>
            </li>
        </ul>
    </fieldset>
    <fieldset id="search-filters">
        <legend>Search Filters</legend>
        <ul>
            <li>
                <span>
                    <label>Min Age</label>
                    <input type="text" id="minAge" size="3" maxlength="3" value="<?php echo $minAge ?>"/>

                </span>
                <span>
                    <label>Max Age</label>
                    <input type="text" id="maxAge" size="3" maxlength="3" value="<?php echo $maxAge ?>"/>

                </span>
                <span>
                    <label for="facility_group">Facility Group</label>
                    <select id="facility_group">
                        <option value="all">All</option>
                        <?php if (isset($facilityGroups)) : ?>
                            <?php foreach ($facilityGroups as $facilityGroup): ?>
                                <option value="<?php echo $facilityGroup['facility_group'] ?>">
                                    <?php echo $facilityGroup['facility_group'] ?>
                                </option>
                            <?php endforeach; ?>
                        <?php endif; ?>
                    </select>

                </span> 
                <span>
                    <label for="facility">Facility Name</label>
                    <select id="facility">
                        <option value="all">All</option>
                        <?php foreach ($facilities as $facility): ?>
                            <option value="<?php echo $facility['facility_uuid'] ?>">
                                <?php echo $facility['facility_name'] ?>
                            </option>
                        <?php endforeach; ?>
                    </select>

                </span>
            </li>
        </ul>
    </fieldset>
</div>

<div class="search-results">
    <div class="search-nav">
        <ul>
            <li>
                <span> 
                    <a href="index.php?stream=text&amp;mod=crs&amp;act=excel_export">
                        Export <img src="theme/<?php echo $conf['theme']; ?>/img/spreadsheet.png" />
                    </a></span>
            </li>
            <li>
                Rows
                <select id="maxlimit">
                    <?php
                    foreach (range(1, 3) as $v):
                        ?>
                        <?php $v = $v * 10 ?>
                        <?php $selected = (isset($_SESSION['limit']) && $v == $_SESSION['limit']) ? " selected" : "" ?>
                        <option value="<?php echo $v ?>"<?php echo $selected ?>><?php echo $v ?></option>
                    <?php endforeach; ?>
                </select>
            </li>
            <li><a id="first" href="#">&lt;&lt; First</a></li>
            <li><a id="prev" href="#">&lt; Previous</a></li>
            <li>
                Page
                <input type="text" id="pagenum" name="pagenum" size="3" value="<?php echo (isset($_SESSION['page'])) ? $_SESSION['page'] : "" ?>"/>
                of <span id="totalpages"></span>
            </li>
            <li><a id="next" href="#">Next &gt;</a></li>
            <li><a id="last" href="#">Last &gt;&gt;</a></li>
            <li>
                <span>Found: <b><span id="results"></span></b> Clients</span>
            </li>
        </ul>
    </div>
    <table id="userdata">
        <thead>
            <tr>
                <th></th>
            </tr>
        </thead>
        <tbody><tr><td></td></tr></tbody>
    </table>
</div>