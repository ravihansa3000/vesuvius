<?
/**
 * @name         PL User Services
 * @version      25
 * @package      plus
 * @author       Greg Miernicki <g@miernicki.com> <gregory.miernicki@nih.gov>
 * @about        Developed in whole or part by the U.S. National Library of Medicine
 * @link         https://pl.nlm.nih.gov/about
 * @license	 http://www.gnu.org/licenses/lgpl-2.1.html GNU Lesser General Public License (LGPL)
 * @lastModified 2012.0529
 */


// PLUS SOAP Server

global $global;
global $server;
global $conf;
require_once($global['approot'].'/mod/lpf/lib_lpf.inc');
require_once($global['approot'].'/mod/plus/lib_plus.inc');
require_once($global['approot'].'/mod/plus/errors.php');
require_once($global['approot'].'3rd/nusoap/lib/nusoap.php');

// determine if web services are on or off
if(!isset($conf['enable_plus_web_services']) || (isset($conf['enable_plus_web_services']) &&  $conf['enable_plus_web_services'] == false)) {
	echo "web services disabled";
	die();
}

// figure out which api version to load ~ not specified, load default
if(isset($_GET['api']) && file_exists($global['approot']."/mod/plus/api_".$_GET['api'].".inc")) {

	require_once($global['approot']."/mod/plus/api_".$_GET['api'].".inc");
	require_once($global['approot'].'/mod/plus/api_'.$_GET['api'].'f.inc');
	$versionString = "&amp;api=".$_GET['api'];
	$global['apiVersion'] = $_GET['api'];

} else {
	require_once($global['approot'].'/mod/plus/api_'.$conf['mod_plus_latest_api'].".inc");
	require_once($global['approot'].'/mod/plus/api_'.$conf['mod_plus_latest_api'].'f.inc');
	$versionString = "";
	$global['apiVersion'] = $conf['mod_plus_latest_api'];
}

// init vars
/* OLD server used domains prefixed---
$serviceName = "plusWebServices";
$ns          = makeBaseUrlMinusEvent()."soap/".$serviceName;
$endpoint    = makeBaseUrl()."?wsdl".$versionString;
*/
$serviceName = "plusWebServices";
$ns          = "soap/".$serviceName;
$endpoint    = "?wsdl".$versionString;

$server = new nusoap_server;
$server->configureWSDL($serviceName, $ns, $endpoint, 'document');
$server->wsdl->schemaTargetNamespace = $ns;

shn_plus_registerAll($ns);

// if in safe mode, raw post data not set:
if(!isset($HTTP_RAW_POST_DATA)) {
	$HTTP_RAW_POST_DATA = implode("\r\n", file('php://input'));
}

$server->service($HTTP_RAW_POST_DATA);




