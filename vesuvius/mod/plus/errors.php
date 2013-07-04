<?
/**
 * @name         PL User Services
 * @version      28
 * @package      plus
 * @author       Greg Miernicki <g@miernicki.com> <gregory.miernicki@nih.gov>
 * @about        Developed in whole or part by the U.S. National Library of Medicine
 * @link         https://pl.nlm.nih.gov/about
 * @license	 http://www.gnu.org/licenses/lgpl-2.1.html GNU Lesser General Public License (LGPL)
 * @lastModified 2013.0211
 */



// define error codes
define("ERRORCODES", serialize(array(
	0    => "",
	1    => "Invalid username, password, or api key.",
	2    => "User account is not active. Please check your email for a link to activate your account.",
	3    => "User account is banned. Please contact support for help with this issue.",
	4    => "User account locked due to many failed authentication attempts.",
	5    => "Account activated.",
	6    => "User exists with provided email address.",
	7    => "Username already in use.",
	8    => "Invalid email address.",
	9    => "Password does not meet the following criteria: 1. The minimum length of the password is 8 characters. 2. The maximum length of the password is 16 characters. 3. Must have at least one uppercase character. 4. Must have at least one lowercase character. 5. Must have at least one numeral (0-9). 6. The password cannot contain your username.",
	10   => "User does not exist or user's account is not active.",
	11   => "Email address is not associated with any user account.",
	12   => "Username does not exist.",
	13   => "Invalid confirmation request.",
	20   => "User is not a part of Hospital Staff or Hospital Staff Admin groups.",
	100  => "No hospital registered with this id.",
	200  => "sessionTimeout value missing from database.",
	201  => "Invalid number of uuids requested, value must be between 2 and 100.",
	300  => "Insufficient privileges to access data in this event.",
	301  => "Authentication required to access this non-public event.",
	302  => "Event does not exist with this shortname.",
	400  => "Invalid enumeration.",
	402  => "Invalid p_uuid ~ out of range.",
	403  => "Error parsing XML.",
	405  => "Event is closed to reporting.",
	406  => "Invalid event.",
	407  => "No record associated with this mass casualty ID exists.",
	408  => "Insufficient permission to revise this record.",
	410  => "No record available with the given uuid.",
	411  => "A provided token value is out of range.",
	412  => "Invalid or future unix timestamp.",
	413  => "Record has already expired.",
	414  => "Invalid datetime value specified.",
	415  => "This record has no EDXL component.",
	416  => "Only records reported via web services can be revised by web services.",
	417  => "Invalid stride size, must be an integer value between 1 and 1,000,000.",
	418  => "Insufficient permission to access this record.",
	419  => "Invalid image mimetype(s) ~ image(s) not added to record.",
	420  => "SHA-1 mismatch(s) or bad image data ~ image(s) rejected.",
	421  => "No image present with the given sha1.",
	422  => "One or more duplicate image(s) were detected for this person and rejected.",
	423  => "Bad image deletion request: invalid sha1 or image url.",
	424  => "One or more duplicate voiceNotes(s) were detected for this person and rejected.",
	425  => "Bad voiceNote deletion request: invalid sha1 or image url.",
	430  => "Invalid identifier provided.",
	431  => "User has already flagged this record for abuse.",
	9000 => "Database Error.",
	9998 => "Function not yet implemented (stub).",
	9999 => "Unknown error.",
)));
