<?php

/**
 * @package     pfif
 * @version      1.1
 * @author       Carl H. Cornwell <ccornwell@mail.nih.gov>
 * @author       Leif Neve <lneve@mail.nih.gov>
 * LastModified: 2011:0719
 * License:      LGPL
 * @link         TBD
 */
// print "Configuring error reporting  ...\n";
error_reporting(E_ALL ^ E_NOTICE); // E_NOTICE);
// print "Configuring error display  ...\n";
ini_set("display_errors", "stdout");

// cron job task for for pfif export
// set approot since we don't know it yet
$global['approot'] = getcwd() . "/../../";

require_once("../../conf/sahana.conf");
require_once("../../3rd/adodb/adodb.inc.php");
require_once("../../inc/handler_db.inc");
require_once("pfif.inc");
require_once("pfif_repository.inc");
require_once("pfif_croninit.inc");

/**
 *  Log harvest end
 */
function update_harvest_log($r, $req_params, $status) {
   $pfif_info = $_SESSION['pfif_info'];
   $pfif_info['end_time'] = time();
   //var_dump("ending harvest with pfif_info:", $pfif_info);
   $r->end_harvest($status, $req_params, $pfif_info);
}

print "database = " . $conf['db_name'] . "\n";
// Store Japanese correctly.
$global['db']->Execute("SET NAMES 'utf8'");

// Get all PFIF repository sources.
$repositories = Pfif_Repository::find_sink();
if (!$repositories) {
   die("No repositories ready for harvest.\n");
}
//var_dump("Found repositories for export", $repositories);

$sched_time = time();
$export_repos = array();
foreach ($repositories as $r) {
   if ($r->is_ready_for_harvest($sched_time)) {
      add_pfif_service($r);             // initializes pfif_conf
      $export_repos[$r->id] = $r;
      //var_dump("exporting to repository",$r);
   }
}
unset($r);
unset($repositories);
$export_queue = $pfif_conf['services'];
//print "Queued exports:\n".print_r($export_queue,true)."\n";

foreach ($export_queue as $service_name => $service) {
   $repos = $export_repos[$pfif_conf['map'][$service_name]];
   $incident_conf = $pfif_conf['service_to_incident'][$service_name];
   //var_dump("repository", $repos, "conf", $incident_conf);

   $req_params = $repos->get_request_params();
   $min_entry_date = $req_params['min_entry_date'];
   $skip = $req_params['skip'];
   $subdomain = empty($service['subdomain'])? '' : '?subdomain='.$service['subdomain'];
   $auth_key = empty($service['auth_key'])? '' : '?key='.$service['auth_key'];
   $pfif_uri = $service['post_url'].$auth_key.$subdomain;
   $p = new Pfif();
   $p->setPfifConf($pfif_conf, $service_name);
   //print_r($pfif_conf);
   //print_r($export_params);

   $repos->start_harvest('scheduled', 'out');
   print "\n\nExport started to $pfif_uri at " . $repos->get_log()->start_time . "\n";
   if (true) {
      $local_date = local_date($min_entry_date);
      $loaded = $p->loadFromDatabase('', $local_date, $skip);
      print "load for post to $service_name for records after $local_date " . ($loaded ? "suceeded with $loaded records" : "failed") . "\n";
   } else {
      $id = 'pl.nlm.nih.gov/person.10505'; // e.g. 'japan.person-finder.appspot.com/person.4440739'
      $loaded = $p->loadFromDatabase($id);
      print "load $id" . ($loaded ? " suceeded with $loaded records" : " failed") . "\n";
   }

   if ($loaded && $loaded > 0) {
      $xml = $p->storeInXML(); 
      $fh = fopen('crontest.xml', 'w');
      $charstowrite = strlen($xml);
      $written = fwrite($fh, $xml, $charstowrite);
      fclose($fh);
      print "Logged $written of $charstowrite characters to crontest.xml\n";
   
      //$post_status = $p->postDbToService('LPFp-46833',$service_name); // TESTING
      //$post_status = "TESTING: record(s) not uploaded";
      $post_status = $p->postToService('xml',$xml,$service_name);
      // TODO: Adjust count depending on post_status.
      $_SESSION['pfif_info']['pfif_person_count'] = $loaded;
      update_harvest_log($repos, $req_params, 'completed');
      print "Post status:\n $post_status \n";
   } else {
      update_harvest_log($repos, $req_params, 'error');
      print "Export failed: no records to upload\n";
   }
}