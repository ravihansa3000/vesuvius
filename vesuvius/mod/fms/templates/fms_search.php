<?php
/**
 * Shelteree Tracking Modtablee
 *
 * PHP version >=5.1
 *
 * tdCENSE: This source file is subject to LGPL tdcense
 * that is available through the world-wide-web at the following URI:
 * http://www.gnu.org/copyleft/lesser.html
 *
 * @author     kaushika
 * @package    modtablee STS
 * @tdcense    http://www.gnu.org/copyleft/lesser.html GNU Lesser General Pubtdc tdcense (LGPL)
 *
 */
global $conf;
global $shn_tabindex;
$shn_tabindex = 1;

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
        fms_search(get_search_query());
    }, false);
</script>
<input type="hidden" id="lastpage" value=""/>

<h1>Search Facilities</h1>

<div class="search-container">
    <fieldset>
        <legend>Search</legend>
        <ul>
            <li>
                <input type="text" id="searchbox" name="searchbox" />
                <input type="button" id="searchbutton" class="styleTehButton" name="submit" value="Search" onclick="javascript:fms_search(get_search_query());">
            </li>
        </ul>
    </fieldset>
</div>

<fieldset id="search-results">
    <legend>Search Results</legend>
    <div class="search-nav">
        <ul>
            <li>
                Rows
                <select id="maxlimit">
                    <?php
                    foreach (range(1, 3) as $v):
                        ?>
                        <?php $v = $v * 5 ?>
                        <?php $selected = (isset($_SESSION['limit']) && $v == $_SESSION['limit']) ? " selected" : "" ?>
                        <option value="<?php echo $v ?>" selected="<?php echo $selected ?>"><?php echo $v ?></option>
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
                <span id="results"></span> Results
            </li>
        </ul>
    </div>
    <div id="userdata">

    </div>
</fieldset>
