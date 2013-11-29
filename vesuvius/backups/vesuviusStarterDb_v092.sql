-- phpMyAdmin SQL Dump
-- version 4.0.4.1
-- http://www.phpmyadmin.net
--
-- Host: localhost
-- Generation Time: Sep 17, 2013 at 03:00 PM
-- Server version: 5.6.12
-- PHP Version: 5.5.1

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;

--
-- Database: `Sahana`
--
CREATE DATABASE IF NOT EXISTS `Sahana` DEFAULT CHARACTER SET latin1 COLLATE latin1_swedish_ci;
USE `Sahana`;

DELIMITER $$
--
-- Procedures
--
CREATE DEFINER=`root`@`localhost` PROCEDURE `delete_reported_person`(IN id VARCHAR(128),IN deleteNotes BOOLEAN)
BEGIN


DELETE p.* FROM person_uuid p, person_to_report pr WHERE pr.rep_uuid = p.p_uuid AND pr.p_uuid = id AND pr.rep_uuid NOT IN (SELECT p_uuid FROM users);


DELETE person_uuid.* FROM person_uuid WHERE p_uuid = id;


DELETE pfif_person.* FROM pfif_person WHERE p_uuid = id;

IF deleteNotes THEN

  DELETE pfif_note.* FROM pfif_note WHERE p_uuid = id;


  UPDATE pfif_note SET linked_person_record_id = NULL WHERE linked_person_record_id = id;
END IF;

END$$

CREATE DEFINER=`root`@`localhost` PROCEDURE `PLSearch`(IN searchTerms CHAR(255),
         IN statusFilter VARCHAR(100),
         IN genderFilter VARCHAR(100),
         IN ageFilter VARCHAR(100),
         IN hospitalFilter VARCHAR(100),
         IN incidentName VARCHAR(100),
         IN sortBy VARCHAR(100),
         IN pageStart INT,
         IN perPage INT)
BEGIN

        DROP TABLE IF EXISTS tmp_names;
    IF searchTerms = '' THEN
            CREATE TEMPORARY TABLE tmp_names AS (
            SELECT SQL_NO_CACHE pu.*
                FROM person_uuid pu
                   JOIN incident i  ON (pu.incident_id = i.incident_id AND i.shortname = incidentName)
                LIMIT 5000
            );
    ELSEIF searchTerms = 'unknown' THEN
            CREATE TEMPORARY TABLE  tmp_names AS (
            SELECT SQL_NO_CACHE pu.*
                FROM person_uuid pu
                   JOIN incident i  ON (pu.incident_id = i.incident_id AND i.shortname = incidentName)
                WHERE (full_name = '' OR full_name IS NULL)
                LIMIT 5000
            );
    ELSE
            CREATE TEMPORARY TABLE  tmp_names AS (
            SELECT SQL_NO_CACHE pu.*
                FROM person_uuid pu
                   JOIN incident i  ON (pu.incident_id = i.incident_id AND i.shortname = incidentName)
                WHERE full_name like CONCAT(searchTerms , '%')
                LIMIT 5000
            );
     END IF;

    SET @sqlString = CONCAT("SELECT  SQL_NO_CACHE `tn`.`p_uuid`       AS `p_uuid`,
                                `tn`.`full_name`    AS `full_name`,
                                `tn`.`given_name`   AS `given_name`,
                                `tn`.`family_name`  AS `family_name`,
                                (CASE WHEN `ps`.`opt_status` NOT IN ('ali', 'mis', 'inj', 'dec', 'fnd') THEN 'unk' ELSE `ps`.`opt_status` END) AS `opt_status`,
                                CONVERT_TZ(ps.last_updated,'America/New_York','UTC') AS updated,
                                (CASE WHEN `pd`.`opt_gender` NOT IN ('mal', 'fml') OR `pd`.`opt_gender` IS NULL THEN 'unk' ELSE `pd`.`opt_gender` END) AS `opt_gender`,
                                `pd`.`years_old` as `years_old`,
                                `pd`.`minAge` as `minAge`,
                                `pd`.`maxAge` as `maxAge`,
                                `i`.`image_height` AS `image_height`,
                                `i`.`image_width`  AS `image_width`,
                                `i`.`url_thumb`    AS `url_thumb`,
                                (CASE WHEN `h`.`short_name` NOT IN ('nnmc', 'suburban') OR `h`.`short_name` IS NULL THEN 'other' ELSE `h`.`short_name` END)  AS `hospital`,
                                (CASE WHEN (`h`.`hospital_uuid` = -(1)) THEN NULL ELSE `h`.`icon_url` END) AS `icon_url`,
                                `pd`.last_seen,
                                `pd`.other_comments as comments,
                                 ecl.person_id as mass_casualty_id
                         FROM tmp_names tn
             JOIN person_status ps  ON (tn.p_uuid = ps.p_uuid and (`tn`.`expiry_date` > NOW() OR `tn`.`expiry_date` IS NULL) AND INSTR(?,       (CASE WHEN ps.opt_status NOT IN ('ali', 'mis', 'inj', 'dec', 'fnd') OR ps.opt_status IS NULL THEN 'unk' ELSE  ps.opt_status END)))
             JOIN person_details pd ON (tn.p_uuid = pd.p_uuid AND INSTR(?, (CASE WHEN `opt_gender` NOT IN ('mal', 'fml') OR `opt_gender` IS NULL THEN 'unk' ELSE `opt_gender` END))
                                                                                                                          AND INSTR(?, (CASE WHEN CONVERT(`pd`.`years_old`, UNSIGNED INTEGER) IS NOT NULL THEN
                                                        (CASE WHEN `pd`.`years_old` < 18 THEN 'youth'
                                                                  WHEN `pd`.`years_old` >= 18 THEN 'adult' END)
                                         WHEN CONVERT(`pd`.`minAge`, UNSIGNED INTEGER) IS NOT NULL AND CONVERT(`pd`.`maxAge`, UNSIGNED INTEGER) IS NOT NULL
                                                  AND `pd`.`minAge` < 18 AND `pd`.`maxAge` >= 18 THEN 'both'
                                         WHEN CONVERT(`pd`.`minAge`, UNSIGNED INTEGER) IS NOT NULL AND `pd`.`minAge` >= 18 THEN 'adult'
                                         WHEN CONVERT(`pd`.`maxAge`, UNSIGNED INTEGER) IS NOT NULL AND `pd`.`maxAge` < 18 THEN 'youth'
                                         ELSE 'unknown'
                                         END)))
                         LEFT
                         JOIN hospital h        ON (tn.hospital_uuid = h.hospital_uuid AND INSTR(?, (CASE WHEN `h`.`short_name` NOT IN ('nnmc', 'suburban') OR `h`.`short_name` IS NULL THEN 'other' ELSE `h`.`short_name` END)))
             LEFT
                         JOIN image i                   ON (tn.p_uuid = i.p_uuid AND i.principal = TRUE)
                         LEFT
                         JOIN edxl_co_lpf ecl   ON tn.p_uuid = ecl.p_uuid
           ORDER BY ", sortBy, " LIMIT ?, ?;");

      PREPARE stmt FROM @sqlString;

      SET @statusFilter = statusFilter;
      SET @genderFilter = genderFilter;
      SET @ageFilter = ageFilter;
      SET @hospitalFilter = hospitalFilter;

      SET @pageStart = pageStart;
      SET @perPage = perPage;
      SET NAMES utf8;
      EXECUTE stmt USING @statusFilter, @genderFilter, @ageFilter, @hospitalFilter, @pageStart, @perPage;

      DEALLOCATE PREPARE stmt;
      DROP TABLE tmp_names;
END$$

CREATE DEFINER=`root`@`localhost` PROCEDURE `PLSearch2`(IN searchTerms CHAR(255),
	 IN statusFilter VARCHAR(100),
	 IN genderFilter VARCHAR(100),
	 IN ageFilter VARCHAR(100),
	 IN hospitalFilter VARCHAR(100),
	 IN incident VARCHAR(100),
	 IN sortBy VARCHAR(100),
	 IN pageStart INT,
	 IN perPage INT,
   OUT totalRows INT)
BEGIN




  SET @sqlString = "
		SELECT STRAIGHT_JOIN SQL_NO_CACHE
				`a`.`p_uuid`       AS `p_uuid`,
				`a`.`full_name`    AS `full_name`,
				`a`.`given_name`   AS `given_name`,
				`a`.`family_name`  AS `family_name`,
				(CASE WHEN `b`.`opt_status` NOT IN ('ali', 'mis', 'inj', 'dec', 'unk') OR `b`.`opt_status` IS NULL THEN 'unk' ELSE `b`.`opt_status` END) AS `opt_status`,
        DATE_FORMAT(b.updated, '%m/%e/%y @ %l:%i:%s %p') as updated,
				(CASE WHEN `c`.`opt_gender` NOT IN ('mal', 'fml') OR `c`.`opt_gender` IS NULL THEN 'unk' ELSE `c`.`opt_gender` END) AS `opt_gender`,
				(CASE WHEN CAST(`c`.`years_old` AS UNSIGNED) < 18 THEN 'child' WHEN CAST(`c`.`years_old` AS UNSIGNED) >= 18 THEN 'adult' ELSE 'unknown' END) as `age_group`,
				`i`.`image_height` AS `image_height`,
				`i`.`image_width`  AS `image_width`,
				`i`.`url_thumb`    AS `url_thumb`,
				(CASE WHEN `h`.`short_name` NOT IN ('nnmc', 'suburban') OR `h`.`short_name` IS NULL THEN 'other' ELSE `h`.`short_name` END)  AS `hospital`,
				(CASE WHEN (`h`.`hospital_uuid` = -(1)) THEN NULL ELSE `h`.`icon_url` END) AS `icon_url`,
				`inc`.`shortname`  AS `shortname`,
        `pm`.last_seen, `pm`.comments

		   FROM `person_uuid` `a`
		   JOIN `person_status` `b`     ON (`a`.`p_uuid` = `b`.`p_uuid` AND `b`.`isVictim` = 1 )
	  LEFT JOIN `image` `i`           ON `a`.`p_uuid` = `i`.`x_uuid`
		   JOIN `person_details` `c`    ON `a`.`p_uuid` = `c`.`p_uuid`
		   JOIN `incident` `inc`        ON (`inc`.`incident_id` = `a`.`incident_id` AND `a`.`incident_id` <> 0 )
	  LEFT JOIN `hospital` `h`        ON `h`.`hospital_uuid` = `a`.`hospital_uuid`
    LEFT JOIN `person_missing` `pm` ON pm.p_uuid = a.p_uuid
	  WHERE INSTR(?, 	(CASE WHEN `b`.`opt_status` NOT IN ('ali', 'mis', 'inj', 'dec', 'unk') OR `b`.`opt_status` IS NULL THEN 'unk' ELSE `b`.`opt_status` END))
	    AND INSTR(?, (CASE WHEN `c`.`opt_gender` NOT IN ('mal', 'fml') OR `c`.`opt_gender` IS NULL THEN 'unk' ELSE `c`.`opt_gender` END))
		  AND INSTR(?, (CASE WHEN CAST(`c`.`years_old` AS UNSIGNED) < 18 THEN 'child' WHEN CAST(`c`.`years_old` AS UNSIGNED) >= 18 THEN 'adult' ELSE 'unknown' END))
		  AND INSTR(?, (CASE WHEN `h`.`short_name` NOT IN ('nnmc', 'suburban') OR `h`.`short_name` IS NULL THEN 'other' ELSE `h`.`short_name` END))
	    AND `shortname` = ?
      AND (full_name like CONCAT('%', ?, '%'))
    ORDER BY ?
    LIMIT ?, ?";

  PREPARE stmt FROM @sqlString;

  SET @searchTerms = searchTerms;
  SET @statusFilter = statusFilter;
  SET @genderFilter = genderFilter;
  SET @ageFilter = ageFilter;
  SET @hospitalFilter = hospitalFilter;
  SET @incident = incident;
  SET @sortBy = sortBy;
  SET @pageStart = pageStart;
  SET @perPage = perPage;

  EXECUTE stmt USING @statusFilter, @genderFilter, @ageFilter, @hospitalFilter, @incident,
                     @searchTerms,@sortBy, @pageStart, @perPage;

  DEALLOCATE PREPARE stmt;

  SELECT COUNT(p.p_uuid) INTO totalRows
    FROM person_uuid p
    JOIN incident i
      ON p.incident_id = i.incident_id
   WHERE i.shortname = incident;


END$$

CREATE DEFINER=`root`@`localhost` PROCEDURE `PLSearch_Count`(IN searchTerms CHAR(255),
	 IN statusFilter VARCHAR(100),
	 IN genderFilter VARCHAR(100),
	 IN ageFilter VARCHAR(100),
	 IN hospitalFilter VARCHAR(100),
	 IN incident VARCHAR(100))
BEGIN




		SELECT STRAIGHT_JOIN COUNT(a.p_uuid)
		   FROM `person_uuid` `a`
		   JOIN `person_status` `b`          ON (`a`.`p_uuid` = `b`.`p_uuid` AND `b`.`isVictim` = 1 )
		   JOIN `person_details` `c`         ON `a`.`p_uuid` = `c`.`p_uuid`
		   JOIN `incident` `inc`             ON (`inc`.`incident_id` = `a`.`incident_id` AND `a`.`incident_id` <> 0 )
	  LEFT JOIN `hospital` `h`               ON `h`.`hospital_uuid` = `a`.`hospital_uuid`
	  WHERE INSTR(statusFilter, 	(CASE WHEN `b`.`opt_status` NOT IN ('ali', 'mis', 'inj', 'dec', 'unk') OR `b`.`opt_status` IS NULL THEN 'unk' ELSE `b`.`opt_status` END))
	    AND INSTR(genderFilter, (CASE WHEN `c`.`opt_gender` NOT IN ('mal', 'fml') OR `c`.`opt_gender` IS NULL THEN 'unk' ELSE `c`.`opt_gender` END))
		  AND INSTR(ageFilter, (CASE WHEN CAST(`c`.`years_old` AS UNSIGNED) < 18 THEN 'child' WHEN CAST(`c`.`years_old` AS UNSIGNED) >= 18 THEN 'adult' ELSE 'unknown' END))
		  AND INSTR(hospitalFilter, (CASE WHEN `h`.`short_name` NOT IN ('nnmc', 'suburban') OR `h`.`short_name` IS NULL THEN 'other' ELSE `h`.`short_name` END))
	    AND `shortname` = incident
      AND (full_name like CONCAT('%', searchTerms, '%') OR given_name SOUNDS LIKE searchTerms OR family_name SOUNDS LIKE searchTerms);



END$$

DELIMITER ;

-- --------------------------------------------------------

--
-- Table structure for table `adodb_logsql`
--

CREATE TABLE IF NOT EXISTS `adodb_logsql` (
  `created` datetime NOT NULL,
  `sql0` varchar(250) NOT NULL,
  `sql1` text NOT NULL,
  `params` text NOT NULL,
  `tracer` text NOT NULL,
  `timer` decimal(16,6) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- --------------------------------------------------------

--
-- Table structure for table `alt_logins`
--

CREATE TABLE IF NOT EXISTS `alt_logins` (
  `p_uuid` varchar(128) NOT NULL,
  `user_name` varchar(100) NOT NULL,
  `type` varchar(60) DEFAULT 'openid',
  PRIMARY KEY (`p_uuid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- --------------------------------------------------------

--
-- Table structure for table `arrival_rate`
--

CREATE TABLE IF NOT EXISTS `arrival_rate` (
  `person_uuid` varchar(128) NOT NULL,
  `incident_id` bigint(20) NOT NULL,
  `arrival_time` datetime NOT NULL,
  `source_all` int(32) NOT NULL,
  `source_triagepic` int(32) NOT NULL,
  `source_reunite` int(32) NOT NULL,
  `source_website` int(32) NOT NULL,
  `source_pfif` int(32) NOT NULL,
  `source_vanilla_email` int(32) NOT NULL,
  PRIMARY KEY (`person_uuid`),
  KEY `incident_index` (`incident_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

--
-- Dumping data for table `arrival_rate`
--

INSERT INTO `arrival_rate` (`person_uuid`, `incident_id`, `arrival_time`, `source_all`, `source_triagepic`, `source_reunite`, `source_website`, `source_pfif`, `source_vanilla_email`) VALUES
('/vesuvius/vesuvius/wwwperson.101', 2, '2013-07-11 11:46:32', 1, 0, 0, 1, 0, 0);

-- --------------------------------------------------------

--
-- Table structure for table `audit`
--

CREATE TABLE IF NOT EXISTS `audit` (
  `audit_id` bigint(20) NOT NULL AUTO_INCREMENT,
  `updated` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `x_uuid` varchar(128) NOT NULL,
  `u_uuid` varchar(128) NOT NULL,
  `change_type` varchar(3) NOT NULL,
  `change_table` varchar(100) NOT NULL,
  `change_field` varchar(100) NOT NULL,
  `prev_val` text,
  `new_val` text,
  PRIMARY KEY (`audit_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 AUTO_INCREMENT=1 ;

-- --------------------------------------------------------

--
-- Table structure for table `config`
--

CREATE TABLE IF NOT EXISTS `config` (
  `config_id` bigint(20) NOT NULL AUTO_INCREMENT,
  `module_id` varchar(20) DEFAULT NULL,
  `confkey` varchar(50) NOT NULL,
  `value` varchar(100) DEFAULT NULL,
  PRIMARY KEY (`config_id`),
  KEY `module_id` (`module_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 AUTO_INCREMENT=1 ;

-- --------------------------------------------------------

--
-- Table structure for table `contact`
--

CREATE TABLE IF NOT EXISTS `contact` (
  `p_uuid` varchar(128) NOT NULL,
  `opt_contact_type` varchar(10) NOT NULL,
  `contact_value` varchar(100) DEFAULT NULL,
  PRIMARY KEY (`p_uuid`,`opt_contact_type`),
  KEY `contact_value` (`contact_value`),
  KEY `p_uuid` (`p_uuid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- --------------------------------------------------------

--
-- Table structure for table `dao_error_log`
--

CREATE TABLE IF NOT EXISTS `dao_error_log` (
  `time` timestamp NULL DEFAULT CURRENT_TIMESTAMP,
  `file` text,
  `line` text,
  `method` text,
  `class` text,
  `function` text,
  `error_message` text,
  `other` text
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='logs errors encountered in the DAO objects';

--
-- Dumping data for table `dao_error_log`
--

INSERT INTO `dao_error_log` (`time`, `file`, `line`, `method`, `class`, `function`, `error_message`, `other`) VALUES
('2013-07-04 12:59:38', '/opt/lampp/htdocs/vesuvius/vesuvius/mod/em/xajax.inc', '119', 'em_show_events', '', 'em_show_events', 'You have an error in your SQL syntax; check the manual that corresponds to your MySQL server version for the right syntax to use near '''' at line 3', 'show scenario 1'),
('2013-07-04 13:00:02', '/opt/lampp/htdocs/vesuvius/vesuvius/mod/em/xajax.inc', '119', 'em_show_events', '', 'em_show_events', 'You have an error in your SQL syntax; check the manual that corresponds to your MySQL server version for the right syntax to use near '''' at line 3', 'show scenario 1'),
('2013-07-04 13:00:27', '/opt/lampp/htdocs/vesuvius/vesuvius/mod/em/xajax.inc', '119', 'em_show_events', '', 'em_show_events', 'You have an error in your SQL syntax; check the manual that corresponds to your MySQL server version for the right syntax to use near '''' at line 3', 'show scenario 1'),
('2013-07-04 13:00:35', '/opt/lampp/htdocs/vesuvius/vesuvius/mod/em/xajax.inc', '119', 'em_show_events', '', 'em_show_events', 'You have an error in your SQL syntax; check the manual that corresponds to your MySQL server version for the right syntax to use near '''' at line 3', 'show scenario 1'),
('2013-07-04 13:01:17', '/opt/lampp/htdocs/vesuvius/vesuvius/mod/em/xajax.inc', '119', 'em_show_events', '', 'em_show_events', 'You have an error in your SQL syntax; check the manual that corresponds to your MySQL server version for the right syntax to use near ''=='' at line 3', 'show scenario 1'),
('2013-07-04 13:03:03', '/opt/lampp/htdocs/vesuvius/vesuvius/mod/em/xajax.inc', '119', 'em_show_events', '', 'em_show_events', 'You have an error in your SQL syntax; check the manual that corresponds to your MySQL server version for the right syntax to use near ''== '''' at line 3', 'show scenario 1'),
('2013-07-04 13:04:33', '/opt/lampp/htdocs/vesuvius/vesuvius/mod/em/xajax.inc', '119', 'em_show_events', '', 'em_show_events', 'You have an error in your SQL syntax; check the manual that corresponds to your MySQL server version for the right syntax to use near ''== '''' at line 3', 'show scenario 1'),
('2013-07-04 13:05:55', '/opt/lampp/htdocs/vesuvius/vesuvius/mod/em/xajax.inc', '119', 'em_show_events', '', 'em_show_events', 'You have an error in your SQL syntax; check the manual that corresponds to your MySQL server version for the right syntax to use near ''== '''' at line 3', 'show scenario 1'),
('2013-07-04 13:06:34', '/opt/lampp/htdocs/vesuvius/vesuvius/mod/em/xajax.inc', '119', 'em_show_events', '', 'em_show_events', 'You have an error in your SQL syntax; check the manual that corresponds to your MySQL server version for the right syntax to use near ''== '''' at line 3', 'show scenario 1'),
('2013-07-04 13:08:51', '/opt/lampp/htdocs/vesuvius/vesuvius/mod/em/xajax.inc', '119', 'em_show_events', '', 'em_show_events', 'You have an error in your SQL syntax; check the manual that corresponds to your MySQL server version for the right syntax to use near ''== '''' at line 3', 'show scenario 1'),
('2013-07-04 13:10:43', '/opt/lampp/htdocs/vesuvius/vesuvius/mod/em/xajax.inc', '119', 'em_show_events', '', 'em_show_events', 'You have an error in your SQL syntax; check the manual that corresponds to your MySQL server version for the right syntax to use near '''''' at line 3', 'show scenario 1'),
('2013-07-04 13:11:29', '/opt/lampp/htdocs/vesuvius/vesuvius/mod/em/xajax.inc', '120', 'em_show_events', '', 'em_show_events', 'You have an error in your SQL syntax; check the manual that corresponds to your MySQL server version for the right syntax to use near '''''' at line 3', 'show scenario 1'),
('2013-07-04 13:12:30', '/opt/lampp/htdocs/vesuvius/vesuvius/mod/em/xajax.inc', '120', 'em_show_events', '', 'em_show_events', 'You have an error in your SQL syntax; check the manual that corresponds to your MySQL server version for the right syntax to use near '''''' at line 3', 'show scenario 1'),
('2013-07-04 13:15:25', '/opt/lampp/htdocs/vesuvius/vesuvius/mod/em/xajax.inc', '120', 'em_show_events', '', 'em_show_events', 'You have an error in your SQL syntax; check the manual that corresponds to your MySQL server version for the right syntax to use near '''''' at line 3', 'show scenario 1'),
('2013-07-04 13:16:59', '/opt/lampp/htdocs/vesuvius/vesuvius/mod/em/xajax.inc', '119', 'em_show_events', '', 'em_show_events', 'You have an error in your SQL syntax; check the manual that corresponds to your MySQL server version for the right syntax to use near '''''' at line 3', 'show scenario 1'),
('2013-07-04 13:17:12', '/opt/lampp/htdocs/vesuvius/vesuvius/mod/em/xajax.inc', '119', 'em_show_events', '', 'em_show_events', 'You have an error in your SQL syntax; check the manual that corresponds to your MySQL server version for the right syntax to use near '''''' at line 3', 'show scenario 1'),
('2013-07-04 13:19:09', '/opt/lampp/htdocs/vesuvius/vesuvius/mod/em/xajax.inc', '119', 'em_show_events', '', 'em_show_events', 'You have an error in your SQL syntax; check the manual that corresponds to your MySQL server version for the right syntax to use near '''''' at line 3', 'show scenario 1'),
('2013-07-04 13:29:14', '/opt/lampp/htdocs/vesuvius/vesuvius/mod/em/xajax.inc', '119', 'em_show_events', '', 'em_show_events', 'You have an error in your SQL syntax; check the manual that corresponds to your MySQL server version for the right syntax to use near '''''' at line 3', 'show scenario 1'),
('2013-07-04 13:29:28', '/opt/lampp/htdocs/vesuvius/vesuvius/mod/em/xajax.inc', '119', 'em_show_events', '', 'em_show_events', 'You have an error in your SQL syntax; check the manual that corresponds to your MySQL server version for the right syntax to use near '''''' at line 3', 'show scenario 1'),
('2013-07-04 13:32:55', '/opt/lampp/htdocs/vesuvius/vesuvius/mod/em/xajax.inc', '120', 'em_show_events', '', 'em_show_events', 'You have an error in your SQL syntax; check the manual that corresponds to your MySQL server version for the right syntax to use near ''''1'' at line 3', 'show scenario 1'),
('2013-07-04 13:33:27', '/opt/lampp/htdocs/vesuvius/vesuvius/mod/em/xajax.inc', '120', 'em_show_events', '', 'em_show_events', 'You have an error in your SQL syntax; check the manual that corresponds to your MySQL server version for the right syntax to use near ''''1'' at line 3', 'show scenario 1'),
('2013-07-06 12:55:18', '/opt/lampp/htdocs/vesuvius/vesuvius/mod/em/xajax.inc', '753', 'em_perform_save', '', 'em_perform_save', 'Cannot add or update a child row: a foreign key constraint fails (`Sahana`.`incident`, CONSTRAINT `incident_ibfk_1` FOREIGN KEY (`private_group`) REFERENCES `sys_user_groups` (`group_id`) ON DELETE CASCADE ON UPDATE CASCADE)', 'save event 3'),
('2013-07-06 12:58:23', '/opt/lampp/htdocs/vesuvius/vesuvius/mod/em/xajax.inc', '753', 'em_perform_save', '', 'em_perform_save', 'Cannot add or update a child row: a foreign key constraint fails (`Sahana`.`incident`, CONSTRAINT `incident_ibfk_1` FOREIGN KEY (`private_group`) REFERENCES `sys_user_groups` (`group_id`) ON DELETE CASCADE ON UPDATE CASCADE)', 'save event 3'),
('2013-07-06 12:58:46', '/opt/lampp/htdocs/vesuvius/vesuvius/mod/em/xajax.inc', '753', 'em_perform_save', '', 'em_perform_save', 'Cannot add or update a child row: a foreign key constraint fails (`Sahana`.`incident`, CONSTRAINT `incident_ibfk_1` FOREIGN KEY (`private_group`) REFERENCES `sys_user_groups` (`group_id`) ON DELETE CASCADE ON UPDATE CASCADE)', 'save event 3'),
('2013-07-06 13:00:08', '/opt/lampp/htdocs/vesuvius/vesuvius/mod/em/xajax.inc', '753', 'em_perform_save', '', 'em_perform_save', 'Cannot add or update a child row: a foreign key constraint fails (`Sahana`.`incident`, CONSTRAINT `incident_ibfk_1` FOREIGN KEY (`private_group`) REFERENCES `sys_user_groups` (`group_id`) ON DELETE CASCADE ON UPDATE CASCADE)', 'save event 3'),
('2013-07-06 13:00:19', '/opt/lampp/htdocs/vesuvius/vesuvius/mod/em/xajax.inc', '753', 'em_perform_save', '', 'em_perform_save', 'Cannot add or update a child row: a foreign key constraint fails (`Sahana`.`incident`, CONSTRAINT `incident_ibfk_1` FOREIGN KEY (`private_group`) REFERENCES `sys_user_groups` (`group_id`) ON DELETE CASCADE ON UPDATE CASCADE)', 'save event 3'),
('2013-07-06 13:01:36', '/opt/lampp/htdocs/vesuvius/vesuvius/mod/em/xajax.inc', '753', 'em_perform_save', '', 'em_perform_save', 'Cannot add or update a child row: a foreign key constraint fails (`Sahana`.`incident`, CONSTRAINT `incident_ibfk_1` FOREIGN KEY (`private_group`) REFERENCES `sys_user_groups` (`group_id`) ON DELETE CASCADE ON UPDATE CASCADE)', 'save event 3'),
('2013-07-06 13:50:40', '/opt/lampp/htdocs/vesuvius/vesuvius/mod/em/xajax.inc', '753', 'em_perform_save', '', 'em_perform_save', 'Cannot add or update a child row: a foreign key constraint fails (`Sahana`.`incident`, CONSTRAINT `incident_ibfk_1` FOREIGN KEY (`private_group`) REFERENCES `sys_user_groups` (`group_id`) ON DELETE CASCADE ON UPDATE CASCADE)', 'save event 3'),
('2013-07-06 14:12:32', '/opt/lampp/htdocs/vesuvius/vesuvius/mod/em/xajax.inc', '753', 'em_perform_save', '', 'em_perform_save', 'Cannot add or update a child row: a foreign key constraint fails (`Sahana`.`incident`, CONSTRAINT `incident_ibfk_1` FOREIGN KEY (`private_group`) REFERENCES `sys_user_groups` (`group_id`) ON DELETE CASCADE ON UPDATE CASCADE)', 'save event 3'),
('2013-07-06 18:06:41', '/opt/lampp/htdocs/vesuvius/vesuvius/mod/em/xajax.inc', '753', 'em_perform_save', '', 'em_perform_save', 'Cannot add or update a child row: a foreign key constraint fails (`Sahana`.`incident`, CONSTRAINT `incident_ibfk_1` FOREIGN KEY (`private_group`) REFERENCES `sys_user_groups` (`group_id`) ON DELETE CASCADE ON UPDATE CASCADE)', 'save event 3'),
('2013-07-06 18:11:11', '/opt/lampp/htdocs/vesuvius/vesuvius/mod/em/xajax.inc', '753', 'em_perform_save', '', 'em_perform_save', 'Cannot add or update a child row: a foreign key constraint fails (`Sahana`.`incident`, CONSTRAINT `incident_ibfk_1` FOREIGN KEY (`private_group`) REFERENCES `sys_user_groups` (`group_id`) ON DELETE CASCADE ON UPDATE CASCADE)', 'save event 3'),
('2013-07-06 18:35:55', '/opt/lampp/htdocs/vesuvius/vesuvius/mod/em/xajax.inc', '753', 'em_perform_save', '', 'em_perform_save', 'Cannot add or update a child row: a foreign key constraint fails (`Sahana`.`incident`, CONSTRAINT `incident_ibfk_1` FOREIGN KEY (`private_group`) REFERENCES `sys_user_groups` (`group_id`) ON DELETE CASCADE ON UPDATE CASCADE)', 'save event 3'),
('2013-07-06 18:36:05', '/opt/lampp/htdocs/vesuvius/vesuvius/mod/em/xajax.inc', '753', 'em_perform_save', '', 'em_perform_save', 'Cannot add or update a child row: a foreign key constraint fails (`Sahana`.`incident`, CONSTRAINT `incident_ibfk_1` FOREIGN KEY (`private_group`) REFERENCES `sys_user_groups` (`group_id`) ON DELETE CASCADE ON UPDATE CASCADE)', 'save event 3'),
('2013-07-06 19:13:48', '/opt/lampp/htdocs/vesuvius/vesuvius/mod/em/xajax.inc', '759', 'em_perform_save', '', 'em_perform_save', 'Cannot add or update a child row: a foreign key constraint fails (`Sahana`.`incident`, CONSTRAINT `incident_ibfk_1` FOREIGN KEY (`private_group`) REFERENCES `sys_user_groups` (`group_id`) ON DELETE CASCADE ON UPDATE CASCADE)', 'save event 3'),
('2013-07-06 19:14:23', '/opt/lampp/htdocs/vesuvius/vesuvius/mod/em/xajax.inc', '759', 'em_perform_save', '', 'em_perform_save', 'Cannot add or update a child row: a foreign key constraint fails (`Sahana`.`incident`, CONSTRAINT `incident_ibfk_1` FOREIGN KEY (`private_group`) REFERENCES `sys_user_groups` (`group_id`) ON DELETE CASCADE ON UPDATE CASCADE)', 'save event 3'),
('2013-07-06 19:14:55', '/opt/lampp/htdocs/vesuvius/vesuvius/mod/em/xajax.inc', '759', 'em_perform_save', '', 'em_perform_save', 'Cannot add or update a child row: a foreign key constraint fails (`Sahana`.`incident`, CONSTRAINT `incident_ibfk_1` FOREIGN KEY (`private_group`) REFERENCES `sys_user_groups` (`group_id`) ON DELETE CASCADE ON UPDATE CASCADE)', 'save event 3'),
('2013-07-08 15:22:42', '/opt/lampp/htdocs/vesuvius/vesuvius/mod/em/xajax.inc', '766', 'em_perform_save', '', 'em_perform_save', 'Cannot add or update a child row: a foreign key constraint fails (`Sahana`.`incident`, CONSTRAINT `incident_ibfk_1` FOREIGN KEY (`private_group`) REFERENCES `sys_user_groups` (`group_id`) ON DELETE CASCADE ON UPDATE CASCADE)', 'save event 3'),
('2013-07-08 15:22:44', '/opt/lampp/htdocs/vesuvius/vesuvius/mod/em/xajax.inc', '766', 'em_perform_save', '', 'em_perform_save', 'Cannot add or update a child row: a foreign key constraint fails (`Sahana`.`incident`, CONSTRAINT `incident_ibfk_1` FOREIGN KEY (`private_group`) REFERENCES `sys_user_groups` (`group_id`) ON DELETE CASCADE ON UPDATE CASCADE)', 'save event 3'),
('2013-07-08 15:23:00', '/opt/lampp/htdocs/vesuvius/vesuvius/mod/em/xajax.inc', '766', 'em_perform_save', '', 'em_perform_save', 'Cannot add or update a child row: a foreign key constraint fails (`Sahana`.`incident`, CONSTRAINT `incident_ibfk_1` FOREIGN KEY (`private_group`) REFERENCES `sys_user_groups` (`group_id`) ON DELETE CASCADE ON UPDATE CASCADE)', 'save event 3'),
('2013-07-08 15:24:04', '/opt/lampp/htdocs/vesuvius/vesuvius/mod/em/xajax.inc', '766', 'em_perform_save', '', 'em_perform_save', 'Cannot add or update a child row: a foreign key constraint fails (`Sahana`.`incident`, CONSTRAINT `incident_ibfk_1` FOREIGN KEY (`private_group`) REFERENCES `sys_user_groups` (`group_id`) ON DELETE CASCADE ON UPDATE CASCADE)', 'save event 3'),
('2013-07-09 10:16:39', '/opt/lampp/htdocs/vesuvius/vesuvius/mod/em/xajax.inc', '775', 'em_perform_save', '', 'em_perform_save', 'Cannot add or update a child row: a foreign key constraint fails (`Sahana`.`incident`, CONSTRAINT `incident_ibfk_1` FOREIGN KEY (`private_group`) REFERENCES `sys_user_groups` (`group_id`) ON DELETE CASCADE ON UPDATE CASCADE)', 'save event 3'),
('2013-07-09 12:52:18', '/opt/lampp/htdocs/vesuvius/vesuvius/mod/em/xajax.inc', '765', 'em_perform_save', '', 'em_perform_save', 'Unknown column ''disaster_id'' in ''field list''', 'insert categories'),
('2013-07-09 12:52:19', '/opt/lampp/htdocs/vesuvius/vesuvius/mod/em/xajax.inc', '787', 'em_perform_save', '', 'em_perform_save', 'Cannot add or update a child row: a foreign key constraint fails (`Sahana`.`incident`, CONSTRAINT `incident_ibfk_1` FOREIGN KEY (`private_group`) REFERENCES `sys_user_groups` (`group_id`) ON DELETE CASCADE ON UPDATE CASCADE)', 'save event 3'),
('2013-07-09 12:52:30', '/opt/lampp/htdocs/vesuvius/vesuvius/mod/em/xajax.inc', '765', 'em_perform_save', '', 'em_perform_save', 'Unknown column ''disaster_id'' in ''field list''', 'insert categories'),
('2013-07-09 12:52:30', '/opt/lampp/htdocs/vesuvius/vesuvius/mod/em/xajax.inc', '787', 'em_perform_save', '', 'em_perform_save', 'Cannot add or update a child row: a foreign key constraint fails (`Sahana`.`incident`, CONSTRAINT `incident_ibfk_1` FOREIGN KEY (`private_group`) REFERENCES `sys_user_groups` (`group_id`) ON DELETE CASCADE ON UPDATE CASCADE)', 'save event 3'),
('2013-07-09 12:53:24', '/opt/lampp/htdocs/vesuvius/vesuvius/mod/em/xajax.inc', '765', 'em_perform_save', '', 'em_perform_save', 'Unknown column ''disaster_id'' in ''field list''', 'insert categories'),
('2013-07-09 12:53:24', '/opt/lampp/htdocs/vesuvius/vesuvius/mod/em/xajax.inc', '787', 'em_perform_save', '', 'em_perform_save', 'Cannot add or update a child row: a foreign key constraint fails (`Sahana`.`incident`, CONSTRAINT `incident_ibfk_1` FOREIGN KEY (`private_group`) REFERENCES `sys_user_groups` (`group_id`) ON DELETE CASCADE ON UPDATE CASCADE)', 'save event 3'),
('2013-07-09 12:54:38', '/opt/lampp/htdocs/vesuvius/vesuvius/mod/em/xajax.inc', '765', 'em_perform_save', '', 'em_perform_save', 'Cannot add or update a child row: a foreign key constraint fails (`Sahana`.`scenario`, CONSTRAINT `scenario_ibfk_1` FOREIGN KEY (`incident_id`) REFERENCES `incident` (`incident_id`) ON DELETE CASCADE ON UPDATE CASCADE)', 'insert categories'),
('2013-07-09 12:54:38', '/opt/lampp/htdocs/vesuvius/vesuvius/mod/em/xajax.inc', '787', 'em_perform_save', '', 'em_perform_save', 'Cannot add or update a child row: a foreign key constraint fails (`Sahana`.`incident`, CONSTRAINT `incident_ibfk_1` FOREIGN KEY (`private_group`) REFERENCES `sys_user_groups` (`group_id`) ON DELETE CASCADE ON UPDATE CASCADE)', 'save event 3'),
('2013-07-09 12:56:37', '/opt/lampp/htdocs/vesuvius/vesuvius/mod/em/xajax.inc', '787', 'em_perform_save', '', 'em_perform_save', 'Cannot add or update a child row: a foreign key constraint fails (`Sahana`.`incident`, CONSTRAINT `incident_ibfk_1` FOREIGN KEY (`private_group`) REFERENCES `sys_user_groups` (`group_id`) ON DELETE CASCADE ON UPDATE CASCADE)', 'save event 3'),
('2013-07-09 12:56:45', '/opt/lampp/htdocs/vesuvius/vesuvius/mod/em/xajax.inc', '765', 'em_perform_save', '', 'em_perform_save', 'Cannot add or update a child row: a foreign key constraint fails (`Sahana`.`scenario`, CONSTRAINT `scenario_ibfk_2` FOREIGN KEY (`disaster_type`) REFERENCES `disaster_category` (`category_id`) ON DELETE CASCADE ON UPDATE CASCADE)', 'insert categories'),
('2013-07-09 12:56:45', '/opt/lampp/htdocs/vesuvius/vesuvius/mod/em/xajax.inc', '787', 'em_perform_save', '', 'em_perform_save', 'Cannot add or update a child row: a foreign key constraint fails (`Sahana`.`incident`, CONSTRAINT `incident_ibfk_1` FOREIGN KEY (`private_group`) REFERENCES `sys_user_groups` (`group_id`) ON DELETE CASCADE ON UPDATE CASCADE)', 'save event 3'),
('2013-07-09 12:57:50', '/opt/lampp/htdocs/vesuvius/vesuvius/mod/em/xajax.inc', '787', 'em_perform_save', '', 'em_perform_save', 'Cannot add or update a child row: a foreign key constraint fails (`Sahana`.`incident`, CONSTRAINT `incident_ibfk_1` FOREIGN KEY (`private_group`) REFERENCES `sys_user_groups` (`group_id`) ON DELETE CASCADE ON UPDATE CASCADE)', 'save event 3'),
('2013-07-09 12:58:05', '/opt/lampp/htdocs/vesuvius/vesuvius/mod/em/xajax.inc', '787', 'em_perform_save', '', 'em_perform_save', 'Cannot add or update a child row: a foreign key constraint fails (`Sahana`.`incident`, CONSTRAINT `incident_ibfk_1` FOREIGN KEY (`private_group`) REFERENCES `sys_user_groups` (`group_id`) ON DELETE CASCADE ON UPDATE CASCADE)', 'save event 3'),
('2013-07-09 12:59:31', '/opt/lampp/htdocs/vesuvius/vesuvius/mod/em/xajax.inc', '786', 'em_perform_save', '', 'em_perform_save', 'Cannot add or update a child row: a foreign key constraint fails (`Sahana`.`incident`, CONSTRAINT `incident_ibfk_1` FOREIGN KEY (`private_group`) REFERENCES `sys_user_groups` (`group_id`) ON DELETE CASCADE ON UPDATE CASCADE)', 'save event 3'),
('2013-07-09 12:59:47', '/opt/lampp/htdocs/vesuvius/vesuvius/mod/em/xajax.inc', '786', 'em_perform_save', '', 'em_perform_save', 'Cannot add or update a child row: a foreign key constraint fails (`Sahana`.`incident`, CONSTRAINT `incident_ibfk_1` FOREIGN KEY (`private_group`) REFERENCES `sys_user_groups` (`group_id`) ON DELETE CASCADE ON UPDATE CASCADE)', 'save event 3'),
('2013-07-09 13:00:53', '/opt/lampp/htdocs/vesuvius/vesuvius/mod/em/xajax.inc', '787', 'em_perform_save', '', 'em_perform_save', 'Cannot add or update a child row: a foreign key constraint fails (`Sahana`.`incident`, CONSTRAINT `incident_ibfk_1` FOREIGN KEY (`private_group`) REFERENCES `sys_user_groups` (`group_id`) ON DELETE CASCADE ON UPDATE CASCADE)', 'save event 3'),
('2013-07-09 13:14:13', '/opt/lampp/htdocs/vesuvius/vesuvius/mod/em/xajax.inc', '763', 'em_perform_save', '', 'em_perform_save', 'Cannot add or update a child row: a foreign key constraint fails (`Sahana`.`scenario`, CONSTRAINT `scenario_ibfk_2` FOREIGN KEY (`disaster_type`) REFERENCES `disaster_category` (`category_id`) ON DELETE CASCADE ON UPDATE CASCADE)', 'insert categories'),
('2013-07-09 13:14:13', '/opt/lampp/htdocs/vesuvius/vesuvius/mod/em/xajax.inc', '787', 'em_perform_save', '', 'em_perform_save', 'Cannot add or update a child row: a foreign key constraint fails (`Sahana`.`incident`, CONSTRAINT `incident_ibfk_1` FOREIGN KEY (`private_group`) REFERENCES `sys_user_groups` (`group_id`) ON DELETE CASCADE ON UPDATE CASCADE)', 'save event 3'),
('2013-07-09 13:52:54', '/opt/lampp/htdocs/vesuvius/vesuvius/mod/em/xajax.inc', '787', 'em_perform_save', '', 'em_perform_save', 'Cannot add or update a child row: a foreign key constraint fails (`Sahana`.`incident`, CONSTRAINT `incident_ibfk_1` FOREIGN KEY (`private_group`) REFERENCES `sys_user_groups` (`group_id`) ON DELETE CASCADE ON UPDATE CASCADE)', 'save event 3'),
('2013-07-09 13:53:18', '/opt/lampp/htdocs/vesuvius/vesuvius/mod/em/xajax.inc', '787', 'em_perform_save', '', 'em_perform_save', 'Cannot add or update a child row: a foreign key constraint fails (`Sahana`.`incident`, CONSTRAINT `incident_ibfk_1` FOREIGN KEY (`private_group`) REFERENCES `sys_user_groups` (`group_id`) ON DELETE CASCADE ON UPDATE CASCADE)', 'save event 3'),
('2013-07-09 13:53:59', '/opt/lampp/htdocs/vesuvius/vesuvius/mod/em/xajax.inc', '787', 'em_perform_save', '', 'em_perform_save', 'Cannot add or update a child row: a foreign key constraint fails (`Sahana`.`incident`, CONSTRAINT `incident_ibfk_1` FOREIGN KEY (`private_group`) REFERENCES `sys_user_groups` (`group_id`) ON DELETE CASCADE ON UPDATE CASCADE)', 'save event 3'),
('2013-07-09 14:36:20', '/opt/lampp/htdocs/vesuvius/vesuvius/mod/em/xajax.inc', '1319', 'em_perform_edit', '', 'em_perform_edit', 'Table ''Sahana.disaster_catgeory'' doesn''t exist', 'generate categry 3'),
('2013-07-09 14:36:51', '/opt/lampp/htdocs/vesuvius/vesuvius/mod/em/xajax.inc', '1319', 'em_perform_edit', '', 'em_perform_edit', 'Table ''Sahana.disaster_catgeory'' doesn''t exist', 'generate categry 3'),
('2013-07-09 15:03:09', '/opt/lampp/htdocs/vesuvius/vesuvius/mod/em/xajax.inc', '763', 'em_perform_save', '', 'em_perform_save', 'Cannot add or update a child row: a foreign key constraint fails (`Sahana`.`scenario`, CONSTRAINT `scenario_ibfk_2` FOREIGN KEY (`disaster_type`) REFERENCES `disaster_category` (`category_id`) ON DELETE CASCADE ON UPDATE CASCADE)', 'insert categories'),
('2013-07-12 07:45:25', '/opt/lampp/htdocs/vesuvius/vesuvius/mod/em/xajax.inc', '763', 'em_perform_save', '', 'em_perform_save', 'Cannot add or update a child row: a foreign key constraint fails (`Sahana`.`scenario`, CONSTRAINT `scenario_ibfk_2` FOREIGN KEY (`disaster_type`) REFERENCES `disaster_category` (`category_id`) ON DELETE CASCADE ON UPDATE CASCADE)', 'insert categories'),
('2013-07-16 15:32:48', '/opt/lampp/htdocs/vesuvius/vesuvius/inc/lib_security/lib_acl.inc', '35', 'shn_acl_log_msg', '', 'shn_acl_log_msg', 'Cannot add or update a child row: a foreign key constraint fails (`Sahana`.`password_event_log`, CONSTRAINT `password_event_log_ibfk_1` FOREIGN KEY (`p_uuid`) REFERENCES `person_uuid` (`p_uuid`) ON DELETE CASCADE ON UPDATE CASCADE)', 'ACL LOG (\n		insert into password_event_log(p_uuid, user_name, comment, event_type)\n		values(''-2'', ''Anonymous'',''Password Change error: password does not comply with the policy'','''');\n	)'),
('2013-07-17 13:12:57', '/opt/lampp/htdocs/vesuvius/vesuvius/mod/em/xajax.inc', '1071', 'em_perform_new', '', 'em_perform_new', 'Cannot add or update a child row: a foreign key constraint fails (`Sahana`.`pfif_repository`, CONSTRAINT `pfif_repository_ibfk_1` FOREIGN KEY (`incident_id`) REFERENCES `incident` (`incident_id`) ON DELETE CASCADE ON UPDATE CASCADE)', 'new 5'),
('2013-07-17 13:12:57', '/opt/lampp/htdocs/vesuvius/vesuvius/mod/em/xajax.inc', '1077', 'em_perform_new', '', 'em_perform_new', 'Cannot add or update a child row: a foreign key constraint fails (`Sahana`.`pfif_repository`, CONSTRAINT `pfif_repository_ibfk_1` FOREIGN KEY (`incident_id`) REFERENCES `incident` (`incident_id`) ON DELETE CASCADE ON UPDATE CASCADE)', 'new 6'),
('2013-07-17 13:12:57', '/opt/lampp/htdocs/vesuvius/vesuvius/mod/em/xajax.inc', '1083', 'em_perform_new', '', 'em_perform_new', 'Cannot add or update a child row: a foreign key constraint fails (`Sahana`.`pfif_repository`, CONSTRAINT `pfif_repository_ibfk_1` FOREIGN KEY (`incident_id`) REFERENCES `incident` (`incident_id`) ON DELETE CASCADE ON UPDATE CASCADE)', 'new 7'),
('2013-08-17 16:13:15', '/opt/lampp/htdocs/Vesuvius/mod/rez/xajax.inc', '568', 'rez_perform_new', '', 'rez_perform_new', 'Duplicate entry ''2'' for key ''PRIMARY''', 'rez new 2'),
('2013-08-17 16:14:30', '/opt/lampp/htdocs/Vesuvius/mod/rez/xajax.inc', '568', 'rez_perform_new', '', 'rez_perform_new', 'Duplicate entry ''2'' for key ''PRIMARY''', 'rez new 2'),
('2013-08-18 05:00:51', '/opt/lampp/htdocs/Vesuvius/mod/rez/xajax.inc', '568', 'rez_perform_new', '', 'rez_perform_new', 'Duplicate entry ''2'' for key ''PRIMARY''', 'rez new 2'),
('2013-08-18 05:01:05', '/opt/lampp/htdocs/Vesuvius/mod/rez/xajax.inc', '568', 'rez_perform_new', '', 'rez_perform_new', 'Duplicate entry ''2'' for key ''PRIMARY''', 'rez new 2'),
('2013-08-18 05:04:35', '/opt/lampp/htdocs/Vesuvius/mod/rez/xajax.inc', '552', 'rez_perform_new', '', 'rez_perform_new', 'You have an error in your SQL syntax; check the manual that corresponds to your MySQL server version for the right syntax to use near ''DES'' at line 3', 'rez new 2'),
('2013-08-18 05:05:11', '/opt/lampp/htdocs/Vesuvius/mod/rez/xajax.inc', '552', 'rez_perform_new', '', 'rez_perform_new', 'You have an error in your SQL syntax; check the manual that corresponds to your MySQL server version for the right syntax to use near ''DSC'' at line 3', 'rez new 2'),
('2013-08-18 05:05:47', '/opt/lampp/htdocs/Vesuvius/mod/rez/xajax.inc', '568', 'rez_perform_new', '', 'rez_perform_new', 'Cannot add or update a child row: a foreign key constraint fails (`Sahana`.`rez_page_template`, CONSTRAINT `rez_page_template_ibfk_2` FOREIGN KEY (`created_by`) REFERENCES `users` (`user_id`) ON UPDATE CASCADE)', 'rez new 2'),
('2013-08-18 05:18:34', '/opt/lampp/htdocs/Vesuvius/mod/rez/xajax.inc', '568', 'rez_perform_new', '', 'rez_perform_new', 'Cannot add or update a child row: a foreign key constraint fails (`Sahana`.`rez_page_template`, CONSTRAINT `rez_page_template_ibfk_2` FOREIGN KEY (`created_by`) REFERENCES `users` (`user_id`) ON UPDATE CASCADE)', 'rez new 2'),
('2013-08-18 05:43:42', '/opt/lampp/htdocs/Vesuvius/mod/rez/xajax.inc', '568', 'rez_perform_new', '', 'rez_perform_new', 'Cannot add or update a child row: a foreign key constraint fails (`Sahana`.`rez_page_template`, CONSTRAINT `rez_page_template_ibfk_2` FOREIGN KEY (`created_by`) REFERENCES `users` (`user_id`) ON UPDATE CASCADE)', 'rez new 2'),
('2013-08-18 05:43:42', '/opt/lampp/htdocs/Vesuvius/mod/rez/xajax.inc', '741', 'rez_perform_edit', '', 'rez_perform_edit', 'You have an error in your SQL syntax; check the manual that corresponds to your MySQL server version for the right syntax to use near '''' at line 3', 'generate categry 1'),
('2013-08-18 05:50:31', '/opt/lampp/htdocs/Vesuvius/mod/rez/xajax.inc', '568', 'rez_perform_new', '', 'rez_perform_new', 'Cannot add or update a child row: a foreign key constraint fails (`Sahana`.`rez_page_template`, CONSTRAINT `rez_page_template_ibfk_2` FOREIGN KEY (`created_by`) REFERENCES `users` (`user_id`) ON UPDATE CASCADE)', 'rez new 2'),
('2013-08-18 05:50:31', '/opt/lampp/htdocs/Vesuvius/mod/rez/xajax.inc', '741', 'rez_perform_edit', '', 'rez_perform_edit', 'You have an error in your SQL syntax; check the manual that corresponds to your MySQL server version for the right syntax to use near '''' at line 3', 'generate categry 1'),
('2013-08-18 06:23:10', '/opt/lampp/htdocs/Vesuvius/mod/rez/xajax.inc', '568', 'rez_perform_new', '', 'rez_perform_new', 'Cannot add or update a child row: a foreign key constraint fails (`Sahana`.`rez_page_template`, CONSTRAINT `rez_page_template_ibfk_2` FOREIGN KEY (`created_by`) REFERENCES `users` (`user_id`) ON UPDATE CASCADE)', 'rez new 2'),
('2013-08-18 06:23:10', '/opt/lampp/htdocs/Vesuvius/mod/rez/xajax.inc', '741', 'rez_perform_edit', '', 'rez_perform_edit', 'You have an error in your SQL syntax; check the manual that corresponds to your MySQL server version for the right syntax to use near '''' at line 3', 'generate categry 1'),
('2013-08-18 09:35:57', '/opt/lampp/htdocs/Vesuvius/inc/lib_security/lib_acl.inc', '35', 'shn_acl_log_msg', '', 'shn_acl_log_msg', 'Cannot add or update a child row: a foreign key constraint fails (`Sahana`.`password_event_log`, CONSTRAINT `password_event_log_ibfk_1` FOREIGN KEY (`p_uuid`) REFERENCES `person_uuid` (`p_uuid`) ON DELETE CASCADE ON UPDATE CASCADE)', 'ACL LOG (\n		insert into password_event_log(p_uuid, user_name, comment, event_type)\n		values(''anonymous'', ''Anonymous User'',''Login Failed : Invalid user name or password.'','''');\n	)'),
('2013-08-18 14:42:04', '/opt/lampp/htdocs/Vesuvius/mod/rez/xajax.inc', '569', 'rez_perform_new', '', 'rez_perform_new', 'Cannot add or update a child row: a foreign key constraint fails (`Sahana`.`rez_page_template`, CONSTRAINT `rez_page_template_ibfk_2` FOREIGN KEY (`created_by`) REFERENCES `users` (`user_id`) ON UPDATE CASCADE)', 'rez new 2'),
('2013-08-18 14:42:04', '/opt/lampp/htdocs/Vesuvius/mod/rez/xajax.inc', '800', 'rez_perform_edit', '', 'rez_perform_edit', 'You have an error in your SQL syntax; check the manual that corresponds to your MySQL server version for the right syntax to use near '''' at line 3', 'generate categry 1'),
('2013-08-18 14:52:15', '/opt/lampp/htdocs/Vesuvius/mod/rez/xajax.inc', '569', 'rez_perform_new', '', 'rez_perform_new', 'Cannot add or update a child row: a foreign key constraint fails (`Sahana`.`rez_page_template`, CONSTRAINT `rez_page_template_ibfk_2` FOREIGN KEY (`created_by`) REFERENCES `users` (`user_id`) ON UPDATE CASCADE)', 'rez new 2'),
('2013-08-18 14:52:15', '/opt/lampp/htdocs/Vesuvius/mod/rez/xajax.inc', '800', 'rez_perform_edit', '', 'rez_perform_edit', 'You have an error in your SQL syntax; check the manual that corresponds to your MySQL server version for the right syntax to use near '''' at line 3', 'generate categry 1'),
('2013-08-18 14:56:25', '/opt/lampp/htdocs/Vesuvius/mod/rez/xajax.inc', '569', 'rez_perform_new', '', 'rez_perform_new', 'Cannot add or update a child row: a foreign key constraint fails (`Sahana`.`rez_page_template`, CONSTRAINT `rez_page_template_ibfk_2` FOREIGN KEY (`created_by`) REFERENCES `users` (`user_id`) ON UPDATE CASCADE)', 'rez new 2'),
('2013-08-18 14:56:25', '/opt/lampp/htdocs/Vesuvius/mod/rez/xajax.inc', '800', 'rez_perform_edit', '', 'rez_perform_edit', 'You have an error in your SQL syntax; check the manual that corresponds to your MySQL server version for the right syntax to use near '''' at line 3', 'generate categry 1'),
('2013-08-18 14:58:20', '/opt/lampp/htdocs/Vesuvius/mod/rez/xajax.inc', '569', 'rez_perform_new', '', 'rez_perform_new', 'Cannot add or update a child row: a foreign key constraint fails (`Sahana`.`rez_page_template`, CONSTRAINT `rez_page_template_ibfk_2` FOREIGN KEY (`created_by`) REFERENCES `users` (`user_id`) ON UPDATE CASCADE)', 'rez new 2'),
('2013-08-18 14:58:20', '/opt/lampp/htdocs/Vesuvius/mod/rez/xajax.inc', '800', 'rez_perform_edit', '', 'rez_perform_edit', 'You have an error in your SQL syntax; check the manual that corresponds to your MySQL server version for the right syntax to use near '''' at line 3', 'generate categry 1'),
('2013-08-18 16:38:30', '/opt/lampp/htdocs/Vesuvius/mod/rez/xajax.inc', '569', 'rez_perform_new', '', 'rez_perform_new', 'Cannot add or update a child row: a foreign key constraint fails (`Sahana`.`rez_page_template`, CONSTRAINT `rez_page_template_ibfk_2` FOREIGN KEY (`created_by`) REFERENCES `users` (`user_id`) ON UPDATE CASCADE)', 'rez new 2'),
('2013-08-18 16:38:31', '/opt/lampp/htdocs/Vesuvius/mod/rez/xajax.inc', '800', 'rez_perform_edit', '', 'rez_perform_edit', 'You have an error in your SQL syntax; check the manual that corresponds to your MySQL server version for the right syntax to use near '''' at line 3', 'generate categry 1'),
('2013-08-18 18:46:44', '/opt/lampp/htdocs/Vesuvius/mod/rez/xajax.inc', '667', 'rez_perform_save', '', 'rez_perform_save', 'You have an error in your SQL syntax; check the manual that corresponds to your MySQL server version for the right syntax to use near ''s details.<br>\n<br>\nUnder the search box is the number of records found that mat'' at line 2', 'rez new 2'),
('2013-08-18 19:00:22', '/opt/lampp/htdocs/Vesuvius/mod/rez/xajax.inc', '667', 'rez_perform_save', '', 'rez_perform_save', 'You have an error in your SQL syntax; check the manual that corresponds to your MySQL server version for the right syntax to use near ''s details.<br>\n<br>\nUnder the search box is the number of records found that mat'' at line 2', 'rez new 2'),
('2013-08-18 20:13:42', '/opt/lampp/htdocs/Vesuvius/mod/rez/xajax.inc', '669', 'rez_perform_save', '', 'rez_perform_save', 'You have an error in your SQL syntax; check the manual that corresponds to your MySQL server version for the right syntax to use near ''s details.<br>\n<br>\nUnder the search box is the number of records found that mat'' at line 2', 'rez new 2'),
('2013-08-18 20:15:21', '/opt/lampp/htdocs/Vesuvius/mod/rez/xajax.inc', '669', 'rez_perform_save', '', 'rez_perform_save', 'You have an error in your SQL syntax; check the manual that corresponds to your MySQL server version for the right syntax to use near ''s details.<br>\n<br>\nUnder the search box is the number of records found that mat'' at line 2', 'rez new 2'),
('2013-08-18 20:16:29', '/opt/lampp/htdocs/Vesuvius/mod/rez/xajax.inc', '669', 'rez_perform_save', '', 'rez_perform_save', 'You have an error in your SQL syntax; check the manual that corresponds to your MySQL server version for the right syntax to use near ''s details.<br>\n<br>\nUnder the search box is the number of records found that mat'' at line 2', 'rez new 2'),
('2013-08-18 20:19:19', '/opt/lampp/htdocs/Vesuvius/mod/rez/xajax.inc', '669', 'rez_perform_save', '', 'rez_perform_save', 'You have an error in your SQL syntax; check the manual that corresponds to your MySQL server version for the right syntax to use near ''s details.<br>\n<br>\nUnder the search box is the number of records found that mat'' at line 2', 'rez new 2'),
('2013-08-18 20:22:17', '/opt/lampp/htdocs/Vesuvius/mod/rez/xajax.inc', '669', 'rez_perform_save', '', 'rez_perform_save', 'You have an error in your SQL syntax; check the manual that corresponds to your MySQL server version for the right syntax to use near ''s details.<br>\n<br>\nUnder the search box is the number of records found that mat'' at line 2', 'rez new 2'),
('2013-08-18 20:22:57', '/opt/lampp/htdocs/Vesuvius/mod/rez/xajax.inc', '669', 'rez_perform_save', '', 'rez_perform_save', 'You have an error in your SQL syntax; check the manual that corresponds to your MySQL server version for the right syntax to use near ''s details.<br>\n<br>\nUnder the search box is the number of records found that mat'' at line 2', 'rez new 2'),
('2013-08-19 09:13:10', '/opt/lampp/htdocs/Vesuvius/mod/rez/xajax.inc', '669', 'rez_perform_save', '', 'rez_perform_save', 'You have an error in your SQL syntax; check the manual that corresponds to your MySQL server version for the right syntax to use near ''s details.<br>\n<br>\nUnder the search box is the number of records found that mat'' at line 2', 'rez new 2'),
('2013-08-19 09:17:02', '/opt/lampp/htdocs/Vesuvius/mod/rez/xajax.inc', '669', 'rez_perform_save', '', 'rez_perform_save', 'You have an error in your SQL syntax; check the manual that corresponds to your MySQL server version for the right syntax to use near ''s details.<br>\n<br>\nUnder the search box is the number of records found that mat'' at line 2', 'rez new 2'),
('2013-08-19 14:48:26', '/opt/lampp/htdocs/Vesuvius/mod/rez/xajax.inc', '273', 'rez_perform_change_visibility', '', 'rez_perform_change_visibility', 'Unknown column ''Hidden'' in ''where clause''', 'rez update visibility'),
('2013-08-19 19:40:56', '/opt/lampp/htdocs/Vesuvius/mod/rez/xajax.inc', '577', 'rez_perform_new', '', 'rez_perform_new', 'Cannot add or update a child row: a foreign key constraint fails (`Sahana`.`rez_page_template`, CONSTRAINT `rez_page_template_ibfk_2` FOREIGN KEY (`created_by`) REFERENCES `users` (`user_id`) ON UPDATE CASCADE)', 'rez new 2'),
('2013-08-19 19:40:56', '/opt/lampp/htdocs/Vesuvius/mod/rez/xajax.inc', '819', 'rez_perform_edit', '', 'rez_perform_edit', 'You have an error in your SQL syntax; check the manual that corresponds to your MySQL server version for the right syntax to use near '''' at line 3', 'generate categry 1'),
('2013-08-19 20:24:02', '/opt/lampp/htdocs/Vesuvius/mod/rez/xajax.inc', '674', 'rez_perform_save', '', 'rez_perform_save', 'You have an error in your SQL syntax; check the manual that corresponds to your MySQL server version for the right syntax to use near ''s details.<br>\n<br>\nUnder the search box is the number of records found that mat'' at line 2', 'rez new 2'),
('2013-08-20 14:10:38', '/opt/lampp/htdocs/Vesuvius/mod/rez/xajax.inc', '723', 'rez_template_save', '', 'rez_template_save', 'Unknown column ''rez_menu_order'' in ''order clause''', 'rez new 1'),
('2013-08-20 14:11:03', '/opt/lampp/htdocs/Vesuvius/mod/rez/xajax.inc', '723', 'rez_template_save', '', 'rez_template_save', 'Unknown column ''rez_menu_order'' in ''order clause''', 'rez new 1'),
('2013-08-20 14:16:12', '/opt/lampp/htdocs/Vesuvius/mod/rez/xajax.inc', '723', 'rez_template_save', '', 'rez_template_save', 'Duplicate entry ''4'' for key ''PRIMARY''', 'rez new 2'),
('2013-08-20 14:26:33', '/opt/lampp/htdocs/Vesuvius/mod/rez/xajax.inc', '674', 'rez_perform_save', '', 'rez_perform_save', 'You have an error in your SQL syntax; check the manual that corresponds to your MySQL server version for the right syntax to use near ''s details.<br>\n<br>\nUnder the search box is the number of records found that mat'' at line 2', 'rez new 2'),
('2013-08-20 14:27:38', '/opt/lampp/htdocs/Vesuvius/mod/rez/xajax.inc', '674', 'rez_perform_save', '', 'rez_perform_save', 'You have an error in your SQL syntax; check the manual that corresponds to your MySQL server version for the right syntax to use near ''s details.<br>\n<br>\nUnder the search box is the number of records found that mat'' at line 2', 'rez new 2'),
('2013-09-09 06:14:05', '/opt/lampp/htdocs/Vesuvius/mod/rez/xajax.inc', '668', 'rez_perform_save', '', 'rez_perform_save', 'You have an error in your SQL syntax; check the manual that corresponds to your MySQL server version for the right syntax to use near ''08:14:05)'' at line 2', 'rez page_incident save'),
('2013-09-09 06:59:17', '/opt/lampp/htdocs/Vesuvius/mod/rez/xajax.inc', '668', 'rez_perform_save', '', 'rez_perform_save', 'Duplicate entry ''11-1'' for key ''PRIMARY''', 'rez page_incident save'),
('2013-09-09 07:04:40', '/opt/lampp/htdocs/Vesuvius/mod/rez/xajax.inc', '668', 'rez_perform_save', '', 'rez_perform_save', 'Duplicate entry ''11-6'' for key ''PRIMARY''', 'rez page_incident save'),
('2013-09-09 07:05:05', '/opt/lampp/htdocs/Vesuvius/mod/rez/xajax.inc', '668', 'rez_perform_save', '', 'rez_perform_save', 'Duplicate entry ''11-6'' for key ''PRIMARY''', 'rez page_incident save'),
('2013-09-09 11:01:36', '/opt/lampp/htdocs/Vesuvius/mod/rez/xajax.inc', '331', 'rez_show_templates', '', 'rez_show_templates', 'Unknown column ''disaster_category_id'' in ''field list''', 'rez show templates'),
('2013-09-09 12:14:15', '/opt/lampp/htdocs/Vesuvius/mod/rez/xajax.inc', '943', 'rez_perform_edit', '', 'rez_perform_edit', 'Unknown column ''disaster_category_id'' in ''where clause''', 'rez edit get_disaster3'),
('2013-09-09 12:14:54', '/opt/lampp/htdocs/Vesuvius/mod/rez/xajax.inc', '939', 'rez_perform_edit', '', 'rez_perform_edit', 'Unknown column ''disaster_category_id'' in ''where clause''', 'rez edit get_disaster3'),
('2013-09-09 13:00:39', '/opt/lampp/htdocs/Vesuvius/mod/rez/xajax.inc', '681', 'rez_perform_save', '', 'rez_perform_save', 'Cannot add or update a child row: a foreign key constraint fails (`Sahana`.`rez_incident`, CONSTRAINT `rez_incident_ibfk_1` FOREIGN KEY (`incident_id`) REFERENCES `incident` (`incident_id`) ON DELETE CASCADE ON UPDATE CASCADE)', 'rez page_incident save'),
('2013-09-09 13:36:51', '/opt/lampp/htdocs/Vesuvius/mod/rez/xajax.inc', '751', 'rez_template_save', '', 'rez_template_save', 'Duplicate entry ''1-2'' for key ''PRIMARY''', 'rez template_disaster save'),
('2013-09-09 13:37:00', '/opt/lampp/htdocs/Vesuvius/mod/rez/xajax.inc', '751', 'rez_template_save', '', 'rez_template_save', 'Duplicate entry ''1-2'' for key ''PRIMARY''', 'rez template_disaster save'),
('2013-09-09 13:37:06', '/opt/lampp/htdocs/Vesuvius/mod/rez/xajax.inc', '751', 'rez_template_save', '', 'rez_template_save', 'Duplicate entry ''1-2'' for key ''PRIMARY''', 'rez template_disaster save'),
('2013-09-09 13:37:23', '/opt/lampp/htdocs/Vesuvius/mod/rez/xajax.inc', '751', 'rez_template_save', '', 'rez_template_save', 'Duplicate entry ''1-2'' for key ''PRIMARY''', 'rez template_disaster save'),
('2013-09-09 13:47:52', '/opt/lampp/htdocs/Vesuvius/mod/rez/xajax.inc', '751', 'rez_template_save', '', 'rez_template_save', 'Duplicate entry ''1-2'' for key ''PRIMARY''', 'rez template_disaster save'),
('2013-09-09 13:47:52', '/opt/lampp/htdocs/Vesuvius/mod/rez/xajax.inc', '760', 'rez_template_save', '', 'rez_template_save', 'Unknown column ''rez_template_id'' in ''where clause''', 'rez template_disaster save'),
('2013-09-09 13:51:19', '/opt/lampp/htdocs/Vesuvius/mod/rez/xajax.inc', '751', 'rez_template_save', '', 'rez_template_save', 'Duplicate entry ''1-2'' for key ''PRIMARY''', 'rez template_disaster save'),
('2013-09-09 13:51:24', '/opt/lampp/htdocs/Vesuvius/mod/rez/xajax.inc', '751', 'rez_template_save', '', 'rez_template_save', 'Duplicate entry ''1-2'' for key ''PRIMARY''', 'rez template_disaster save'),
('2013-09-09 13:51:46', '/opt/lampp/htdocs/Vesuvius/mod/rez/xajax.inc', '751', 'rez_template_save', '', 'rez_template_save', 'Duplicate entry ''1-2'' for key ''PRIMARY''', 'rez template_disaster save'),
('2013-09-09 13:51:55', '/opt/lampp/htdocs/Vesuvius/mod/rez/xajax.inc', '751', 'rez_template_save', '', 'rez_template_save', 'Duplicate entry ''1-2'' for key ''PRIMARY''', 'rez template_disaster save'),
('2013-09-09 13:54:04', '/opt/lampp/htdocs/Vesuvius/mod/rez/xajax.inc', '762', 'rez_template_save', '', 'rez_template_save', 'Duplicate entry ''1-2'' for key ''PRIMARY''', 'rez template_disaster save'),
('2013-09-09 14:19:18', '/opt/lampp/htdocs/Vesuvius/mod/rez/xajax.inc', '762', 'rez_template_save', '', 'rez_template_save', 'Duplicate entry ''1-2'' for key ''PRIMARY''', 'rez template_disaster save'),
('2013-09-09 14:19:32', '/opt/lampp/htdocs/Vesuvius/mod/rez/xajax.inc', '762', 'rez_template_save', '', 'rez_template_save', 'Duplicate entry ''3-13'' for key ''PRIMARY''', 'rez template_disaster save'),
('2013-09-09 14:19:40', '/opt/lampp/htdocs/Vesuvius/mod/rez/xajax.inc', '762', 'rez_template_save', '', 'rez_template_save', 'Duplicate entry ''3-13'' for key ''PRIMARY''', 'rez template_disaster save'),
('2013-09-09 14:19:50', '/opt/lampp/htdocs/Vesuvius/mod/rez/xajax.inc', '762', 'rez_template_save', '', 'rez_template_save', 'Duplicate entry ''3-13'' for key ''PRIMARY''', 'rez template_disaster save'),
('2013-09-09 14:20:05', '/opt/lampp/htdocs/Vesuvius/mod/rez/xajax.inc', '762', 'rez_template_save', '', 'rez_template_save', 'Duplicate entry ''1-2'' for key ''PRIMARY''', 'rez template_disaster save'),
('2013-09-09 14:20:39', '/opt/lampp/htdocs/Vesuvius/mod/rez/xajax.inc', '762', 'rez_template_save', '', 'rez_template_save', 'Duplicate entry ''1-2'' for key ''PRIMARY''', 'rez template_disaster save'),
('2013-09-09 14:59:47', '/opt/lampp/htdocs/Vesuvius/mod/rez/xajax.inc', '681', 'rez_perform_save', '', 'rez_perform_save', 'Cannot add or update a child row: a foreign key constraint fails (`Sahana`.`rez_incident`, CONSTRAINT `rez_incident_ibfk_1` FOREIGN KEY (`incident_id`) REFERENCES `incident` (`incident_id`) ON DELETE CASCADE ON UPDATE CASCADE)', 'rez page_incident save'),
('2013-09-09 15:04:36', '/opt/lampp/htdocs/Vesuvius/mod/rez/xajax.inc', '762', 'rez_template_save', '', 'rez_template_save', 'Duplicate entry ''1-2'' for key ''PRIMARY''', 'rez template_disaster save'),
('2013-09-09 15:05:42', '/opt/lampp/htdocs/Vesuvius/mod/rez/xajax.inc', '762', 'rez_template_save', '', 'rez_template_save', 'Duplicate entry ''1-2'' for key ''PRIMARY''', 'rez template_disaster save'),
('2013-09-11 13:17:40', '/opt/lampp/htdocs/Vesuvius/mod/rez/xajax.inc', '762', 'rez_template_save', '', 'rez_template_save', 'Duplicate entry ''1-2'' for key ''PRIMARY''', 'rez template_disaster save'),
('2013-09-11 13:26:52', '/opt/lampp/htdocs/Vesuvius/mod/rez/xajax.inc', '595', 'rez_perform_new', '', 'rez_perform_new', 'Unknown column ''disaster_category_id'' in ''field list''', 'rez new 2'),
('2013-09-11 13:26:52', '/opt/lampp/htdocs/Vesuvius/mod/rez/xajax.inc', '880', 'rez_perform_edit', '', 'rez_perform_edit', 'You have an error in your SQL syntax; check the manual that corresponds to your MySQL server version for the right syntax to use near '''' at line 3', 'generate categry 1'),
('2013-09-11 13:29:11', '/opt/lampp/htdocs/Vesuvius/mod/rez/xajax.inc', '595', 'rez_perform_new', '', 'rez_perform_new', 'Unknown column ''disaster_category_id'' in ''field list''', 'rez new 2'),
('2013-09-11 13:29:11', '/opt/lampp/htdocs/Vesuvius/mod/rez/xajax.inc', '880', 'rez_perform_edit', '', 'rez_perform_edit', 'You have an error in your SQL syntax; check the manual that corresponds to your MySQL server version for the right syntax to use near '''' at line 3', 'generate categry 1'),
('2013-09-11 13:31:33', '/opt/lampp/htdocs/Vesuvius/mod/rez/xajax.inc', '595', 'rez_perform_new', '', 'rez_perform_new', 'Unknown column ''disaster_category_id'' in ''field list''', 'rez new 2'),
('2013-09-11 13:31:33', '/opt/lampp/htdocs/Vesuvius/mod/rez/xajax.inc', '879', 'rez_perform_edit', '', 'rez_perform_edit', 'You have an error in your SQL syntax; check the manual that corresponds to your MySQL server version for the right syntax to use near '''' at line 3', 'generate categry 1'),
('2013-09-11 13:31:33', '/opt/lampp/htdocs/Vesuvius/mod/rez/render_fields.inc', '85', 'show_template_list', '', 'show_template_list', 'You have an error in your SQL syntax; check the manual that corresponds to your MySQL server version for the right syntax to use near '''' at line 3', 'rez show_event list'),
('2013-09-11 13:39:35', '/opt/lampp/htdocs/Vesuvius/mod/rez/xajax.inc', '595', 'rez_perform_new', '', 'rez_perform_new', 'Unknown column ''disaster_category_id'' in ''field list''', 'rez new 2'),
('2013-09-11 13:39:35', '/opt/lampp/htdocs/Vesuvius/mod/rez/xajax.inc', '879', 'rez_perform_edit', '', 'rez_perform_edit', 'You have an error in your SQL syntax; check the manual that corresponds to your MySQL server version for the right syntax to use near '''' at line 3', 'generate categry 1'),
('2013-09-11 13:45:18', '/opt/lampp/htdocs/Vesuvius/mod/rez/xajax.inc', '595', 'rez_perform_new', '', 'rez_perform_new', 'Unknown column ''disaster_category_id'' in ''field list''', 'rez new 2'),
('2013-09-11 13:45:18', '/opt/lampp/htdocs/Vesuvius/mod/rez/xajax.inc', '603', 'rez_perform_new', '', 'rez_perform_new', 'Cannot add or update a child row: a foreign key constraint fails (`Sahana`.`rez_template_scenario`, CONSTRAINT `rez_template_scenario_ibfk_3` FOREIGN KEY (`template_id`) REFERENCES `rez_page_template` (`rez_template_id`) ON DELETE CASCADE ON UPDATE CASCADE)', 'rez template_disaster save'),
('2013-09-11 13:45:18', '/opt/lampp/htdocs/Vesuvius/mod/rez/xajax.inc', '888', 'rez_perform_edit', '', 'rez_perform_edit', 'You have an error in your SQL syntax; check the manual that corresponds to your MySQL server version for the right syntax to use near '''' at line 3', 'generate categry 1'),
('2013-09-11 13:45:45', '/opt/lampp/htdocs/Vesuvius/mod/rez/xajax.inc', '595', 'rez_perform_new', '', 'rez_perform_new', 'Unknown column ''disaster_category_id'' in ''field list''', 'rez new 2'),
('2013-09-11 13:45:45', '/opt/lampp/htdocs/Vesuvius/mod/rez/xajax.inc', '603', 'rez_perform_new', '', 'rez_perform_new', 'Cannot add or update a child row: a foreign key constraint fails (`Sahana`.`rez_template_scenario`, CONSTRAINT `rez_template_scenario_ibfk_3` FOREIGN KEY (`template_id`) REFERENCES `rez_page_template` (`rez_template_id`) ON DELETE CASCADE ON UPDATE CASCADE)', 'rez template_disaster save'),
('2013-09-11 13:45:45', '/opt/lampp/htdocs/Vesuvius/mod/rez/xajax.inc', '888', 'rez_perform_edit', '', 'rez_perform_edit', 'You have an error in your SQL syntax; check the manual that corresponds to your MySQL server version for the right syntax to use near '''' at line 3', 'generate categry 1'),
('2013-09-11 19:42:09', '/opt/lampp/htdocs/Vesuvius/mod/rez/xajax.inc', '684', 'rez_perform_save', '', 'rez_perform_save', 'Duplicate entry ''11-6'' for key ''PRIMARY''', 'rez page_incident save'),
('2013-09-11 19:42:20', '/opt/lampp/htdocs/Vesuvius/mod/rez/xajax.inc', '684', 'rez_perform_save', '', 'rez_perform_save', 'Duplicate entry ''11-6'' for key ''PRIMARY''', 'rez page_incident save'),
('2013-09-11 19:42:47', '/opt/lampp/htdocs/Vesuvius/mod/rez/xajax.inc', '684', 'rez_perform_save', '', 'rez_perform_save', 'Duplicate entry ''11-1'' for key ''PRIMARY''', 'rez page_incident save'),
('2013-09-12 08:23:16', '/opt/lampp/htdocs/Vesuvius/mod/rez/xajax.inc', '695', 'rez_perform_save', '', 'rez_perform_save', 'Cannot add or update a child row: a foreign key constraint fails (`Sahana`.`rez_incident`, CONSTRAINT `rez_incident_ibfk_1` FOREIGN KEY (`incident_id`) REFERENCES `incident` (`incident_id`) ON DELETE CASCADE ON UPDATE CASCADE)', 'rez page_incident save'),
('2013-09-12 12:07:48', '/opt/lampp/htdocs/Vesuvius/mod/rez/xajax.inc', '695', 'rez_perform_save', '', 'rez_perform_save', 'Cannot add or update a child row: a foreign key constraint fails (`Sahana`.`rez_incident`, CONSTRAINT `rez_incident_ibfk_1` FOREIGN KEY (`incident_id`) REFERENCES `incident` (`incident_id`) ON DELETE CASCADE ON UPDATE CASCADE)', 'rez page_incident save'),
('2013-09-12 13:59:45', '/opt/lampp/htdocs/Vesuvius/mod/rez/xajax.inc', '689', 'rez_perform_save', '', 'rez_perform_save', 'You have an error in your SQL syntax; check the manual that corresponds to your MySQL server version for the right syntax to use near ''en_US'''' at line 2', 'rez save');
INSERT INTO `dao_error_log` (`time`, `file`, `line`, `method`, `class`, `function`, `error_message`, `other`) VALUES
('2013-09-12 14:49:03', '/opt/lampp/htdocs/Vesuvius/mod/rez/xajax.inc', '815', 'rez_template_save', '', 'rez_template_save', 'You have an error in your SQL syntax; check the manual that corresponds to your MySQL server version for the right syntax to use near '''' at line 2', 'rez template_disaster save 2'),
('2013-09-12 14:49:18', '/opt/lampp/htdocs/Vesuvius/mod/rez/xajax.inc', '815', 'rez_template_save', '', 'rez_template_save', 'You have an error in your SQL syntax; check the manual that corresponds to your MySQL server version for the right syntax to use near '''' at line 2', 'rez template_disaster save 2'),
('2013-09-12 15:13:55', '/opt/lampp/htdocs/Vesuvius/mod/rez/xajax.inc', '815', 'rez_template_save', '', 'rez_template_save', 'You have an error in your SQL syntax; check the manual that corresponds to your MySQL server version for the right syntax to use near '''' at line 2', 'rez template_disaster save 2'),
('2013-09-12 15:14:38', '/opt/lampp/htdocs/Vesuvius/mod/rez/xajax.inc', '815', 'rez_template_save', '', 'rez_template_save', 'You have an error in your SQL syntax; check the manual that corresponds to your MySQL server version for the right syntax to use near '''' at line 2', 'rez template_disaster save 2'),
('2013-09-12 15:20:23', '/opt/lampp/htdocs/Vesuvius/mod/rez/xajax.inc', '831', 'rez_template_save', '', 'rez_template_save', 'Unknown column ''disaster_category_id'' in ''field list''', 'rez new 2'),
('2013-09-16 11:55:01', '/opt/lampp/htdocs/Vesuvius/mod/rez/xajax.inc', '831', 'rez_template_save', '', 'rez_template_save', 'Unknown column ''disaster_category_id'' in ''field list''', 'rez new 2'),
('2013-09-16 17:46:00', '/opt/lampp/htdocs/Vesuvius/mod/rez/xajax.inc', '343', 'rez_show_templates', '', 'rez_show_templates', 'You have an error in your SQL syntax; check the manual that corresponds to your MySQL server version for the right syntax to use near '''' at line 3', 'getting disaster categories');

-- --------------------------------------------------------

--
-- Table structure for table `disaster_category`
--

CREATE TABLE IF NOT EXISTS `disaster_category` (
  `category_id` int(11) NOT NULL AUTO_INCREMENT,
  `name` varchar(100) NOT NULL,
  PRIMARY KEY (`category_id`)
) ENGINE=InnoDB  DEFAULT CHARSET=latin1 COMMENT='Stores Disaster Category' AUTO_INCREMENT=23 ;

--
-- Dumping data for table `disaster_category`
--

INSERT INTO `disaster_category` (`category_id`, `name`) VALUES
(0, 'Default'),
(1, 'Fire'),
(2, 'Flood'),
(3, 'Earthquake'),
(4, 'Drought'),
(5, 'Chemical Emergency'),
(6, 'Flu'),
(7, 'Hurricane'),
(8, 'Landslide'),
(9, 'Tornado'),
(10, 'Tsunami'),
(11, 'Volcano'),
(12, 'ThunderStorm'),
(13, 'WildFire'),
(14, 'Winter Storm'),
(15, 'Terrorism'),
(16, 'Radiation'),
(17, 'Pet care'),
(18, 'Crashes'),
(19, 'HighWay Safety'),
(20, 'Food Safety'),
(21, 'Dxemo Category'),
(22, 'New categ');

-- --------------------------------------------------------

--
-- Table structure for table `edxl_co_header`
--

CREATE TABLE IF NOT EXISTS `edxl_co_header` (
  `de_id` int(11) NOT NULL,
  `co_id` int(11) NOT NULL,
  `p_uuid` varchar(128) DEFAULT NULL COMMENT 'ties the contentObject to a person',
  `type` enum('lpf','tep','pix') DEFAULT NULL COMMENT 'defines the type of the contentObject',
  `content_descr` varchar(255) DEFAULT NULL COMMENT 'Content description',
  `incident_id` varchar(255) DEFAULT NULL,
  `incident_descr` varchar(255) DEFAULT NULL COMMENT 'Incident description',
  `confidentiality` varchar(255) DEFAULT NULL,
  PRIMARY KEY (`co_id`),
  KEY `de_id` (`de_id`),
  KEY `p_uuid_idx` (`p_uuid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- --------------------------------------------------------

--
-- Table structure for table `edxl_co_header_seq`
--

CREATE TABLE IF NOT EXISTS `edxl_co_header_seq` (
  `id` bigint(20) NOT NULL AUTO_INCREMENT COMMENT 'stores next id in sequence for the edxl_co_header table',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB  DEFAULT CHARSET=utf8 AUTO_INCREMENT=2 ;

--
-- Dumping data for table `edxl_co_header_seq`
--

INSERT INTO `edxl_co_header_seq` (`id`) VALUES
(1);

-- --------------------------------------------------------

--
-- Table structure for table `edxl_co_keywords`
--

CREATE TABLE IF NOT EXISTS `edxl_co_keywords` (
  `co_id` int(11) NOT NULL,
  `keyword_num` int(11) NOT NULL,
  `keyword_urn` varchar(255) NOT NULL,
  `keyword` varchar(255) NOT NULL,
  PRIMARY KEY (`co_id`,`keyword_num`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- --------------------------------------------------------

--
-- Table structure for table `edxl_co_lpf`
--

CREATE TABLE IF NOT EXISTS `edxl_co_lpf` (
  `co_id` int(11) NOT NULL,
  `p_uuid` varchar(255) NOT NULL COMMENT 'Sahana person ID',
  `schema_version` varchar(255) NOT NULL,
  `login_machine` varchar(255) NOT NULL,
  `login_account` varchar(255) NOT NULL,
  `person_id` varchar(255) NOT NULL COMMENT 'Mass casualty patient ID',
  `event_name` varchar(255) NOT NULL,
  `event_long_name` varchar(255) NOT NULL,
  `org_name` varchar(255) NOT NULL,
  `org_id` varchar(255) NOT NULL,
  `last_name` varchar(255) DEFAULT NULL,
  `first_name` varchar(255) DEFAULT NULL,
  `gender` enum('M','F','U','C') NOT NULL,
  `peds` tinyint(1) NOT NULL,
  `triage_category` enum('Green','BH Green','Yellow','Red','Gray','Black') NOT NULL,
  PRIMARY KEY (`co_id`,`p_uuid`),
  KEY `p_uuid` (`p_uuid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='LPF is an example of an "other xml" content object, e.g., ot';

-- --------------------------------------------------------

--
-- Table structure for table `edxl_co_photos`
--

CREATE TABLE IF NOT EXISTS `edxl_co_photos` (
  `co_id` int(11) NOT NULL,
  `p_uuid` varchar(255) NOT NULL COMMENT 'Sahana person ID',
  `mimeType` varchar(255) NOT NULL COMMENT 'As in ''image/jpeg''',
  `uri` varchar(255) NOT NULL COMMENT 'Photo filename = Mass casualty patient ID + zone + ''s#'' if secondary + optional caption after hypen',
  `contentData` mediumtext CHARACTER SET ascii NOT NULL COMMENT 'Base-64 encoded image',
  `image_id` int(20) DEFAULT NULL COMMENT 'reference to the image.image_id field',
  `sha1` varchar(40) DEFAULT NULL COMMENT 'sha1 calculated hash of the image',
  PRIMARY KEY (`co_id`,`p_uuid`),
  KEY `p_uuid` (`p_uuid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='LPF is an example of an "other xml" content object, e.g., ot';

-- --------------------------------------------------------

--
-- Table structure for table `edxl_co_roles`
--

CREATE TABLE IF NOT EXISTS `edxl_co_roles` (
  `co_id` int(11) NOT NULL,
  `role_num` int(11) NOT NULL DEFAULT '0',
  `of_originator` tinyint(1) NOT NULL COMMENT '0 = false = of consumer',
  `role_urn` varchar(255) NOT NULL,
  `role` varchar(255) NOT NULL,
  PRIMARY KEY (`co_id`,`role_num`),
  KEY `role_num` (`role_num`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- --------------------------------------------------------

--
-- Table structure for table `edxl_de_header`
--

CREATE TABLE IF NOT EXISTS `edxl_de_header` (
  `de_id` int(11) NOT NULL,
  `when_sent` datetime NOT NULL,
  `sender_id` varchar(255) NOT NULL COMMENT 'Email, phone num, etc.  Not always URI, URN, URL',
  `distr_id` varchar(255) NOT NULL COMMENT 'Distribution ID.  Sender may or may not choose to vary.',
  `distr_status` enum('Actual','Exercise','System','Test') NOT NULL,
  `distr_type` enum('Ack','Cancel','Dispatch','Error','Report','Request','Response','Update') NOT NULL COMMENT 'Not included: types for sensor grids',
  `combined_conf` varchar(255) NOT NULL COMMENT 'Combined confidentiality of all content objects',
  `language` varchar(255) DEFAULT NULL,
  `when_here` datetime NOT NULL COMMENT 'Received or sent from here.  [local]',
  `inbound` tinyint(1) NOT NULL DEFAULT '1' COMMENT 'BOOLEAN [local]',
  PRIMARY KEY (`de_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='Overall message base, defined by EDXL Distribution Element';

-- --------------------------------------------------------

--
-- Table structure for table `edxl_de_header_seq`
--

CREATE TABLE IF NOT EXISTS `edxl_de_header_seq` (
  `id` bigint(20) NOT NULL AUTO_INCREMENT COMMENT 'stores next id in sequence for the edxl_de_header table',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB  DEFAULT CHARSET=utf8 AUTO_INCREMENT=2 ;

--
-- Dumping data for table `edxl_de_header_seq`
--

INSERT INTO `edxl_de_header_seq` (`id`) VALUES
(1);

-- --------------------------------------------------------

--
-- Table structure for table `edxl_de_keywords`
--

CREATE TABLE IF NOT EXISTS `edxl_de_keywords` (
  `de_id` int(11) NOT NULL,
  `keyword_num` int(11) NOT NULL DEFAULT '0',
  `keyword_urn` varchar(255) NOT NULL,
  `keyword` varchar(255) CHARACTER SET latin1 NOT NULL,
  PRIMARY KEY (`de_id`,`keyword_num`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- --------------------------------------------------------

--
-- Table structure for table `edxl_de_prior_messages`
--

CREATE TABLE IF NOT EXISTS `edxl_de_prior_messages` (
  `de_id` int(11) NOT NULL,
  `prior_msg_num` int(11) NOT NULL DEFAULT '0',
  `when_sent` datetime NOT NULL COMMENT 'external time',
  `sender_id` varchar(255) NOT NULL COMMENT 'external ID',
  `distr_id` varchar(255) NOT NULL COMMENT 'external distribution ID',
  PRIMARY KEY (`de_id`,`prior_msg_num`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- --------------------------------------------------------

--
-- Table structure for table `edxl_de_roles`
--

CREATE TABLE IF NOT EXISTS `edxl_de_roles` (
  `de_id` int(11) NOT NULL,
  `role_num` int(11) NOT NULL DEFAULT '0',
  `of_sender` tinyint(1) NOT NULL,
  `role_urn` varchar(255) NOT NULL,
  `role` varchar(255) NOT NULL,
  PRIMARY KEY (`de_id`,`role_num`),
  KEY `role_idx` (`role_num`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- --------------------------------------------------------

--
-- Table structure for table `edxl_de_target_addresses`
--

CREATE TABLE IF NOT EXISTS `edxl_de_target_addresses` (
  `de_id` int(11) NOT NULL,
  `address_num` int(11) NOT NULL DEFAULT '0',
  `scheme` varchar(255) NOT NULL COMMENT 'Like "e-mail"',
  `value` varchar(255) NOT NULL,
  PRIMARY KEY (`de_id`,`address_num`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- --------------------------------------------------------

--
-- Table structure for table `edxl_de_target_circles`
--

CREATE TABLE IF NOT EXISTS `edxl_de_target_circles` (
  `de_id` int(11) NOT NULL,
  `circle_num` int(11) NOT NULL DEFAULT '0',
  `latitude` float NOT NULL,
  `longitude` float NOT NULL,
  `radius_km` float NOT NULL,
  PRIMARY KEY (`de_id`,`circle_num`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- --------------------------------------------------------

--
-- Table structure for table `edxl_de_target_codes`
--

CREATE TABLE IF NOT EXISTS `edxl_de_target_codes` (
  `de_id` int(11) NOT NULL,
  `codes_num` int(11) NOT NULL DEFAULT '0',
  `code_type` enum('country','subdivision','locCodeUN') DEFAULT NULL COMMENT 'Respectively (1) ISO 3166-1 2-letter country code (2) ISO 3166-2 code: country + "-" + per-country 2-3 char code like state, e.g., "US-MD". (3) UN transport hub code: country + "-" + 2-3 char code (cap ASCII or 2-9), e.g., "US-BWI"',
  `code` varchar(6) DEFAULT NULL COMMENT 'See format examples for code_type field',
  PRIMARY KEY (`de_id`,`codes_num`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- --------------------------------------------------------

--
-- Table structure for table `edxl_de_target_polygons`
--

CREATE TABLE IF NOT EXISTS `edxl_de_target_polygons` (
  `de_id` int(11) NOT NULL,
  `poly_num` int(11) NOT NULL DEFAULT '0',
  `point_num` int(11) NOT NULL DEFAULT '0' COMMENT 'Point within this polygon',
  `latitude` float NOT NULL,
  `longitude` float NOT NULL,
  PRIMARY KEY (`de_id`,`poly_num`,`point_num`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- --------------------------------------------------------

--
-- Table structure for table `expiry_queue`
--

CREATE TABLE IF NOT EXISTS `expiry_queue` (
  `index` int(20) NOT NULL AUTO_INCREMENT,
  `p_uuid` varchar(128) DEFAULT NULL,
  `requested_by_user_id` int(16) DEFAULT NULL,
  `requested_when` timestamp NULL DEFAULT CURRENT_TIMESTAMP,
  `requested_why` text COMMENT 'the reason (optional) why a expiration/deletion was requested',
  `queued` tinyint(1) DEFAULT NULL COMMENT 'true when an expiration is requested',
  `approved_by_user_id` int(16) DEFAULT NULL,
  `approved_when` timestamp NULL DEFAULT NULL,
  `approved_why` text COMMENT 'the reason why an approval was accepted or rejected',
  `expired` tinyint(1) DEFAULT NULL COMMENT 'true when a expiration is approved',
  PRIMARY KEY (`index`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='person expiry request management queue and related informati' AUTO_INCREMENT=1 ;

-- --------------------------------------------------------

--
-- Table structure for table `field_options`
--

CREATE TABLE IF NOT EXISTS `field_options` (
  `field_name` varchar(100) DEFAULT NULL,
  `option_code` varchar(10) DEFAULT NULL,
  `option_description` varchar(50) DEFAULT NULL,
  `display_order` int(8) DEFAULT NULL,
  KEY `option_code` (`option_code`),
  KEY `option_description` (`option_description`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

--
-- Dumping data for table `field_options`
--

INSERT INTO `field_options` (`field_name`, `option_code`, `option_description`, `display_order`) VALUES
('opt_status', 'ali', 'Alive & Well', NULL),
('opt_status', 'mis', 'Missing', NULL),
('opt_status', 'inj', 'Injured', NULL),
('opt_status', 'dec', 'Deceased', NULL),
('opt_gender', 'mal', 'Male', NULL),
('opt_gender', 'fml', 'Female', NULL),
('opt_contact_type', 'home', 'Home(permanent address)', NULL),
('opt_contact_type', 'name', 'Contact Person', NULL),
('opt_contact_type', 'pmob', 'Personal Mobile', NULL),
('opt_contact_type', 'curr', 'Current Phone', NULL),
('opt_contact_type', 'cmob', 'Current Mobile', NULL),
('opt_contact_type', 'email', 'Email address', NULL),
('opt_contact_type', 'fax', 'Fax Number', NULL),
('opt_contact_type', 'web', 'Website', NULL),
('opt_contact_type', 'inst', 'Instant Messenger', NULL),
('opt_eye_color', 'GRN', 'Green', NULL),
('opt_eye_color', 'GRY', 'Gray', NULL),
('opt_race', 'R1', 'American Indian or Alaska Native', NULL),
('opt_race', NULL, 'Unknown', NULL),
('opt_eye_color', 'BRO', 'Brown', NULL),
('opt_eye_color', 'BLU', 'Blue', NULL),
('opt_eye_color', 'BLK', 'Black', NULL),
('opt_skin_color', 'DRK', 'Dark', NULL),
('opt_skin_color', 'BLK', 'Black', NULL),
('opt_skin_color', 'ALB', 'Albino', NULL),
('opt_hair_color', 'BLN', 'Blond or Strawberry', NULL),
('opt_hair_color', 'BLK', 'Black', NULL),
('opt_hair_color', 'BLD', 'Bald', NULL),
('opt_location_type', '2', 'Town or Neighborhood', NULL),
('opt_location_type', '1', 'County or Equivalent', NULL),
('opt_contact_type', 'zip', 'Zip Code', NULL),
('opt_eye_color', NULL, 'Unknown', NULL),
('opt_eye_color', 'HAZ', 'Hazel', NULL),
('opt_eye_color', 'MAR', 'Maroon', NULL),
('opt_eye_color', 'MUL', 'Multicolored', NULL),
('opt_eye_color', 'PNK', 'Pink', NULL),
('opt_skin_color', 'DBR', 'Dark Brown', NULL),
('opt_skin_color', 'FAR', 'Fair', NULL),
('opt_skin_color', 'LGT', 'Light', NULL),
('opt_skin_color', 'LBR', 'Light Brown', NULL),
('opt_skin_color', 'MED', 'Medium', NULL),
('opt_skin_color', NULL, 'Unknown', NULL),
('opt_skin_color', 'OLV', 'Olive', NULL),
('opt_skin_color', 'RUD', 'Ruddy', NULL),
('opt_skin_color', 'SAL', 'Sallow', NULL),
('opt_skin_color', 'YEL', 'Yellow', NULL),
('opt_hair_color', 'BLU', 'Blue', NULL),
('opt_hair_color', 'BRO', 'Brown', NULL),
('opt_hair_color', 'GRY', 'Gray', NULL),
('opt_hair_color', 'GRN', 'Green', NULL),
('opt_hair_color', 'ONG', 'Orange', NULL),
('opt_hair_color', 'PLE', 'Purple', NULL),
('opt_hair_color', 'PNK', 'Pink', NULL),
('opt_hair_color', 'RED', 'Red or Auburn', NULL),
('opt_hair_color', 'SDY', 'Sandy', NULL),
('opt_hair_color', 'WHI', 'White', NULL),
('opt_race', 'R2', 'Asian', NULL),
('opt_race', 'R3', 'Black or African American', NULL),
('opt_race', 'R4', 'Native Hawaiian or Other Pacific Islander', NULL),
('opt_race', 'R5', 'White', NULL),
('opt_race', 'R9', 'Other Race', NULL),
('opt_religion', 'PEV', 'Protestant, Evangelical', 1),
('opt_religion', 'PML', 'Protestant, Mainline', 2),
('opt_religion', 'PHB', 'Protestant, Historically Black', 3),
('opt_religion', 'CAT', 'Catholic', 4),
('opt_religion', 'MOM', 'Mormon', 5),
('opt_religion', 'JWN', 'Jehovah''s Witness', 6),
('opt_religion', 'ORT', 'Orthodox', 7),
('opt_religion', 'COT', 'Other Christian', 8),
('opt_religion', 'JEW', 'Jewish', 9),
('opt_religion', 'BUD', 'Buddhist', 10),
('opt_religion', 'HIN', 'Hindu', 11),
('opt_religion', 'MOS', 'Muslim', 12),
('opt_religion', 'OTH', 'Other Faiths', 13),
('opt_religion', 'NOE', 'Unaffiliated', 14),
('opt_religion', NULL, 'Unknown', 15),
('opt_hair_color', NULL, 'Unknown', NULL),
('opt_skin_color', 'MBR', 'Medium Brown', NULL),
('opt_gender', NULL, 'Unknown', NULL),
('opt_gender', 'cpx', 'Complex', NULL),
('opt_status', 'unk', 'Unknown', NULL),
('opt_status', 'fnd', 'Found', NULL),
('opt_status_color', 'fnd', '#7E2217', NULL),
('opt_status_color', 'ali', '#167D21', NULL),
('opt_status_color', 'inj', '#FF0000', NULL),
('opt_status_color', 'unk', '#808080', NULL),
('opt_status_color', 'dec', '#000000', NULL),
('opt_status_color', 'mis', '#0000FF', NULL);

-- --------------------------------------------------------

--
-- Table structure for table `hospital`
--

CREATE TABLE IF NOT EXISTS `hospital` (
  `hospital_uuid` int(32) NOT NULL AUTO_INCREMENT,
  `name` varchar(60) NOT NULL DEFAULT 'enter name here',
  `short_name` varchar(30) NOT NULL DEFAULT 'shortname',
  `street1` varchar(120) DEFAULT NULL,
  `street2` varchar(120) DEFAULT NULL,
  `city` varchar(60) DEFAULT NULL,
  `county` varchar(60) DEFAULT NULL,
  `region` varchar(60) DEFAULT NULL,
  `postal_code` varchar(16) DEFAULT NULL,
  `country` varchar(32) DEFAULT NULL,
  `latitude` double NOT NULL DEFAULT '38.995523',
  `longitude` double NOT NULL DEFAULT '-77.096597',
  `phone` varchar(16) DEFAULT NULL,
  `fax` varchar(16) DEFAULT NULL,
  `email` varchar(64) DEFAULT NULL,
  `www` varchar(256) DEFAULT NULL,
  `npi` varchar(32) DEFAULT NULL,
  `patient_id_prefix` varchar(32) DEFAULT NULL,
  `patient_id_suffix_variable` tinyint(1) NOT NULL DEFAULT '1',
  `patient_id_suffix_fixed_length` int(11) NOT NULL DEFAULT '0',
  `creation_time` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `icon_url` varchar(128) DEFAULT NULL,
  `legalese` text COMMENT 'legalese',
  `legaleseAnon` text COMMENT 'anonymized legalese',
  `legaleseTimestamp` timestamp NULL DEFAULT NULL COMMENT 'when legalese was last updated',
  `legaleseAnonTimestamp` timestamp NULL DEFAULT NULL COMMENT 'when legaleseAnon was last updated',
  `photo_required` tinyint(1) NOT NULL DEFAULT '1' COMMENT 'Whether a photo is requred for incoming patients.',
  `honor_no_photo_request` tinyint(1) NOT NULL DEFAULT '1' COMMENT 'Whether to honor requests no to save patient images.',
  `photographer_name_required` tinyint(1) NOT NULL DEFAULT '0' COMMENT 'Whether to require if a photographer include his/her name.',
  PRIMARY KEY (`hospital_uuid`)
) ENGINE=InnoDB  DEFAULT CHARSET=utf8 AUTO_INCREMENT=11 ;

--
-- Dumping data for table `hospital`
--

INSERT INTO `hospital` (`hospital_uuid`, `name`, `short_name`, `street1`, `street2`, `city`, `county`, `region`, `postal_code`, `country`, `latitude`, `longitude`, `phone`, `fax`, `email`, `www`, `npi`, `patient_id_prefix`, `patient_id_suffix_variable`, `patient_id_suffix_fixed_length`, `creation_time`, `icon_url`, `legalese`, `legaleseAnon`, `legaleseTimestamp`, `legaleseAnonTimestamp`, `photo_required`, `honor_no_photo_request`, `photographer_name_required`) VALUES
(1, 'Suburban Hospital', 'Suburban', '8600 Old Georgetown Road', '', 'Bethesda', 'Montgomery', 'MD', '20814-1497', 'USA', 43.4420293, -71.300549, '301-896-3118', '', 'info@suburbanhospital.org', 'www.suburbanhospital.org', '1205896446', '911-', 0, 5, '2010-01-01 06:01:01', 'theme/lpf3/img/suburban.png', '[This document is a straw man example, based on a reworking of the HIPAA policy statement at www.hhs.gov/hipaafaq/providers/hipaa-1068.html.  It has not been reviewed by legal council, nor reviewed or approved by Suburban Hospital or any other BHEPP member.]\n\n[The following is an example statement for attachment to a full record generated by "TriagePic".  Partial records (e.g., de-identified) have different statements.]\n\nNotice of Privacy Practices and Information Distribution\n========================================================\nSuburban Hospital is covered by HIPAA, so the provided disaster victim information (the "record", which may include a photo) is governed by provisions of the HIPAA Privacy Rule.\n\nDuring a disaster, HIPPA permits disclosures for treatment purposes, and certain disclosures to disaster relief organizations (like the American Red Cross) so that family members can be notified of the patient''s location.  (See CFR 164.510(b)(4).\n\nThe primary of purpose of the record is for family reunification.  Secondary usages may include in-hospital patient tracking, treatment/continuity-of-care on patient transfer, disaster situational awareness and resource management, and feedback to emergency medical service providers who provide pre-hospital treatment.  The record (in various forms) will be distributed within Suburban Hospital, and to and within the institutions with which Suburban Hospital partners through the Bethesda Hospital Emergency Preparedness Partnership.  These are the NIH Clinical Center, Walter Reed National Military Medical Hospital, and National Library of Medicine.  In particular, the record is sent to NLM''s Lost Person Finder database for exposure through the web site, with appropriate filtering and verification.  HIPAA allows patients to be listed in the facility directory, and some aspects of Lost Person Finder are analogous to that.  For more, see the Notice of Privacy Practices associated with the LPF web site.  \n\nThe record was generated by a "TriagePic" application, operated by Suburban Hospital personnel.  The application includes the ability to express the following [TO DO]:\n\n- patient agrees to let hospital personnel speak with family members or friends involved in the patient''s care (45 CFR 164.510(b));\n- patient wishes to opt out of the facility directory (45 CFR 164.510(a));  [THIS MIGHT BE INTERPRETED AS OPTING OUT OF LPF.  MORE TO UNDERSTAND.]\n- patient requests privacy restrictions (45 CFR 164.522(a))\n- patient requests confidential communications (45 CFR 164.522(b))\n\n[IMPLICATIONS OF THESE CHOICES ON RECORD GENERATION NOT YET KNOWN.]\n\nIn addition, there is a requirement to distribute a notice of privacy practices, addressed by this attachment and the LPF Notice of Privacy Practices.\n\nPenalties for non-compliance with the above five rules may be waived, for a limited time in a limited geographical area, if the President declares an emergency or disaster AND the Health and Human Services Secretary declares a public health emergency.  Within that declared timespan, a hospital may rely on the waiver (which covers all patients present) only from the moment of instituting its disaster protocol to at most 72 hours later.  For more, see www.hhs.gov/hipaafaq/providers/hipaa-1068.html.  The waiver is authorized under the Project Bioshield Act of 2004 (PL 108-276) and section 1135(b) of the Social Security Act.\n\nPhoto Copyright\n===============\nThe attached photo if any was taken by an employee or volunteer of Suburban Hospital and is copyright Suburban Hospital in the year given by the reporting date.  Reproduction and distribution is permitted to the extent governed by policy for the overall record.  Please credit Suburban Hospital and the employee(s)/volunteer(s) listed.', '[This document is a straw man example.  It has not been reviewed by legal council, nor reviewed or approved by Suburban Hospital or any other BHEPP member.]\n\n[The following is an example statement for attachment to an anonymized, de-identified record generated by "TriagePic".  Full records (e.g., patient-identified) may have different statements.]\n\nNotice of Privacy Practices and Information Distribution\n========================================================\nThe attached record was generated by a "TriagePic" application, operated by Suburban Hospital personnel at the point where disaster victims arrive at the hospital.  While the application does generate real-time patient-specific information (text and photo), the attached record has been anonymized/de-identified to remove patient identifiers (e.g., no photo), and is intended for public release.  It retains categorial information on gender and adult-vs.-child ("Peds"), as well as arrival time, and the hospital personnel involved.  It is intended to be useful for disaster situational awareness and rate-of-arrival purposes.\n\nNo system of de-identification that retains any useful information is perfect.  The recipient who uses this record in an effort to re-identify patients is responsible for any legal ramifications and potential violations of HIPAA or other laws pertaining to patient confidentiality.', '2011-10-04 16:50:51', '2011-10-04 16:50:51', 1, 1, 0),
(2, 'Walter Reed National Military Medical Center', 'WRNMMC', '8901 Rockville Pike', '', 'Bethesda', 'Montgomery', 'MD', '20889-0001', 'USA', 39.00204, -77.0945, '301-295-4611', '', '', 'www.bethesda.med.navy.mil', '1356317069', '', 1, 0, '2010-09-22 22:49:34', 'theme/lpf3/img/nnmc.png', '[This document is a straw man example, based on a reworking of the HIPAA policy statement at www.hhs.gov/hipaafaq/providers/hipaa-1068.html.  It has not been reviewed by legal council, nor reviewed or approved by Walter Reed National Naval Medical Center or any other BHEPP member.]\n\n[The following is an example statement for attachment to a full record generated by "TriagePic".  Partial records (e.g., de-identified) have different statements.]\n\nNotice of Privacy Practices and Information Distribution\n========================================================\nWalter Reed National Medical Medical Center is covered by HIPAA, so the provided disaster victim information (the "record", which may include a photo) is governed by provisions of the HIPAA Privacy Rule.\n\nDuring a disaster, HIPPA permits disclosures for treatment purposes, and certain disclosures to disaster relief organizations (like the American Red Cross) so that family members can be notified of the patient''s location.  (See CFR 164.510(b)(4).\n\nThe primary of purpose of the record is for family reunification.  Secondary usages may include in-hospital patient tracking, treatment/continuity-of-care on patient transfer, disaster situational awareness and resource management, and feedback to emergency medical service providers who provide pre-hospital treatment.  The record (in various forms) will be distributed within Walter Reed National Medical Medical Center, and to and within the institutions with which National Naval Medical Center partners through the Bethesda Hospital Emergency Preparedness Partnership.  These are the NIH Clinical Center, Walter Reed National Medical Medical Hospital, and National Library of Medicine.  In particular, the record is sent to NLM''s People Locator database for exposure through the web site, with appropriate filtering and verification.  HIPAA allows patients to be listed in the facility directory, and some aspects of People Locator are analogous to that.  For more, see the Notice of Privacy Practices associated with the PL web site.  \n\nThe record was generated by a "TriagePic" application, operated by Walter Reed National Military Medical Center personnel.  The application includes the ability to express the following [TO DO]:\n\n- patient agrees to let hospital personnel speak with family members or friends involved in the patient''s care (45 CFR 164.510(b));\n- patient wishes to opt out of the facility directory (45 CFR 164.510(a));  [THIS MIGHT BE INTERPRETED AS OPTING OUT OF PL.  MORE TO UNDERSTAND.]\n- patient requests privacy restrictions (45 CFR 164.522(a))\n- patient requests confidential communications (45 CFR 164.522(b))\n\n[IMPLICATIONS OF THESE CHOICES ON RECORD GENERATION NOT YET KNOWN.]\n\nIn addition, there is a requirement to distribute a notice of privacy practices, addressed by this attachment and the PL Notice of Privacy Practices.\n\nPenalties for non-compliance with the above five rules may be waived, for a limited time in a limited geographical area, if the President declares an emergency or disaster AND the Health and Human Services Secretary declares a public health emergency.  Within that declared timespan, a hospital may rely on the waiver (which covers all patients present) only from the moment of instituting its disaster protocol to at most 72 hours later.  For more, see www.hhs.gov/hipaafaq/providers/hipaa-1068.html.  The waiver is authorized under the Project Bioshield Act of 2004 (PL 108-276) and section 1135(b) of the Social Security Act.\n\nPhoto Copyright\n===============\nThe attached photo if any was taken by an employee or volunteer of Walter Reed National Military Medical Center and is copyright Walter Reed National Military Medical Center in the year given by the reporting date.  Reproduction and distribution is permitted to the extent governed by policy for the overall record.  Please credit Walter Reed National Military Medical Center and the employee(s)/volunteer(s) listed.', '[This document is a straw man example.  It has not been reviewed by legal council, nor reviewed or approved by Walter Reed National Military Medical Center or any other BHEPP member.]\n\n[The following is an example statement for attachment to an anonymized, de-identified record generated by "TriagePic".  Full records (e.g., patient-identified) may have different statements.]\n\nNotice of Privacy Practices and Information Distribution\n========================================================\nThe attached record was generated by a "TriagePic" application, operated by Walter Reed National Military Medical Center personnel at the point where disaster victims arrive at the hospital.  While the application does generate real-time patient-specific information (text and photo), the attached record has been anonymized/de-identified to remove patient identifiers (e.g., no photo), and is intended for public release.  It retains categorial information on gender and adult-vs.-child ("Peds"), as well as arrival time, and the hospital personnel involved.  It is intended to be useful for disaster situational awareness and rate-of-arrival purposes.\n\nNo system of de-identification that retains any useful information is perfect.  The recipient who uses this record in an effort to re-identify patients is responsible for any legal ramifications and potential violations of HIPAA or other laws pertaining to patient confidentiality.', '2011-10-04 16:52:15', '2011-10-04 16:52:15', 1, 1, 0),
(3, 'NLM (testing)', 'NLM', '9000 Rockville Pike', '', 'Bethesda', 'Montgomery', 'MD', '20892', 'USA', 38.995523, -77.096597, '', '', '', 'www.nlm.nih.gov', '1234567890', '911-', 1, -1, '2011-05-02 17:35:40', '', '[This document is a straw man example, based on a reworking of the HIPAA policy statement at www.hhs.gov/hipaafaq/providers/hipaa-1068.html.  It has not been reviewed by legal council, nor reviewed or approved by NLM or any other BHEPP member.]\n\n[The following is an example statement for attachment to a full record generated by "TriagePic".  Partial records (e.g., de-identified) have different statements.]\n\nNotice of Privacy Practices and Information Distribution\n========================================================\nRecords generated by NLM personnel during TriagePic testing do not generally represent real disaster victims.  Photos are most often of drill participants or NLM employees or contractors.\n\nFor any real disaster victim records gathered by a participating BHEPP hospital and provided to NLM:\n\nHospital records are generally covered by HIPAA, so the provided disaster victim information (the "record", which may include a photo) is governed by provisions of the HIPAA Privacy Rule.\n\nDuring a disaster, HIPPA permits disclosures for treatment purposes, and certain disclosures to disaster relief organizations (like the American Red Cross) so that family members can be notified of the patient''s location.  (See CFR 164.510(b)(4).\n\nThe primary of purpose of the record is for family reunification.  Secondary usages may include in-hospital patient tracking, treatment/continuity-of-care on patient transfer, disaster situational awareness and resource management, and feedback to emergency medical service providers who provide pre-hospital treatment.  The record (in various forms) will be distributed within the originating hospital, within NLM (acting as if a hospital while testing by developers), and to and within the institutions with which NLM partners through the Bethesda Hospital Emergency Preparedness Partnership.  These are the NIH Clinical Center, Walter Reed National Medical Medical Hospital, and Suburban Hospital.  In particular, the record is sent to NLM''s Person Locator database for exposure through the web site, with appropriate filtering and verification.  HIPAA allows patients to be listed in the facility directory, and some aspects of Person Locator are analogous to that.  For more, see the Notice of Privacy Practices associated with the PL web site.  \n\nThe record was generated by a "TriagePic" application, operated by BHEPP hospital personnel.  The application includes the ability to express the following [TO DO]:\n\n- patient agrees to let hospital personnel speak with family members or friends involved in the patient''s care (45 CFR 164.510(b));\n- patient wishes to opt out of the facility directory (45 CFR 164.510(a));  [THIS MIGHT BE INTERPRETED AS OPTING OUT OF LPF.  MORE TO UNDERSTAND.]\n- patient requests privacy restrictions (45 CFR 164.522(a))\n- patient requests confidential communications (45 CFR 164.522(b))\n\n[IMPLICATIONS OF THESE CHOICES ON RECORD GENERATION NOT YET KNOWN.]\n\nIn addition, there is a requirement to distribute a notice of privacy practices, addressed by this attachment and the PL Notice of Privacy Practices.\n\nPenalties for non-compliance with the above five rules may be waived, for a limited time in a limited geographical area, if the President declares an emergency or disaster AND the Health and Human Services Secretary declares a public health emergency.  Within that declared timespan, a hospital may rely on the waiver (which covers all patients present) only from the moment of instituting its disaster protocol to at most 72 hours later.  For more, see www.hhs.gov/hipaafaq/providers/hipaa-1068.html.  The waiver is authorized under the Project Bioshield Act of 2004 (PL 108-276) and section 1135(b) of the Social Security Act.\n\nPhoto Copyright\n===============\nThe attached photo if any was taken by an employee, contractor, or volunteer of NLM or of another institutional member of the BHEPP partnership and may be in some cases copyrighted by that institution in the year given by the reporting date or earlier.  Reproduction and distribution is permitted to the extent governed by policy for the overall record.  Please credit the NLM and/or BHEPP and the staffers listed.', '[This document is a straw man example.  It has not been reviewed by legal council, nor reviewed or approved by NLM or any other BHEPP member.]\n\n[The following is an example statement for attachment to an anonymized, de-identified record generated by "TriagePic".  Full records (e.g., patient-identified) may have different statements.]\n\nNotice of Privacy Practices and Information Distribution\n========================================================\nThe attached record was generated by a "TriagePic" application, operated by NLM personnel with test data, or, less likely, by actual BHEPP-partner hospital personnel at the point where disaster victims arrive at the hospital.  While the application does generate real-time patient-specific information (text and photo), the attached record has been anonymized/de-identified to remove patient identifiers (e.g., no photo), and is intended for public release.  It retains categorial information on gender and adult-vs.-child ("Peds"), as well as arrival time, and the hospital personnel involved.  It is intended to be useful for disaster situational awareness and rate-of-arrival purposes.\n\nNo system of de-identification that retains any useful information is perfect.  The recipient who uses this record in an effort to re-identify patients is responsible for any legal ramifications and potential violations of HIPAA or other laws pertaining to patient confidentiality.', '2011-10-04 16:53:01', '2011-10-04 16:53:01', 1, 1, 0),
(4, 'Shady Grove Adventist Hospital', 'Shady Grove', '9901 Medical Center Drive', '', 'Rockville', 'Montgomery', 'MD', '20850', 'USA', 39.096975, -77.199597, '(240) 826-6000', '', '', 'www.shadygroveadventisthospital.com', '1376754457', '', 1, 0, '2011-09-16 14:22:01', '', '...', '...', '2011-10-04 16:53:30', '2011-10-04 16:53:30', 1, 1, 0),
(5, 'Holy Cross Hospital', 'Holy Cross', '1500 Forest Glen Road', '', 'Silver Spring', 'Montgomery', 'MD', '20910', 'USA', 39.015784, -77.0359073, '301-754-7000', '', '', 'www.holycrosshealth.org', '1225067101', '', 1, 0, '2011-09-16 14:28:57', '', '...', '...', '2011-10-04 16:53:47', '2011-10-04 16:53:47', 1, 1, 0),
(7, 'Virginia Hospital Center, Arlington', 'VHC Arlington', '1701 N. George Mason Drive', '', 'Arlington', 'Arlington', 'VA', '22205-3698', 'USA', 38.889643, -77.126661, '(703) 558-5000', '(703) 558-5787', '', 'www.virginiahospitalcenter.com', '1790785996', '', 1, 0, '2011-09-16 14:38:11', '', '...', '...', '2011-10-04 16:53:57', '2011-10-04 16:53:57', 1, 1, 0),
(8, 'St. Francis BG', 'SFBG', '1600 Albany Street', '', 'Beech Grove', '', 'Indiana', '46107', '', 39, -77.101, '', '', '', 'http://www.stfrancishospitals.org/DesktopDefault.aspx?tabid=67', '1205931706', '', 1, 0, '2012-01-17 18:06:25', '', '', '', NULL, NULL, 1, 1, 0),
(9, 'St. Francis IND', 'STF-IND', '8111 S Emerson Ave', '', 'Indianapolis', '', 'IN', '46237', '', 39.6483541, -86.0823324, '(317) 528-5000', '', '', 'http://www.stfrancishospitals.org/DesktopDefault.aspx?tabid=67', '1386749893 ', '', 1, 0, '2012-01-17 18:08:32', '', '', '', NULL, NULL, 1, 1, 0),
(10, 'enter name here', 'shortname', NULL, NULL, NULL, NULL, NULL, NULL, NULL, 38.995523, -77.096597, NULL, NULL, NULL, NULL, NULL, NULL, 1, 0, '2013-07-11 09:46:17', NULL, NULL, NULL, NULL, NULL, 1, 1, 0);

-- --------------------------------------------------------

--
-- Table structure for table `image`
--

CREATE TABLE IF NOT EXISTS `image` (
  `image_id` bigint(20) NOT NULL,
  `p_uuid` varchar(128) NOT NULL,
  `image_type` varchar(100) NOT NULL,
  `image_height` int(11) DEFAULT NULL,
  `image_width` int(11) DEFAULT NULL,
  `created` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `url` varchar(512) DEFAULT NULL,
  `url_thumb` varchar(512) DEFAULT NULL,
  `original_filename` varchar(64) DEFAULT NULL,
  `principal` tinyint(1) NOT NULL DEFAULT '1',
  PRIMARY KEY (`image_id`),
  KEY `p_uuid` (`p_uuid`),
  KEY `principal` (`principal`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- --------------------------------------------------------

--
-- Table structure for table `image_seq`
--

CREATE TABLE IF NOT EXISTS `image_seq` (
  `id` bigint(20) NOT NULL AUTO_INCREMENT COMMENT 'stores next id in sequence for the image table',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB  DEFAULT CHARSET=utf8 AUTO_INCREMENT=2 ;

--
-- Dumping data for table `image_seq`
--

INSERT INTO `image_seq` (`id`) VALUES
(1);

-- --------------------------------------------------------

--
-- Table structure for table `image_tag`
--

CREATE TABLE IF NOT EXISTS `image_tag` (
  `tag_id` int(12) NOT NULL AUTO_INCREMENT,
  `image_id` bigint(20) NOT NULL,
  `tag_x` int(12) NOT NULL,
  `tag_y` int(12) NOT NULL,
  `tag_w` int(12) NOT NULL,
  `tag_h` int(12) NOT NULL,
  `tag_text` varchar(128) NOT NULL,
  PRIMARY KEY (`tag_id`),
  KEY `tag_id` (`tag_id`,`image_id`),
  KEY `image_id` (`image_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 AUTO_INCREMENT=1 ;

-- --------------------------------------------------------

--
-- Table structure for table `image_tag_seq`
--

CREATE TABLE IF NOT EXISTS `image_tag_seq` (
  `id` bigint(20) NOT NULL AUTO_INCREMENT COMMENT 'stores next id in sequence for the image_tag table',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB  DEFAULT CHARSET=utf8 AUTO_INCREMENT=2 ;

--
-- Dumping data for table `image_tag_seq`
--

INSERT INTO `image_tag_seq` (`id`) VALUES
(1);

-- --------------------------------------------------------

--
-- Table structure for table `incident`
--

CREATE TABLE IF NOT EXISTS `incident` (
  `incident_id` bigint(20) NOT NULL AUTO_INCREMENT,
  `parent_id` bigint(20) DEFAULT NULL,
  `search_id` varchar(60) DEFAULT NULL,
  `name` varchar(60) DEFAULT NULL,
  `shortname` varchar(16) DEFAULT NULL,
  `date` date DEFAULT NULL,
  `type` varchar(32) DEFAULT NULL,
  `latitude` double DEFAULT NULL,
  `longitude` double DEFAULT NULL,
  `default` tinyint(1) DEFAULT NULL,
  `private_group` int(11) DEFAULT NULL,
  `closed` tinyint(1) NOT NULL DEFAULT '0',
  `description` varchar(1024) DEFAULT NULL,
  `street` varchar(256) DEFAULT NULL,
  `external_report` varchar(8192) DEFAULT NULL,
  PRIMARY KEY (`incident_id`),
  UNIQUE KEY `shortname_idx` (`shortname`),
  KEY `parent_id` (`parent_id`),
  KEY `private_group` (`private_group`)
) ENGINE=InnoDB  DEFAULT CHARSET=utf8 AUTO_INCREMENT=14 ;

--
-- Dumping data for table `incident`
--

INSERT INTO `incident` (`incident_id`, `parent_id`, `search_id`, `name`, `shortname`, `date`, `type`, `latitude`, `longitude`, `default`, `private_group`, `closed`, `description`, `street`, `external_report`) VALUES
(1, NULL, NULL, 'Uttarakhand Flood', 'utflood', '2000-01-01', 'TEST', 0, 0, NULL, NULL, 0, 'for the Test Exercise', '', ''),
(2, NULL, NULL, 'Katrina', 'katrina', '2013-07-09', 'TEST', 39, -77.101, NULL, NULL, 0, '', '', ''),
(3, NULL, NULL, 'Boston Bombings', 'bostonbomb', '2013-07-09', 'TEST', 39, -77.101, NULL, NULL, 0, '', '', ''),
(4, NULL, NULL, 'Flooding in Central Europe 2013', 'floodeu2013', '2013-07-17', 'TEST', 39, -77.101, NULL, NULL, 0, '', '', ''),
(5, NULL, NULL, 'Japan Earthquake, 2011 ', 'japanquake', '2013-07-17', 'TEST', 39, -77.101, NULL, NULL, 0, '', '', ''),
(6, NULL, NULL, 'Haiti Earthquake,2010', 'haitiquake', '2013-07-17', 'TEST', 39, -77.101, NULL, NULL, 0, '', '', ''),
(7, NULL, NULL, 'Chile Earthquake,2010', 'chilequake', '2013-07-17', 'TEST', 39, -77.101, NULL, NULL, 0, '', '', ''),
(8, NULL, NULL, 'H1N1 Flu (Swine Flu) 2009', 'swineflu', '2013-07-17', 'TEST', 39, -77.101, NULL, NULL, 0, '', '', ''),
(9, 5, NULL, 'Japan Nuclear Disaster', 'japanradiation', '2013-07-17', 'TEST', 39, -77.101, NULL, NULL, 0, '', '', ''),
(10, NULL, NULL, 'Sandy Hurricane', 'sandy', '2013-07-17', 'TEST', 39, -77.101, NULL, NULL, 0, '', '', ''),
(11, NULL, NULL, 'China Floods', 'chinaflood', '2013-07-17', 'TEST', 39, -77.101, NULL, NULL, 0, '', '', ''),
(12, NULL, NULL, 'Oklahoma Tornado', 'okltornado', '2013-07-17', 'TEST', 39, -77.101, NULL, NULL, 0, '', '', ''),
(13, NULL, NULL, 'Arizona Wildfire', 'arizonafire', '2013-07-17', 'TEST', 39, -77.101, NULL, NULL, 0, '', '', '');

-- --------------------------------------------------------

--
-- Table structure for table `incident_seq`
--

CREATE TABLE IF NOT EXISTS `incident_seq` (
  `id` bigint(20) NOT NULL AUTO_INCREMENT COMMENT 'stores next id in sequence for the incident table',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB  DEFAULT CHARSET=utf8 AUTO_INCREMENT=15 ;

--
-- Dumping data for table `incident_seq`
--

INSERT INTO `incident_seq` (`id`) VALUES
(14);

-- --------------------------------------------------------

--
-- Table structure for table `mpres_log`
--

CREATE TABLE IF NOT EXISTS `mpres_log` (
  `log_index` int(11) NOT NULL AUTO_INCREMENT,
  `p_uuid` varchar(128) NOT NULL,
  `email_subject` varchar(256) NOT NULL,
  `email_from` varchar(128) NOT NULL,
  `email_date` varchar(64) NOT NULL,
  `update_time` datetime NOT NULL,
  `xml_format` varchar(16) DEFAULT NULL COMMENT 'MPRES (unstructured) or XML Format of Incoming Email',
  PRIMARY KEY (`log_index`),
  KEY `p_uuid` (`p_uuid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 AUTO_INCREMENT=1 ;

-- --------------------------------------------------------

--
-- Table structure for table `mpres_messages`
--

CREATE TABLE IF NOT EXISTS `mpres_messages` (
  `ix` int(32) NOT NULL AUTO_INCREMENT COMMENT 'the index',
  `when` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP COMMENT 'when the script was last executed',
  `messages` text COMMENT 'the message log from the execution',
  `error_code` int(12) DEFAULT NULL,
  PRIMARY KEY (`ix`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='stores the message log from mpres module' AUTO_INCREMENT=1 ;

-- --------------------------------------------------------

--
-- Table structure for table `mpres_seq`
--

CREATE TABLE IF NOT EXISTS `mpres_seq` (
  `id` bigint(20) NOT NULL AUTO_INCREMENT,
  `last_executed` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `last_message` text,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB  DEFAULT CHARSET=utf8 COMMENT='stores status of mpres module' AUTO_INCREMENT=2 ;

--
-- Dumping data for table `mpres_seq`
--

INSERT INTO `mpres_seq` (`id`, `last_executed`, `last_message`) VALUES
(1, '2012-04-01 04:00:00', '');

-- --------------------------------------------------------

--
-- Table structure for table `old_passwords`
--

CREATE TABLE IF NOT EXISTS `old_passwords` (
  `p_uuid` varchar(60) NOT NULL,
  `password` varchar(100) NOT NULL DEFAULT '',
  `changed_timestamp` bigint(20) NOT NULL,
  PRIMARY KEY (`p_uuid`,`password`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- --------------------------------------------------------

--
-- Table structure for table `page_translations`
--

CREATE TABLE IF NOT EXISTS `page_translations` (
  `page_id` int(11) NOT NULL,
  `locale` varchar(10) NOT NULL,
  KEY `page_id` (`page_id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Table structure for table `password_event_log`
--

CREATE TABLE IF NOT EXISTS `password_event_log` (
  `log_id` bigint(20) NOT NULL AUTO_INCREMENT,
  `changed_timestamp` timestamp NULL DEFAULT CURRENT_TIMESTAMP,
  `p_uuid` varchar(128) NOT NULL,
  `user_name` varchar(100) NOT NULL,
  `comment` varchar(100) NOT NULL,
  `event_type` varchar(32) DEFAULT NULL,
  PRIMARY KEY (`log_id`),
  KEY `p_uuid` (`p_uuid`)
) ENGINE=InnoDB  DEFAULT CHARSET=utf8 AUTO_INCREMENT=49 ;

--
-- Dumping data for table `password_event_log`
--

INSERT INTO `password_event_log` (`log_id`, `changed_timestamp`, `p_uuid`, `user_name`, `comment`, `event_type`) VALUES
(1, '2013-07-04 13:41:34', '1', 'root', 'Created a new event using Event Manager', 'EVENT CREATE'),
(2, '2013-07-04 13:44:16', '1', 'root', 'Purged the event named <b>new event 1884740524</b> using Event Manager', 'EVENT PURGE'),
(3, '2013-07-04 13:44:27', '1', 'root', 'Deleted the event named <b>new event 1884740524</b> using Event Manager', 'EVENT DELETE'),
(4, '2013-07-04 17:34:02', '1', 'root', 'Created a new event using Event Manager', 'EVENT CREATE'),
(5, '2013-07-04 18:21:33', '1', 'root', 'Deleted the event named <b>new event 389548936</b> using Event Manager', 'EVENT DELETE'),
(6, '2013-07-04 18:21:35', '1', 'root', 'Created a new event using Event Manager', 'EVENT CREATE'),
(7, '2013-07-04 18:26:23', '1', 'root', 'Deleted the event named <b>new event 376112318</b> using Event Manager', 'EVENT DELETE'),
(8, '2013-07-06 10:37:01', '1', 'root', 'Login Failed : Invalid Password.', '1'),
(9, '2013-07-06 18:55:24', '1', 'root', 'Created a new event using Event Manager', 'EVENT CREATE'),
(10, '2013-07-06 19:13:38', '1', 'root', 'Deleted the event named <b>new event 1357697803</b> using Event Manager', 'EVENT DELETE'),
(11, '2013-07-09 13:14:02', '1', 'root', 'Created a new event using Event Manager', 'EVENT CREATE'),
(12, '2013-07-09 13:14:29', '1', 'root', 'Deleted the event named <b>new event 1010742404</b> using Event Manager', 'EVENT DELETE'),
(13, '2013-07-09 13:17:45', '1', 'root', 'Created a new event using Event Manager', 'EVENT CREATE'),
(14, '2013-07-09 13:18:04', '1', 'root', 'Deleted the event named <b>new event 1041342142</b> using Event Manager', 'EVENT DELETE'),
(15, '2013-07-09 13:48:07', '1', 'root', 'Created a new event using Event Manager', 'EVENT CREATE'),
(16, '2013-07-09 13:53:05', '1', 'root', 'Deleted the event named <b>new event 1991860193</b> using Event Manager', 'EVENT DELETE'),
(17, '2013-07-09 13:53:22', '1', 'root', 'Created a new event using Event Manager', 'EVENT CREATE'),
(18, '2013-07-09 13:55:43', '1', 'root', 'Deleted the event named <b>new event 1770177777</b> using Event Manager', 'EVENT DELETE'),
(19, '2013-07-09 15:02:12', '1', 'root', 'Created a new event using Event Manager', 'EVENT CREATE'),
(20, '2013-07-09 15:02:40', '1', 'root', 'Created a new event using Event Manager', 'EVENT CREATE'),
(21, '2013-07-13 16:48:48', '1', 'root', 'Login Failed : Invalid Password.', '1'),
(22, '2013-07-13 18:28:11', '1', 'root', 'Login Failed : Invalid Password.', '1'),
(23, '2013-07-13 18:28:19', '1', 'root', 'Login Failed : Invalid Password.', '1'),
(24, '2013-07-14 11:06:08', '1', 'root', 'Login Failed : Invalid Password.', '1'),
(25, '2013-07-17 12:33:14', '1', 'root', 'Login Failed : Invalid Password.', '1'),
(26, '2013-07-17 12:36:13', '1', 'root', 'Created a new event using Event Manager', 'EVENT CREATE'),
(27, '2013-07-17 12:41:24', '1', 'root', 'Login Failed : Invalid Password.', '1'),
(28, '2013-07-17 13:05:44', '1', 'root', 'Deleted the event named <b>new event 1941391296</b> using Event Manager', 'EVENT DELETE'),
(29, '2013-07-17 13:08:07', '1', 'root', 'Created a new event using Event Manager', 'EVENT CREATE'),
(30, '2013-07-17 13:12:05', '1', 'root', 'Created a new event using Event Manager', 'EVENT CREATE'),
(31, '2013-07-17 13:12:49', '1', 'root', 'Deleted the event named <b>new event 266313943</b> using Event Manager', 'EVENT DELETE'),
(32, '2013-07-17 13:12:57', '1', 'root', 'Created a new event using Event Manager', 'EVENT CREATE'),
(33, '2013-07-17 13:13:23', '1', 'root', 'Deleted the event named <b>new event 1886261147</b> using Event Manager', 'EVENT DELETE'),
(34, '2013-07-17 13:13:39', '1', 'root', 'Created a new event using Event Manager', 'EVENT CREATE'),
(35, '2013-07-17 13:14:07', '1', 'root', 'Deleted the event named <b>new event 417881988</b> using Event Manager', 'EVENT DELETE'),
(36, '2013-07-17 13:14:18', '1', 'root', 'Created a new event using Event Manager', 'EVENT CREATE'),
(37, '2013-07-17 13:15:29', '1', 'root', 'Created a new event using Event Manager', 'EVENT CREATE'),
(38, '2013-07-17 13:15:55', '1', 'root', 'Created a new event using Event Manager', 'EVENT CREATE'),
(39, '2013-07-17 13:18:44', '1', 'root', 'Created a new event using Event Manager', 'EVENT CREATE'),
(40, '2013-07-17 13:19:55', '1', 'root', 'Created a new event using Event Manager', 'EVENT CREATE'),
(41, '2013-07-17 13:20:34', '1', 'root', 'Created a new event using Event Manager', 'EVENT CREATE'),
(42, '2013-07-17 13:21:43', '1', 'root', 'Created a new event using Event Manager', 'EVENT CREATE'),
(43, '2013-07-17 13:27:22', '1', 'root', 'Created a new event using Event Manager', 'EVENT CREATE'),
(44, '2013-07-17 13:30:07', '1', 'root', 'Created a new event using Event Manager', 'EVENT CREATE'),
(45, '2013-08-18 17:09:08', '1', 'root', 'Login Failed : Invalid Password.', '1'),
(46, '2013-09-09 14:16:08', '1', 'root', 'Login Failed : Invalid Password.', '1'),
(47, '2013-09-13 13:53:52', '1', 'root', 'Created a new event using Event Manager', 'EVENT CREATE'),
(48, '2013-09-13 13:54:14', '1', 'root', 'Deleted the event named <b>new event 1438213369</b> using Event Manager', 'EVENT DELETE');

-- --------------------------------------------------------

--
-- Table structure for table `person_deceased`
--

CREATE TABLE IF NOT EXISTS `person_deceased` (
  `deceased_id` int(11) NOT NULL AUTO_INCREMENT,
  `p_uuid` varchar(128) NOT NULL,
  `details` text,
  `date_of_death` date DEFAULT NULL,
  `location` varchar(20) DEFAULT NULL,
  `place_of_death` text,
  `comments` text,
  PRIMARY KEY (`deceased_id`),
  UNIQUE KEY `p_uuid` (`p_uuid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 AUTO_INCREMENT=1 ;

-- --------------------------------------------------------

--
-- Table structure for table `person_details`
--

CREATE TABLE IF NOT EXISTS `person_details` (
  `details_id` int(11) NOT NULL AUTO_INCREMENT,
  `p_uuid` varchar(128) NOT NULL,
  `birth_date` date DEFAULT NULL,
  `opt_race` varchar(10) DEFAULT NULL,
  `opt_religion` varchar(10) DEFAULT NULL,
  `opt_gender` varchar(10) DEFAULT NULL,
  `years_old` int(7) DEFAULT NULL,
  `minAge` int(7) DEFAULT NULL,
  `maxAge` int(7) DEFAULT NULL,
  `last_seen` text,
  `last_clothing` text,
  `other_comments` text,
  PRIMARY KEY (`details_id`),
  UNIQUE KEY `p_uuid` (`p_uuid`),
  KEY `opt_gender` (`opt_gender`),
  KEY `years_old` (`years_old`)
) ENGINE=InnoDB  DEFAULT CHARSET=utf8 AUTO_INCREMENT=2996979 ;

--
-- Dumping data for table `person_details`
--

INSERT INTO `person_details` (`details_id`, `p_uuid`, `birth_date`, `opt_race`, `opt_religion`, `opt_gender`, `years_old`, `minAge`, `maxAge`, `last_seen`, `last_clothing`, `other_comments`) VALUES
(2995458, '3', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL),
(2996977, '4', NULL, NULL, NULL, NULL, NULL, 18, 150, 'NLM (testing) Hospital', NULL, 'LPF notification - disaster victim arrives at hospital triage station'),
(2996978, '/vesuvius/vesuvius/wwwperson.101', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);

-- --------------------------------------------------------

--
-- Table structure for table `person_followers`
--

CREATE TABLE IF NOT EXISTS `person_followers` (
  `id` int(16) NOT NULL AUTO_INCREMENT,
  `p_uuid` varchar(128) NOT NULL,
  `follower_p_uuid` varchar(128) NOT NULL,
  PRIMARY KEY (`id`),
  KEY `p_uuid` (`p_uuid`),
  KEY `follower_p_uuid` (`follower_p_uuid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 AUTO_INCREMENT=1 ;

-- --------------------------------------------------------

--
-- Table structure for table `person_notes`
--

CREATE TABLE IF NOT EXISTS `person_notes` (
  `note_id` int(11) NOT NULL AUTO_INCREMENT,
  `note_about_p_uuid` varchar(128) NOT NULL,
  `note_written_by_p_uuid` varchar(128) NOT NULL,
  `note` varchar(1024) NOT NULL,
  `when` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `suggested_status` varchar(3) DEFAULT NULL COMMENT 'the status of the person as suggested by the note maker',
  PRIMARY KEY (`note_id`),
  KEY `note_about_p_uuid` (`note_about_p_uuid`),
  KEY `note_written_by_p_uuid` (`note_written_by_p_uuid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 AUTO_INCREMENT=1 ;

-- --------------------------------------------------------

--
-- Table structure for table `person_physical`
--

CREATE TABLE IF NOT EXISTS `person_physical` (
  `physical_id` int(11) NOT NULL AUTO_INCREMENT,
  `p_uuid` varchar(128) NOT NULL,
  `opt_blood_type` varchar(10) DEFAULT NULL,
  `height` varchar(10) DEFAULT NULL,
  `weight` varchar(10) DEFAULT NULL,
  `opt_eye_color` varchar(50) DEFAULT NULL,
  `opt_skin_color` varchar(50) DEFAULT NULL,
  `opt_hair_color` varchar(50) DEFAULT NULL,
  `injuries` text,
  `comments` text,
  PRIMARY KEY (`physical_id`),
  UNIQUE KEY `p_uuid` (`p_uuid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 AUTO_INCREMENT=1 ;

-- --------------------------------------------------------

--
-- Stand-in structure for view `person_search`
--
CREATE TABLE IF NOT EXISTS `person_search` (
`p_uuid` varchar(128)
,`full_name` varchar(100)
,`given_name` varchar(50)
,`family_name` varchar(50)
,`expiry_date` datetime
,`updated` datetime
,`updated_db` datetime
,`opt_status` varchar(3)
,`opt_gender` varchar(10)
,`years_old` bigint(11)
,`minAge` bigint(11)
,`maxAge` bigint(11)
,`ageGroup` varchar(7)
,`image_height` int(11)
,`image_width` int(11)
,`url_thumb` varchar(512)
,`icon_url` varchar(128)
,`shortname` varchar(16)
,`hospital` varchar(30)
,`comments` text
,`last_seen` text
,`mass_casualty_id` varchar(255)
);
-- --------------------------------------------------------

--
-- Table structure for table `person_seq`
--

CREATE TABLE IF NOT EXISTS `person_seq` (
  `id` int(11) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

--
-- Dumping data for table `person_seq`
--

INSERT INTO `person_seq` (`id`) VALUES
(101);

-- --------------------------------------------------------

--
-- Table structure for table `person_status`
--

CREATE TABLE IF NOT EXISTS `person_status` (
  `status_id` int(11) NOT NULL AUTO_INCREMENT,
  `p_uuid` varchar(128) NOT NULL,
  `opt_status` varchar(3) NOT NULL DEFAULT 'unk',
  `last_updated` datetime DEFAULT NULL,
  `creation_time` datetime DEFAULT NULL,
  `last_updated_db` datetime DEFAULT NULL COMMENT 'Last DB update. (For SOLR indexing.)',
  PRIMARY KEY (`status_id`),
  UNIQUE KEY `p_uuid` (`p_uuid`),
  KEY `last_updated_db` (`last_updated_db`),
  KEY `opt_status` (`opt_status`),
  KEY `last_updated` (`last_updated`)
) ENGINE=InnoDB  DEFAULT CHARSET=utf8 AUTO_INCREMENT=2 ;

--
-- Dumping data for table `person_status`
--

INSERT INTO `person_status` (`status_id`, `p_uuid`, `opt_status`, `last_updated`, `creation_time`, `last_updated_db`) VALUES
(1, '/vesuvius/vesuvius/wwwperson.101', 'unk', '2013-07-11 11:46:32', '2013-07-11 11:46:32', NULL);

-- --------------------------------------------------------

--
-- Table structure for table `person_to_report`
--

CREATE TABLE IF NOT EXISTS `person_to_report` (
  `p_uuid` varchar(128) NOT NULL,
  `rep_uuid` varchar(128) NOT NULL,
  PRIMARY KEY (`p_uuid`,`rep_uuid`),
  KEY `rep_uuid` (`rep_uuid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

--
-- Dumping data for table `person_to_report`
--

INSERT INTO `person_to_report` (`p_uuid`, `rep_uuid`) VALUES
('/vesuvius/vesuvius/wwwperson.101', '1');

-- --------------------------------------------------------

--
-- Table structure for table `person_updates`
--

CREATE TABLE IF NOT EXISTS `person_updates` (
  `update_index` int(32) NOT NULL AUTO_INCREMENT,
  `p_uuid` varchar(128) NOT NULL,
  `update_time` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `updated_table` varchar(64) NOT NULL,
  `updated_column` varchar(64) NOT NULL,
  `old_value` varchar(512) DEFAULT NULL,
  `new_value` varchar(512) DEFAULT NULL,
  `updated_by_p_uuid` varchar(128) NOT NULL,
  PRIMARY KEY (`update_index`),
  KEY `p_uuid` (`p_uuid`),
  KEY `updated_by_p_uuid` (`updated_by_p_uuid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 AUTO_INCREMENT=1 ;

-- --------------------------------------------------------

--
-- Table structure for table `person_uuid`
--

CREATE TABLE IF NOT EXISTS `person_uuid` (
  `p_uuid` varchar(128) NOT NULL,
  `full_name` varchar(100) DEFAULT NULL,
  `family_name` varchar(50) DEFAULT NULL,
  `given_name` varchar(50) DEFAULT NULL,
  `incident_id` bigint(20) DEFAULT NULL,
  `hospital_uuid` int(32) DEFAULT NULL,
  `expiry_date` datetime DEFAULT NULL,
  PRIMARY KEY (`p_uuid`),
  KEY `full_name_idx` (`full_name`),
  KEY `incident_id_index` (`incident_id`),
  KEY `hospital_index` (`hospital_uuid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

--
-- Dumping data for table `person_uuid`
--

INSERT INTO `person_uuid` (`p_uuid`, `full_name`, `family_name`, `given_name`, `incident_id`, `hospital_uuid`, `expiry_date`) VALUES
('/vesuvius/vesuvius/wwwperson.101', NULL, NULL, NULL, 2, NULL, '0000-01-01 00:00:00'),
('1', 'Root /', '/', 'Root', NULL, NULL, NULL),
('2', 'Email System', 'System', 'Email', NULL, NULL, NULL),
('3', 'Anonymous User', 'User', 'Anonymous', NULL, NULL, NULL),
('4', 'TestFrom WebServices', 'WebServices', 'TestFrom', NULL, NULL, NULL);

-- --------------------------------------------------------

--
-- Table structure for table `pfif_export_log`
--

CREATE TABLE IF NOT EXISTS `pfif_export_log` (
  `log_index` int(11) NOT NULL AUTO_INCREMENT,
  `repository_id` int(11) DEFAULT '0',
  `direction` varchar(3) NOT NULL DEFAULT 'in',
  `status` varchar(10) NOT NULL,
  `start_mode` varchar(10) NOT NULL,
  `start_time` datetime DEFAULT NULL,
  `end_time` datetime DEFAULT NULL,
  `first_entry` datetime NOT NULL,
  `last_entry` datetime NOT NULL,
  `last_count` int(11) DEFAULT '0',
  `person_count` int(11) DEFAULT '0',
  `note_count` int(11) DEFAULT '0',
  PRIMARY KEY (`log_index`),
  KEY `repository_id` (`repository_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 AUTO_INCREMENT=1 ;

-- --------------------------------------------------------

--
-- Table structure for table `pfif_harvest_note_log`
--

CREATE TABLE IF NOT EXISTS `pfif_harvest_note_log` (
  `log_index` int(11) NOT NULL AUTO_INCREMENT,
  `repository_id` int(11) DEFAULT '0',
  `direction` varchar(3) NOT NULL DEFAULT 'in',
  `status` varchar(10) NOT NULL,
  `start_mode` varchar(10) NOT NULL,
  `start_time` datetime DEFAULT NULL,
  `end_time` datetime DEFAULT NULL,
  `first_entry` datetime NOT NULL,
  `last_entry` datetime NOT NULL,
  `last_count` int(11) DEFAULT '0',
  `note_count` int(11) DEFAULT '0',
  PRIMARY KEY (`log_index`),
  KEY `repository_id` (`repository_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 AUTO_INCREMENT=1 ;

-- --------------------------------------------------------

--
-- Table structure for table `pfif_harvest_person_log`
--

CREATE TABLE IF NOT EXISTS `pfif_harvest_person_log` (
  `log_index` int(11) NOT NULL AUTO_INCREMENT,
  `repository_id` int(11) DEFAULT '0',
  `direction` varchar(3) NOT NULL DEFAULT 'in',
  `status` varchar(10) NOT NULL,
  `start_mode` varchar(10) NOT NULL,
  `start_time` datetime DEFAULT NULL,
  `end_time` datetime DEFAULT NULL,
  `first_entry` datetime NOT NULL,
  `last_entry` datetime NOT NULL,
  `last_count` int(11) DEFAULT '0',
  `person_count` int(11) DEFAULT '0',
  `images_in` int(11) DEFAULT '0',
  `images_retried` int(11) DEFAULT '0',
  `images_failed` int(11) DEFAULT '0',
  PRIMARY KEY (`log_index`),
  KEY `repository_id` (`repository_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 AUTO_INCREMENT=1 ;

-- --------------------------------------------------------

--
-- Table structure for table `pfif_note`
--

CREATE TABLE IF NOT EXISTS `pfif_note` (
  `note_record_id` varchar(128) NOT NULL,
  `p_uuid` varchar(128) NOT NULL,
  `source_version` varchar(10) NOT NULL DEFAULT '1.3',
  `source_repository_id` int(11) DEFAULT NULL,
  `linked_person_record_id` varchar(128) DEFAULT NULL,
  `entry_date` datetime NOT NULL,
  `author_name` varchar(100) DEFAULT NULL,
  `author_email` varchar(100) DEFAULT NULL,
  `author_phone` varchar(100) DEFAULT NULL,
  `source_date` datetime NOT NULL,
  `found` varchar(5) DEFAULT NULL,
  `status` varchar(20) DEFAULT NULL,
  `email_of_found_person` varchar(100) DEFAULT NULL,
  `phone_of_found_person` varchar(100) DEFAULT NULL,
  `last_known_location` text,
  `text` text,
  PRIMARY KEY (`note_record_id`),
  KEY `p_uuid` (`p_uuid`),
  KEY `source_repository_id` (`source_repository_id`),
  KEY `linked_person_record_id` (`linked_person_record_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='IMPORT WILL FAIL if you add foreign key constraints.';

-- --------------------------------------------------------

--
-- Table structure for table `pfif_note_seq`
--

CREATE TABLE IF NOT EXISTS `pfif_note_seq` (
  `id` int(11) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

--
-- Dumping data for table `pfif_note_seq`
--

INSERT INTO `pfif_note_seq` (`id`) VALUES
(1);

-- --------------------------------------------------------

--
-- Table structure for table `pfif_person`
--

CREATE TABLE IF NOT EXISTS `pfif_person` (
  `p_uuid` varchar(128) NOT NULL,
  `source_version` varchar(10) NOT NULL,
  `source_repository_id` int(11) NOT NULL,
  `entry_date` datetime NOT NULL,
  `expiry_date` datetime DEFAULT NULL,
  `author_name` varchar(100) DEFAULT NULL,
  `author_email` varchar(100) DEFAULT NULL,
  `author_phone` varchar(100) DEFAULT NULL,
  `source_name` varchar(100) DEFAULT NULL,
  `source_date` datetime DEFAULT NULL,
  `source_url` varchar(512) DEFAULT NULL,
  `full_name` varchar(128) DEFAULT NULL,
  `first_name` varchar(100) DEFAULT NULL,
  `last_name` varchar(100) DEFAULT NULL,
  `home_city` varchar(100) DEFAULT NULL,
  `home_state` varchar(15) DEFAULT NULL,
  `home_country` varchar(2) DEFAULT NULL,
  `home_neighborhood` varchar(100) DEFAULT NULL,
  `home_street` varchar(100) DEFAULT NULL,
  `home_postal_code` varchar(16) DEFAULT NULL,
  `photo_url` varchar(512) DEFAULT NULL,
  `sex` varchar(10) DEFAULT NULL,
  `date_of_birth` date DEFAULT NULL,
  `age` varchar(10) DEFAULT NULL,
  `other` text,
  PRIMARY KEY (`p_uuid`),
  KEY `source_repository_id` (`source_repository_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- --------------------------------------------------------

--
-- Table structure for table `pfif_repository`
--

CREATE TABLE IF NOT EXISTS `pfif_repository` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `name` varchar(100) NOT NULL,
  `incident_id` bigint(11) DEFAULT NULL,
  `base_url` varchar(512) DEFAULT NULL,
  `subdomain` varchar(32) DEFAULT 'YMz6eKr-yEA3TXGp',
  `auth_key` varchar(16) DEFAULT 'YMz6eKr-yEA3TXGp',
  `resource_type` varchar(6) DEFAULT NULL,
  `role` varchar(6) DEFAULT NULL,
  `granularity` varchar(20) NOT NULL DEFAULT 'YYYY-MM-DDThh:mm:ssZ',
  `deleted_record` varchar(10) NOT NULL DEFAULT 'no',
  `sched_interval_minutes` int(11) NOT NULL DEFAULT '0',
  `log_granularity` varchar(20) NOT NULL DEFAULT '24:00:00',
  `first_entry` datetime DEFAULT NULL,
  `last_entry` datetime DEFAULT NULL,
  `total_persons` int(11) NOT NULL DEFAULT '0',
  `total_notes` int(11) NOT NULL DEFAULT '0',
  PRIMARY KEY (`id`),
  KEY `incident_id` (`incident_id`)
) ENGINE=InnoDB  DEFAULT CHARSET=utf8 AUTO_INCREMENT=61 ;

--
-- Dumping data for table `pfif_repository`
--

INSERT INTO `pfif_repository` (`id`, `name`, `incident_id`, `base_url`, `subdomain`, `auth_key`, `resource_type`, `role`, `granularity`, `deleted_record`, `sched_interval_minutes`, `log_granularity`, `first_entry`, `last_entry`, `total_persons`, `total_notes`) VALUES
(13, 'googlekatrina', 2, '', 'YMz6eKr-yEA3TXGp', 'YMz6eKr-yEA3TXGp', 'person', 'source', 'YYYY-MM-DDThh:mm:ssZ', 'no', 0, '24:00:00', NULL, NULL, 0, 0),
(14, 'googlekatrina', 2, '', 'YMz6eKr-yEA3TXGp', 'YMz6eKr-yEA3TXGp', 'note', 'source', 'YYYY-MM-DDThh:mm:ssZ', 'no', 0, '24:00:00', NULL, NULL, 0, 0),
(15, 'googlekatrina', 2, '', 'YMz6eKr-yEA3TXGp', 'YMz6eKr-yEA3TXGp', 'both', 'sink', 'YYYY-MM-DDThh:mm:ssZ', 'no', 0, '24:00:00', NULL, NULL, 0, 0),
(16, 'googlebostonbomb', 3, '', 'YMz6eKr-yEA3TXGp', 'YMz6eKr-yEA3TXGp', 'person', 'source', 'YYYY-MM-DDThh:mm:ssZ', 'no', 0, '24:00:00', NULL, NULL, 0, 0),
(17, 'googlebostonbomb', 3, '', 'YMz6eKr-yEA3TXGp', 'YMz6eKr-yEA3TXGp', 'note', 'source', 'YYYY-MM-DDThh:mm:ssZ', 'no', 0, '24:00:00', NULL, NULL, 0, 0),
(18, 'googlebostonbomb', 3, '', 'YMz6eKr-yEA3TXGp', 'YMz6eKr-yEA3TXGp', 'both', 'sink', 'YYYY-MM-DDThh:mm:ssZ', 'no', 0, '24:00:00', NULL, NULL, 0, 0),
(22, 'googlefloodeu2013', 4, '', 'YMz6eKr-yEA3TXGp', 'YMz6eKr-yEA3TXGp', 'person', 'source', 'YYYY-MM-DDThh:mm:ssZ', 'no', 0, '24:00:00', NULL, NULL, 0, 0),
(23, 'googlefloodeu2013', 4, '', 'YMz6eKr-yEA3TXGp', 'YMz6eKr-yEA3TXGp', 'note', 'source', 'YYYY-MM-DDThh:mm:ssZ', 'no', 0, '24:00:00', NULL, NULL, 0, 0),
(24, 'googlefloodeu2013', 4, '', 'YMz6eKr-yEA3TXGp', 'YMz6eKr-yEA3TXGp', 'both', 'sink', 'YYYY-MM-DDThh:mm:ssZ', 'no', 0, '24:00:00', NULL, NULL, 0, 0),
(34, 'googlejapanquake', 5, '', 'YMz6eKr-yEA3TXGp', 'YMz6eKr-yEA3TXGp', 'person', 'source', 'YYYY-MM-DDThh:mm:ssZ', 'no', 0, '24:00:00', NULL, NULL, 0, 0),
(35, 'googlejapanquake', 5, '', 'YMz6eKr-yEA3TXGp', 'YMz6eKr-yEA3TXGp', 'note', 'source', 'YYYY-MM-DDThh:mm:ssZ', 'no', 0, '24:00:00', NULL, NULL, 0, 0),
(36, 'googlejapanquake', 5, '', 'YMz6eKr-yEA3TXGp', 'YMz6eKr-yEA3TXGp', 'both', 'sink', 'YYYY-MM-DDThh:mm:ssZ', 'no', 0, '24:00:00', NULL, NULL, 0, 0),
(37, 'googlehaitiquake', 6, '', 'YMz6eKr-yEA3TXGp', 'YMz6eKr-yEA3TXGp', 'person', 'source', 'YYYY-MM-DDThh:mm:ssZ', 'no', 0, '24:00:00', NULL, NULL, 0, 0),
(38, 'googlehaitiquake', 6, '', 'YMz6eKr-yEA3TXGp', 'YMz6eKr-yEA3TXGp', 'note', 'source', 'YYYY-MM-DDThh:mm:ssZ', 'no', 0, '24:00:00', NULL, NULL, 0, 0),
(39, 'googlehaitiquake', 6, '', 'YMz6eKr-yEA3TXGp', 'YMz6eKr-yEA3TXGp', 'both', 'sink', 'YYYY-MM-DDThh:mm:ssZ', 'no', 0, '24:00:00', NULL, NULL, 0, 0),
(40, 'googlechilequake', 7, '', 'YMz6eKr-yEA3TXGp', 'YMz6eKr-yEA3TXGp', 'person', 'source', 'YYYY-MM-DDThh:mm:ssZ', 'no', 0, '24:00:00', NULL, NULL, 0, 0),
(41, 'googlechilequake', 7, '', 'YMz6eKr-yEA3TXGp', 'YMz6eKr-yEA3TXGp', 'note', 'source', 'YYYY-MM-DDThh:mm:ssZ', 'no', 0, '24:00:00', NULL, NULL, 0, 0),
(42, 'googlechilequake', 7, '', 'YMz6eKr-yEA3TXGp', 'YMz6eKr-yEA3TXGp', 'both', 'sink', 'YYYY-MM-DDThh:mm:ssZ', 'no', 0, '24:00:00', NULL, NULL, 0, 0),
(43, 'googleswineflu', 8, '', 'YMz6eKr-yEA3TXGp', 'YMz6eKr-yEA3TXGp', 'person', 'source', 'YYYY-MM-DDThh:mm:ssZ', 'no', 0, '24:00:00', NULL, NULL, 0, 0),
(44, 'googleswineflu', 8, '', 'YMz6eKr-yEA3TXGp', 'YMz6eKr-yEA3TXGp', 'note', 'source', 'YYYY-MM-DDThh:mm:ssZ', 'no', 0, '24:00:00', NULL, NULL, 0, 0),
(45, 'googleswineflu', 8, '', 'YMz6eKr-yEA3TXGp', 'YMz6eKr-yEA3TXGp', 'both', 'sink', 'YYYY-MM-DDThh:mm:ssZ', 'no', 0, '24:00:00', NULL, NULL, 0, 0),
(46, 'googlejapanradiation', 9, '', 'YMz6eKr-yEA3TXGp', 'YMz6eKr-yEA3TXGp', 'person', 'source', 'YYYY-MM-DDThh:mm:ssZ', 'no', 0, '24:00:00', NULL, NULL, 0, 0),
(47, 'googlejapanradiation', 9, '', 'YMz6eKr-yEA3TXGp', 'YMz6eKr-yEA3TXGp', 'note', 'source', 'YYYY-MM-DDThh:mm:ssZ', 'no', 0, '24:00:00', NULL, NULL, 0, 0),
(48, 'googlejapanradiation', 9, '', 'YMz6eKr-yEA3TXGp', 'YMz6eKr-yEA3TXGp', 'both', 'sink', 'YYYY-MM-DDThh:mm:ssZ', 'no', 0, '24:00:00', NULL, NULL, 0, 0),
(49, 'googlesandy', 10, '', 'YMz6eKr-yEA3TXGp', 'YMz6eKr-yEA3TXGp', 'person', 'source', 'YYYY-MM-DDThh:mm:ssZ', 'no', 0, '24:00:00', NULL, NULL, 0, 0),
(50, 'googlesandy', 10, '', 'YMz6eKr-yEA3TXGp', 'YMz6eKr-yEA3TXGp', 'note', 'source', 'YYYY-MM-DDThh:mm:ssZ', 'no', 0, '24:00:00', NULL, NULL, 0, 0),
(51, 'googlesandy', 10, '', 'YMz6eKr-yEA3TXGp', 'YMz6eKr-yEA3TXGp', 'both', 'sink', 'YYYY-MM-DDThh:mm:ssZ', 'no', 0, '24:00:00', NULL, NULL, 0, 0),
(52, 'googlechinaflood', 11, '', 'YMz6eKr-yEA3TXGp', 'YMz6eKr-yEA3TXGp', 'person', 'source', 'YYYY-MM-DDThh:mm:ssZ', 'no', 0, '24:00:00', NULL, NULL, 0, 0),
(53, 'googlechinaflood', 11, '', 'YMz6eKr-yEA3TXGp', 'YMz6eKr-yEA3TXGp', 'note', 'source', 'YYYY-MM-DDThh:mm:ssZ', 'no', 0, '24:00:00', NULL, NULL, 0, 0),
(54, 'googlechinaflood', 11, '', 'YMz6eKr-yEA3TXGp', 'YMz6eKr-yEA3TXGp', 'both', 'sink', 'YYYY-MM-DDThh:mm:ssZ', 'no', 0, '24:00:00', NULL, NULL, 0, 0),
(55, 'googleokltornado', 12, '', 'YMz6eKr-yEA3TXGp', 'YMz6eKr-yEA3TXGp', 'person', 'source', 'YYYY-MM-DDThh:mm:ssZ', 'no', 0, '24:00:00', NULL, NULL, 0, 0),
(56, 'googleokltornado', 12, '', 'YMz6eKr-yEA3TXGp', 'YMz6eKr-yEA3TXGp', 'note', 'source', 'YYYY-MM-DDThh:mm:ssZ', 'no', 0, '24:00:00', NULL, NULL, 0, 0),
(57, 'googleokltornado', 12, '', 'YMz6eKr-yEA3TXGp', 'YMz6eKr-yEA3TXGp', 'both', 'sink', 'YYYY-MM-DDThh:mm:ssZ', 'no', 0, '24:00:00', NULL, NULL, 0, 0),
(58, 'googlearizonafire', 13, '', 'YMz6eKr-yEA3TXGp', 'YMz6eKr-yEA3TXGp', 'person', 'source', 'YYYY-MM-DDThh:mm:ssZ', 'no', 0, '24:00:00', NULL, NULL, 0, 0),
(59, 'googlearizonafire', 13, '', 'YMz6eKr-yEA3TXGp', 'YMz6eKr-yEA3TXGp', 'note', 'source', 'YYYY-MM-DDThh:mm:ssZ', 'no', 0, '24:00:00', NULL, NULL, 0, 0),
(60, 'googlearizonafire', 13, '', 'YMz6eKr-yEA3TXGp', 'YMz6eKr-yEA3TXGp', 'both', 'sink', 'YYYY-MM-DDThh:mm:ssZ', 'no', 0, '24:00:00', NULL, NULL, 0, 0);

-- --------------------------------------------------------

--
-- Table structure for table `phonetic_word`
--

CREATE TABLE IF NOT EXISTS `phonetic_word` (
  `encode1` varchar(50) DEFAULT NULL,
  `encode2` varchar(50) DEFAULT NULL,
  `pgl_uuid` varchar(128) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- --------------------------------------------------------

--
-- Table structure for table `plus_access_log`
--

CREATE TABLE IF NOT EXISTS `plus_access_log` (
  `access_id` int(16) NOT NULL AUTO_INCREMENT,
  `access_time` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `application` varchar(32) DEFAULT NULL,
  `version` varchar(16) DEFAULT NULL,
  `ip` varchar(16) DEFAULT NULL,
  `call` varchar(64) DEFAULT NULL,
  `api_version` varchar(8) DEFAULT NULL,
  `latitude` double DEFAULT NULL COMMENT 'lat of the ip address',
  `longitude` double DEFAULT NULL COMMENT 'lon of the ip address',
  `user_name` varchar(100) DEFAULT NULL COMMENT 'users.user_name that make the call',
  PRIMARY KEY (`access_id`),
  KEY `user_idx` (`user_name`),
  KEY `ip_idx` (`ip`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 AUTO_INCREMENT=1 ;

-- --------------------------------------------------------

--
-- Table structure for table `plus_report_log`
--

CREATE TABLE IF NOT EXISTS `plus_report_log` (
  `report_id` int(16) NOT NULL AUTO_INCREMENT,
  `p_uuid` varchar(128) NOT NULL,
  `report_time` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `enum` varchar(16) DEFAULT NULL,
  PRIMARY KEY (`report_id`),
  KEY `p_uuid` (`p_uuid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 AUTO_INCREMENT=1 ;

-- --------------------------------------------------------

--
-- Table structure for table `pop_outlog`
--

CREATE TABLE IF NOT EXISTS `pop_outlog` (
  `outlog_index` int(11) NOT NULL AUTO_INCREMENT,
  `mod_accessed` varchar(8) NOT NULL,
  `time_sent` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  `send_status` varchar(8) NOT NULL,
  `error_message` varchar(512) NOT NULL,
  `email_subject` varchar(256) NOT NULL,
  `email_from` varchar(128) NOT NULL,
  `email_recipients` varchar(256) NOT NULL,
  PRIMARY KEY (`outlog_index`)
) ENGINE=InnoDB  DEFAULT CHARSET=utf8 AUTO_INCREMENT=40 ;

--
-- Dumping data for table `pop_outlog`
--

INSERT INTO `pop_outlog` (`outlog_index`, `mod_accessed`, `time_sent`, `send_status`, `error_message`, `email_subject`, `email_from`, `email_recipients`) VALUES
(1, 'xst', '2013-07-04 13:41:34', 'ERROR', '<strong>Invalid address: </strong><br />\n', '{ Sahana Vesuvius Event Manager Audit }', '', 'audit@example.com'),
(2, 'xst', '2013-07-04 13:44:16', 'ERROR', '<strong>Invalid address: </strong><br />\n', '{ Sahana Vesuvius Event Manager Audit }', '', 'audit@example.com'),
(3, 'xst', '2013-07-04 13:44:27', 'ERROR', '<strong>Invalid address: </strong><br />\n', '{ Sahana Vesuvius Event Manager Audit }', '', 'audit@example.com'),
(4, 'xst', '2013-07-04 17:34:02', 'ERROR', '<strong>Invalid address: </strong><br />\n', '{ Sahana Vesuvius Event Manager Audit }', '', 'audit@example.com'),
(5, 'xst', '2013-07-04 18:21:33', 'ERROR', '<strong>Invalid address: </strong><br />\n', '{ Sahana Vesuvius Event Manager Audit }', '', 'audit@example.com'),
(6, 'xst', '2013-07-04 18:21:35', 'ERROR', '<strong>Invalid address: </strong><br />\n', '{ Sahana Vesuvius Event Manager Audit }', '', 'audit@example.com'),
(7, 'xst', '2013-07-04 18:26:23', 'ERROR', '<strong>Invalid address: </strong><br />\n', '{ Sahana Vesuvius Event Manager Audit }', '', 'audit@example.com'),
(8, 'xst', '2013-07-06 18:55:24', 'ERROR', '<strong>Invalid address: </strong><br />\n', '{ Sahana Vesuvius Event Manager Audit }', '', 'audit@example.com'),
(9, 'xst', '2013-07-06 19:13:37', 'ERROR', '<strong>Invalid address: </strong><br />\n', '{ Sahana Vesuvius Event Manager Audit }', '', 'audit@example.com'),
(10, 'xst', '2013-07-09 13:14:02', 'ERROR', '<strong>Invalid address: </strong><br />\n', '{ Sahana Vesuvius Event Manager Audit }', '', 'audit@example.com'),
(11, 'xst', '2013-07-09 13:14:29', 'ERROR', '<strong>Invalid address: </strong><br />\n', '{ Sahana Vesuvius Event Manager Audit }', '', 'audit@example.com'),
(12, 'xst', '2013-07-09 13:17:45', 'ERROR', '<strong>Invalid address: </strong><br />\n', '{ Sahana Vesuvius Event Manager Audit }', '', 'audit@example.com'),
(13, 'xst', '2013-07-09 13:18:04', 'ERROR', '<strong>Invalid address: </strong><br />\n', '{ Sahana Vesuvius Event Manager Audit }', '', 'audit@example.com'),
(14, 'xst', '2013-07-09 13:48:07', 'ERROR', '<strong>Invalid address: </strong><br />\n', '{ Sahana Vesuvius Event Manager Audit }', '', 'audit@example.com'),
(15, 'xst', '2013-07-09 13:53:05', 'ERROR', '<strong>Invalid address: </strong><br />\n', '{ Sahana Vesuvius Event Manager Audit }', '', 'audit@example.com'),
(16, 'xst', '2013-07-09 13:53:22', 'ERROR', '<strong>Invalid address: </strong><br />\n', '{ Sahana Vesuvius Event Manager Audit }', '', 'audit@example.com'),
(17, 'xst', '2013-07-09 13:55:43', 'ERROR', '<strong>Invalid address: </strong><br />\n', '{ Sahana Vesuvius Event Manager Audit }', '', 'audit@example.com'),
(18, 'xst', '2013-07-09 15:02:12', 'ERROR', '<strong>Invalid address: </strong><br />\n', '{ Sahana Vesuvius Event Manager Audit }', '', 'audit@example.com'),
(19, 'xst', '2013-07-09 15:02:40', 'ERROR', '<strong>Invalid address: </strong><br />\n', '{ Sahana Vesuvius Event Manager Audit }', '', 'audit@example.com'),
(20, 'xst', '2013-07-17 12:36:13', 'ERROR', '<strong>Invalid address: </strong><br />\n', '{ Sahana Vesuvius Event Manager Audit }', '', 'audit@example.com'),
(21, 'xst', '2013-07-17 13:05:44', 'ERROR', '<strong>Invalid address: </strong><br />\n', '{ Sahana Vesuvius Event Manager Audit }', '', 'audit@example.com'),
(22, 'xst', '2013-07-17 13:08:07', 'ERROR', '<strong>Invalid address: </strong><br />\n', '{ Sahana Vesuvius Event Manager Audit }', '', 'audit@example.com'),
(23, 'xst', '2013-07-17 13:12:05', 'ERROR', '<strong>Invalid address: </strong><br />\n', '{ Sahana Vesuvius Event Manager Audit }', '', 'audit@example.com'),
(24, 'xst', '2013-07-17 13:12:49', 'ERROR', '<strong>Invalid address: </strong><br />\n', '{ Sahana Vesuvius Event Manager Audit }', '', 'audit@example.com'),
(25, 'xst', '2013-07-17 13:12:57', 'ERROR', '<strong>Invalid address: </strong><br />\n', '{ Sahana Vesuvius Event Manager Audit }', '', 'audit@example.com'),
(26, 'xst', '2013-07-17 13:13:23', 'ERROR', '<strong>Invalid address: </strong><br />\n', '{ Sahana Vesuvius Event Manager Audit }', '', 'audit@example.com'),
(27, 'xst', '2013-07-17 13:13:39', 'ERROR', '<strong>Invalid address: </strong><br />\n', '{ Sahana Vesuvius Event Manager Audit }', '', 'audit@example.com'),
(28, 'xst', '2013-07-17 13:14:07', 'ERROR', '<strong>Invalid address: </strong><br />\n', '{ Sahana Vesuvius Event Manager Audit }', '', 'audit@example.com'),
(29, 'xst', '2013-07-17 13:14:17', 'ERROR', '<strong>Invalid address: </strong><br />\n', '{ Sahana Vesuvius Event Manager Audit }', '', 'audit@example.com'),
(30, 'xst', '2013-07-17 13:15:29', 'ERROR', '<strong>Invalid address: </strong><br />\n', '{ Sahana Vesuvius Event Manager Audit }', '', 'audit@example.com'),
(31, 'xst', '2013-07-17 13:15:55', 'ERROR', '<strong>Invalid address: </strong><br />\n', '{ Sahana Vesuvius Event Manager Audit }', '', 'audit@example.com'),
(32, 'xst', '2013-07-17 13:18:44', 'ERROR', '<strong>Invalid address: </strong><br />\n', '{ Sahana Vesuvius Event Manager Audit }', '', 'audit@example.com'),
(33, 'xst', '2013-07-17 13:19:55', 'ERROR', '<strong>Invalid address: </strong><br />\n', '{ Sahana Vesuvius Event Manager Audit }', '', 'audit@example.com'),
(34, 'xst', '2013-07-17 13:20:34', 'ERROR', '<strong>Invalid address: </strong><br />\n', '{ Sahana Vesuvius Event Manager Audit }', '', 'audit@example.com'),
(35, 'xst', '2013-07-17 13:21:43', 'ERROR', '<strong>Invalid address: </strong><br />\n', '{ Sahana Vesuvius Event Manager Audit }', '', 'audit@example.com'),
(36, 'xst', '2013-07-17 13:27:22', 'ERROR', '<strong>Invalid address: </strong><br />\n', '{ Sahana Vesuvius Event Manager Audit }', '', 'audit@example.com'),
(37, 'xst', '2013-07-17 13:30:07', 'ERROR', '<strong>Invalid address: </strong><br />\n', '{ Sahana Vesuvius Event Manager Audit }', '', 'audit@example.com'),
(38, 'xst', '2013-09-13 13:53:52', 'ERROR', '<strong>Invalid address: </strong><br />\n', '{ Sahana Vesuvius Event Manager Audit }', '', 'audit@example.com'),
(39, 'xst', '2013-09-13 13:54:13', 'ERROR', '<strong>Invalid address: </strong><br />\n', '{ Sahana Vesuvius Event Manager Audit }', '', 'audit@example.com');

-- --------------------------------------------------------

--
-- Table structure for table `rap_log`
--

CREATE TABLE IF NOT EXISTS `rap_log` (
  `rap_id` int(16) NOT NULL AUTO_INCREMENT,
  `p_uuid` varchar(128) NOT NULL,
  `report_time` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  PRIMARY KEY (`rap_id`),
  KEY `p_uuid` (`p_uuid`)
) ENGINE=InnoDB  DEFAULT CHARSET=utf8 AUTO_INCREMENT=2 ;

--
-- Dumping data for table `rap_log`
--

INSERT INTO `rap_log` (`rap_id`, `p_uuid`, `report_time`) VALUES
(1, '/vesuvius/vesuvius/wwwperson.101', '2013-07-11 09:46:32');

-- --------------------------------------------------------

--
-- Table structure for table `rez_incident`
--

CREATE TABLE IF NOT EXISTS `rez_incident` (
  `incident_id` bigint(20) NOT NULL,
  `page_id` int(11) NOT NULL,
  `timestamp` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  PRIMARY KEY (`page_id`,`incident_id`),
  KEY `incident_id` (`incident_id`,`page_id`),
  KEY `timestamp` (`timestamp`),
  KEY `page_id` (`page_id`),
  KEY `timestamp_2` (`timestamp`),
  KEY `timestamp_3` (`timestamp`),
  KEY `page_id_2` (`page_id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1 COMMENT='This table handles mapping between pages and incident';

--
-- Dumping data for table `rez_incident`
--

INSERT INTO `rez_incident` (`incident_id`, `page_id`, `timestamp`) VALUES
(1, 11, '2013-09-12 11:16:15'),
(4, 11, '2013-09-12 11:16:15'),
(4, 50, '2013-09-12 11:17:08'),
(4, 51, '2013-09-12 11:38:43'),
(4, 24, '2013-09-12 11:40:05'),
(13, 24, '2013-09-12 11:40:05'),
(4, 53, '2013-09-12 11:44:55'),
(4, 54, '2013-09-12 11:45:37'),
(11, 55, '2013-09-12 11:46:24');

-- --------------------------------------------------------

--
-- Table structure for table `rez_pages`
--

CREATE TABLE IF NOT EXISTS `rez_pages` (
  `rez_page_id` int(11) NOT NULL,
  `rez_menu_title` varchar(64) NOT NULL,
  `rez_menu_order` int(11) NOT NULL,
  `rez_content` mediumtext NOT NULL,
  `rez_timestamp` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  `rez_visibility` varchar(16) NOT NULL,
  `rez_locale` varchar(6) NOT NULL DEFAULT 'en_US',
  `parent_template_id` varchar(11) NOT NULL,
  `parent_page_id` varchar(11) NOT NULL,
  PRIMARY KEY (`rez_page_id`,`rez_locale`),
  KEY `rez_page_id` (`rez_page_id`),
  KEY `rez_locale` (`rez_locale`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

--
-- Dumping data for table `rez_pages`
--

INSERT INTO `rez_pages` (`rez_page_id`, `rez_menu_title`, `rez_menu_order`, `rez_content`, `rez_timestamp`, `rez_visibility`, `rez_locale`, `parent_template_id`, `parent_page_id`) VALUES
(-46, 'Demo System Page', 0, 'Default Content', '2013-09-12 11:39:09', 'Hidden', 'en_US', '', ''),
(-45, 'PLUS Web Service API', 38, '<a href="https://docs.google.com/document/d/17pApAVZvg4g93sjZOY3Rp8-MfSu8wSRMycUca3LXNJc/edit?hl=en_US">Click here to open the Google Doc for the PLUS Web Services</a><br>', '2012-03-01 15:59:28', 'Hidden', 'en_US', '', ''),
(-30, 'ABOUT', 36, '<meta http-equiv="content-type" content="text/html; charset=utf-8"><span class="Apple-style-span" style="font-family: ''Times New Roman''; font-size: medium; "><pre style="word-wrap: break-word; white-space: pre-wrap; ">Some of the Sahana Agasti modules are being actively developed, maintained, or customized by the U.S. National Library of Medicine (NLM), located on the National Institutes of Health (NIH) campus in Bethesda, Maryland. NLM is in a community partnership with 3 nearby hospitals (National Naval Medical Center, NIH Clinical Center, Suburban Hospital) to improve emergency responses to a mass disaster impacting those hospitals. The partnership, called the Bethesda Hospitals'' Emergency Preparedness Partnership (BHEPP), received initial federal funding for LPF and other NLM IT projects in 2008-9. The LPF project is currently supported by the Intramural Research Program of the NIH, through NLMâ€™s Lister Hill National Center for Biomedical Communications (LHNCBC). Software development is headed by LHNCBC''s Communication Engineering Branch (CEB), with additional content from LHNCBC''s Computer Science Branch (CSB) and the Disaster Information Management Research Center (DIMRC), part of NLM''s Specialized Information Services.</pre></span>', '2012-03-01 15:59:33', 'Hidden', 'en_US', '', ''),
(-20, 'Access Denied', -20, 'You do not have permission to access this event. If you believe this is in error, please contact lpfsupport@mail.nih.gov', '2011-06-14 11:29:53', 'Hidden', 'en_US', '', ''),
(-6, 'Password Reset.', 8, '<div><br></div><div>Your password has been successfully reset and the new password emailed to you.</div>', '2011-06-14 11:39:49', 'Hidden', 'en_US', '', ''),
(-5, 'Account activated.', 7, '<div><br></div><div>Your account has been successfully activated. You may now <a href="index.php?mod=pref&amp;act=loginForm" title="login" target="">login to the site</a> to begin using it.</div>', '2011-06-14 11:39:49', 'Hidden', 'en_US', '', ''),
(-4, 'Account already active.', 6, '<div><br></div><div>This confirmation link is no longer valid. The account attached to it is already active.</div>', '2011-06-14 11:36:55', 'Hidden', 'en_US', '', ''),
(11, 'How do I search for a person?', 35, '<h2>Searching</h2>\n1) Enter a name in the search box.<br>\n2) Click on the "Search" button, or hit Enter <br>\n<br>\n<i>Examples:</i><br>\n<br>\n Joseph Doe<br>\n Doe, Jane<br>\n Joseph Joe Joey<br>\n<br>\nIt is best to leave off titles ("Dr.", "Mrs.") and suffixes ("Jr") at this time.<br>\n<br>\n<br>\n<h2>Search Options</h2>\nOnce you have performed a search, you may also limit your results by status, gender, and age.<br>\n<br>\nStatus choices are missing (blue), alive and well (green), injured (red), deceased (black), found (brown) or unknown (gray).<br>\n<br>\nGender choices are male, female, and other/unknown.<br>\n<br>\nAge choices are 0-17, 18+, or unknown.<br><br>If you want to see only records that have photos, include "[image]" in the search box.&nbsp; Use "[-image]" to see only records that have no photos.<br>\n<br>\n<br>\n<h2>Results</h2>\nResults include any of the search terms.&nbsp; To tolerate misspellings, results are not limited to exact matches.&nbsp; Matches may include names found in certain fields, like Notes, that will be visible only if you consult the record''s details.<br>\n<br>\nUnder the search box is the number of records found that match your search, and the total number in the database (e.g., Found 2 out of 43).<br>\n<br>\nYou may sort your results by Time, Name, Age, or Status.&nbsp; By Name orders by last name, then within that, first.&nbsp; By Age will use a calculated midpoint age for each record with an age range. <br>\n<br>\nInteractive mode displays results by page.  The default is 25 per page.  You may change it to 50 or 100 per page via the pull down menu at the top of the results.<br>\n<br>\nHands Free mode displays results as several horizontally-scrolling rows of boxes with a photograph or text.  The boxes always distribute themselves evenly among the rows, starting at the right side and from top row to bottom.  If there are more boxes than can be shown at once, the rows will become animated to scroll horizontally with wrap-around.  There is no meaning to the ordering of the images at this time.<br>\n<br>\n<br>\n<h2>Getting Details about a Given Results<br></h2>\nClick on the results (photo or text) for more information.<br>\n<br>\n<br>\n<h2>Pause and Play Buttons</h2>\nIf horizontal scrolling is occurring, Pause will stop that, and Play will resume it.  Even while paused, the search will be repeated every minute to look for fresh content.<br>\n<br>\n<br>\n<h2>Search and Data Updates</h2>\nOnce a set of results for a search is loaded, the search will be quietly repeated every minute to see if there is new content.<br><br>New Information can be input via the Report a Person page, or sent to us directly by email or web service, e.g., from apps like ReUnite and TriagePic.\n<br>\n<br><br>', '2013-09-12 15:23:22', 'Public', 'en_US', '1', ''),
(24, 'Links to other organizations', 18, '<h2>Find Family and Friends</h2>\n<a href="https://safeandwell.communityos.org/cms//" title="Red Cross Safe and Well List">Red Cross Safe and Well List</a><br>\n<a href="http://www.nokr.org/nok/restricted/home.htm" title="Next of Kin National Registry">Next of Kin National Registry</a><br>\n<a href="http://www.lrcf.net/create-flyer/" title="Missing Person Flier Creation Tools">Missing Person Flier Creation Tools</a><br>\n<br>\n\n<h2>Disaster Resources - General</h2>\n<a target="" title="Disaster Assistance" href="http://www.disasterassistance.gov/">Disaster Assistance</a><br>\n<a href="http://app.redcross.org/nss-app/" title="Red Cross Provides Shelters and Food">Red Cross Provides Shelters and Food</a><br>\n<a target="" title="NLM''s Disaster Information Management Resource Center" href="http://disasterinfo.nlm.nih.gov">NLM''s Disaster Information Management Resource Center</a>, currently highlighting flood and hurricane information.<br> \n\n<h2>Disaster Resources - Tornadoes</h2>\n<a target="" title="Tornado Information from the Disaster Information Management Resource Center" href="http://disaster.nlm.nih.gov/enviro/tornados.html">From the Disaster Information Management Resource Center</a><br>\n<a target="" title="Tornado Information  from MedlinePlus" href="http://www.nlm.nih.gov/medlineplus/tornadoes.html">From MedlinePlus</a><br>\n<a target="" title="NOAA 2011 Tornado Information" href="http://www.noaanews.noaa.gov/2011_tornado_information.html">From NOAA 2011</a><br>\n<a target="" title="From Joplin Globe''s Facebook page" href="http://www.poynter.org/latest-news/making-sense-of-news/133446/joplin-globes-facebook-page-locates-reunites-missing-people-in-tornado-aftermath/">About Joplin Globe''s Facebook page -  locates, reunites missing people in tornado aftermath</a>\n\n\n\n\n', '2013-09-12 15:23:22', 'Public', 'en_US', '1', ''),
(43, 'How do I report a missing or found person?', 14, '<div style="">\n<script type="text/javascript" src="res/js/jquery-1.4.4.min.js"></script>\n<script type="text/javascript" src="res/js/animatedcollapse.js"></script>\n<script>\nanimatedcollapse.addDiv(''more_reunite_en'', ''fade=1,hide=1'');\nanimatedcollapse.addDiv(''more_email_en'', ''fade=1,hide=1'');\nanimatedcollapse.addDiv(''more_reunite_es'', ''fade=1,hide=1'');\nanimatedcollapse.addDiv(''more_email_es'', ''fade=1,hide=1'');\nanimatedcollapse.addDiv(''more_reunite_fr'', ''fade=1,hide=1'');\nanimatedcollapse.addDiv(''more_email_fr'', ''fade=1,hide=1'');\nanimatedcollapse.init();\nanimatedcollapse.ontoggle=function($, divobj, state){}\n</script>\n\n<h1>Reporting a Missing or Found Person</h1>\n<ul>\n  <li><b>By Browser.</b> Use the "Report a Person" link at left.  This is the way to update a report, too.</li>\n  <li><b>By iPhone, iPod Touch, or iPad.</b> Get our free <a href="http://lpf.nlm.nih.gov/" title="">“ReUnite” app</a> from the Apple Store (<a href="#reunite_en">details...</a>).</li>\n  <li><b>By Email.</b> Put name and status in the subject line, as in "Jane Doe missing", attach a photo, and send to <a href="mailto:disaster@mail.nih.gov">disaster@mail.nih.gov</a> (<a href="#email_en">details...</a>).</li>\n  <li><b>By Specialized Software for Hospitals.</b> Ask NLM about our "TriagePic" Windows software for triage stations.</li>\n</ul>\n<br>\n<p>[<a href="#reporting_es">[TO DO: In Spanish]</a>]</p>\n<p>[<a href="#reporting_fr">En Français</a>]</p>\n<br>\n<h1>Details</h1>\n\n<a id="reunite_en"><br><h4>Reporting using our iPhone™ App, “ReUnite”</h4></a>\n\nOf particular interest to aid workers, we provide a free iPhone app called <a href="http://lpf.nlm.nih.gov/" title="">ReUnite</a> through the Apple Store.&nbsp;\nThis creates structured content with associated photographs (limited to 1 per submission so far).&nbsp;\nMore information can be transmitted to us this way than using the unstructured email method below.<br>\n<br>\nReUnite currently supports iPhone 3G and iPhone 4 with iPhone OS 3.0 or later.&nbsp; iPod Touch and iPad are also usable.<br>\n<br>\n&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href="javascript:animatedcollapse.toggle(''more_reunite_en'')">Show/Hide More...</a>\n<br>\n<br>\n<div id="more_reunite_en" display="none;">\nUsers can choose to take a new photo using their iPhone’s camera\nor use an existing image from their camera roll/photo library.&nbsp;\nThey are then able to tag certain information about the person in the photo.&nbsp;\nThe following fields, all optional, are available for editing:<br>\n    <ul>\n      <li>Given Name</li>\n      <li>Family Name</li>\n      <li>Health Status: (Alive &amp; Well / Injured / Deceased / Unknown)</li>\n      <li>Gender: (Male / Female / Unknown)</li>\n      <li>Age Group: (0-17 / 18+ / Unknown) <i>(or enter an estimated age instead)</i></li>\n      <li>Estimated Age, in years</li>\n      <li>Location Status: (Missing / Known)</li>\n      <li>Last Known Location <i>(if missing)</i> / Current Location <i>(otherwise)</i></li>\n    </ul>\n    <p>Street <i>(typically)</i></p>\n    <p>Town <i>(typically)</i></p>\n    <ul>\n      <li>ID Tag <i>Automatically generated by default. Aid workers can substitute organization’s triage number.</i></li>\n      <li>Voice Note</li>\n      <li>Text Note</li>\n    </ul>\n    <p>In addition, the following info is generated at the time the record is created:</p>\n    <ul>\n      <li>GPS Location <span style="font-style: italic;">(if enabled)</span><br></li>\n      <li>Date and time</li>\n    </ul>\nThe image and corresponding information can then be sent by web service (or as backup, by email) to the PL server automatically.&nbsp;\n    The info is also embedded into the image’s EXIF tags.&nbsp;\n    The records (including photos) are stored locally on the iPhone in an SQLite database format.&nbsp;\n    This database can be sent by email to <a href="mailto:lpfsupport@mail.nih.gov">lpfsupport@mail.nih.gov</a>,\n    where support personnel can arrange to have it included in our database.&nbsp;\n    Consequently, data can be collected when cell phone connectivity is not available,\n    and subsequently sent when connectivity becomes available.<br>\n</div>\n<br>\n<a id="email_en"><br><h4>Quick Reporting by Email of Name, Status, and Photo</h4></a>\n\nYou may also report a name and simple status directly to us by email (such as by cell phone).  You may also attach a photograph (limited to 1 at the moment).  Acceptable formats are .jpg (or .jpeg), .png, and .gif . For now, content in the email body is ignored.<br>\n<br>\n<p>Email to: <a href="mailto:disaster@mail.nih.gov">disaster@mail.nih.gov</a></p>\n<p>Subject Line: <i>Name of Missing or Found Person</i> <b>Status</b></p>\n<p>where <b>Status</b>, whose case doesn''t matter, is</p>\n<ul>\n   <li>Missing</li>\n   <li>Alive and Well</li>\n   <li>Injured</li>\n   <li>Deceased</li>\n   <li>Found <i>(but it''s better to use a status that indicates health too)</i></li>\n</ul>\n<br>\nExample of subject line: “Jean Baptiste alive and well”<br>\nPunctuation will be treated as spaces.<br>\n<br>\n&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href="javascript:animatedcollapse.toggle(''more_email_en'')">Show/Hide More...</a>\n<br>\n<br>\n<div id="more_email_en" display="none;">\n<p><b>Table of Status Words</b></p>\n <table border="1" cellpadding="0" cellspacing="0">\n  <tbody><tr>\n   <td valign="top" width="163"><p><b>Status Assumed</b></p></td>\n   <td valign="top" width="811"><p><b>Recognized words in subject line (case doesn’t matter)</b></p></td>\n  </tr>\n  <tr>\n   <td valign="top" width="163"><p>Missing</p></td>\n   <td valign="top" width="811"><p>missing, lost, looking for, [to] find</p></td>\n  </tr>\n  <tr>\n   <td valign="top" width="163"></td>\n   <td valign="top" width="811"><p>French: disparu, perdu, trouver, a la recherche de, trouver [SUITE: à la recherche de]</p></td>\n  </tr>\n  <tr>\n   <td valign="top" width="163"><p>Alive and Well</p></td>\n   <td valign="top" width="811"><p>alive, well, okay, OK, good, healthy, recovered, fine</p></td>\n  </tr>\n  <tr>\n   <td valign="top" width="163"></td>\n   <td valign="top" width="811"> <p>French: en vie, vivant, ok, bien portant, en bonne sante, gueri [SUITE: en bonne santé, guéri]</p></td>\n  </tr>\n  <tr>\n   <td valign="top" width="163"><p>Injured</p></td>\n   <td valign="top" width="811"><p>injured, hurt, wounded, sick, treated, recovering</p></td>\n  </tr>\n  <tr>\n   <td valign="top" width="163"></td>\n   <td valign="top" width="811"><p>French: blesse, mal en point, malade, soigne, convalscent [SUITE: blessé, soigné]</p></td>\n  </tr>\n  <tr>\n   <td valign="top" width="163"><p>Deceased</p></td>\n   <td valign="top" width="811"><p>deceased, dead, died, buried</p></td>\n  </tr>\n  <tr>\n   <td valign="top" width="163"></td>\n   <td valign="top" width="811"><p>French: decede, mort, inhume [SUITE: décédé, inhumé ]</p></td>\n  </tr>\n  <tr>\n   <td valign="top" width="163"><p>Found</p></td>\n   <td valign="top" width="811"><p>found</p></td>\n  </tr>\n </tbody></table>\n<p>When entering Status:</p>\n<ul>\n   <li>Avoid using the word “not”</li>\n   <li>Avoid using the word “found” alone, without further indicating health status in one of the three categories.</li>\n</ul>\n</div>\n<br>\n<hr>\n<br>\n<a id="reporting_es"><br><h1>Creando un reporte [TO DO: about a Missing or Found Person][TRANSLATION IN PROGRESS]</h1></a>\n    <br>\n	[TO DO: Bullet points]<br>\n<ul>\n  <li><b>By Browser.</b> Use the "Report a Person" link at left.  This is the way to update a report, too.</li>\n  <li><b>By iPhone, iPod Touch, or iPad.</b> Get our free <a href="http://lpf.nlm.nih.gov/" title="">“ReUnite” app</a> from the Apple Store (<a href="#reunite_es">details...</a>).</li>\n  <li><b>By Email.</b> Put name and status in the subject line, as in "Jane Doe missing", attach a photo, and send to <a href="mailto:disaster@mail.nih.gov">disaster@mail.nih.gov</a> (<a href="#email_es">details...</a>).</li>\n  <li><b>By Specialized Software for Hospitals.</b> Ask NLM about our "TriagePic" Windows software for triage stations.</li>\n</ul>\n<br>\n<h3>[TO DO: Details]</h3>\n	<a id="reunite_es"><h4>Enviando un reporte mediante la aplicación de iPHone, "ReUnite"</h4></a>\n    De particular interés para los trabajadores humanitarios, ofrecemos una aplicación libre de costo para el iPhone llamada\n    <a href="http://itunes.apple.com/us/app/reunite/id368052994?mt=8" title="">ReUnite</a> a través del "Apple Store".&nbsp;\n    Esta aplicación crea un mensaje electrónico con un contenido estructurado con fotografías asociadas (limitado a 1 por reporte por el momento).\n    Esta aplicación de iPhone permite proporcionar más información que mediante el método de correo electrónico no estructurado explicado arriba.\n    <br><br>\n    "ReUnite" actualmente soporta iPhone 3G y iPhone 4 con iPhone OS 3.0 o una versión más actual.<br>\n	{TO DO: iPod Touch and iPad are also usable.]<br>\n<br>\n&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href="javascript:animatedcollapse.toggle(''more_reunite_es'')">{TO DO]Show/Hide More...</a>\n<br>\n<br>\n<div id="more_reunite_es" display="none;">\n    Los usuarios pueden elegir tomar una nueva foto usando la cámara de su iPhone o utilizar una imagen existente en su cámara, en su colección de fotografías\n    digitales. Luego de eso, pueden  etiquetar la fotografía con cierta información sobre la persona. Los siguientes campos, todos opcionales, están disponibles para ser editados:<br>\n    <ul>\n      <li>Nombre</li>\n      <li>Apellido</li>\n      <li>Estado de salud: (Vivo y bien / Herido / Fallecido / Desconocido) </li>\n      <li>Género: (Hombre / Mujer / Desconocido) </li>\n      <li>AGrupo de edad: (0-17 / 18 + / Desconocido) <i>(o ingresar una edad estimada en el siguiente campo en su lugar)</i></li>\n      <li>Edad estimada, en años </li>\n      <li>Estado de la información de ubicación: (Desconocida o Conocida) </li>\n      <li>Última ubicación conocida de la persona <i>(si ubicación actual no es conocida)</i> / Ubicación actual <i>(si es conocida)</i></li>\n      <ul>\n         <li>Calle  <i>(típicamente)</i></li>\n         <li>Ciudad o localidad <i>(típicamente)</i></li>\n      </ul>\n      <li>Etiqueta de identificación generada automáticamente. <i>Trabajadores humanitarios pueden substituir esta etiqueta por un código de triage de su organización.</i></li>\n      <li>Notas [TO DO: Voice Note]</li>\n	  <li>Notas [TO DO: Text Note]</li>\n    </ul>\n    <p>Además, la siguiente información se genera cuando sea crea el registro:</p>\n    <ul>\n      <li>Localización geográfica GPS</li>\n      <li>Fecha y hora</li>\n    </ul>\n    Luego, la imagen y la información correspondiente pueden ser enviadas automáticamente por correo electrónico al servidor LPF.\n    La información es también incluida en las etiquetas EXIF de la imagen. Los registros (incluyendo fotos) se almacenan localmente\n    en el iPhone en un formato de base de datos SQLite. Esta base de datos puede ser enviada por correo electrónico a\n    <a href="mailto:lpfsupport@mail.nih.gov">lpfsupport@mail.nih.gov</a>,\n    donde el personal de soporte puede hacer arreglos para que se incluya en nuestra base de datos. Por lo tanto, datos pueden ser recopilados\n    cuando la conectividad de teléfonos celulares no está disponible, y posteriormente enviados cuando la conectividad esté de nuevo disponible.\n	<br>\n</div>\n<br>\n    <a id="email_es"><h4>Creando un reporte rápidamente mediante email, incluyendo nombre, estado y foto</h4></a>\n    <p>Usted puede también reportar directamente el nombre y estado de una persona mediante correo electrónico (por ejemplo desde su teléfono celular).\n    Se puede también adjuntar una fotografía (solo una foto es permitida por el momento). Los formatos digitales aceptados son .jpg (o .jpeg),\n    .png, y .gif. Por ahora, el contenido del cuerpo del mensaje es ignorado.\n	<br>\n    Envíe su correo electrónico a: <a href="mailto:disaster@nih.gov">disaster@nih.gov</a></p>\n	<p>En la línea de asunto (subject) de su mensaje electrónico ingrese el nombre de la persona encontrada o buscada, y una palabra que indique el <b>"estado"</b>\n	o condición de esta persona [, TO DO where <b>estado</b>, whose case doesn''t matter, is]<br>\n    [TO DO: Spanish status strings are planned]<br>\n    </p><ul>\n      <li>Se busca</li>\n      <li>Vivo y bien</li>\n      <li>Herido/herida</li>\n      <li>Fallecido</li>\n	  <li>Encontrado/encontrada <i>([TO DO: but it''s better to use a status that indicates health too)</i></li>\n	</ul>\n\n	Por ejemplo:<br>\n	Asunto: Juan Perez se busca<br>\n	[TO DO:  Punctuation will be treated as spaces.]<br>\n<br>\n&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href="javascript:animatedcollapse.toggle(''more_email_es'')">[TO DO:]Show/Hide More...</a>\n<br>\n<br>\n<div id="more_email_es" display="none;">\n    <p><b>Tabla de palabras de "estado" o condición de la persona</b></p>\n	[TO DO:  Spanish status strings are planned.  Add English, French rows here too.  Also add spanish to English, French sections]\n <table border="1" cellpadding="0" cellspacing="0">\n  <tbody><tr>\n   <td valign="top" width="163"><p><b>Estados usados por el sistema</b></p></td>\n   <td valign="top" width="811"><p><b>Términos sugeridos para ser usados en "estado" ([TO DO: case doesn''t matter])</b></p></td>\n  </tr>\n  <tr>\n   <td valign="top" width="163"><p>Se busca</p></td>\n   <td valign="top" width="811"><p>Se busca, perdida, perdido, buscando, por encontrar</p></td>\n  </tr>\n  <tr>\n   <td valign="top" width="163"><p>Vivo y bien</p></td>\n   <td valign="top" width="811"><p>Vivo, bien, okey, saludable, recuperado</p></td>\n  </tr>\n  <tr>\n   <td valign="top" width="163"><p>Herido/herida</p></td>\n   <td valign="top" width="811"><p> Herido, herida, lastimado, lastimada, enfermo, enferma, en tratamiento, recuperándose</p></td>\n  </tr>\n  <tr>\n   <td valign="top" width="163"><p>Fallecido</p></td>\n   <td valign="top" width="811"><p>Fallecido, fallecida, muerto, muerta, murió, sepultado, sepultada</p></td>\n  </tr>\n  <tr>\n   <td valign="top" width="163"><p>Encontrado/encontrada</p></td>\n   <td valign="top" width="811"><p>Encontrado, encontrada</p></td>\n  </tr>\n </tbody>\n</table>\n    Cuando se ingresa el estado de la persona:<br>\n    <ul>\n      <li> Evite usar la palabra "no"</li>\n      <li>Evite usar la palabra "encontrado" o "encontrada" sola. Indique el estado de salud en una de las tres categorías: vivo y bien, herido, fallecido</li>\n    </ul>\n</div>\n    <br>\n<hr>\n<br>\n<a id="reporting_fr"><br><h1>Signalement d’une Personne Disparu ou Retrouvé<br></h1></a>\n<ul>\n  <li><b>Par Navigateur.</b> Utilisez le lien "Report a Person" à gauche.  C''est la façon de mettre à jour un rapport, aussi.</li>\n  <li><b>Par iPhone, iPod Touch, ou iPad.</b> Obtenez notre application gratuite, <a href="http://lpf.nlm.nih.gov/" title="">“ReUnite”</a>, disponible via Apple Store (<a href="#reunite_fr">détails...</a>).</li>\n  <li><b>Par Courriel.</b> Mettez le nom et le statut dans la ligne sujet ("Jane Doe disparu" par exemple), joindre une photo, et courriel à <a href="mailto:disaster@mail.nih.gov">disaster@mail.nih.gov</a> (<a href="#email_fr">détails...</a>)</li>\n  <li><b>Par des Logiciels Spécialisés pour les Hôpitaux.</b> Demandez NLM sur notre "TriagePic" logiciel Windows pour les stations de triage.</li>\n</ul>\n<br>\n<h1>Détails</h1>\n\n<a id="reunite_fr"><br><h4>Signalement avec l’application iPhone™, “ReUnite”</h4></a>\n\nEn soutien aux acteurs de l’aide internationale, nous proposons une application iPhone gratuite,\n(<a href="http://lpf.nlm.nih.gov/" title="">“ReUnite”</a>), disponible via Apple Store.&nbsp;\nCette application crée un courriel structuré avec photographie jointe (limité à une photographie par soumission).&nbsp;\nD’autres informations peuvent nous être transmises de cette manière, de préférence à l’utilisation d’un courriel classique, non structuré.<br>\n<br>\n“ReUnite” prend actuellement en charge l’iPhone 3G et 4 de l’iPhone avec iPhone OS 3.0 ou une version ultérieure.&nbsp;\niPod Touch et iPad sont également utilisables.<br>\n<br>\n&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href="javascript:animatedcollapse.toggle(''more_reunite_fr'')">Afficher/Masquer Plus d''Info...</a>\n<br>\n<br>\n<div id="more_reunite_fr" display="none;">\nLes utilisateurs peuvent prendre une photo à l’aide de leur iPhone, ou sélectionner une photo dans leur galerie.&nbsp;\nIls peuvent ensuite joindre certaines informations sur la personne photographiée.&nbsp;\nLes champs suivants (tous optionnels) peuvent être remplis:<br>\n<ul>\n    <li>Prénom</li>\n    <li>Nom de Famille</li>\n    <li>État de santé: (En vie / Blessé /Décédé /Inconnu)</li>\n    <li>Sexe: (Masculin / Féminin / Inconnu)</li>\n    <li>Âge: (0-17 / 18+ / Inconnu) <i>(ou une estimation de l’âge)</i></li>\n    <li>Âge présumé, en années</li>\n    <li>État de position: (Disparu / Connu)</li>\n    <li>Dernière position connue <i>(si disparu)</i> / Position actuelle <i>(sinon)</i></li>\n</ul>\n    <p>Rue <i>(par exemple)</i></p>\n    <p>Ville <i>(par exemple)</i></p>\n<ul>\n    <li>Badge d’identification <i>Généré automatiquement par défaut. Le personnel humanitaire pourra y substituer un numéro de triage spécifique aux organismes.</i></li>\n    <li>Autres remarques et commentaires</li>\n</ul>\nDe plus, les informations suivantes sont automatiquement générées lors de la création du signalement:<br>\n<ul>\n    <li>Position GPS</li>\n    <li>Date et heure</li>\n</ul>\nL’image et les informations associées peuvent ensuite être automatiquement envoyées par courrier électronique au serveur LPF.&nbsp;\nLes informations sont également ajoutées au contenu des tags EXIF de l’image.&nbsp;\nL’ensemble du signalement (y compris l’image) est enregistré localement sur l’iPhone dans une base de données au format SQLite.&nbsp;\nCette base de données peut être envoyée par courrier électronique à <a href="mailto:lpfsupport@mail.nih.gov">lpfsupport@mail.nih.gov</a>,\nafin que notre personnel procède à la mise à jour de notre base de données globale.&nbsp;\nAinsi, les données peuvent être collectées dans des zones sans connexion réseau puis transmises ultérieurement,\ndès qu’une connexion réseau est disponible.<br>\n</div>\n<br>\n\n<a id="email_fr"><br><h4>Signalement rapide par Courriel - envoi de Nom, Statut et Photographie</h4></a>\n\nLe nom et le statut d’une personne peut nous être envoyé\ndirectement par courrier électronique (par exemple, envoyé depuis un\ntéléphone portable).&nbsp; Il vous est également possible de joindre une photographie à votre\nmessage (fonctionnalité limitée à une seule photographie par message\npour l’instant).&nbsp; Les formats acceptés sont .jpg (ou .jpeg), .png, et .gif .&nbsp; Pour l’instant, toute information contenue dans le corps du message\nélectronique est ignorée.<br>\n<br>\n<p>Courriel à : <a href="mailto:disaster@mail.nih.gov">disaster@mail.nih.gov</a></p>\n<p>Sujet: <i>Nom de la victime</i> <b>Statut</b></p>\n<p><span style="font-weight: bold;">Statut</span> =</p>\n<ul>\n  <li>Disparu</li>\n  <li>En Vie</li>\n  <li>Blessé</li>\n  <li>Décédé</li>\n  <li>[Retrouvé] <i>Mais il est préférable d''utiliser un status qui précise la santé, aussi.</i></li>\n</ul>\n<br>\nExemple de sujet d’un courriel: “Jean-Baptiste Dupont En Vie”<br>\nPonctuation seront traités comme des espaces.<br>\n<br>\n&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href="javascript:animatedcollapse.toggle(''more_email_fr'')">Afficher/Masquer Plus d''Info...</a>\n<br>\n<div id="more_email_fr" display="none;">\n<br>\n<p><b>Tableau des mots décrivant le statut</b></p>\n<table border="1" cellpadding="0" cellspacing="0">\n  <tbody><tr>\n   <td valign="top" width="163"><p><b>Statut Présumé</b></p></td>\n   <td valign="top" width="811">\n    <p><b>Mots Recommandés à indiquer dans le sujet du courriel (pas de distinction entre majuscules et minuscules)</b></p></td>\n  </tr>\n  <tr>\n   <td valign="top" width="163"><p>Disparu</p></td>\n   <td valign="top" width="811"><p>disparu, perdu, trouver, a la recherche de, trouver [SUITE: à la recherche de]</p></td>\n  </tr>\n  <tr>\n   <td valign="top" width="163"></td>\n   <td valign="top" width="811"><p>Anglais: missing, lost, looking for, [to] find</p></td>\n  </tr>\n  <tr>\n   <td valign="top" width="163"> <p>En vie</p></td>\n   <td valign="top" width="811"> <p>en vie, vivant, ok, bien portant, en bonne sante, gueri [SUITE: en bonne santé, guéri]</p></td>\n  </tr>\n  <tr>\n   <td valign="top" width="163"></td>\n   <td valign="top" width="811"><p>Anglais: alive, well, okay, OK, good, healthy, recovered, fine</p></td>\n  </tr>\n  <tr>\n   <td valign="top" width="163"> <p>Blessé</p></td>\n   <td valign="top" width="811"><p>blesse, mal en point, malade, soigne, convalscent [SUITE: blessé, soigné]</p></td>\n  </tr>\n  <tr>\n   <td valign="top" width="163"></td>\n   <td valign="top" width="811"><p>Anglais: injured, hurt, wounded, sick, treated, recovering</p></td>\n  </tr>\n  <tr>\n   <td valign="top" width="163"><p>Décédé</p></td>\n   <td valign="top" width="811"><p>decede, mort, inhume [SUITE: décédé, inhumé ]</p></td>\n  </tr>\n  <tr>\n   <td valign="top" width="163"></td>\n   <td valign="top" width="811"><p>Anglais: deceased, dead, died, buried</p></td>\n  </tr>\n </tbody>\n</table>\nLorsque vous renseignez le statut:<br>\n<ul>\n   <li>Évitez les tournures négatives “pas”, “non”</li>\n   <li>Evitez d’utiliser le mot “trouvé” sans information complémentaire sur l’état de santé de la victime.</li>\n</ul>\n</div>\n<br>\n\n</div>\n\n\n\n\n', '2013-09-12 15:23:20', 'Public', 'en_US', '', ''),
(48, 'Staying Safe in the aftermath of Disaster', 36, '<h2>How to Be Safe</h2><br>Although each type of disaster brings its own unique challenges, the steps listed here are applicable to many different situations you may face.\n\n<ul><li>    Check the area around you for safety. In the case of biological, chemical or radiological threats, listen for instructions on local radio or television stations about safe places to go.</li>\n<li> Have injuries treated by a medical professional. Wash small wounds with soap and water. To help prevent infection of small wounds, use bandages and replace them if they become soiled, damaged or waterlogged.</li>\n    <li>Some natural hazards, like severe storms or earthquakes, may recur in the form of new storms or aftershocks over the next several days. Take all safety precautions if the hazard strikes again. For an earthquake aftershock, remember to DROP, COVER and HOLD ON just like you did during the initial earthquake.</li>\n    <li>Avoid using the telephone (cellular or landlines) if a large number of homes in your area have been affected by a disaster. Emergency responders need to have the telephone lines available to coordinate their response. During the immediate post-disaster time period, only use the telephone to report life-threatening conditions and call your out-of-town emergency contact.</li>\n<li>    Remain calm. Pace yourself. You may find yourself in the position of taking charge of other people. Listen carefully to what people are telling you, and deal patiently with urgent situations first.</li>\n<li>    If you had to leave your home, return only when local authorities advise that it is safe to do so. Also, be sure to have photo identification available, because sometimes local authorities will only permit people who own property in a disaster-affected area back into the area.</li>\n    <li>Except in extreme emergencies or unless told to do so by emergency officials, avoid driving during the immediate post-disaster period. Keep roads clear for rescue and emergency vehicles. If you must drive, do not drive on roads covered with water. They could be damaged or eroded. </li><li>Additionally, vehicles can begin to float in as little as six inches of water. Vehicles such as trucks and SUVs have larger tires and are more buoyant. However, even though these vehicles are heavier than a standard sedan, the buoyancy caused by the larger amount of air in their tires actually makes these vehicles more likely to float in water than smaller vehicles.</li>\n<li>    If the disaster was widespread, listen to your radio or television station for instructions from local authorities. Information may change rapidly after a widespread disaster, so continue to listen regularly for updates. If the power is still out, listen to a battery-powered radio, television or car radio.</li>\n   <li> If the area was flooded and children are present, warn them to stay away from storm drains, culverts and ditches. Children can get caught and injured in these areas.</li></ul>\n\n', '2013-08-19 14:03:01', 'Public', 'en_US', '', ''),
(49, 'Checking Home after a Disaster or Emergency ', 37, '<h2> Checking Home after a Disaster</h2>If you had to leave your home, return only when local authorities advise that it is safe to do so. Do not cut or walk past colored tape that was placed over doors or windows to mark damaged areas unless you have been told that it is safe to do so. If a building inspector has placed a color-coded sign on the home, do not enter it until you get more information, advice and instructions from your local authorities.\n\nIf you have children, leave them with a relative or friend while you conduct your first inspection of your home after the disaster. The site may be unsafe for children, and seeing the damage firsthand may upset them even more and cause long-term effects, including nightmares.\n\nMake a careful and thorough inspection of your home’s structural elements:\n\n <ul><li>   Check the outside of your home before you enter. Look for loose power lines, broken or damaged gas lines, foundation cracks, missing support beams or other damage. Damage on the outside can indicate a serious problem inside. Ask a building inspector or contractor to check the structure before you enter.</li>\n<li>    If the door is jammed, don’t force it open – it may be providing support to the rest of your home. Find another way to get inside.</li>\n<li>    Sniff for gas. If you detect natural or propane gas, or hear a hissing noise, leave the property immediately and get far away from it. Call the fire department after you reach safety.</li>\n<li>    If you have a propane tank system, turn off all valves and contact a propane supplier to check the system out before you use it again.</li>\n<li>    Beware of animals, such as rodents, snakes, spiders and insects, that may have entered your home. As you inspect your home, tap loudly and often on the floor with a stick to give notice that you are there.</li>\n<li>    Damaged objects, such as furniture or stairs, may be unstable. Be very cautious when moving near them. Avoid holding, pushing or leaning against damaged building parts.</li>\n<li>  Is your ceiling sagging? That means it got wet – which makes it heavy and dangerous. It will have to be replaced, so you can try to knock it down. Be careful: wear eye protection and a hard hat, use a long stick, and stand away from the damaged area. Poke holes in the ceiling starting from the outside of the bulge to let any water drain out slowly. Striking the center of the damaged area may cause the whole ceiling to collapse.</li>\n<li>    Is the floor sagging? It could collapse under your weight, so don’t walk there! Small sections that are sagging can be bridged by thick plywood panels or thick, strong boards that extend at least 8–12 inches on each side of the sagging area.</li>\n<li>    If the weather is dry, open windows and doors to ventilate and/or dry your home.</li>\n<li>    If power is out, use a flashlight. Do not use any open flame, including candles, to inspect for damage or serve as alternate lighting.</li>\n    <li>Make temporary repairs such as covering holes, bracing walls, and removing debris. Save all receipts.</li>\n<li>    Take photographs of the damage. You may need these to substantiate insurance claims later.</li></ul>', '2013-08-19 15:47:00', 'Public', 'en_US', '', ''),
(50, 'Flooding in Central Europe 2013 : Prepare', 38, '<p>You’ll be better prepared to withstand a flood if you have the following items available – packed and ready to go in case you need to evacuate your home</p><ul><li>Water—at least a 3-day supply; one gallon per person per day</li><li> Food—at least a 3-day supply of non-perishable, easy-to-prepare food</li><li>Flashlight</li><li>Battery-powered or hand-crank radio (NOAA Weather Radio, if possible)</li><li> Extra batteries</li><li> First Aid kit</li><li> Medications (7-day supply) and medical items (hearing aids with extra batteries, glasses, contact lenses, syringes, cane)</li><li>Multi-purpose tool</li><li> Sanitation and personal hygiene items</li><li> Copies of personal documents (medication list and pertinent medical information, deed/lease to home, birth certificates, insurance policies)</li><li> Cell phone with chargers</li><li>Family and emergency contact information</li><li> Extra cash,</li><li>Emergency blanket</li><li> Map(s) of the area</li><li>Baby supplies (bottles, formula, baby food, diapers)</li><li>Pet supplies (collar, leash, ID, food, carrier, bowl)</li><li> Tools/supplies for securing your home</li><li> Extra set of car keys and house keys</li><li>Extra clothing, hat and sturdy shoes</li><li>Rain gear</li><li>Insect repellent and sunscreen</li><li> Camera for photos of damage</li></ul>\n', '2013-09-12 11:17:08', 'Public', 'en_US', '1', ''),
(51, 'Flooding in Central Europe 2013 : Response', 39, '<h2>Responding Appropriately During a Flood</h2> <ul><li>Listen to area radio and television stations and a NOAA Weather Radio for possible flood warnings and reports of flooding in progress or other critical information from the National Weather Service (NWS)</li><li> Be prepared to evacuate at a moment’s notice. </li><li>When a flood or flash flood warning is issued for your area, head for higher ground and stay there. </li><li>Stay away from floodwaters. If you come upon a flowing stream where water is above your ankles, stop, turn around and go another way. Six inches of swiftly moving water can sweep you off of your feet.</li><li> If you come upon a flooded road while driving, turn around and go another way. If you are caught on a flooded road and waters are rising rapidly around you, get out of the car quickly and move to higher ground. Most cars can be swept away by less than two feet of moving water. </li><li>Keep children out of the water. They are curious and often lack judgment about running water or contaminated water.</li><li> Be especially cautious at night when it is harder to recognize flood danger.</li><li> Because standard homeowner’s insurance doesn’t cover flooding, it’s important to have protection from the floods associated with hurricanes, tropical storms, heavy rains and other conditions that impact the U.S. For more flood safety tips and information on flood insurance, please visit the National Flood Insurance Program Web site at www.FloodSmart.gov.</li></ul>', '2013-09-12 15:08:50', 'Public', 'en_US', '8', ''),
(53, 'Flooding in Central Europe 2013 : Demo Template', 41, 'Demo', '2013-09-12 15:17:50', 'Public', 'en_US', '15', ''),
(54, 'Flooding in Central Europe 2013 : Recovery', 42, '<h2>Flood Recovery Tips</h2>\n<ul>\n    <li>Return home only when officials have declared the area safe.</li>\n    <li>Before entering your home, look outside for loose power lines, damaged gas lines, foundation cracks or other damage.</li>\n    <li>Parts of your home may be collapsed or damaged. Approach entrances carefully. See if porch roofs and overhangs have all their supports.</li>\n    <li>Watch out for wild animals, especially poisonous snakes that may have come into your home with the floodwater.</li>\n    <li>If you smell natural or propane gas or hear a hissing noise, leave immediately and call the fire department.</li>\n    <li>If power lines are down outside your home, do not step in puddles or standing water.</li>\n    <li>Keep children and pets away from hazardous sites and floodwater.</li>\n    Materials such as cleaning products, paint, batteries, contaminated fuel and damaged fuel containers are hazardous. Check with local authorities for assistance with disposal to avoid risk.\n    <li>During cleanup, wear protective clothing, including rubber gloves and rubber boots.</li>\n<li>Make sure your food and water are safe. Discard items that have come in contact with floodwater, including canned goods, water bottles, plastic utensils and baby bottle nipples. When in doubt, throw it out!</li></ul>\n    Contact your local or state public health department to see if your water supply might be contaminated. You may need to boil or treat it before use. Do not use water that could be contaminated to wash dishes, brush teeth, prepare food, wash hands, make ice or make baby formula!\n\n', '2013-09-12 15:17:49', 'Public', 'en_US', '1', ''),
(55, 'Flooding in Central Europe 2013 : Prepare', 43, '<p>You’ll be better prepared to withstand a flood if you have the following items available – packed and ready to go in case you need to evacuate your home</p><ul><li>Water—at least a 3-day supply; one gallon per person per day</li><li> Food—at least a 3-day supply of non-perishable, easy-to-prepare food</li><li>Flashlight</li><li>Battery-powered or hand-crank radio (NOAA Weather Radio, if possible)</li><li> Extra batteries</li><li> First Aid kit</li><li> Medications (7-day supply) and medical items (hearing aids with extra batteries, glasses, contact lenses, syringes, cane)</li><li>Multi-purpose tool</li><li> Sanitation and personal hygiene items</li><li> Copies of personal documents (medication list and pertinent medical information, deed/lease to home, birth certificates, insurance policies)</li><li> Cell phone with chargers</li><li>Family and emergency contact information</li><li> Extra cash,</li><li>Emergency blanket</li><li> Map(s) of the area</li><li>Baby supplies (bottles, formula, baby food, diapers)</li><li>Pet supplies (collar, leash, ID, food, carrier, bowl)</li><li> Tools/supplies for securing your home</li><li> Extra set of car keys and house keys</li><li>Extra clothing, hat and sturdy shoes</li><li>Rain gear</li><li>Insect repellent and sunscreen</li><li> Camera for photos of damage</li></ul>\n', '2013-09-16 08:25:25', 'Public', 'en_US', '2', '');

-- --------------------------------------------------------

--
-- Table structure for table `rez_page_template`
--

CREATE TABLE IF NOT EXISTS `rez_page_template` (
  `rez_template_id` int(11) NOT NULL,
  `rez_menu_title` varchar(64) NOT NULL,
  `rez_template_content` mediumtext NOT NULL,
  `rez_template_timestamp` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  `rez_template_visibility` varchar(16) NOT NULL,
  `rez_template_locale` varchar(6) NOT NULL DEFAULT 'en_US',
  `created_by` int(16) NOT NULL,
  PRIMARY KEY (`rez_template_id`,`rez_template_locale`),
  KEY `created_by` (`created_by`),
  KEY `created_by_2` (`created_by`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

--
-- Dumping data for table `rez_page_template`
--

INSERT INTO `rez_page_template` (`rez_template_id`, `rez_menu_title`, `rez_template_content`, `rez_template_timestamp`, `rez_template_visibility`, `rez_template_locale`, `created_by`) VALUES
(1, 'Recovery', '<h2>Flood Recovery Tips</h2>\n<ul>\n    <li>Return home only when officials have declared the area safe.</li>\n    <li>Before entering your home, look outside for loose power lines, damaged gas lines, foundation cracks or other damage.</li>\n    <li>Parts of your home may be collapsed or damaged. Approach entrances carefully. See if porch roofs and overhangs have all their supports.</li>\n    <li>Watch out for wild animals, especially poisonous snakes that may have come into your home with the floodwater.</li>\n    <li>If you smell natural or propane gas or hear a hissing noise, leave immediately and call the fire department.</li>\n    <li>If power lines are down outside your home, do not step in puddles or standing water.</li>\n    <li>Keep children and pets away from hazardous sites and floodwater.</li>\n    Materials such as cleaning products, paint, batteries, contaminated fuel and damaged fuel containers are hazardous. Check with local authorities for assistance with disposal to avoid risk.\n    <li>During cleanup, wear protective clothing, including rubber gloves and rubber boots.</li>\n<li>Make sure your food and water are safe. Discard items that have come in contact with floodwater, including canned goods, water bottles, plastic utensils and baby bottle nipples. When in doubt, throw it out!</li></ul>\n    Contact your local or state public health department to see if your water supply might be contaminated. You may need to boil or treat it before use. Do not use water that could be contaminated to wash dishes, brush teeth, prepare food, wash hands, make ice or make baby formula!\n\n', '2013-09-11 10:38:10', 'Public', 'en_US', 1),
(2, 'Prepare', '<p>You’ll be better prepared to withstand a flood if you have the following items available – packed and ready to go in case you need to evacuate your home</p><ul><li>Water—at least a 3-day supply; one gallon per person per day</li><li> Food—at least a 3-day supply of non-perishable, easy-to-prepare food</li><li>Flashlight</li><li>Battery-powered or hand-crank radio (NOAA Weather Radio, if possible)</li><li> Extra batteries</li><li> First Aid kit</li><li> Medications (7-day supply) and medical items (hearing aids with extra batteries, glasses, contact lenses, syringes, cane)</li><li>Multi-purpose tool</li><li> Sanitation and personal hygiene items</li><li> Copies of personal documents (medication list and pertinent medical information, deed/lease to home, birth certificates, insurance policies)</li><li> Cell phone with chargers</li><li>Family and emergency contact information</li><li> Extra cash,</li><li>Emergency blanket</li><li> Map(s) of the area</li><li>Baby supplies (bottles, formula, baby food, diapers)</li><li>Pet supplies (collar, leash, ID, food, carrier, bowl)</li><li> Tools/supplies for securing your home</li><li> Extra set of car keys and house keys</li><li>Extra clothing, hat and sturdy shoes</li><li>Rain gear</li><li>Insect repellent and sunscreen</li><li> Camera for photos of damage</li></ul>\n', '2013-09-16 08:25:25', '', 'en_US', 1),
(3, 'Recovery', '<h2>Returning Home &amp; Recovering after a Wildfire</h2>\n    <ul><li>Do not enter your home until fire officials say it is safe.</li>\n    <li>Use caution when entering burned areas as hazards may still exist, including hot spots, which can flare up without warning.</li>\n    <li>Avoid damaged or fallen power lines, poles and downed wires.</li>\n    <li>Watch for ash pits and mark them for safety—warn family and neighbors to keep clear of the pits also.</li>\n    <li>Watch animals closely and keep them under your direct control. Hidden embers and hot spots could burn your pets’ paws or hooves.</li>\n    <li>Follow public health guidance on safe cleanup of fire ash and safe use of masks.</li>\n    <li>Wet debris down to minimize breathing dust particles.</li>\n    <li>Wear leather gloves and heavy soled shoes to protect hands and feet.</li>\n    <li>Cleaning products, paint, batteries and damaged fuel containers need to be disposed of properly to avoid risk.</li></ul>\n<br>\n<h2>\nEnsure your food and water are safe\n</h2>\n<ul><li>Discard any food that has been exposed to heat, smoke or soot.</li><li>\nDo NOT ever use water that you think may be contaminated to wash dishes, brush teeth, prepare food, wash hands, make ice or make baby formula.</li></ul>\n<br>\n<h2>Inspecting your home</h2>\n<ul>\n    <li>If there is no power, check to make sure the main breaker is on. Fires may cause breakers to trip. If the breakers are on and power is still not present, contact the utility company.</li>\n    <li>Inspect the roof immediately and extinguish any sparks or embers. Wildfires may have left burning embers that could reignite.</li>\n    <li>For several hours afterward, recheck for smoke and sparks throughout the home, including the attic. The winds of wildfires can blow burning embers anywhere. Keep checking your home for embers that could cause fires.</li>\n    <li>Take precautions while cleaning your property. You may be exposed to potential health risks from hazardous materials. </li>\n\n<li>Debris should be wetted down to minimize health impacts from breathing dust particles.</li>\n\n̶<li>Use a two-strap dust particulate mask with nose clip and coveralls for the best minimal protection.</li>\n\n̶<li>Wear leather gloves to protect hands from sharp objects while removing debris.</li>\n\n̶<li>Wear rubber gloves when working with outhouse remnants, plumbing fixtures, and sewer piping. They can contain high levels of bacteria.</li>\n\n̶<li>Hazardous materials such as kitchen and bathroom cleaning products, paint, batteries, contaminated fuel, and damaged fuel containers need to be properly handled to avoid risk. Check with local authorities for hazardous disposal assistance.</li>\n<li>If you have a propane tank system, contact a propane supplier. Turn off valves on the system, and leave valves closed until the supplier inspects your system.</li>\n<li>If you have a heating oil tank system, contact a heating oil supplier for an inspection of your system before using.</li>\n<li>Visually check the stability of the trees. Any tree that has been weakened by fire may be a hazard.</li>\n<li> Look for burns on the tree trunk. If the bark on the trunk has been burned off or scorched by very high temperatures completely around the circumference, the tree will not survive and should be considered unstable.</li>\n\n<li> Look for burnt roots by probing the ground with a rod around the base of the tree and several feet away from the base. If the roots have been burned, you should consider this tree very unstable.</li>\n\n <li>A scorched tree is one that has lost part or all of its leaves or needles. Healthy deciduous trees are resilient and may produce new branches and leaves as well as sprouts at the base of the tree. Evergreen trees may survive when partially scorched but are at risk for bark beetle attacks</li></ul>', '2013-09-09 10:49:49', 'Public', 'en_US', 1),
(6, 'Prepare', '				<p>Home chemical accidents can result from trying to \nimprove the way a product works by adding one substance to another, not \nfollowing directions for use of a product, or by improper storage or \ndisposal of a chemical. Fortunately, a few simple precautions can help \nyou avoid many chemical emergencies. \n			            \n					\n				</p>\n			<b>Avoid mixing chemicals</b>, even common household products. Some combinations, such as ammonia and bleach, can create toxic gases.<li><b>Always read and follow the directions</b>\n when  using a new product. Some products should not be used in small, \nconfined spaces to avoid inhaling dangerous vapors. Other products \nshould not be used without gloves and eye protection to help prevent the\n chemical from touching your body. </li><li><b>Store chemical products properly.</b>\n Non-food products should be stored tightly closed in their original \ncontainers so you can always identify the contents of each container and\n how to properly use the product. Better yet – don’t store chemicals at \nhome. Buy only as much of a chemical as you think you will use. If you \nhave product left over, try to give it to someone who will use it. Or \nsee below for tips on proper disposal. </li><li><b>Beware of fire.</b>\n Never smoke while using household chemicals. Don''t use hair spray, \ncleaning solutions, paint products, or pesticides near the open flame of\n an appliance, pilot light, lighted candle, fireplace, wood burning \nstove, etc. Although you may not be able to see or smell them, vapor \nparticles in the air could catch fire or explode.</li><li><b>Clean up any spills immediately</b>\n with some rags, being careful to protect your eyes and skin. Allow the \nfumes in the rags to evaporate outdoors in a safe place, then wrap them \nin a newspaper and place the bundle in a sealed plastic bag. Dispose of \nthese materials with your trash. If you don''t already have one, buy a \nfire extinguisher that is labeled for A, B, and C class fires and keep \nit handy.</li><li><b>Dispose of unused chemicals properly</b>. \nImproper disposal can result in harm to yourself or members of your \nfamily, accidentally contaminate our local water supply, or harm other \npeople or wildlife.</li><p align="LEFT">Many \nhousehold chemicals can be taken to your local household hazardous waste\n collection facility. Many facilities accept pesticides, fertilizers, \nhousehold cleaners, oil-based paints, drain and pool cleaners, \nantifreeze, and brake fluid. Some products can be recycled, which is \nbetter for our environment. If you have questions about how to dispose \nof a chemical, call the facility or the environmental or recycling \nagency to learn the proper method of disposal.</p>\n ', '2013-09-11 18:03:24', 'Public', 'en_US', 1),
(7, 'Response', 'There\n are many organizations that help the community in an emergency, such as\n police, fire, and sheriff departments, the American Red Cross, and \ngovernment agencies. All of these groups coordinate their activities \nthrough the local office of emergency management. In many areas there \nare local Hazardous Materials (Haz-Mat) Teams, who are trained to \nrespond to chemical accidents. </p><p align="LEFT">If\n an accident involving hazardous materials occurs, you will be notified \nby the authorities as to what steps to take. You may hear a siren, be \ncalled by telephone, or emergency personnel may drive by and give \ninstructions over a loudspeaker. Officials could even come to your door.\n If you hear a warning signal, you should go indoors and listen to a \nlocal Emergency Alert System (EAS) station for emergency instructions \nfrom county or state officials.</p>\n\n<h1>\nImportant Points to Remember\n</h1>\n	\n  <br><li>In \nthe event of an emergency, follow the instructions of the authorities \ncarefully. They know best how to protect you and your family. Listen to \nyour emergency broadcast stations on radio and TV.</li><li><font><font><font>Use your phone only in life-threatening emergencies, and then call the <a href="http://www.aapcc.org" target="_blank">Poison Control Center</a> (1-800-222-1222), Emergency Medical Services (EMS), 9-1-1, or the operator immediately.</font></font></font></li><li><font><font><font><font>If\n you are told to "shelter in place", go inside, close all windows and \nvents and turn off all fans, heating or cooling systems. Take family \nmembers and pets to a safe room, seal windows and doors, and listen to \nemergency broadcast stations for instructions.</font></font></font></font></li><li><font><font><font>If you are told to evacuate immediately, follow your <a href="http://www.redcross.org/prepare/location/home-family/plan" target="_blank"><i><u>Family Disaster Plan</u></i></a>. Take your <a href="http://www.redcross.org/prepare/location/home-family/get-kit" target="_blank">Family Disaster Supplies Kit</a>.\n Pack only the bare essentials, such as medications, and leave your home\n quickly. Follow the traffic route authorities recommend. Don''t take \nshort cuts on the way to the shelter.</font></font></font></li><li><font><font>If\n you find someone who appears to have been injured from chemical \nexposure, make sure you are not in danger before administering First \nAid.</font></font></li><li><font>And lastly, remember, the best way to protect yourself and your family is to be prepared.</font></li>\n 	\n	\n	\n\n	<h1>\n	In Case of Poisoning\n\n		\n	</h1>\nThe most common\n home chemical emergencies involve small children eating medicines. Keep\n all medicines, cosmetics, cleaning products, and other household \nchemicals out of sight and out of reach of children. Experts in the \nfield of chemical manufacturing suggest that doing so could eliminate up\n to 75 percent of all poisoning of small children.</font></p><p align="LEFT"><font>If\n someone in your home does eat or drink a non-food substance, find the \ncontainer it came out of immediately and take it with you to the phone. \nCall the <a href="http://www.1-800-222-1222.info" target="_blank">Poison Control Center</a> (<b>1-800-222-1222</b>), or Emergency Medical Services (EMS), or 9-1-1, or call the operator and tell them exactly what your child ingested.</font></p><p align="LEFT"><font>Follow\n their instructions carefully. Please be aware that the First Aid advice\n found on the container may not be appropriate. So, do not give anything\n by mouth until you have been advised by medical professionals.</p>', '2013-09-11 18:03:15', 'Public', 'en_US', 1),
(8, 'Response', '<h2>Responding Appropriately During a Flood</h2> <ul><li>Listen to area radio and television stations and a NOAA Weather Radio for possible flood warnings and reports of flooding in progress or other critical information from the National Weather Service (NWS)</li><li> Be prepared to evacuate at a moment’s notice. </li><li>When a flood or flash flood warning is issued for your area, head for higher ground and stay there. </li><li>Stay away from floodwaters. If you come upon a flowing stream where water is above your ankles, stop, turn around and go another way. Six inches of swiftly moving water can sweep you off of your feet.</li><li> If you come upon a flooded road while driving, turn around and go another way. If you are caught on a flooded road and waters are rising rapidly around you, get out of the car quickly and move to higher ground. Most cars can be swept away by less than two feet of moving water. </li><li>Keep children out of the water. They are curious and often lack judgment about running water or contaminated water.</li><li> Be especially cautious at night when it is harder to recognize flood danger.</li><li> Because standard homeowner’s insurance doesn’t cover flooding, it’s important to have protection from the floods associated with hurricanes, tropical storms, heavy rains and other conditions that impact the U.S. For more flood safety tips and information on flood insurance, please visit the National Flood Insurance Program Web site at www.FloodSmart.gov.</li></ul>', '2013-09-11 15:49:59', 'Public', 'en_US', 1),
(9, 'Response', '<h2>Diagnosing and Treating the Flu</h2>\n\nIt may be difficult to tell if you are suffering from the flu or another illness. If you develop flu-like symptoms and are concerned about possible complications, consult your health care provider.<br><br>\n<h3>Common Flu Symptoms</h3>\n<ul>\n   <li> High fever</li>\n    <li>Severe body aches</li>\n    <li>Headache</li>\n    <li>Extreme tiredness</li>\n    <li>Sore throat</li>\n    <li>Cough</li>\n    <li>Runny or stuffy nose</li>\n    <li>Vomiting and/or diarrhea (more common in children than in adults)</li>\n\n</ul>\n<h3>Potential Risks and Serious Complications of the Flu</h3>\n<ul>\n    <li>Bacterial pneumonia</li>\n    <li>Dehydration</li>\n    <li>Worsening of chronic medical conditions</li>\n    <li>Ear infections</li>\n  <li>  Sinus problems</li></ul>\n\n\n<h3>Caregiving – How to Treat the Flu</h3>\n<ul>\n    <li>Designate one person as the caregiver.\n    </li><li>Keep everyone’s personal items separate. All household members should avoid sharing pens, papers, clothes, towels, sheets, blankets, food or eating utensils unless they have been cleaned between uses.</li>\n    <li>Disinfect doorknobs, switches, handles, computers, telephones, toys and other surfaces that are commonly touched around the home or workplace.</li>\n    <li>Wash everyone’s dishes in the dishwasher or by hand using very hot water and soap.</li>\n    <li>Wash everyone’s clothes in a standard washing machine as you normally would. Use detergent and very hot water and wash your hands after handling dirty laundry.</li>\n    <li>Wear disposable gloves when in contact with or cleaning up body fluids</li>\n</ul>\n', '2013-09-11 18:03:18', 'Public', 'en_US', 1),
(10, 'Prepare', '<h2>How to Prepare for a Tsunami</h2><ul>    <li>Find out if your home, school, workplace or other frequently visited locations are in tsunami hazard areas.</li>    <li>Know the height of your street above sea level and the distance of your street from the coast or other high-risk waters. Evacuation orders may be based on these numbers.</li>    <li>Plan evacuation routes from your home, school, workplace and other places you could be where tsunamis present a risk. If possible, pick areas 100 feet (30 meters) above sea level or go as far as 2 miles (3 kilometers) inland, away from the coastline. If you cannot get this high or far, go as high or far as you can. Every foot inland or upward may make a difference. You should be able to reach your safe location on foot within 15 minutes.</li>    <li>Find out what the school evacuation plan is. Find out if the plan requires you to pick your children up from school or from another location. Telephone lines during a tsunami watch or warning may be overloaded, and routes to and from schools may be jammed.</li>    <li>Practice your evacuation routes. Familiarity may save your life. Be able to follow your escape route at night and during inclement weather. Practicing your tsunami survival plan makes the appropriate response more of a reaction, requiring less thinking during an actual emergency.</li>    <li>If you are a tourist, familiarize yourself with local tsunami evacuation protocols. You may be able to safely evacuate to the third floor and higher in reinforced concrete hotel structures.</li></ul>', '2013-09-11 18:03:11', 'Public', 'en_US', 1),
(11, 'Response', '<h3>If you are in a coastal area and feel an earthquake that lasts 20 seconds or longer:</h3><ul><li>    Drop, cover and hold on. You should first protect yourself from the earthquake.</li><li>    When the shaking stops, gather members of your household and move quickly to higher ground away from the coast. A tsunami may be coming within minutes.</li><li>    Avoid downed power lines and stay away from buildings and bridges from which heavy objects might fall during an aftershock.</li></ul><h3>What to Do During a Tsunami Watch</h3><ul><li>    Use a NOAA Weather Radio or tune to a Coast Guard emergency frequency station or a local radio or television station for updated emergency information.</li><li>    Locate household members and review evacuation plans. Be ready to move quickly if a tsunami warning is issued.</li></ul><h3>What to Do During a Tsunami Warning</h3><ul><li>  If you hear an official tsunami warning or detect signs of a tsunami, evacuate at once.</li><li>    Take your emergency preparedness kit. Having supplies will make you more comfortable during the evacuation.</li><li>    Take your pets with you. If it is not safe for you, it’s not safe for them.</li><li>    Get to higher ground as far inland as possible. Watching a tsunami could put you in grave danger. If you can see the wave, you are too close to escape it.</li></ul>', '2013-09-11 18:07:06', 'Public', 'en_US', 1),
(12, 'Recovery', '<h2>What to Do After a Tsunami</h2><ul><li> Continue using a NOAA Weather Radio or tuning to a Coast Guard station or a local radio or television station for the latest updates.</li><li>    Return home only after local officials tell you it is safe. A tsunami is a series of waves that may continue for hours. Do not assume that after one wave the danger is over. The next wave may be larger than the first one.</li><li>    Check yourself for injuries and get first aid as needed before helping injured or trapped persons.</li><li>    If someone needs to be rescued, call professionals with the right equipment to help. Many people have been killed or injured trying to rescue others.</li><li>    Help people who require special assistance—infants, elderly people, those without transportation, people with disabilities and large families who may need additional help in an emergency situation.</li><li>    Avoid disaster areas. Your presence might interfere with emergency response operations and put you at further risk from the residual effects of floods.</li><li>    Use the telephone only for emergency calls.</li><li>    Stay out of any building that has water around it. Tsunami water can cause floors to crack or walls to collapse.</li><li>    Use caution when re-entering buildings or homes. Tsunami-driven floodwater may have damaged buildings where you least expect it. Carefully watch every step you take.</li><li>    To avoid injury, wear protective clothing and be cautious when cleaning up.</li><li>    Watch animals closely and keep them under your direct control.</li></ul>', '2013-09-11 18:16:05', 'Public', 'en_US', 1),
(13, 'Prepare', '\n<h3>Flu Prevention is the Best Preparation<h3>\n\nA flu vaccine is available in the U.S. every year. Get your flu shot as soon as it is available for the best chance of protection.\nAlways Practice Good Health Habits to Maintain Your Body’s Resistance to Infection\n<br/><ul>\n    <li>Eat a balanced diet.</li>\n    <li>Drink plenty of fluids.</li>\n    <li>Exercise daily.</li>\n    <li>Manage stress.</li>\n    <li>Get enough rest and sleep</li></ul>\n<br/>\n<h3>Take These Common Sense Steps to Stop the Spread of Germs</h3>\n<ul>\n   <li> Wash hands frequently with soap and water or an alcohol-based hand sanitizer.</li>\n    <li>Avoid or minimize contact with sick people (a minimum three feet distancing is recommended).</li>\n    <li>Avoid touching your eyes, nose and mouth.</li>\n    <li>Cover your mouth and nose with tissues when you cough and sneeze. If you don’t have a tissue, cough or sneeze into the crook of your elbow.</li>\n    <li>Stay away from others as much as possible when you are sick.</li>\n    <li>Adopt business/school practices that encourage employees/students to stay home when they have flu symptoms.</li>\n</ul>\n', '2013-09-11 18:16:03', 'Public', 'en_US', 1);

-- --------------------------------------------------------

--
-- Table structure for table `rez_template_scenario`
--

CREATE TABLE IF NOT EXISTS `rez_template_scenario` (
  `template_id` int(11) NOT NULL,
  `disaster_type` int(11) NOT NULL,
  PRIMARY KEY (`template_id`,`disaster_type`),
  KEY `incident_id` (`template_id`,`disaster_type`),
  KEY `disaster_type` (`disaster_type`),
  KEY `incident_id_2` (`template_id`),
  KEY `template_id` (`template_id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1 COMMENT='Stores mapping between Disaster Type and Incident';

--
-- Dumping data for table `rez_template_scenario`
--

INSERT INTO `rez_template_scenario` (`template_id`, `disaster_type`) VALUES
(1, 2),
(2, 2),
(8, 2),
(6, 5),
(7, 5),
(9, 6),
(13, 6),
(10, 10),
(11, 10),
(12, 10),
(3, 13);

-- --------------------------------------------------------

--
-- Table structure for table `scenario`
--

CREATE TABLE IF NOT EXISTS `scenario` (
  `incident_id` bigint(20) NOT NULL,
  `disaster_type` int(11) NOT NULL,
  PRIMARY KEY (`incident_id`,`disaster_type`),
  KEY `incident_id` (`incident_id`,`disaster_type`),
  KEY `disaster_type` (`disaster_type`),
  KEY `incident_id_2` (`incident_id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1 COMMENT='Stores mapping between Disaster Type and Incident';

--
-- Dumping data for table `scenario`
--

INSERT INTO `scenario` (`incident_id`, `disaster_type`) VALUES
(1, 2),
(2, 2),
(4, 2),
(11, 2),
(5, 3),
(6, 3),
(7, 3),
(8, 6),
(2, 7),
(10, 7),
(1, 8),
(12, 9),
(5, 10),
(13, 13),
(3, 15),
(9, 16);

-- --------------------------------------------------------

--
-- Table structure for table `sessions`
--

CREATE TABLE IF NOT EXISTS `sessions` (
  `session_id` varchar(64) NOT NULL,
  `sess_key` varchar(64) NOT NULL,
  `secret` varchar(64) NOT NULL,
  `inactive_expiry` bigint(20) NOT NULL,
  `expiry` bigint(20) NOT NULL,
  `data` text,
  PRIMARY KEY (`session_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

--
-- Dumping data for table `sessions`
--

INSERT INTO `sessions` (`session_id`, `sess_key`, `secret`, `inactive_expiry`, `expiry`, `data`) VALUES
('0d6sr6ppdeeod8stuv58j8uac0', '19ed174a0ba892b04939fd196a29d0ae', '49b617922ef3c599bcf0107bbd9faf67', 1374072171, 1374064894, NULL),
('0dt46dusp085djg4372otqk181', 'e62954ff49ddc65c22dc5361af178ab6', '55d81fd263c8a4e960808ac2e1754ee3', 1374006792, 1373997213, NULL),
('0mko5sob18oc0suh4aqcjl7ak1', 'bf0b479c822cadc64342c4632cfe67ce', '2ade314afebdba877f60104925eee3d4', 1377442322, 1377423924, NULL),
('11aqkptrla15uocd1b52qnf1n4', '4aa5b5944a586b0f58131caa6e9b6896', 'c495ca9bed2e84167cbd8b9ebe1f79f5', 1376852426, 1376843474, NULL),
('1ie2v6mj2etokiembvsj4qk1j0', '46e61e4cc80194683db84f379eb8442e', '53e42ec56bcceb97b0850047487b5d02', 1376999352, 1376995242, NULL),
('1rkbkqjqmcttuu4mhqfcefjhs0', '81759584c8b76d44c5a831c1f2f32e26', '1f8d6fbfa11dfb77137168c903b3037b', 1376837899, 1376821206, NULL),
('24q4q7ggqb0bsnk4a9ii40qju5', '2609c439f6fb727309829463848c7654', 'eeb42d79b8da2d7057cc125547b78082', 1374414846, 1374405922, NULL),
('2jp1qs851gabii98e7e7151vv6', '180ba872ba827217cbc3540dc9fa9c29', '0d7a7124509baeade679ad6260590d51', 1376857377, 1376854466, NULL),
('2o35df9207rv2ov5psa2tb5bh5', '7c96ba6b05a6bfedba8548cf95f3083b', '5b1c7b2d2b2f052d46aa3541de93e536', 1378995852, 1378995764, NULL),
('2q3042dargg687o58u7iqbepm0', '1faefd6ee6aa6eea1bdfac63cb906724', '0383fa57d468f95cdc2cd35776b3affe', 1379355079, 1379353543, NULL),
('2qil3e2p831jl4fgo76tto2hq6', 'f07411a5bd1672c886228def4898ef40', '11be25eb43d59860dd0cc9c2e0ba35bd', 1378736581, 1378735828, NULL),
('43phkgt7rd4pkoh372hq5mokg7', '4ee7bb90485737014f77c9f672566247', 'e120da7b1962de6bd9a5f5c553cce520', 1378652131, 1378630224, NULL),
('47p2e3p2msqfsl93fh0vneul02', '411c58d5ca755c12d01804730a384627', 'e643729fa0887f9c773bfb0c3feaa2bb', 1376671299, 1376671275, NULL),
('50a8p7fqgjfjqudl0ir3e40v71', '593c8b6b2052fdea77baf2d7988e72cf', '2c3f369d27e1f218c879bb416b42f4b4', 1379000635, 1378993434, NULL),
('58pk74qc4kknms86c29t15jhk7', '2fcb79c6d7e33a6413d04eaa6ae8630e', '3a8109a0b394b28cfde691ef5079a12c', 1373107678, 1373107028, NULL),
('5k87keth5gdk8ferogbrl327g0', '22783938afd22795e5ce45d259232391', 'dd79dc1086e31eb311f7478442280238', 1373571431, 1373530864, NULL),
('5msttln3q0fmr1gn4bg6dm0dg3', 'afb387fe43ad7c914023cc30364ca212', '912913dea8c8a28edf8d3effa74ad762', 1372947537, 1372936429, NULL),
('63lk7sisr0gh1kvlu47ep65h93', '2415a39d3396448d617c09cca83cd067', '1fe2ac359ba6a25a9a5fd81f074836fe', 1377938452, 1377938344, NULL),
('69mb5g09n2gu7s498mme5o9lo4', '2497148975c713f1a6a74f54f5c05d48', 'f9843f7eea0332a979d988e1e14a9485', 1373804882, 1373799975, NULL),
('6s6qnlcubgk096udhbibn524u6', '087ca54fd97e060bba2e8a8c42bf4cbd', 'fccc9d76f7ae60c8946ea26f20d3fdd3', 1376765520, 1376746346, NULL),
('79noad5og5jg1uj1bjh4l8pt97', '5b79236fb408bd0e41fdf773264491c6', 'c66f8910e8eb7cb1dcc55ab2c4844afe', 1379162581, 1379162399, NULL),
('9h98vh9jkg3r6jq1e0g7ji0r17', '7d3b4c3711bdd9990170aff053c174a5', '489a7629de2bee31a69655c51ab40baf', 1378064650, 1378017860, NULL),
('aoc9uujoldurq0a7mk695qjv50', '6d46cb70e8dc7b6539618b7cc943094b', '4559f8466b457e3cd0ec1531aa96ab6a', 1376661736, 1376656946, NULL),
('b68ehusog70v4pn346rabkfim6', '80684a2e4ddcb00d7b90cd20408262a8', 'a96b0bc7eb932aa87c5abd8a8bd8d9f4', 1373309605, 1373292155, NULL),
('bbon5hnodjjnqo590kdn97o0r4', '8fce1d31bbd847394db99f8529cb11b3', 'b81d024e4da84f0957e98af932c75295', 1376910717, 1376901563, NULL),
('bc7fmt81aeino265d6frp875r0', '5b4c91cc43b22370e695a058a77b1ef6', 'ff73479b65dbd7b6cc40738cf12b68e0', 1374432647, 1374431315, NULL),
('bvd2ddho2apfcb1fpggakevlb7', '04caed3d7fd13913dd886ef5eea7ef67', 'e3858e29dd90262401f535b31693681c', 1374061862, 1374051166, NULL),
('c1eq5dehdtql36a67ojd97t204', 'f055bda4e9ed55babd5d6527fb1dbdd2', '4208ddcc5d341b59f29416d8da389890', 1379333448, 1379332476, NULL),
('cf8sl0kn21095sago0bivvh286', '7267e3c64b644e09d1eeebb20144dfd3', '6c9549c96efda961ba956c973ce827d1', 1378734855, 1378702863, NULL),
('d87v6ekat6uqhiqrlo2gvcasj3', 'cef9e9c0ef35c4f0efc50864fa1fb090', '5a8c51abce2cd15e1470bd6c4b00e644', 1376943844, 1376923160, NULL),
('escav90dai9l7i7cpkt4f3a1h6', 'b9e190074595a863cbbe1b1ae717fd58', '5eddc57d7826d6d232b81583c3a6cb59', 1376667979, 1376664766, NULL),
('evr5k125jfdh75936htimn0t73', 'd02bd473bf0e6be3cb4b3c967cb32d58', 'd181771214e56474d2990542fb36df60', 1376806989, 1376801939, NULL),
('f5b5aah37357snmi75mqcbf1d0', '8a5dbebee87c44bc91c71c7115923998', '21260e5b239dda4f3fe143e05eacaa7e', 1379341781, 1379341664, NULL),
('fgo55dqcevd9pdcgmjl01mg3q4', '240f2e2ce37e4517921661e7dd4772ca', '0253679f1c47c67499bf31a12328355c', 1373744301, 1373740112, NULL),
('g3f68hj3q07haftg88vjnjbkl2', '2b9aec7af2d0dc141affcb57f41d5f44', '6ca17d8e6fb7bf5ae0cd6811ddec27ed', 1378735565, 1378735564, NULL),
('hojvagv8ts2p17s571mjr831m1', '2f397092cc2f8e2d6a6eb0a470fa1382', '866568b01b688edd41daa21ef9cc7176', 1374166868, 1374149318, NULL),
('iagu4i9c6c9dedlgik6437dkq1', '788dfc8424f663dc4bbd6072f711ae32', '3bff8e43d98df10bd40c9ed0e0bbd13c', 1373905969, 1373905960, NULL),
('ibbpkl05s372u5b7rje3c4qsq3', 'e967d35a62a86f1950a243dc3f7343aa', '4b223e6d70b906bab2338c6243ac4365', 1378974203, 1378969574, NULL),
('irprde4h6bv74753fke5mip565', '5fb0c27caae86bd905b0d0ec2aa30329', 'b961db71b9312f5c54ace6aec843b63d', 1373385931, 1373362733, NULL),
('j7bnu46bdm4fa0k8k2d7mglee5', 'e53945f3942593b098f3ffc8bb71781b', '59e3929b1af6a99f60b43e921d7bc8a8', 1373886197, 1373883324, NULL),
('jbqoa5cn3nsu9h5an0qdvd7rk1', '9287f7480c93658ed9cfcc4b802c12d9', '9ebb765a11b9c03d1b9af639e2dccfd9', 1376922652, 1376922651, NULL),
('juc4fj5ujnal8kvq850f329sm2', '36919031545c152678676212f4ce433f', '49cc50ea8a9697acd342fa30bb7ed569', 1377984487, 1377970834, NULL),
('k0q4cjshe428j95n9md6bg12o0', '937a1002c8a144dd87a467da28db6f91', 'a84cc264fdc266b41b54103e7fe3c224', 1376563827, 1376560235, NULL),
('k704ukd5h53lvvkq16jptk8pf6', '45833b1191a3c3edeac003c2eb6738c2', 'c0d98fa07dd04cf5a345bc1b525c77a7', 1373061410, 1373021048, NULL),
('l7pv2i4n2upmpi3gh0otspm4k5', '343e7f4aa7d11a7233ffe4c2e8ac2854', 'c84a90b2b622253a9aaf1b05d3da28ec', 1376845838, 1376845755, NULL),
('lkighddb1ul9l6b5cisg23jd14', '431d564a837841894f3102f32d0a50ed', '1599c338d06344cf683000ff68c12763', 1378701744, 1378701583, NULL),
('mvqv91aqog80bnet055mi3mh37', '28c318035fee3d978948f1bc893d6d78', 'a0aff61f8d53503f3bece893a6582e6a', 1377014269, 1377007776, NULL),
('nu2ua6kns3shc0fs9npshu0ut0', '8221f3d3cbfbd85de9cfc7173963c9fc', '50f70f739c692828737810aae46fb2a9', 1373138095, 1373115250, NULL),
('oh38ctkkofc4jnldn4jhot68p3', 'c6112351b26ee3ef41f547137398c5b0', '434c53a357d16de956e591596b86ceda', 1374269318, 1374206367, NULL),
('oo7c5det542ce58tghorm55ti0', '63c05edc88a424b4e2f23c1b47f211cb', '0a395691598d3a288497392a0180d6e5', 1373932762, 1373904132, NULL),
('p14n80i2hh0b18e6m2bs9d4ej6', '851990948b8a301fd62a869cbe481592', 'd609700400f9682156b2df2d37948de9', 1373738834, 1373734134, NULL),
('p2dh3dijr0n8lebgcnd603q9h0', '6de003d5f8cb1b3399866787adf5b97f', '77c162a2a4c2c222d9f21f3b2d5431ff', 1373631291, 1373611171, NULL),
('q37mv79upk7go61onouq8cvt62', '890dc041df8b9ee843e74906acf7fdcd', 'c4fa7c1d3263bee2955bbe6f27662b23', 1378736448, 1378736184, NULL),
('q6bd6lc0j7hk1698gk1fu00dg0', 'cc9cf80f034909785a7d1fa771a8d06f', 'f06669d306c28cd00254cd2f041be3f8', 1378150427, 1378146109, NULL),
('r4u3aa1u2di3aqf8ploqmppfj3', 'a9694dcc62b22f1d434436a6f5892ec1', '66ab2b4d08e8e2a3a11ff435f6b09346', 1373486286, 1373448751, NULL),
('rds0hgi4suib3uq7tjlt4kcsc7', 'a47615e17a307e32bf22e02d5b091318', '1be737cf4d08808f315d4f232cc20688', 1378997374, 1378987424, NULL),
('rhop6mn6e36i8v99qg909676s3', 'a954c11e7c5b774019faec55a0d0505e', 'dfd2d1621d340e0ea77faa7909fe40ac', 1376818590, 1376818567, NULL),
('rodlgsorct0t9ar0j72n6emca1', '04bb0d42b260926d3da23b403883194f', '400c14a9e4af9192ef0aabc4270b5ebc', 1379080453, 1379078614, NULL),
('rso6ndmv1r0q3k353s5c319v74', '70a9644373c759963e925f6aee3800a3', 'a5b7c7e8771aac95dd2661b2adda5cd4', 1374351283, 1374316454, NULL),
('s99l24huftan6g5tge4o70rmt2', '8ca71722e3f9587fc8ba4e1005084165', 'd8be3c391188040bd12dec31a4c51978', 1372963070, 1372956576, NULL),
('tiupubbfu61sef0ji9n25gv7o3', '111a4f54d02f8e18d0f8cf088989c979', 'a327f8207e103bf6485973c93f3a62f7', 1377928568, 1377928567, NULL),
('ulepv6bbnatbcicvspm652gtl2', 'd75ce5e16895cc71bf74bc29c8d03750', '6e90b4ef29b786580e93a6d6c28875f6', 1376576137, 1376573224, NULL),
('v58trmjr72l8sbna945q5rkpt6', '985d42d18c787364fce95ffb77aad4c3', '9a412ab91f685a8035d7e22a5ba06d1d', 1373227963, 1373227822, NULL),
('vs90gvg8vr8dre894bha4fnhe6', '1e5dc0ff537b98e7115f1b85783cd473', 'c1635d0177e941ebf8ce748a517220c1', 1378929018, 1378904352, NULL);

-- --------------------------------------------------------

--
-- Table structure for table `sys_group_to_module`
--

CREATE TABLE IF NOT EXISTS `sys_group_to_module` (
  `group_id` int(11) NOT NULL,
  `module` varchar(60) NOT NULL,
  `status` varchar(60) NOT NULL,
  PRIMARY KEY (`group_id`,`module`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

--
-- Dumping data for table `sys_group_to_module`
--

INSERT INTO `sys_group_to_module` (`group_id`, `module`, `status`) VALUES
(1, 'admin', 'enabled'),
(1, 'arrive', 'enabled'),
(1, 'eap', 'enabled'),
(1, 'em', 'enabled'),
(1, 'ha', 'enabled'),
(1, 'home', 'enabled'),
(1, 'inw', 'enabled'),
(1, 'mayank', 'enabled'),
(1, 'mpres', 'enabled'),
(1, 'pfif', 'enabled'),
(1, 'plus', 'enabled'),
(1, 'pop', 'enabled'),
(1, 'pref', 'enabled'),
(1, 'report', 'enabled'),
(1, 'rez', 'enabled'),
(1, 'stat', 'enabled'),
(1, 'xst', 'enabled'),
(2, 'eap', 'enabled'),
(2, 'home', 'enabled'),
(2, 'inw', 'enabled'),
(2, 'pref', 'enabled'),
(2, 'report', 'enabled'),
(2, 'rez', 'enabled'),
(2, 'xst', 'enabled'),
(3, 'eap', 'enabled'),
(3, 'home', 'enabled'),
(3, 'inw', 'enabled'),
(3, 'report', 'enabled'),
(3, 'rez', 'enabled'),
(3, 'xst', 'enabled'),
(5, 'eap', 'enabled'),
(5, 'home', 'enabled'),
(5, 'inw', 'enabled'),
(5, 'pref', 'enabled'),
(5, 'report', 'enabled'),
(5, 'rez', 'enabled'),
(5, 'stat', 'enabled'),
(5, 'tp', 'enabled'),
(5, 'xst', 'enabled'),
(6, 'eap', 'enabled'),
(6, 'em', 'enabled'),
(6, 'ha', 'enabled'),
(6, 'home', 'enabled'),
(6, 'inw', 'enabled'),
(6, 'pref', 'enabled'),
(6, 'report', 'enabled'),
(6, 'rez', 'enabled'),
(6, 'stat', 'enabled'),
(6, 'tp', 'enabled'),
(6, 'xst', 'enabled'),
(7, 'eap', 'enabled'),
(7, 'home', 'enabled'),
(7, 'inw', 'enabled'),
(7, 'pref', 'enabled'),
(7, 'report', 'enabled'),
(7, 'rez', 'enabled'),
(7, 'stat', 'enabled'),
(7, 'xst', 'enabled');

-- --------------------------------------------------------

--
-- Table structure for table `sys_user_groups`
--

CREATE TABLE IF NOT EXISTS `sys_user_groups` (
  `group_id` int(11) NOT NULL,
  `group_name` varchar(40) NOT NULL,
  PRIMARY KEY (`group_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

--
-- Dumping data for table `sys_user_groups`
--

INSERT INTO `sys_user_groups` (`group_id`, `group_name`) VALUES
(1, 'Administrator'),
(2, 'Registered User'),
(3, 'Anonymous User'),
(5, 'Hospital Staff'),
(6, 'Hospital Staff Admin'),
(7, 'Researchers');

-- --------------------------------------------------------

--
-- Table structure for table `sys_user_to_group`
--

CREATE TABLE IF NOT EXISTS `sys_user_to_group` (
  `group_id` int(11) NOT NULL,
  `p_uuid` varchar(128) NOT NULL,
  KEY `p_uuid` (`p_uuid`),
  KEY `group_id` (`group_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

--
-- Dumping data for table `sys_user_to_group`
--

INSERT INTO `sys_user_to_group` (`group_id`, `p_uuid`) VALUES
(3, '3'),
(1, '1'),
(3, '2');

-- --------------------------------------------------------

--
-- Table structure for table `template_translations`
--

CREATE TABLE IF NOT EXISTS `template_translations` (
  `template_id` int(11) NOT NULL,
  `locale` varchar(10) NOT NULL,
  KEY `template_id` (`template_id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Table structure for table `users`
--

CREATE TABLE IF NOT EXISTS `users` (
  `user_id` int(16) NOT NULL AUTO_INCREMENT,
  `p_uuid` varchar(128) NOT NULL,
  `user_name` varchar(100) NOT NULL,
  `password` varchar(128) DEFAULT NULL,
  `salt` varchar(100) DEFAULT NULL,
  `changed_timestamp` bigint(20) NOT NULL DEFAULT '0',
  `status` varchar(60) DEFAULT 'active',
  `confirmation` varchar(255) DEFAULT NULL,
  `oauth_id` varchar(32) DEFAULT NULL COMMENT 'the oauth user id',
  `profile_link` varchar(256) DEFAULT NULL COMMENT 'url to profile',
  `profile_picture` varchar(256) DEFAULT NULL COMMENT 'url to profile pic',
  `locale` varchar(8) DEFAULT NULL COMMENT 'language locale',
  `verified_email` tinyint(1) DEFAULT NULL COMMENT 'true if email verified',
  PRIMARY KEY (`user_id`),
  UNIQUE KEY `user_name` (`user_name`),
  KEY `p_uuid` (`p_uuid`)
) ENGINE=InnoDB  DEFAULT CHARSET=utf8 AUTO_INCREMENT=4 ;

--
-- Dumping data for table `users`
--

INSERT INTO `users` (`user_id`, `p_uuid`, `user_name`, `password`, `salt`, `changed_timestamp`, `status`, `confirmation`, `oauth_id`, `profile_link`, `profile_picture`, `locale`, `verified_email`) VALUES
(1, '1', 'root', 'c77ce1c91f65ec039c255b7b4981a452', 'e5cb9f3624f2d81964', 1334258322, 'active', NULL, NULL, NULL, NULL, NULL, NULL),
(2, '2', 'mpres', NULL, NULL, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL),
(3, '3', 'anonymous', NULL, NULL, 0, 'active', NULL, NULL, NULL, NULL, NULL, NULL);

-- --------------------------------------------------------

--
-- Table structure for table `user_preference`
--

CREATE TABLE IF NOT EXISTS `user_preference` (
  `pref_id` int(16) NOT NULL AUTO_INCREMENT,
  `p_uuid` varchar(128) NOT NULL,
  `module_id` varchar(20) NOT NULL,
  `pref_key` varchar(60) NOT NULL,
  `value` varchar(100) DEFAULT NULL,
  PRIMARY KEY (`pref_id`),
  KEY `p_uuid` (`p_uuid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 AUTO_INCREMENT=1 ;

-- --------------------------------------------------------

--
-- Table structure for table `voice_note`
--

CREATE TABLE IF NOT EXISTS `voice_note` (
  `voice_note_id` bigint(20) NOT NULL,
  `p_uuid` varchar(128) NOT NULL,
  `length` int(16) DEFAULT NULL,
  `format` varchar(16) DEFAULT NULL,
  `sample_rate` int(16) DEFAULT NULL,
  `channels` int(8) DEFAULT NULL,
  `speaker` enum('Person','Reporter','Other') DEFAULT NULL COMMENT 'Used to identify speaker.',
  `url_original` varchar(1024) DEFAULT NULL COMMENT 'url of the original audio',
  `url_resampled_mp3` varchar(1024) DEFAULT NULL COMMENT 'url of the resampled audio in mp3 format',
  `url_resampled_ogg` varchar(1024) DEFAULT NULL COMMENT 'url of the resampled audio in ogg format',
  PRIMARY KEY (`voice_note_id`),
  KEY `p_uuid` (`p_uuid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- --------------------------------------------------------

--
-- Table structure for table `voice_note_seq`
--

CREATE TABLE IF NOT EXISTS `voice_note_seq` (
  `id` bigint(20) NOT NULL AUTO_INCREMENT COMMENT 'stores next id in sequence for the voice_note table',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB  DEFAULT CHARSET=utf8 AUTO_INCREMENT=2 ;

--
-- Dumping data for table `voice_note_seq`
--

INSERT INTO `voice_note_seq` (`id`) VALUES
(1);

-- --------------------------------------------------------

--
-- Structure for view `person_search`
--
DROP TABLE IF EXISTS `person_search`;

CREATE ALGORITHM=UNDEFINED DEFINER=`root`@`localhost` SQL SECURITY DEFINER VIEW `person_search` AS select `pu`.`p_uuid` AS `p_uuid`,`pu`.`full_name` AS `full_name`,`pu`.`given_name` AS `given_name`,`pu`.`family_name` AS `family_name`,`pu`.`expiry_date` AS `expiry_date`,`ps`.`last_updated` AS `updated`,`ps`.`last_updated_db` AS `updated_db`,(case when (`ps`.`opt_status` not in (_utf8'ali',_utf8'mis',_utf8'inj',_utf8'dec',_utf8'unk',_utf8'fnd')) then _utf8'unk' else `ps`.`opt_status` end) AS `opt_status`,(case when ((`pd`.`opt_gender` not in (_utf8'mal',_utf8'fml')) or isnull(`pd`.`opt_gender`)) then _utf8'unk' else `pd`.`opt_gender` end) AS `opt_gender`,(case when isnull(cast(`pd`.`years_old` as unsigned)) then -(1) else `pd`.`years_old` end) AS `years_old`,(case when isnull(cast(`pd`.`minAge` as unsigned)) then -(1) else `pd`.`minAge` end) AS `minAge`,(case when isnull(cast(`pd`.`maxAge` as unsigned)) then -(1) else `pd`.`maxAge` end) AS `maxAge`,(case when (cast(`pd`.`years_old` as unsigned) is not null) then (case when (`pd`.`years_old` < 18) then _utf8'youth' when (`pd`.`years_old` >= 18) then _utf8'adult' end) when ((cast(`pd`.`minAge` as unsigned) is not null) and (cast(`pd`.`maxAge` as unsigned) is not null) and (`pd`.`minAge` < 18) and (`pd`.`maxAge` >= 18)) then _utf8'both' when ((cast(`pd`.`minAge` as unsigned) is not null) and (`pd`.`minAge` >= 18)) then _utf8'adult' when ((cast(`pd`.`maxAge` as unsigned) is not null) and (`pd`.`maxAge` < 18)) then _utf8'youth' else _utf8'unknown' end) AS `ageGroup`,`i`.`image_height` AS `image_height`,`i`.`image_width` AS `image_width`,`i`.`url_thumb` AS `url_thumb`,(case when (`h`.`hospital_uuid` = -(1)) then NULL else `h`.`icon_url` end) AS `icon_url`,`inc`.`shortname` AS `shortname`,(case when ((`pu`.`hospital_uuid` not in (1,2,3)) or isnull(`pu`.`hospital_uuid`)) then _utf8'public' else lcase(`h`.`short_name`) end) AS `hospital`,`pd`.`other_comments` AS `comments`,`pd`.`last_seen` AS `last_seen`,`ecl`.`person_id` AS `mass_casualty_id` from ((((((`person_uuid` `pu` join `person_status` `ps` on((`pu`.`p_uuid` = `ps`.`p_uuid`))) left join `image` `i` on(((`pu`.`p_uuid` = `i`.`p_uuid`) and (`i`.`principal` = 1)))) join `person_details` `pd` on((`pu`.`p_uuid` = `pd`.`p_uuid`))) join `incident` `inc` on((`inc`.`incident_id` = `pu`.`incident_id`))) left join `hospital` `h` on((`h`.`hospital_uuid` = `pu`.`hospital_uuid`))) left join `edxl_co_lpf` `ecl` on((`ecl`.`p_uuid` = `pu`.`p_uuid`)));

--
-- Constraints for dumped tables
--

--
-- Constraints for table `arrival_rate`
--
ALTER TABLE `arrival_rate`
  ADD CONSTRAINT `arrival_rate_ibfk_1` FOREIGN KEY (`person_uuid`) REFERENCES `person_uuid` (`p_uuid`) ON DELETE CASCADE ON UPDATE CASCADE,
  ADD CONSTRAINT `arrival_rate_ibfk_2` FOREIGN KEY (`incident_id`) REFERENCES `incident` (`incident_id`) ON DELETE CASCADE ON UPDATE CASCADE;

--
-- Constraints for table `contact`
--
ALTER TABLE `contact`
  ADD CONSTRAINT `contact_ibfk_1` FOREIGN KEY (`p_uuid`) REFERENCES `person_uuid` (`p_uuid`) ON DELETE CASCADE ON UPDATE CASCADE;

--
-- Constraints for table `edxl_co_header`
--
ALTER TABLE `edxl_co_header`
  ADD CONSTRAINT `edxl_co_header_ibfk_1` FOREIGN KEY (`de_id`) REFERENCES `edxl_de_header` (`de_id`) ON DELETE CASCADE ON UPDATE CASCADE,
  ADD CONSTRAINT `edxl_co_header_ibfk_2` FOREIGN KEY (`p_uuid`) REFERENCES `person_uuid` (`p_uuid`) ON DELETE CASCADE ON UPDATE CASCADE;

--
-- Constraints for table `edxl_co_keywords`
--
ALTER TABLE `edxl_co_keywords`
  ADD CONSTRAINT `edxl_co_keywords_ibfk_1` FOREIGN KEY (`co_id`) REFERENCES `edxl_co_header` (`co_id`) ON DELETE CASCADE ON UPDATE CASCADE;

--
-- Constraints for table `edxl_co_lpf`
--
ALTER TABLE `edxl_co_lpf`
  ADD CONSTRAINT `edxl_co_lpf_ibfk_1` FOREIGN KEY (`co_id`) REFERENCES `edxl_co_header` (`co_id`) ON DELETE CASCADE ON UPDATE CASCADE,
  ADD CONSTRAINT `edxl_co_lpf_ibfk_2` FOREIGN KEY (`p_uuid`) REFERENCES `person_uuid` (`p_uuid`) ON DELETE CASCADE ON UPDATE CASCADE;

--
-- Constraints for table `edxl_co_photos`
--
ALTER TABLE `edxl_co_photos`
  ADD CONSTRAINT `edxl_co_photos_ibfk_1` FOREIGN KEY (`co_id`) REFERENCES `edxl_co_header` (`co_id`) ON DELETE CASCADE ON UPDATE CASCADE,
  ADD CONSTRAINT `edxl_co_photos_ibfk_2` FOREIGN KEY (`p_uuid`) REFERENCES `person_uuid` (`p_uuid`) ON DELETE CASCADE ON UPDATE CASCADE;

--
-- Constraints for table `edxl_co_roles`
--
ALTER TABLE `edxl_co_roles`
  ADD CONSTRAINT `edxl_co_roles_ibfk_1` FOREIGN KEY (`co_id`) REFERENCES `edxl_co_header` (`co_id`) ON DELETE CASCADE ON UPDATE CASCADE,
  ADD CONSTRAINT `edxl_co_roles_ibfk_2` FOREIGN KEY (`role_num`) REFERENCES `edxl_de_roles` (`role_num`) ON DELETE CASCADE ON UPDATE CASCADE;

--
-- Constraints for table `edxl_de_keywords`
--
ALTER TABLE `edxl_de_keywords`
  ADD CONSTRAINT `edxl_de_keywords_ibfk_1` FOREIGN KEY (`de_id`) REFERENCES `edxl_de_header` (`de_id`) ON DELETE CASCADE ON UPDATE CASCADE;

--
-- Constraints for table `edxl_de_prior_messages`
--
ALTER TABLE `edxl_de_prior_messages`
  ADD CONSTRAINT `edxl_de_prior_messages_ibfk_1` FOREIGN KEY (`de_id`) REFERENCES `edxl_de_header` (`de_id`) ON DELETE CASCADE ON UPDATE CASCADE;

--
-- Constraints for table `edxl_de_roles`
--
ALTER TABLE `edxl_de_roles`
  ADD CONSTRAINT `edxl_de_roles_ibfk_1` FOREIGN KEY (`de_id`) REFERENCES `edxl_de_header` (`de_id`) ON DELETE CASCADE ON UPDATE CASCADE;

--
-- Constraints for table `edxl_de_target_addresses`
--
ALTER TABLE `edxl_de_target_addresses`
  ADD CONSTRAINT `edxl_de_target_addresses_ibfk_1` FOREIGN KEY (`de_id`) REFERENCES `edxl_de_header` (`de_id`) ON DELETE CASCADE ON UPDATE CASCADE;

--
-- Constraints for table `edxl_de_target_circles`
--
ALTER TABLE `edxl_de_target_circles`
  ADD CONSTRAINT `edxl_de_target_circles_ibfk_1` FOREIGN KEY (`de_id`) REFERENCES `edxl_de_header` (`de_id`) ON DELETE CASCADE ON UPDATE CASCADE;

--
-- Constraints for table `edxl_de_target_codes`
--
ALTER TABLE `edxl_de_target_codes`
  ADD CONSTRAINT `edxl_de_target_codes_ibfk_1` FOREIGN KEY (`de_id`) REFERENCES `edxl_de_header` (`de_id`) ON DELETE CASCADE ON UPDATE CASCADE;

--
-- Constraints for table `edxl_de_target_polygons`
--
ALTER TABLE `edxl_de_target_polygons`
  ADD CONSTRAINT `edxl_de_target_polygons_ibfk_1` FOREIGN KEY (`de_id`) REFERENCES `edxl_de_header` (`de_id`) ON DELETE CASCADE ON UPDATE CASCADE;

--
-- Constraints for table `image`
--
ALTER TABLE `image`
  ADD CONSTRAINT `image_ibfk_1` FOREIGN KEY (`p_uuid`) REFERENCES `person_uuid` (`p_uuid`) ON DELETE CASCADE ON UPDATE CASCADE;

--
-- Constraints for table `image_tag`
--
ALTER TABLE `image_tag`
  ADD CONSTRAINT `image_tag_ibfk_1` FOREIGN KEY (`image_id`) REFERENCES `image` (`image_id`) ON DELETE CASCADE ON UPDATE CASCADE;

--
-- Constraints for table `incident`
--
ALTER TABLE `incident`
  ADD CONSTRAINT `incident_ibfk_1` FOREIGN KEY (`private_group`) REFERENCES `sys_user_groups` (`group_id`) ON DELETE CASCADE ON UPDATE CASCADE;

--
-- Constraints for table `mpres_log`
--
ALTER TABLE `mpres_log`
  ADD CONSTRAINT `mpres_log_ibfk_1` FOREIGN KEY (`p_uuid`) REFERENCES `person_uuid` (`p_uuid`) ON DELETE CASCADE ON UPDATE CASCADE;

--
-- Constraints for table `page_translations`
--
ALTER TABLE `page_translations`
  ADD CONSTRAINT `page_translations_ibfk_1` FOREIGN KEY (`page_id`) REFERENCES `rez_pages` (`rez_page_id`) ON DELETE CASCADE ON UPDATE CASCADE;

--
-- Constraints for table `password_event_log`
--
ALTER TABLE `password_event_log`
  ADD CONSTRAINT `password_event_log_ibfk_1` FOREIGN KEY (`p_uuid`) REFERENCES `person_uuid` (`p_uuid`) ON DELETE CASCADE ON UPDATE CASCADE;

--
-- Constraints for table `person_deceased`
--
ALTER TABLE `person_deceased`
  ADD CONSTRAINT `person_deceased_ibfk_1` FOREIGN KEY (`p_uuid`) REFERENCES `person_uuid` (`p_uuid`) ON DELETE CASCADE ON UPDATE CASCADE;

--
-- Constraints for table `person_details`
--
ALTER TABLE `person_details`
  ADD CONSTRAINT `person_details_ibfk_1` FOREIGN KEY (`p_uuid`) REFERENCES `person_uuid` (`p_uuid`) ON DELETE CASCADE ON UPDATE CASCADE;

--
-- Constraints for table `person_followers`
--
ALTER TABLE `person_followers`
  ADD CONSTRAINT `person_followers_ibfk_1` FOREIGN KEY (`p_uuid`) REFERENCES `person_uuid` (`p_uuid`) ON DELETE CASCADE ON UPDATE CASCADE,
  ADD CONSTRAINT `person_followers_ibfk_2` FOREIGN KEY (`follower_p_uuid`) REFERENCES `person_uuid` (`p_uuid`) ON DELETE CASCADE ON UPDATE CASCADE;

--
-- Constraints for table `person_notes`
--
ALTER TABLE `person_notes`
  ADD CONSTRAINT `person_notes_ibfk_1` FOREIGN KEY (`note_about_p_uuid`) REFERENCES `person_uuid` (`p_uuid`) ON DELETE CASCADE ON UPDATE CASCADE,
  ADD CONSTRAINT `person_notes_ibfk_2` FOREIGN KEY (`note_written_by_p_uuid`) REFERENCES `person_uuid` (`p_uuid`) ON DELETE CASCADE ON UPDATE CASCADE;

--
-- Constraints for table `person_physical`
--
ALTER TABLE `person_physical`
  ADD CONSTRAINT `person_physical_ibfk_1` FOREIGN KEY (`p_uuid`) REFERENCES `person_uuid` (`p_uuid`) ON DELETE CASCADE ON UPDATE CASCADE;

--
-- Constraints for table `person_status`
--
ALTER TABLE `person_status`
  ADD CONSTRAINT `person_status_ibfk_1` FOREIGN KEY (`p_uuid`) REFERENCES `person_uuid` (`p_uuid`) ON DELETE CASCADE ON UPDATE CASCADE;

--
-- Constraints for table `person_to_report`
--
ALTER TABLE `person_to_report`
  ADD CONSTRAINT `person_to_report_ibfk_1` FOREIGN KEY (`p_uuid`) REFERENCES `person_uuid` (`p_uuid`) ON DELETE CASCADE ON UPDATE CASCADE;

--
-- Constraints for table `person_updates`
--
ALTER TABLE `person_updates`
  ADD CONSTRAINT `person_updates_ibfk_1` FOREIGN KEY (`p_uuid`) REFERENCES `person_uuid` (`p_uuid`) ON DELETE CASCADE ON UPDATE CASCADE,
  ADD CONSTRAINT `person_updates_ibfk_2` FOREIGN KEY (`updated_by_p_uuid`) REFERENCES `person_uuid` (`p_uuid`) ON DELETE CASCADE ON UPDATE CASCADE;

--
-- Constraints for table `person_uuid`
--
ALTER TABLE `person_uuid`
  ADD CONSTRAINT `person_uuid_ibfk_1` FOREIGN KEY (`incident_id`) REFERENCES `incident` (`incident_id`) ON DELETE CASCADE ON UPDATE CASCADE,
  ADD CONSTRAINT `person_uuid_ibfk_2` FOREIGN KEY (`hospital_uuid`) REFERENCES `hospital` (`hospital_uuid`) ON DELETE SET NULL ON UPDATE CASCADE;

--
-- Constraints for table `pfif_export_log`
--
ALTER TABLE `pfif_export_log`
  ADD CONSTRAINT `pfif_export_log_ibfk_1` FOREIGN KEY (`repository_id`) REFERENCES `pfif_repository` (`id`) ON DELETE CASCADE ON UPDATE CASCADE,
  ADD CONSTRAINT `pfif_export_log_ibfk_2` FOREIGN KEY (`repository_id`) REFERENCES `pfif_repository` (`id`) ON DELETE CASCADE ON UPDATE CASCADE;

--
-- Constraints for table `pfif_harvest_note_log`
--
ALTER TABLE `pfif_harvest_note_log`
  ADD CONSTRAINT `pfif_harvest_note_log_ibfk_1` FOREIGN KEY (`repository_id`) REFERENCES `pfif_repository` (`id`) ON DELETE CASCADE ON UPDATE CASCADE,
  ADD CONSTRAINT `pfif_harvest_note_log_ibfk_2` FOREIGN KEY (`repository_id`) REFERENCES `pfif_repository` (`id`) ON DELETE CASCADE ON UPDATE CASCADE;

--
-- Constraints for table `pfif_harvest_person_log`
--
ALTER TABLE `pfif_harvest_person_log`
  ADD CONSTRAINT `pfif_harvest_person_log_ibfk_1` FOREIGN KEY (`repository_id`) REFERENCES `pfif_repository` (`id`) ON DELETE CASCADE ON UPDATE CASCADE;

--
-- Constraints for table `pfif_repository`
--
ALTER TABLE `pfif_repository`
  ADD CONSTRAINT `pfif_repository_ibfk_1` FOREIGN KEY (`incident_id`) REFERENCES `incident` (`incident_id`) ON DELETE CASCADE ON UPDATE CASCADE;

--
-- Constraints for table `plus_access_log`
--
ALTER TABLE `plus_access_log`
  ADD CONSTRAINT `plus_access_log_ibfk_1` FOREIGN KEY (`user_name`) REFERENCES `users` (`user_name`) ON DELETE CASCADE ON UPDATE CASCADE;

--
-- Constraints for table `plus_report_log`
--
ALTER TABLE `plus_report_log`
  ADD CONSTRAINT `plus_report_log_ibfk_1` FOREIGN KEY (`p_uuid`) REFERENCES `person_uuid` (`p_uuid`) ON DELETE CASCADE ON UPDATE CASCADE;

--
-- Constraints for table `rap_log`
--
ALTER TABLE `rap_log`
  ADD CONSTRAINT `rap_log_ibfk_1` FOREIGN KEY (`p_uuid`) REFERENCES `person_uuid` (`p_uuid`) ON DELETE CASCADE ON UPDATE CASCADE;

--
-- Constraints for table `rez_page_template`
--
ALTER TABLE `rez_page_template`
  ADD CONSTRAINT `rez_page_template_ibfk_2` FOREIGN KEY (`created_by`) REFERENCES `users` (`user_id`) ON UPDATE CASCADE;

--
-- Constraints for table `scenario`
--
ALTER TABLE `scenario`
  ADD CONSTRAINT `scenario_ibfk_1` FOREIGN KEY (`incident_id`) REFERENCES `incident` (`incident_id`) ON DELETE CASCADE ON UPDATE CASCADE,
  ADD CONSTRAINT `scenario_ibfk_2` FOREIGN KEY (`disaster_type`) REFERENCES `disaster_category` (`category_id`) ON DELETE CASCADE ON UPDATE CASCADE;

--
-- Constraints for table `sys_user_to_group`
--
ALTER TABLE `sys_user_to_group`
  ADD CONSTRAINT `sys_user_to_group_ibfk_1` FOREIGN KEY (`p_uuid`) REFERENCES `person_uuid` (`p_uuid`) ON DELETE CASCADE ON UPDATE CASCADE,
  ADD CONSTRAINT `sys_user_to_group_ibfk_2` FOREIGN KEY (`group_id`) REFERENCES `sys_user_groups` (`group_id`) ON DELETE CASCADE ON UPDATE CASCADE;

--
-- Constraints for table `template_translations`
--
ALTER TABLE `template_translations`
  ADD CONSTRAINT `template_translations_ibfk_1` FOREIGN KEY (`template_id`) REFERENCES `rez_page_template` (`rez_template_id`) ON DELETE CASCADE ON UPDATE CASCADE;

--
-- Constraints for table `users`
--
ALTER TABLE `users`
  ADD CONSTRAINT `users_ibfk_1` FOREIGN KEY (`p_uuid`) REFERENCES `person_uuid` (`p_uuid`) ON DELETE CASCADE ON UPDATE CASCADE;

--
-- Constraints for table `user_preference`
--
ALTER TABLE `user_preference`
  ADD CONSTRAINT `user_preference_ibfk_1` FOREIGN KEY (`p_uuid`) REFERENCES `person_uuid` (`p_uuid`) ON DELETE CASCADE ON UPDATE CASCADE;

--
-- Constraints for table `voice_note`
--
ALTER TABLE `voice_note`
  ADD CONSTRAINT `voice_note_ibfk_1` FOREIGN KEY (`p_uuid`) REFERENCES `person_uuid` (`p_uuid`) ON DELETE CASCADE ON UPDATE CASCADE;

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
