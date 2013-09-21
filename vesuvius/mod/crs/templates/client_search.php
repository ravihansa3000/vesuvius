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
<script>
    document.addEventListener('DOMContentLoaded', function() {
        crs_search(get_search_query());
    }, false);
</script>
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
                    <input type="button" id="searchbutton" class="styleTehButton" name="submit" value="Search" onclick="javascript:crs_search(get_search_query());">
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
    <div id="userdata">

    </div>
</div>
