<?php
/**
 * Module >> Report a Person :: Main Controller
 *
 * PHP version 5+
 *
 * LICENSE: This source file is subject to LGPL license
 * that is available through the world-wide-web at the following URI:
 * http://www.gnu.org/copyleft/lesser.html
 *
 * @author     Greg Miernicki <g@miernicki.com>
 * @package    module
 * @subpackage rap
 * @version    1.0.1
 * @license    http://www.gnu.org/copyleft/lesser.html GNU Lesser General Public License (LGPL)
 */


include_once($global['approot']."/inc/lib_uuid.inc");
include_once($global['approot']."/inc/lib_location.inc");



/**
 * Check Credz
 *
 * @access public
 * @return void
 */
function shn_rap_checkCredz() {
	$good = false;
	if(isset($_SESSION['logged_in']) && $_SESSION['logged_in'] == 1) {
		if(isset($_SESSION['group_id']) && $_SESSION['group_id'] != 3) { // anonymous
			$good = true; // allowed to report
		}
	}

	// check if EM (Event Manager) is installed, if so, verify this event allows reporting


	return $good;
}



/**
 * Default function
 *
 * @access public
 * @return void
 */
function shn_rap_default() {
	if(shn_rap_checkClosed()) {
		shn_rap_showClosedMessage();
		return;
	}
	?>
	<h2>Before you begin...</h2>
	<ul>
		<li>Make sure the person you are reporting is not already in the system.</li>
		<li>Perform a <a href="index.php?mod=inw&act=default">search</a> to confirm you are not reporting someone that has already been reported.</li>
	</ul>
	<br>
	<br>
	<?php
	if(shn_rap_checkCredz()) {
		echo '<a href="index.php?mod=rap&act=controller"><span class="styleTehButton">Begin the process of Reporting a new Person</span></a>';
	} else {

		echo '<b>You must first login to add a person to the registry. If you do not yet have an account, <a href="index.php?act=signup">please create one first.</a></b>';
	}
}



/**
 * Shows a messgage informing the user that the current event is closed.
 *
 * @access public
 * @return void
 */
function shn_rap_showClosedMessage() {
	
	// hack to routie reporting to google for NZ
	if($_GET['shortname'] == "christchurch") {
		echo '
			<iframe
			src="http://christchurch-2011.person-finder.appspot.com/?small=yes&lang=en"
			width=400 height=300 frameborder=0
			style="border: dashed 2px #77c"></iframe>
		';
	} else {
		echo "
			<h2>Error #21</h2>
			This event is closed and is no longer accepting new person reports.
		";
	}
}



/**
 * Event CLOSED Check
 *
 * @access public
 * @return void
 */
function shn_rap_checkClosed() {
	global $global;
	$q = "
		SELECT *
		FROM incident
		WHERE shortname = '".mysql_real_escape_string($_GET['shortname'])."';
	";
	$res = $global['db']->Execute($q);
	$row = $res->FetchRow();
	$closed = $row['closed'];
	if($closed != null) {
		return true;	// event closed
	} else {
		return false;	// event open
	}
}





/**
 * This is the RAP Controller
 * @access public
 * @return void
 */
function shn_rap_controller() {
	global $global;

	if(shn_rap_checkClosed()) {
		shn_rap_showClosedMessage();
		return;
	}

	//Controller
	$case = isset($_REQUEST['seq']) ? $_REQUEST['seq'] : null;
	switch($case) {
		case 'entry' :
			if (shn_rap_validate()) {
				//clean the $_POST to remove the conflicts
				$_POST = null;
				shn_rap_uploadPic();
			} else {
				shn_rap_entry(true);
			}
		break;


		case 'uploadPic' :
			shn_rap_cropPic();
			break;

		case 'cropPic' :
			shn_rap_commitPicture();
			break;

		default :
			shn_rap_entry();
		break;
	}
}




/**
 * This is the Report a Person Form
 * @param mixed $errors
 * @access public
 * @return void
 */
function shn_rap_entry($errors=false) {
	global $global;
	global $conf;

	if(!shn_rap_checkCredz()) {
		echo '<b>You must first login to add a person to the registry. If you do not yet have an account, <a href="index.php?act=signup">please create one first.</a><b>';
		return;
	}

	//echo "<h1>Reporting a New Person</h1>";

	if($errors) {
		display_errors();
	}

	shn_form_fopen("controller", null, array('enctype'=>'enctype="multipart/form-data"', 'req_message' => true));
		shn_form_hidden(array('seq'=>'entry'));

		shn_form_fsopen(_t('Information Privacy'));
			?><p><?php echo _t("<span class=\"publicText\">Public</span> Denotes information that, if provided, can be exposed to the general public.<br><br>".
			"<span class=\"privateText\">Private</span> Denotes information that will not be available to the public. <br><br>Private information may be made available to or shared with other family members and friends you designate, emergency responders, medical providers, clergy/councilors, law enforcement, or authorized displaced-person agencies."); ?></p><?php
		shn_form_fsclose();

		shn_form_fsopen(_t('Person\'s Status'));
			$q="select * from field_options where field_name='opt_status'";
			$res = $global['db']->Execute($q);
			$opt_status = array();
			while (!$res->EOF) {
				$opt_status[$res->fields['option_code']] = $res->fields['option_description'];
				$res->MoveNext();
			}
			shn_form_select($opt_status, "Health / Locational Status", "opt_status", "onchange=\"checkMissing();\"", array('public'=>true, 'help'=>'Please epcify to the best of your knowledge the condition of the person you are reporting.'));
		shn_form_fsclose();

		echo "
			<script>
				function checkMissing() {
					var opt_status = document.getElementById('opt_status');
					if(opt_status.value == 'mis') {
						var m = document.getElementById('missingDetails');
						m.style['display'] = 'block';
					} else {
						var m = document.getElementById('missingDetails');
						m.style['display'] = 'none';
					}
				}
			</script>
		";

		shn_form_fsopen(_t('Basic Person Details'));
			?><p><?php echo  _t('Please enter any of the following details of the person:'); ?></p>	<?php
			shn_form_text(_t('First Name '),'given_name','size="30"',array('req'=>true, 'help'=>'This is the person\'s first name or the name by which he/she is most frequently referred to as.', 'public'=>true));
			shn_form_text(_t('Last Name'),'family_name','size="30"', array('req'=>true, 'help'=>_t('Also known as the surname.  Typically the last name or the first.'), 'public'=>true));
			shn_form_text(_t('Age'),'age','size="8"', array('help'=>_t('The exact age for the person (in years) or a best guess.'), 'public'=>true));
			shn_form_text(_t('Lower Age Range'),'age_lower','size="8"', array('help'=>_t('The lower limit for a person\'s age if an exact age is not known.'), 'public'=>true));
			shn_form_text(_t('Upper Age Range'),'age_upper','size="8"', array('help'=>_t('The upper limit for a person\'s age if an exact age is not known.'), 'public'=>true));
			shn_form_opt_select("opt_gender",_t('Gender'),null,array('sort'=>'display_order, option_description ASC', 'value'=>'unk', 'public'=>true));
			shn_form_opt_select("opt_religion",_t('Religion'),null,array('sort'=>'display_order, option_description ASC', 'value'=>'UNK', 'private'=>true));
			shn_form_opt_select("opt_race",_t('Race'),null,array('sort'=>'display_order, option_description ASC', 'value'=>'U', 'private'=>true, 'help'=>'The Race choices are those defined by the US Government, e.g., the FBI when reporting missing people.'));
		shn_form_fsclose();

		shn_form_fsopen(_t('Person\'s Contact Information'));
			shn_form_textarea(_t('Home Address'), "address", null, array('cols'=>48, 'help' => _t('Please specify the address of the person you are reporting.'), 'private'=>true));
			shn_form_text(_t('Home Zip Code'), 'zip', 'size="15"', array('private'=>true));
			shn_form_text(_t('Home Phone Number'), 'phone', 'size="15"', array('private'=>true));
			shn_form_text(_t('Mobile Phone Number'), 'mobile', 'size="15"', array('private'=>true));
			shn_form_text(_t('Email Address'), 'email', 'size="25"', array('private'=>true));
		shn_form_fsclose();

		shn_form_fsopen(_t('Person\'s Physical Details'));
			shn_form_opt_select("opt_eye_color",_t('Eye Color'),null,array('sort'=>'display_order, option_description ASC', 'value'=>'UNK', 'public'=>true, 'help'=>'The choices for Skin Color are those used by governmental agencies and law enforcement when reporting missing people.  These are frequently-used descriptive words, rather than precise colors.  Adjectives like “light” and “dark” may be considered in conjunction with the choices for “Race” and “Ethnicity” fields above.'));
			shn_form_opt_select("opt_skin_color",_t('Skin Color'),null,array('sort'=>'display_order, option_description ASC', 'value'=>'UNK', 'public'=>true));
			shn_form_opt_select("opt_hair_color",_t('Hair Color'),null,array('sort'=>'display_order, option_description ASC', 'value'=>'UNK', 'public'=>true));

			shn_form_text(_t("Height"),'height','size="10"',array('public'=>true, 'help'=>'Enter value to the nearest ½ inch.  Examples: 5\' 3 1/2", 60.5", 59"'));
			shn_form_text(_t('Weight'),'weight','size="10"',array('public'=>true, 'help'=>'Enter value to the nearest pound.'));

			shn_form_textarea(_t('Other Obvious Distinctive Features'),'physical', null, array('cols'=>48, 'public'=>true));
		shn_form_fsclose();

		shn_form_fsopen(_t('Missing Person Details'), "missingDetails");
			shn_form_textarea(_t('Last Seen Location'),'last_seen', null, array('cols'=>48, 'public'=>true));
			shn_form_textarea(_t('Clothing and Appearance when Last Seen'),'last_clothing', null, array('cols'=>48, 'public'=>true));
			shn_form_textarea(_t('Other Information to Help Locate this Person'),'comments', null, array('cols'=>48, 'public'=>true));
		shn_form_fsclose();

		shn_form_submit("Next", "class=\"styleTehButton\"");
	shn_form_fclose();
	echo "<br><br>";
}



/**
 * This is the Add Entry Validation function
 * @access public
 * @return void
 */
function shn_rap_validate() {
	global $global;
	global $conf;
	require_once($global['approot']."/inc/lib_validate.inc");

	$error_flag = false;

	//clean the post -- trim them all
	foreach($_POST as $k => $v) {
		$v = trim($v);
		if($v != '') {
			$local_post[$k] = $v;
		}
	}

	$_SESSION['rap']['entry'] = $local_post;
	if(!isset($_SESSION['rap']['entry']['p_uuid'])) {
		$_SESSION['rap']['entry']['p_uuid'] = shn_create_uuid();
	}

	// anything entered?
	if(empty($local_post)) {
		add_error(_t("Please fill out the form with as much information as possible about the person you are reporting."));
		return false;
	}

	// fullname entered?
	if(!isset($local_post['given_name'])) {
		add_error(_t("Please enter the Given Name of the person you are reporting."));
		$error_flag = true;
	}

	if($error_flag) {
		return false;
	} else {
		// set session
		return true;
	}
}




/**
 * This is the image upload form
 * @param mixed $errors
 * @access public
 * @return void
 */
function shn_rap_uploadPic($errors=false) {
	echo "<h1>Add a Photograph of the Person</h1>";

	if(!shn_rap_checkCredz()) {
		echo '<b>You must first login to add a person to the registry. If you do not yet have an account, <a href="index.php?act=signup">please create one first.</a><b>';
		return;
	}

	if($errors) {
		display_errors();
	}

	?><br>If you have an image of the person you are reporting, click browse to add it to the registry. Otherwise, click next to continue.<br><br><br><?php

	shn_form_fopen("controller", null, array('enctype'=>'enctype="multipart/form-data"'));
		shn_form_hidden(array('seq'=>'uploadPic'));
		shn_form_fsopen("Upload Picture");
			shn_form_upload("Upload Picture", "picture");
		shn_form_fsclose();
		shn_form_submit("Next", "class=\"styleTehButton\"");
	shn_form_fclose();
}




function shn_rap_cropPic() {
	if(!shn_rap_checkCredz()) {
		echo '<b>You must first login to add a person to the registry. If you do not yet have an account, <a href="index.php?act=signup">please create one first.</a><b>';
		return;
	}

	// No file was uploaded ignore
	if($_FILES['picture']['error'] != 0) {
		shn_form_hidden(array('seq'=>'cropPic'));
		shn_rap_commitPerson();

	} else {


		// Get the file information
		$userfile_name = $_FILES["picture"]["name"];
		$userfile_tmp  = $_FILES["picture"]["tmp_name"];
		$userfile_size = $_FILES["picture"]["size"];


		$error         = "";
		/*
		if (($file_ext!="jpg") && ($userfile_size > $max_file)) {
			$error= "ONLY jpeg images under 1MB are accepted for upload";
		}
		} else {
			$error= "Select a jpeg image for upload";
		}
		*/

		// Everything is ok!
		if(strlen($error) == 0) {

			$info = getimagesize($_FILES['picture']['tmp_name']);
			//check the image type
			if (!$info) {
				add_error(_t("Invalid image type please try again"));
				$error = true;
			}

			$width  = $info[0];
			$height = $info[1];
			$mime   = $info['mime'];

			if($width > 500) {
				$ratio = 500 / $width;
				$useWidth = 500;
				$useHeight = $height * $ratio;
			} else {
				$ratio = 1.0;
				$useWidth = $width;
				$useHeight = $height;
			}

			$extR = explode("/", $info['mime']);
			$ext = $extR[1];
			$cwd = getcwd();

			$date = date("Ymd_His_");

			$filename             = basename($_FILES["picture"]["name"]);
			$filename             = str_replace(" ", "+", $filename);
			$file_ext             = substr($filename, strrpos($filename, ".") + 1);
			$large_image_location = $cwd."/tmp/rap_cache/".$date.$filename;
			$thumb_image_location = $cwd."/tmp/rap_cache/".$date."thumb_".$filename;
			$imgPath              =             "tmp/rap_cache/".$date.$filename;
			$thumbPath            =             "tmp/rap_cache/".$date."thumb_".$filename;

/*
			$uploadfile = $upload_dir .'orgnl_'.$_SESSION['rap']['entry']['p_uuid'].$ext;
			$desc_path  = $upload_dir .'thumb_'.$_SESSION['rap']['entry']['p_uuid'].$ext;
			move_uploaded_file($_FILES['picture']['tmp_name'], $uploadfile);
*/

			move_uploaded_file($userfile_tmp, $large_image_location);
			chmod ($large_image_location, 0777);


			// make thumb 100X100 or some thing like that ;-)
			//shn_image_resize($uploadfile, $desc_path, 100, 100);

			require_once("../inc/lib_image.inc");
			shn_image_resize_height($imgPath, $thumbPath, 320);
			chmod ($thumbPath, 0777);





		?>

		<script type="text/javascript" src="res/js/jquery-1.4.4.min.js"></script>
		<script type="text/javascript" src="res/js/jquery.imgareaselect.pack.js"></script>


		<h1>Crop the Image</h1>

		<ul>
			<li>Click and drag to draw a box around the important part of the image.</li>
			<li>You may then resize the rectangular selection grid to refine your rectangular selection area.</li>
			<li>You may also drag to move the selection box around inside the image.</li>
			<li>A good selection is to highlight the area around the person's face so that it becomes more prominant in the thumbnail image.</li>
			<li>We will use this cropped portion of the image to generate a thumbnail image.</li>
		</ul>

		<?php
			shn_form_fopen("controller", null, array('enctype'=>'enctype="multipart/form-data"'));
				shn_form_hidden(array('seq'=>'cropPic'));
				shn_form_hidden(array('ratio'=>$ratio));
				shn_form_hidden(array('width'=>$width));
				shn_form_hidden(array('height'=>$height));
				shn_form_hidden(array('mime'=>$mime));
				shn_form_hidden(array('wwwPath'=>$imgPath));
				shn_form_hidden(array('thumbPath'=>$thumbPath));
				shn_form_hidden(array('realPath'=>$large_image_location));
				shn_form_fsopen(_t(''));
		?>
		<div style="display: block;">

			<p style="font-size: 110%; font-weight: bold; padding-left: 0.1em; margin: 0px 0px 0px 20px;">Original Image</p>
			<div style="float: left; padding: 10px; background-color: #fff; border: 1px solid #000; display:block; margin: 0px 20px 20px 20px;">
				<img src="<?php echo $imgPath; ?>" id="photo" width="<?php echo $useWidth; ?>">
			</div>

			<div style="float: left; width: 400px;">
				<p style="font-size: 110%; font-weight: bold; padding-left: 0.1em;">Thumbnail Preview</p>
				<div class="frame" style="margin: 0 1em; width: 200px; height: 150px;">
					<div id="preview" style="width: 200px; height: 150px; overflow: hidden; padding: 0px; background-color: #fff; border: 1px solid #000; display: block;">
						<img src="<?php echo $imgPath; ?>" style="width: 200px; height: 150px;">
					</div>
				</div>
				<table style="margin-top: 40px;">
					<thead>
						<tr>
							<th colspan="2" style="font-size: 110%; font-weight: bold; text-align: left; padding-left: 0.1em;">Coordinates</th>
							<th colspan="2" style="font-size: 110%; font-weight: bold; text-align: left; padding-left: 0.1em;">Dimensions</th>
						</tr>
					</thead>
					<tbody>
						<tr>
							<td style="width: 10%;">
								<b>X<sub>1</sub>:</b>
							</td>
							<td style="width: 30%;">
								<input type="text" name="x1" id="x1" value="10" readonly>
							</td>
							<td style="width: 20%;">
								<b>Width:</b>
							</td>
							<td>
								<input type="text" name="w"  id="w"  value="80" readonly>
							</td>
						</tr>
						<tr>
							<td>
								<b>Y<sub>1</sub>:</b>
							</td>
							<td>
								<input type="text" name="y1" id="y1" value="10" readonly>
							</td>
							<td>
								<b>Height:</b>
							</td>
							<td>
								<input type="text" name="h"  id="h"  value="60" readonly>
							</td>
						</tr>
						<tr>
							<td>
								<b>X<sub>2</sub>:</b>
							</td>
							<td>
								<input type="text" name="x2" id="x2" value="90" readonly>
							</td>
							<td>&nbsp;</td>
							<td>&nbsp;</td>
						</tr>
						<tr>
							<td><b>Y<sub>2</sub>:</b></td>
							<td>
								<input type="text" name="y2" id="y2" value="70" readonly>
							</td >
							<td></td>
							<td></td>
						</tr>
					</tbody>
				</table>
			</div>


			<script type="text/javascript">
				function preview(img, selection) {
					if (!selection.width || !selection.height)
						return;
					var scaleX = 200 / selection.width;
					var scaleY = 150 / selection.height;
					$('#preview img').css( {
						width: Math.round(scaleX * <?php echo $useWidth; ?>),
						height: Math.round(scaleY * <?php echo $useHeight; ?>),
						marginLeft: -Math.round(scaleX * selection.x1),
						marginTop: -Math.round(scaleY * selection.y1)
					});
					$('#x1').val(selection.x1);
					$('#y1').val(selection.y1);
					$('#x2').val(selection.x2);
					$('#y2').val(selection.y2);
					$('#w').val(selection.width);
					$('#h').val(selection.height);
				}

				$(function () {
					$('#photo').imgAreaSelect( {
						x1: 10, y1: 10, x2: 90, y2: 70,
						handles: true,
						fadeSpeed: 200,
						persistent: true,
						onInit: preview,
						onSelectChange: preview
					});
				});
				// aspectRatio: '4:3', ------REMOVED
			</script>
		</div>
		<div style="clear: both; height: 50px;"></div>

		<?php
				shn_form_fsclose();
				shn_form_submit("Finish and Save this Person", "class=\"styleTehButton\"");
			shn_form_fclose();
		}



	}
}



function shn_rap_commitPicture() {
	shn_rap_commitPerson(true);
}




/**
 * This function commits the given Missing person's data to the databsae.
 * @access public
 * @return void
 */
function shn_rap_commitPerson($saveImage = false) {
	global $global;
	global $conf;

	if(!shn_rap_checkCredz()) {
		echo '<b>You must first login to add a person to the registry. If you do not yet have an account, <a href="index.php?act=signup">please create one first.</a><b>';
		return;
	}

	include_once($global['approot'].'inc/lib_uuid.inc');

	isset($_SESSION['rap']['entry']['opt_status'])     ? $status      = "'".mysql_real_escape_string($_SESSION['rap']['entry']['opt_status'])."'"     : $status      = "'unk'";
	isset($_SESSION['rap']['entry']['given_name'])     ? $given_name  =                              $_SESSION['rap']['entry']['given_name']          : $given_name  = "'unknown'";
	isset($_SESSION['rap']['entry']['family_name'])    ? $family_name =                              $_SESSION['rap']['entry']['family_name']         : $family_name = "'unknown'";
	isset($_SESSION['rap']['entry']['opt_gender'])     ? $gender      = "'".mysql_real_escape_string($_SESSION['rap']['entry']['opt_gender'])."'"     : $gender      = "'unk'";
	isset($_SESSION['rap']['entry']['age'])            ? $age         = "'".mysql_real_escape_string($_SESSION['rap']['entry']['age'])."'"            : $age         = "'0'";
	isset($_SESSION['rap']['entry']['age_lower'])      ? $minAge      = "'".mysql_real_escape_string($_SESSION['rap']['entry']['age_lower'])."'"      : $minAge      = "'0'";
	isset($_SESSION['rap']['entry']['age_upper'])      ? $maxAge      = "'".mysql_real_escape_string($_SESSION['rap']['entry']['age_upper'])."'"      : $maxAge      = "'150'";
	isset($_SESSION['rap']['entry']['opt_religion'])   ? $religion    = "'".mysql_real_escape_string($_SESSION['rap']['entry']['opt_religion'])."'"   : $religion    = "'UNK'";
	isset($_SESSION['rap']['entry']['opt_race'])       ? $race        = "'".mysql_real_escape_string($_SESSION['rap']['entry']['opt_race'])."'"       : $race        = "'U'";

	isset($_SESSION['rap']['entry']['address'])        ? $address     = "'".mysql_real_escape_string($_SESSION['rap']['entry']['address'])."'"        : $address     = "'unknown'";
	isset($_SESSION['rap']['entry']['zip'])            ? $zip         = "'".mysql_real_escape_string($_SESSION['rap']['entry']['zip'])."'"            : $zip         = "'unknown'";
	isset($_SESSION['rap']['entry']['phone'])          ? $phone       = "'".mysql_real_escape_string($_SESSION['rap']['entry']['phone'])."'"          : $phone       = "'unknown'";
	isset($_SESSION['rap']['entry']['mobile'])         ? $mobile      = "'".mysql_real_escape_string($_SESSION['rap']['entry']['mobile'])."'"         : $mobile      = "'unknown'";
	isset($_SESSION['rap']['entry']['email'])          ? $email       = "'".mysql_real_escape_string($_SESSION['rap']['entry']['email'])."'"          : $email       = "'unknown'";

	isset($_SESSION['rap']['entry']['opt_eye_color'])  ? $eye         = "'".mysql_real_escape_string($_SESSION['rap']['entry']['opt_eye_color'])."'"  : $eye         = "'UNK'";
	isset($_SESSION['rap']['entry']['opt_skin_color']) ? $skin        = "'".mysql_real_escape_string($_SESSION['rap']['entry']['opt_skin_color'])."'" : $skin        = "'UNK'";
	isset($_SESSION['rap']['entry']['opt_hair_color']) ? $hair        = "'".mysql_real_escape_string($_SESSION['rap']['entry']['opt_hair_color'])."'" : $hair        = "'UNK'";
	isset($_SESSION['rap']['entry']['height'])         ? $height      = "'".mysql_real_escape_string($_SESSION['rap']['entry']['height'])."'"         : $height      = "'unknown'";
	isset($_SESSION['rap']['entry']['weight'])         ? $weight      = "'".mysql_real_escape_string($_SESSION['rap']['entry']['weight'])."'"         : $weight      = "'unknown'";
	isset($_SESSION['rap']['entry']['physical'])       ? $physical    = "'".mysql_real_escape_string($_SESSION['rap']['entry']['physical'])."'"       : $physical    = "'unknown'";
	isset($_SESSION['rap']['entry']['last_seen'])      ? $seen        = "'".mysql_real_escape_string($_SESSION['rap']['entry']['last_seen'])."'"      : $seen        = "'unknown'";
	isset($_SESSION['rap']['entry']['last_clothing'])  ? $clothing    = "'".mysql_real_escape_string($_SESSION['rap']['entry']['last_clothing'])."'"  : $clothing    = "'unknown'";
	isset($_SESSION['rap']['entry']['comments'])       ? $comments    = "'".mysql_real_escape_string($_SESSION['rap']['entry']['comments'])."'"       : $comments    = "'unknown'";

	// NULL values that come in as "select"
	if($race     == "'select'") { $race     = "NULL"; }
	if($religion == "'select'") { $religion = "NULL"; }
	if($gender   == "'select'") { $gender   = "NULL"; }
	if($hair     == "'select'") { $hair     = "NULL"; }
	if($eye      == "'select'") { $eye      = "NULL"; }
	if($skin     == "'select'") { $skin     = "NULL"; }

	$q0  = "SELECT * FROM incident WHERE shortname = '".mysql_real_escape_string($_GET['shortname'])."';";
	$r0  = $global['db']->Execute($q0);
	$row = $r0->FetchRow();
	$id = $row['incident_id'];

	$uuid = $_SESSION['rap']['entry']['p_uuid'];

	// insert person
	$q1 = "	INSERT INTO person_uuid (p_uuid, full_name, family_name, given_name)
		VALUES (
			'".$uuid."',
			'".mysql_real_escape_string($given_name)." ".mysql_real_escape_string($family_name)."',
			'".mysql_real_escape_string($family_name)."',
			'".mysql_real_escape_string($given_name)."'
		);
	";
	$res = $global['db']->Execute($q1);


	// insert person's missing information if missing
	if($status == "'mis'") {
		$q2 = "INSERT INTO person_missing (p_uuid, last_seen, last_clothing, comments) VALUES ('".$uuid."', ".$seen.", ".$clothing.", ".$comments.");";
		$res = $global['db']->Execute($q2);
	}

	// add person to the null hospital
	$q3 = "
		INSERT INTO person_to_hospital (`p_uuid`, `hospital_uuid`)
		VALUE ('".$uuid."', '-1');
	";
	$r3 = $global['db']->Execute($q3);

	// insert an assignment of this person to the correct incident
	$q4 = "INSERT INTO resource_to_incident (x_uuid, incident_id) VALUES ('".$uuid."','".$id."');";
	$res = $global['db']->Execute($q4);

	// insert person's status
	$q5 = "INSERT INTO person_status (p_uuid, opt_status, updated, isvictim, updated_server) VALUES ( '".$uuid."', ".$status.", CURRENT_TIMESTAMP, TRUE, CURRENT_TIMESTAMP);";
	$res = $global['db']->Execute($q5);

	// insert into rap_log
	$q7 = "INSERT INTO rap_log (p_uuid) VALUES ('".$uuid."');";
	$res = $global['db']->Execute($q7);

	// insert person's details
	$q8 = "INSERT INTO person_details (p_uuid, opt_race, opt_religion, opt_gender, years_old, minAge, maxAge) VALUES ('".$uuid."', ".$race.", ".$religion.", ".$gender.", ".$age.", ".$minAge.", ".$maxAge.");";
	$res = $global['db']->Execute($q8);

	// insert person
	$q11 = "INSERT INTO person_to_report (p_uuid, rep_uuid) VALUES ('".$uuid."','".$_SESSION['user_p_uuid']."');";
	$res = $global['db']->Execute($q11);

	// insert personal physical
	$q11 = "
		INSERT INTO person_physical (p_uuid, height, weight, opt_eye_color, opt_skin_color, opt_hair_color, comments)
		VALUES ('".$uuid."', ".$height.", ".$weight.", ".$eye.", ".$skin.", ".$hair.", ".$physical.");";
	$res = $global['db']->Execute($q11);


	// insert phone
	$q13 = "
		INSERT INTO contact (pgoc_uuid, opt_contact_type, contact_value)
		VALUES ('".$uuid."', 'curr', ".$phone.");";
	$res = $global['db']->Execute($q13);


	// insert phone mobile
	$q14 = "
		INSERT INTO contact (pgoc_uuid, opt_contact_type, contact_value)
		VALUES ('".$uuid."', 'cmob', ".$mobile.");";
	$res = $global['db']->Execute($q14);


	// insert address
	$q15 = "
		INSERT INTO contact (pgoc_uuid, opt_contact_type, contact_value)
		VALUES ('".$uuid."', 'home', ".$address.");";
	$res = $global['db']->Execute($q15);


	// insert zip
	$q16 = "
		INSERT INTO contact (pgoc_uuid, opt_contact_type, contact_value)
		VALUES ('".$uuid."', 'zip', ".$zip.");";
	$res = $global['db']->Execute($q16);


	// insert email
	$q17 = "
		INSERT INTO contact (pgoc_uuid, opt_contact_type, contact_value)
		VALUES ('".$uuid."', 'email', ".$phone.");";
	$res = $global['db']->Execute($q17);


	// save an image if we have one
	if($saveImage) {

		$image_type   = mysql_real_escape_string($_REQUEST['mime']);
		$image_height = mysql_real_escape_string($_REQUEST['height']);
		$image_width  = mysql_real_escape_string($_REQUEST['width']);
		$url          = mysql_real_escape_string($_REQUEST['wwwPath']);
		$url_thumb    = mysql_real_escape_string($_REQUEST['thumbPath']);
		$full_path    = mysql_real_escape_string($_REQUEST['realPath']);

		$ratio = $_REQUEST['ratio'];
		$x1    = $_REQUEST['x1'];
		$y1    = $_REQUEST['y1'];
		$w     = $_REQUEST['w'];
		$h     = $_REQUEST['h'];

		$crop_x = $x1 / $ratio;
		$crop_y = $y1 / $ratio;
		$crop_w = $w  / $ratio;
		$crop_h = $h  / $ratio;

		$q18 = "
			INSERT INTO image (x_uuid, image_type, image_height, image_width, url, url_thumb, crop_x, crop_y, crop_w, crop_h, full_path)
			VALUES ('".$uuid."', '".$image_type."', '".$image_height."', '".$image_width."', '".$url."', '".$url_thumb."', '".$crop_x."', '".$crop_y."', '".$crop_w."', '".$crop_h."', '".$full_path."');";
		$res = $global['db']->Execute($q18);
	}




/*

Array
(
    [shortname] => cmax2009
    [mod] => rap
    [act] => controller
    [seq] => cropPic
    [ratio] => 0.533049040512
    [width] => 938
    [height] => 778
    [mime] => image/png
    [wwwPath] => tmp/rap_cache/20100910_033148_rent2.png
    [thumbPath] => tmp/rap_cache/20100910_033148_thumb_rent2.png
    [realPath] => /home/gmiernicki/public_html/sahanaDev/www/tmp/rap_cache/20100910_033148_rent2.png
    [x1] => 10
    [w] => 80
    [y1] => 10
    [h] => 60
    [x2] => 90
    [y2] => 70
    [PHPSESSID] => eae03304bf43364655ab53aa6d205de0

*/

	add_confirmation("You have successfully registered <b>".$given_name." ".$family_name."</b> into the registry.");
	add_confirmation("You can follow this link: <b><a href=\"index.php?mod=eap&act=edit&uuid=".base64_encode($uuid)."\">".$uuid."</a></b> to make changes to the person's record or use the tools available there to share this information with others.");
	add_confirmation("If you come back to the site at a later time, you can find this person again by visiting the <a href=\"index.php?mod=pref&act=tracking\"><b>My People</b></a> link above.");
}


