<?php
/** ******************************************************************************************************************************************************************
*********************************************************************************************************************************************************************
********************************************************************************************************************************************************************
*
* @class        pop
* @version      12
* @author       Greg Miernicki <g@miernicki.com>
*
********************************************************************************************************************************************************************
*********************************************************************************************************************************************************************
**********************************************************************************************************************************************************************/

class pop {

	private $smtp_host;
	private $smtp_port;
	private $smtp_ssl;
	private $smtp_auth;
	private $smtp_reply_address;
	private $pop_username;
	private $pop_password;

	public  $messages;
	public  $startTime;
	public  $stopTime;
	public  $sentStatus;


	/** Constructor: */
	public function __construct() {

		global $conf;

		// get configuration settings
		$this->smtp_host          = shn_db_get_config("pop","smtp_host1");
		$this->smtp_port          = shn_db_get_config("pop","smtp_port1");
		$this->smtp_ssl           = shn_db_get_config("pop","smtp_ssl1");
		$this->smtp_auth          = shn_db_get_config("pop","smtp_auth1");
		$this->smtp_reply_address = shn_db_get_config("pop","smtp_reply_address1");
		$this->pop_username       = $conf['pop_username'];
		$this->pop_password       = $conf['pop_password'];

		$this->messages          = "scriptExecutedAtTime >> ".date("Ymd:Gis.u")."\n";
		$this->startTime         = microtime(true);
		$this->stopTime          = null;
		$this->sentStatus        = FALSE;
	}



	/** Destructor */
	public function __destruct() {}


	/** Sends an Email to a recipient. */
	public function sendMessage($toEmail, $toName, $subject, $bodyHTML, $bodyAlt, $attachmentPath = null) {

		global $global;
		global $conf;

		$messageLog = "";
		$sendStatus = "";
		require_once($global['approot']."3rd/phpmailer/class.phpmailer.php");

		try {
			$mail = new PHPMailer(true);  // true=enable exceptions

			$mail->IsSMTP();
			$mail->SMTPAuth   = ($this->smtp_auth == 1) ? true  : false; // enable SMTP authentication
			$mail->Port       = $this->smtp_port;                        // set the SMTP port
			$mail->Host       = $this->smtp_host;                        // sets SMTP server
			$mail->Username   = $this->pop_username;                     // username
			$mail->Password   = $this->pop_password;                     // password
			//$mail->IsSendmail();                                       // tell the class to use Sendmail
			$mail->SMTPDebug  = false;                                   // enables SMTP debug information (for testing)
			$mail->SMTPSecure = ($this->smtp_ssl  == 1) ? "ssl" : "";    // sets the prefix to the servier

			$mail->AddReplyTo($this->smtp_reply_address, $conf['site_name']);
			$mail->From       = $this->smtp_reply_address;
			$mail->FromName   = $conf['site_name'];

			$mail->AddAddress($toEmail, $toName);
			$mail->Subject = $subject;
			$mail->AltBody = $bodyAlt;
			$mail->MsgHTML($bodyHTML);
			$mail->WordWrap = 80;
			$mail->IsHTML(true); // send as HTML

			if($attachmentPath != null ) {
				$mail->AddAttachment($attachmentPath);
			}
			$mail->Send();
			$sendStatus = "SUCCESS\n";
			$this->messages .= "Successfully sent the message.\n";
			$this->sentStatus = true;

		} catch (phpmailerException $e) {

			$sendStatus = "ERROR";
			$this->messages .= $e->errorMessage(); // pretty error messages from phpmailer
			$messageLog .= $e->errorMessage();

		} catch (Exception $e) {

			$sendStatus = "ERROR";
			$this->messages .= $e->getMessage();   // boring error messages from anything else!
			$messageLog .= $e->getMessage();
		}

		$this->messages .= $sendStatus;

		// log that we sent out an email ....
		$mod = isset($global['module']) ? $global['module'] : "cron";

		$q = "
			INSERT INTO pop_outlog (
				`mod_accessed`,
				`time_sent`,
				`send_status`,
				`error_message`,
				`email_subject`,
				`email_from`,
				`email_recipients` )
			VALUES (
				'".$mod."',
				CURRENT_TIMESTAMP,
				'".$sendStatus."',
				'".$messageLog."',
				'".$subject."',
				'".$this->smtp_reply_address."',
				'".$toEmail."' ) ;
		";
		$result = $global['db']->Execute($q);
		if($result === false) { daoErrorLog(__FILE__, __LINE__, __METHOD__, __CLASS__, __FUNCTION__, $this->db->ErrorMsg(), "pop send message ((".$q."))"); }
	}


	/** Prints the message log */
	public function spit() {

		$this->stopTime = microtime(true);
		$totalTime = $this->stopTime - $this->startTime;
		$this->messages .= "scriptExecutedIn >> ".$totalTime." seconds.\n";
		echo $this->messages."\n";
	}
}




