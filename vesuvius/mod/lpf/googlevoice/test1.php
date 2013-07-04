<?
/**
 * @name         PL User Services
 * @version      24
 * @package      plus
 * @author       Greg Miernicki <g@miernicki.com> <gregory.miernicki@nih.gov>
 * @about        Developed in whole or part by the U.S. National Library of Medicine
 * @link         https://pl.nlm.nih.gov/about
 * @license	 http://www.gnu.org/lcenses/lgpl-2.1.html GNU Lesser General Public License (LGPL)
 * @lastModified 2012.0221
 */

$user = "g";
$pass = "PeLo2012";
/*
$user = "imagestats";
$pass = "1mageStats";
*/

//$uuid = "pl.nlm.nih.gov/person.2962640";
$uuid = "ceb-stage-lx.nlm.nih.gov/~miernickig/vesuvius/vesuvius/www/person.4002511";
require_once("../vesuvius/vesuvius/3rd/nusoap/lib/nusoap.php");

//$wsdl = "https://pl.nlm.nih.gov/?wsdl&api=26";
//$wsdl = "https://plstage.nlm.nih.gov/?wsdl&api=26";
$wsdl = "http://ceb-stage-lx.nlm.nih.gov/~miernickig/vesuvius/vesuvius/www/?wsdl&api=26";
//$wsdl = "https://lhce-pl-web01.nlm.nih.gov/?wsdl&api=25";
//$wsdl = "https://lhce-pl-web02.nlm.nih.gov/?wsdl&api=25";
//$wsdl = "https://pl.wip.nlm.nih.gov/?wsdl&api=25";
$client = new nusoap_client($wsdl);

//$x = file_get_contents("1.xml");
$x = file_get_contents("../vesuvius/vesuvius/mod/plus/reference_REUNITE4.xml");
//$result = $client->call('reportPerson', array('personXML'=>$x, 'eventShortName'=>'test', 'xmlFormat'=>'REUNITE4', 'username'=>$user, 'password'=>$pass));
$result = $client->call('reReportPerson', array('uuid'=>'ceb-stage-lx.nlm.nih.gov/~miernickig/vesuvius/vesuvius/www/person.4002525', 'personXML'=>$x, 'eventShortName'=>'test', 'xmlFormat'=>'REUNITE4', 'username'=>$user, 'password'=>$pass));

$x = file_get_contents("../vesuvius/vesuvius/mod/plus/reference_TRIAGEPIC1.xml");
//$x = file_get_contents("3.xml");
//$result = $client->call('reportPerson', array('personXML'=>$x, 'eventShortName'=>'test', 'xmlFormat'=>'TRIAGEPIC1', 'username'=>$user, 'password'=>$pass));
//$result = $client->call('reReportPerson', array('uuid'=>'pl.nlm.nih.gov/person.2965467', 'personXML'=>$x, 'eventShortName'=>'test', 'xmlFormat'=>'TRIAGEPIC1', 'username'=>$user, 'password'=>$pass));
//$result = $client->call('findMostRecentReportAsFiltered', array('machine'=>'', 'login'=>'', 'username'=>'', 'hospitalShortName'=>'', 'eventShortName'=>'', 'closedEventsIncl'=>'', 'hospitalEventsIncl'=>'', 'publicEventsIncl'=>'', 'practiceMode'=>'', 'expiredIncl'=>''));
//$result = $client->call('version', array(null));
//$result = $client->call('getPersonPermissions', array('uuid'=>$uuid, 'username'=>$user, 'password'=>$pass));
//$result = $client->call('addComment', array('uuid'=>$uuid, 'comment'=>'comment!!!', 'suggested_status'=>'dec', 'suggested_location'=>'', 'suggested_image'=>'', 'username'=>$user, 'password'=>$pass));
//$result = $client->call('getEventListUser', array('username'=>$user, 'password'=>$pass));
//$result = $client->call('getEventList', array(null));
//$result = $client->call('ping', array(null));
//$result = $client->call('pingWithEcho', array('machineName'=>'test', 'latency'=>9999));
//$result = $client->call('getNullTokenList', array('tokenStart'=>'0', 'tokenEnd'=>'120', 'username'=>$user, 'password'=>$pass));
//$result = $client->call('getImageListBlock', array('tokenStart'=>'28', 'stride'=>2, 'username'=>$user, 'password'=>$pass));
//$result = $client->call('getImageList', array('tokenStart'=>'28', 'tokenEnd'=>'28', 'username'=>$user, 'password'=>$pass));
//$result = $client->call('getImageCountsAndTokens', array('username'=>$user, 'password'=>$pass));
//$result = $client->call('expirePerson', array('uuid'=>'pl.nlm.nih.gov/person.2962639', 'explanation'=>'because!!!!', 'username'=>$user, 'password'=>$pass));
//$result = $client->call('setPersonExpiryDate', array('uuid'=>'pl.nlm.nih.gov/person.2962640', 'expiryDate'=>'2014-01-01 01:23:46', 'username'=>$user, 'password'=>$pass));
//$result = $client->call('setPersonExpiryDateOneYear', array('uuid'=>'pl.nlm.nih.gov/person.2962640', 'username'=>$user, 'password'=>$pass));
//$result = $client->call('changeMassCasualtyId', array('newMcid'=>'XXX', 'uuid'=>'3', 'username'=>$user, 'password'=>$pass));
//$result = $client->call('getUuidByMassCasualtyId', array('mcid'=>'91140', 'shortname'=>'test', 'username'=>$user, 'password'=>$pass));
//$result = $client->call('getPersonExpiryDate', array('uuid'=>'pl.nlm.nih.gov/person.2962640'));
//$result = $client->call('hasRecordBeenRevised', array('uuid'=>'pl.nlm.nih.gov/person.2962640', 'username'=>$user, 'password'=>$pass));
//$result = $client->call('checkUserAuth', array('username'=>$user, 'password'=>$pass));
//$result = $client->call('checkUserAuthHospital', array('username'=>$user, 'password'=>$pass));
//$result = $client->call('getHospitalPolicy', array('hospital_uuid'=>1));
//$result = $client->call('getHospitalLegaleseTimestamps', array('hospital_uuid'=>1));
//$result = $client->call('registerUser', array('username'=>'zhirongli1', 'emailAddress'=>'zhirongli@hotmail.com', 'password'=>'Password9999', 'givenName'=>'Zhirong', 'familyName'=>'Li'));
//$result = $client->call('changeUserPassword', array('username'=>$user, 'oldPassword'=>$pass, 'newPassword'=>$pass));
//$result = $client->call('resetUserPassword', array('email'=>"pl@tehk.org"));
//$result = $client->call('forgotUsername', array('email'=>"testCase@email.com"));
//$result = $client->call('createPersonUuid', array('username'=>'testDontDelete', 'password'=>'dontDelete99'));
//$result = $client->call('createPersonUuidBatch', array('number'=>5, 'username'=>'testDontDelete', 'password'=>'dontDelete99'));

/*
$result = $client->call('searchComplete', array(
	'eventShortname'=>'test',
	'searchTerm'=>'p_uuid:"plstage.nlm.nih.gov/person.2960997"',
	'filterStatusMissing'=>true,
	'filterStatusAlive'=>true,
	'filterStatusInjured'=>true,
	'filterStatusDeceased'=>true,
	'filterStatusUnknown'=>true,
	'filterStatusFound'=>true,
	'filterGenderComplex'=>true,
	'filterGenderMale'=>true,
	'filterGenderFemale'=>true,
	'filterGenderUnknown'=>true,
	'filterAgeChild'=>true,
	'filterAgeAdult'=>true,
	'filterAgeUnknown'=>true,
	'filterHospitalSH'=>true,
	'filterHospitalWRNMMC'=>true,
	'filterHospitalOther'=>true,
	'pageStart'=>0,
	'perPage'=> 20,
	'sortBy'=>''
));

echo "<pre>";
echo $result['resultSet'];
*/

echo "
	<h2>wsdl: ".$wsdl."</h2>
	".var_export($result, true)."
	<h2>Request</h2>
	<pre>".htmlspecialchars($client->request, ENT_QUOTES)."</pre>
	<h2>Response</h2>
	<pre>".htmlspecialchars($client->response, ENT_QUOTES)."</pre>
	<h2>Debug</h2>
	<pre>".htmlspecialchars($client->debug_str, ENT_QUOTES)."</pre>
";



