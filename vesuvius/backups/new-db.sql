-- MySQL dump 10.13  Distrib 5.5.29, for osx10.6 (i386)
--
-- Host: localhost    Database: tmp
-- ------------------------------------------------------
-- Server version	5.5.29

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;
/*!40103 SET @OLD_TIME_ZONE=@@TIME_ZONE */;
/*!40103 SET TIME_ZONE='+00:00' */;
/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

--
-- Table structure for table `abuse_queue`
--

DROP TABLE IF EXISTS `abuse_queue`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `abuse_queue` (
  `p_uuid` varchar(128) NOT NULL,
  `identifier` varchar(64) NOT NULL COMMENT 'identifies user to some degree',
  `ip` varchar(16) NOT NULL COMMENT 'identifies the user to more of a degree',
  KEY `p_uuid` (`p_uuid`),
  CONSTRAINT `abuse_queue_ibfk_1` FOREIGN KEY (`p_uuid`) REFERENCES `person_uuid` (`p_uuid`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `abuse_queue`
--

LOCK TABLES `abuse_queue` WRITE;
/*!40000 ALTER TABLE `abuse_queue` DISABLE KEYS */;
/*!40000 ALTER TABLE `abuse_queue` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `alt_logins`
--

DROP TABLE IF EXISTS `alt_logins`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `alt_logins` (
  `p_uuid` varchar(128) NOT NULL,
  `user_name` varchar(100) NOT NULL,
  `type` varchar(60) DEFAULT 'openid',
  PRIMARY KEY (`p_uuid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `alt_logins`
--

LOCK TABLES `alt_logins` WRITE;
/*!40000 ALTER TABLE `alt_logins` DISABLE KEYS */;
/*!40000 ALTER TABLE `alt_logins` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `app_check`
--

DROP TABLE IF EXISTS `app_check`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `app_check` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `query_string` text,
  `url` text,
  `text` text,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `app_check`
--

LOCK TABLES `app_check` WRITE;
/*!40000 ALTER TABLE `app_check` DISABLE KEYS */;
/*!40000 ALTER TABLE `app_check` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `arrival_rate`
--

DROP TABLE IF EXISTS `arrival_rate`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `arrival_rate` (
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
  KEY `incident_index` (`incident_id`),
  CONSTRAINT `arrival_rate_ibfk_1` FOREIGN KEY (`person_uuid`) REFERENCES `person_uuid` (`p_uuid`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `arrival_rate_ibfk_2` FOREIGN KEY (`incident_id`) REFERENCES `incident` (`incident_id`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `arrival_rate`
--

LOCK TABLES `arrival_rate` WRITE;
/*!40000 ALTER TABLE `arrival_rate` DISABLE KEYS */;
/*!40000 ALTER TABLE `arrival_rate` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `audit`
--

DROP TABLE IF EXISTS `audit`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `audit` (
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
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `audit`
--

LOCK TABLES `audit` WRITE;
/*!40000 ALTER TABLE `audit` DISABLE KEYS */;
/*!40000 ALTER TABLE `audit` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `config`
--

DROP TABLE IF EXISTS `config`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `config` (
  `config_id` bigint(20) NOT NULL AUTO_INCREMENT,
  `module_id` varchar(20) DEFAULT NULL,
  `confkey` varchar(50) NOT NULL,
  `value` mediumtext,
  PRIMARY KEY (`config_id`),
  KEY `module_id` (`module_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `config`
--

LOCK TABLES `config` WRITE;
/*!40000 ALTER TABLE `config` DISABLE KEYS */;
/*!40000 ALTER TABLE `config` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `contact`
--

DROP TABLE IF EXISTS `contact`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `contact` (
  `p_uuid` varchar(128) NOT NULL,
  `opt_contact_type` varchar(10) NOT NULL,
  `contact_value` varchar(100) DEFAULT NULL,
  PRIMARY KEY (`p_uuid`,`opt_contact_type`),
  KEY `contact_value` (`contact_value`),
  KEY `p_uuid` (`p_uuid`),
  CONSTRAINT `contact_ibfk_1` FOREIGN KEY (`p_uuid`) REFERENCES `person_uuid` (`p_uuid`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `contact`
--

LOCK TABLES `contact` WRITE;
/*!40000 ALTER TABLE `contact` DISABLE KEYS */;
/*!40000 ALTER TABLE `contact` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `dao_error_log`
--

DROP TABLE IF EXISTS `dao_error_log`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `dao_error_log` (
  `time` timestamp NULL DEFAULT CURRENT_TIMESTAMP,
  `file` text,
  `line` text,
  `method` text,
  `class` text,
  `function` text,
  `error_message` text,
  `other` longtext
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='logs errors encountered in the DAO objects';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `dao_error_log`
--

LOCK TABLES `dao_error_log` WRITE;
/*!40000 ALTER TABLE `dao_error_log` DISABLE KEYS */;
/*!40000 ALTER TABLE `dao_error_log` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `edxl_co_header`
--

DROP TABLE IF EXISTS `edxl_co_header`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `edxl_co_header` (
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
  KEY `p_uuid_idx` (`p_uuid`),
  CONSTRAINT `edxl_co_header_ibfk_1` FOREIGN KEY (`de_id`) REFERENCES `edxl_de_header` (`de_id`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `edxl_co_header_ibfk_2` FOREIGN KEY (`p_uuid`) REFERENCES `person_uuid` (`p_uuid`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `edxl_co_header`
--

LOCK TABLES `edxl_co_header` WRITE;
/*!40000 ALTER TABLE `edxl_co_header` DISABLE KEYS */;
/*!40000 ALTER TABLE `edxl_co_header` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `edxl_co_header_seq`
--

DROP TABLE IF EXISTS `edxl_co_header_seq`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `edxl_co_header_seq` (
  `id` bigint(20) NOT NULL AUTO_INCREMENT COMMENT 'stores next id in sequence for the edxl_co_header table',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=1593 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `edxl_co_header_seq`
--

LOCK TABLES `edxl_co_header_seq` WRITE;
/*!40000 ALTER TABLE `edxl_co_header_seq` DISABLE KEYS */;
INSERT INTO `edxl_co_header_seq` (`id`) VALUES (1592);
/*!40000 ALTER TABLE `edxl_co_header_seq` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `edxl_co_keywords`
--

DROP TABLE IF EXISTS `edxl_co_keywords`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `edxl_co_keywords` (
  `co_id` int(11) NOT NULL,
  `keyword_num` int(11) NOT NULL,
  `keyword_urn` varchar(255) NOT NULL,
  `keyword` varchar(255) NOT NULL,
  PRIMARY KEY (`co_id`,`keyword_num`),
  CONSTRAINT `edxl_co_keywords_ibfk_1` FOREIGN KEY (`co_id`) REFERENCES `edxl_co_header` (`co_id`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `edxl_co_keywords`
--

LOCK TABLES `edxl_co_keywords` WRITE;
/*!40000 ALTER TABLE `edxl_co_keywords` DISABLE KEYS */;
/*!40000 ALTER TABLE `edxl_co_keywords` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `edxl_co_lpf`
--

DROP TABLE IF EXISTS `edxl_co_lpf`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `edxl_co_lpf` (
  `co_id` int(11) NOT NULL,
  `p_uuid` varchar(255) NOT NULL COMMENT 'Sahana person ID',
  `schema_version` varchar(255) NOT NULL,
  `login_machine` varchar(255) NOT NULL,
  `login_account` varchar(255) NOT NULL,
  `person_id` varchar(255) NOT NULL COMMENT 'Mass casualty patient ID',
  `event_name` varchar(255) NOT NULL,
  `event_long_name` varchar(255) NOT NULL,
  `org_name` varchar(255) NOT NULL,
  `org_id` varchar(255) DEFAULT NULL,
  `last_name` varchar(255) DEFAULT NULL,
  `first_name` varchar(255) DEFAULT NULL,
  `gender` enum('M','F','U','C') NOT NULL,
  `peds` tinyint(1) NOT NULL,
  `triage_category` enum('Green','BH Green','Yellow','Red','Gray','Black') NOT NULL,
  PRIMARY KEY (`co_id`,`p_uuid`),
  KEY `p_uuid` (`p_uuid`),
  CONSTRAINT `edxl_co_lpf_ibfk_1` FOREIGN KEY (`co_id`) REFERENCES `edxl_co_header` (`co_id`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `edxl_co_lpf_ibfk_2` FOREIGN KEY (`p_uuid`) REFERENCES `person_uuid` (`p_uuid`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='LPF is an example of an "other xml" content object, e.g., ot';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `edxl_co_lpf`
--

LOCK TABLES `edxl_co_lpf` WRITE;
/*!40000 ALTER TABLE `edxl_co_lpf` DISABLE KEYS */;
/*!40000 ALTER TABLE `edxl_co_lpf` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `edxl_co_photos`
--

DROP TABLE IF EXISTS `edxl_co_photos`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `edxl_co_photos` (
  `co_id` int(11) NOT NULL,
  `p_uuid` varchar(255) NOT NULL COMMENT 'Sahana person ID',
  `mimeType` varchar(255) NOT NULL COMMENT 'As in ''image/jpeg''',
  `uri` varchar(255) NOT NULL COMMENT 'Photo filename = Mass casualty patient ID + zone + ''s#'' if secondary + optional caption after hypen',
  `contentData` mediumtext CHARACTER SET ascii NOT NULL COMMENT 'Base-64 encoded image',
  `image_id` int(20) DEFAULT NULL COMMENT 'reference to the image.image_id field',
  `sha1` varchar(40) DEFAULT NULL COMMENT 'sha1 calculated hash of the image',
  PRIMARY KEY (`co_id`,`p_uuid`),
  KEY `p_uuid` (`p_uuid`),
  CONSTRAINT `edxl_co_photos_ibfk_1` FOREIGN KEY (`co_id`) REFERENCES `edxl_co_header` (`co_id`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `edxl_co_photos_ibfk_2` FOREIGN KEY (`p_uuid`) REFERENCES `person_uuid` (`p_uuid`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='LPF is an example of an "other xml" content object, e.g., ot';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `edxl_co_photos`
--

LOCK TABLES `edxl_co_photos` WRITE;
/*!40000 ALTER TABLE `edxl_co_photos` DISABLE KEYS */;
/*!40000 ALTER TABLE `edxl_co_photos` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `edxl_co_roles`
--

DROP TABLE IF EXISTS `edxl_co_roles`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `edxl_co_roles` (
  `co_id` int(11) NOT NULL,
  `role_num` int(11) NOT NULL DEFAULT '0',
  `of_originator` tinyint(1) NOT NULL COMMENT '0 = false = of consumer',
  `role_urn` varchar(255) NOT NULL,
  `role` varchar(255) NOT NULL,
  PRIMARY KEY (`co_id`,`role_num`),
  KEY `role_num` (`role_num`),
  CONSTRAINT `edxl_co_roles_ibfk_1` FOREIGN KEY (`co_id`) REFERENCES `edxl_co_header` (`co_id`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `edxl_co_roles_ibfk_2` FOREIGN KEY (`role_num`) REFERENCES `edxl_de_roles` (`role_num`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `edxl_co_roles`
--

LOCK TABLES `edxl_co_roles` WRITE;
/*!40000 ALTER TABLE `edxl_co_roles` DISABLE KEYS */;
/*!40000 ALTER TABLE `edxl_co_roles` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `edxl_de_header`
--

DROP TABLE IF EXISTS `edxl_de_header`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `edxl_de_header` (
  `de_id` int(11) NOT NULL,
  `when_sent` datetime NOT NULL,
  `sender_id` varchar(255) NOT NULL COMMENT 'Email, phone num, etc.  Not always URI, URN, URL',
  `distr_id` varchar(255) NOT NULL COMMENT 'Distribution ID.  Sender may or may not choose to vary.',
  `distr_status` enum('Actual','Exercise','System','Test') DEFAULT NULL,
  `distr_type` enum('Ack','Cancel','Dispatch','Error','Report','Request','Response','Update') NOT NULL COMMENT 'Not included: types for sensor grids',
  `combined_conf` varchar(255) NOT NULL COMMENT 'Combined confidentiality of all content objects',
  `language` varchar(255) DEFAULT NULL,
  `when_here` datetime NOT NULL COMMENT 'Received or sent from here.  [local]',
  `inbound` tinyint(1) NOT NULL DEFAULT '1' COMMENT 'BOOLEAN [local]',
  PRIMARY KEY (`de_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='Overall message base, defined by EDXL Distribution Element';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `edxl_de_header`
--

LOCK TABLES `edxl_de_header` WRITE;
/*!40000 ALTER TABLE `edxl_de_header` DISABLE KEYS */;
/*!40000 ALTER TABLE `edxl_de_header` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `edxl_de_header_seq`
--

DROP TABLE IF EXISTS `edxl_de_header_seq`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `edxl_de_header_seq` (
  `id` bigint(20) NOT NULL AUTO_INCREMENT COMMENT 'stores next id in sequence for the edxl_de_header table',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=590 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `edxl_de_header_seq`
--

LOCK TABLES `edxl_de_header_seq` WRITE;
/*!40000 ALTER TABLE `edxl_de_header_seq` DISABLE KEYS */;
INSERT INTO `edxl_de_header_seq` (`id`) VALUES (589);
/*!40000 ALTER TABLE `edxl_de_header_seq` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `edxl_de_keywords`
--

DROP TABLE IF EXISTS `edxl_de_keywords`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `edxl_de_keywords` (
  `de_id` int(11) NOT NULL,
  `keyword_num` int(11) NOT NULL DEFAULT '0',
  `keyword_urn` varchar(255) NOT NULL,
  `keyword` varchar(255) CHARACTER SET latin1 NOT NULL,
  PRIMARY KEY (`de_id`,`keyword_num`),
  CONSTRAINT `edxl_de_keywords_ibfk_1` FOREIGN KEY (`de_id`) REFERENCES `edxl_de_header` (`de_id`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `edxl_de_keywords`
--

LOCK TABLES `edxl_de_keywords` WRITE;
/*!40000 ALTER TABLE `edxl_de_keywords` DISABLE KEYS */;
/*!40000 ALTER TABLE `edxl_de_keywords` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `edxl_de_prior_messages`
--

DROP TABLE IF EXISTS `edxl_de_prior_messages`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `edxl_de_prior_messages` (
  `de_id` int(11) NOT NULL,
  `prior_msg_num` int(11) NOT NULL DEFAULT '0',
  `when_sent` datetime NOT NULL COMMENT 'external time',
  `sender_id` varchar(255) NOT NULL COMMENT 'external ID',
  `distr_id` varchar(255) NOT NULL COMMENT 'external distribution ID',
  PRIMARY KEY (`de_id`,`prior_msg_num`),
  CONSTRAINT `edxl_de_prior_messages_ibfk_1` FOREIGN KEY (`de_id`) REFERENCES `edxl_de_header` (`de_id`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `edxl_de_prior_messages`
--

LOCK TABLES `edxl_de_prior_messages` WRITE;
/*!40000 ALTER TABLE `edxl_de_prior_messages` DISABLE KEYS */;
/*!40000 ALTER TABLE `edxl_de_prior_messages` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `edxl_de_roles`
--

DROP TABLE IF EXISTS `edxl_de_roles`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `edxl_de_roles` (
  `de_id` int(11) NOT NULL,
  `role_num` int(11) NOT NULL DEFAULT '0',
  `of_sender` tinyint(1) NOT NULL,
  `role_urn` varchar(255) NOT NULL,
  `role` varchar(255) NOT NULL,
  PRIMARY KEY (`de_id`,`role_num`),
  KEY `role_idx` (`role_num`),
  CONSTRAINT `edxl_de_roles_ibfk_1` FOREIGN KEY (`de_id`) REFERENCES `edxl_de_header` (`de_id`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `edxl_de_roles`
--

LOCK TABLES `edxl_de_roles` WRITE;
/*!40000 ALTER TABLE `edxl_de_roles` DISABLE KEYS */;
/*!40000 ALTER TABLE `edxl_de_roles` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `edxl_de_target_addresses`
--

DROP TABLE IF EXISTS `edxl_de_target_addresses`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `edxl_de_target_addresses` (
  `de_id` int(11) NOT NULL,
  `address_num` int(11) NOT NULL DEFAULT '0',
  `scheme` varchar(255) NOT NULL COMMENT 'Like "e-mail"',
  `value` varchar(255) NOT NULL,
  PRIMARY KEY (`de_id`,`address_num`),
  CONSTRAINT `edxl_de_target_addresses_ibfk_1` FOREIGN KEY (`de_id`) REFERENCES `edxl_de_header` (`de_id`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `edxl_de_target_addresses`
--

LOCK TABLES `edxl_de_target_addresses` WRITE;
/*!40000 ALTER TABLE `edxl_de_target_addresses` DISABLE KEYS */;
/*!40000 ALTER TABLE `edxl_de_target_addresses` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `edxl_de_target_circles`
--

DROP TABLE IF EXISTS `edxl_de_target_circles`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `edxl_de_target_circles` (
  `de_id` int(11) NOT NULL,
  `circle_num` int(11) NOT NULL DEFAULT '0',
  `latitude` float NOT NULL,
  `longitude` float NOT NULL,
  `radius_km` float NOT NULL,
  PRIMARY KEY (`de_id`,`circle_num`),
  CONSTRAINT `edxl_de_target_circles_ibfk_1` FOREIGN KEY (`de_id`) REFERENCES `edxl_de_header` (`de_id`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `edxl_de_target_circles`
--

LOCK TABLES `edxl_de_target_circles` WRITE;
/*!40000 ALTER TABLE `edxl_de_target_circles` DISABLE KEYS */;
/*!40000 ALTER TABLE `edxl_de_target_circles` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `edxl_de_target_codes`
--

DROP TABLE IF EXISTS `edxl_de_target_codes`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `edxl_de_target_codes` (
  `de_id` int(11) NOT NULL,
  `codes_num` int(11) NOT NULL DEFAULT '0',
  `code_type` enum('country','subdivision','locCodeUN') DEFAULT NULL COMMENT 'Respectively (1) ISO 3166-1 2-letter country code (2) ISO 3166-2 code: country + "-" + per-country 2-3 char code like state, e.g., "US-MD". (3) UN transport hub code: country + "-" + 2-3 char code (cap ASCII or 2-9), e.g., "US-BWI"',
  `code` varchar(6) DEFAULT NULL COMMENT 'See format examples for code_type field',
  PRIMARY KEY (`de_id`,`codes_num`),
  CONSTRAINT `edxl_de_target_codes_ibfk_1` FOREIGN KEY (`de_id`) REFERENCES `edxl_de_header` (`de_id`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `edxl_de_target_codes`
--

LOCK TABLES `edxl_de_target_codes` WRITE;
/*!40000 ALTER TABLE `edxl_de_target_codes` DISABLE KEYS */;
/*!40000 ALTER TABLE `edxl_de_target_codes` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `edxl_de_target_polygons`
--

DROP TABLE IF EXISTS `edxl_de_target_polygons`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `edxl_de_target_polygons` (
  `de_id` int(11) NOT NULL,
  `poly_num` int(11) NOT NULL DEFAULT '0',
  `point_num` int(11) NOT NULL DEFAULT '0' COMMENT 'Point within this polygon',
  `latitude` float NOT NULL,
  `longitude` float NOT NULL,
  PRIMARY KEY (`de_id`,`poly_num`,`point_num`),
  CONSTRAINT `edxl_de_target_polygons_ibfk_1` FOREIGN KEY (`de_id`) REFERENCES `edxl_de_header` (`de_id`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `edxl_de_target_polygons`
--

LOCK TABLES `edxl_de_target_polygons` WRITE;
/*!40000 ALTER TABLE `edxl_de_target_polygons` DISABLE KEYS */;
/*!40000 ALTER TABLE `edxl_de_target_polygons` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `expiry_queue`
--

DROP TABLE IF EXISTS `expiry_queue`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `expiry_queue` (
  `index` int(20) NOT NULL AUTO_INCREMENT,
  `p_uuid` varchar(128) DEFAULT NULL,
  `requested_by_user_id` int(16) DEFAULT NULL,
  `requested_when` timestamp NULL DEFAULT CURRENT_TIMESTAMP,
  `requested_why` text COMMENT 'the reason (optional) why a expiration/deletion was requested',
  `queued` tinyint(1) DEFAULT NULL COMMENT 'true when an expiration is requested',
  `approved_by_user_id` int(16) DEFAULT NULL,
  `approved_when` timestamp NULL DEFAULT NULL,
  `approved_why` text COMMENT 'the reason why an approval was accepted or rejected',
  `expired` tinyint(1) DEFAULT '0' COMMENT 'true when a expiration is approved',
  PRIMARY KEY (`index`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='person expiry request management queue and related informati';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `expiry_queue`
--

LOCK TABLES `expiry_queue` WRITE;
/*!40000 ALTER TABLE `expiry_queue` DISABLE KEYS */;
/*!40000 ALTER TABLE `expiry_queue` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `field_options`
--

DROP TABLE IF EXISTS `field_options`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `field_options` (
  `field_name` varchar(100) DEFAULT NULL,
  `option_code` varchar(10) DEFAULT NULL,
  `option_description` varchar(50) DEFAULT NULL,
  `display_order` int(8) DEFAULT NULL,
  KEY `option_code` (`option_code`),
  KEY `option_description` (`option_description`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `field_options`
--

LOCK TABLES `field_options` WRITE;
/*!40000 ALTER TABLE `field_options` DISABLE KEYS */;
INSERT INTO `field_options` (`field_name`, `option_code`, `option_description`, `display_order`) VALUES ('opt_status','ali','Alive & Well',NULL),('opt_status','mis','Missing',NULL),('opt_status','inj','Injured',NULL),('opt_status','dec','Deceased',NULL),('opt_gender','mal','Male',NULL),('opt_gender','fml','Female',NULL),('opt_contact_type','home','Home(permanent address)',NULL),('opt_contact_type','name','Contact Person',NULL),('opt_contact_type','pmob','Personal Mobile',NULL),('opt_contact_type','curr','Current Phone',NULL),('opt_contact_type','cmob','Current Mobile',NULL),('opt_contact_type','email','Email address',NULL),('opt_contact_type','fax','Fax Number',NULL),('opt_contact_type','web','Website',NULL),('opt_contact_type','inst','Instant Messenger',NULL),('opt_eye_color','GRN','Green',NULL),('opt_eye_color','GRY','Gray',NULL),('opt_race','R1','American Indian or Alaska Native',NULL),('opt_race',NULL,'Unknown',NULL),('opt_eye_color','BRO','Brown',NULL),('opt_eye_color','BLU','Blue',NULL),('opt_eye_color','BLK','Black',NULL),('opt_skin_color','DRK','Dark',NULL),('opt_skin_color','BLK','Black',NULL),('opt_skin_color','ALB','Albino',NULL),('opt_hair_color','BLN','Blond or Strawberry',NULL),('opt_hair_color','BLK','Black',NULL),('opt_hair_color','BLD','Bald',NULL),('opt_location_type','2','Town or Neighborhood',NULL),('opt_location_type','1','County or Equivalent',NULL),('opt_contact_type','zip','Zip Code',NULL),('opt_eye_color',NULL,'Unknown',NULL),('opt_eye_color','HAZ','Hazel',NULL),('opt_eye_color','MAR','Maroon',NULL),('opt_eye_color','MUL','Multicolored',NULL),('opt_eye_color','PNK','Pink',NULL),('opt_skin_color','DBR','Dark Brown',NULL),('opt_skin_color','FAR','Fair',NULL),('opt_skin_color','LGT','Light',NULL),('opt_skin_color','LBR','Light Brown',NULL),('opt_skin_color','MED','Medium',NULL),('opt_skin_color',NULL,'Unknown',NULL),('opt_skin_color','OLV','Olive',NULL),('opt_skin_color','RUD','Ruddy',NULL),('opt_skin_color','SAL','Sallow',NULL),('opt_skin_color','YEL','Yellow',NULL),('opt_hair_color','BLU','Blue',NULL),('opt_hair_color','BRO','Brown',NULL),('opt_hair_color','GRY','Gray',NULL),('opt_hair_color','GRN','Green',NULL),('opt_hair_color','ONG','Orange',NULL),('opt_hair_color','PLE','Purple',NULL),('opt_hair_color','PNK','Pink',NULL),('opt_hair_color','RED','Red or Auburn',NULL),('opt_hair_color','SDY','Sandy',NULL),('opt_hair_color','WHI','White',NULL),('opt_race','R2','Asian',NULL),('opt_race','R3','Black or African American',NULL),('opt_race','R4','Native Hawaiian or Other Pacific Islander',NULL),('opt_race','R5','White',NULL),('opt_race','R9','Other Race',NULL),('opt_religion','PEV','Protestant, Evangelical',1),('opt_religion','PML','Protestant, Mainline',2),('opt_religion','PHB','Protestant, Historically Black',3),('opt_religion','CAT','Catholic',4),('opt_religion','MOM','Mormon',5),('opt_religion','JWN','Jehovah\'s Witness',6),('opt_religion','ORT','Orthodox',7),('opt_religion','COT','Other Christian',8),('opt_religion','JEW','Jewish',9),('opt_religion','BUD','Buddhist',10),('opt_religion','HIN','Hindu',11),('opt_religion','MOS','Muslim',12),('opt_religion','OTH','Other Faiths',13),('opt_religion','NOE','Unaffiliated',14),('opt_religion',NULL,'Unknown',15),('opt_hair_color',NULL,'Unknown',NULL),('opt_skin_color','MBR','Medium Brown',NULL),('opt_gender',NULL,'Unknown',NULL),('opt_gender','cpx','Complex',NULL),('opt_status','unk','Unknown',NULL),('opt_status','fnd','Found',NULL),('opt_status_color','fnd','#802A2A',NULL),('opt_status_color','ali','#802A2A',NULL),('opt_status_hospital_color','BH Green','#98FB98',NULL),('opt_status_hospital_color','Red','#FF0000',NULL),('opt_status_hospital_color','Gray','#A9A9A9',NULL),('opt_status_hospital_color','Yellow','#FFFF00',NULL),('opt_status_hospital_color','Green','#008000',NULL),('opt_status_hospital_color','Black','#000000',NULL);
/*!40000 ALTER TABLE `field_options` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `hospital`
--

DROP TABLE IF EXISTS `hospital`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `hospital` (
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
) ENGINE=InnoDB AUTO_INCREMENT=4 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `hospital`
--

LOCK TABLES `hospital` WRITE;
/*!40000 ALTER TABLE `hospital` DISABLE KEYS */;
INSERT INTO `hospital` (`hospital_uuid`, `name`, `short_name`, `street1`, `street2`, `city`, `county`, `region`, `postal_code`, `country`, `latitude`, `longitude`, `phone`, `fax`, `email`, `www`, `npi`, `patient_id_prefix`, `patient_id_suffix_variable`, `patient_id_suffix_fixed_length`, `creation_time`, `icon_url`, `legalese`, `legaleseAnon`, `legaleseTimestamp`, `legaleseAnonTimestamp`, `photo_required`, `honor_no_photo_request`, `photographer_name_required`) VALUES (1,'Suburban Hospital','Suburban','8600 Old Georgetown Road','','Bethesda','Montgomery','MD','20814-1497','USA',38.99731,-77.10984,'301-896-3118','','info@suburbanhospital.org','www.suburbanhospital.org','1205896446','911-',0,8,'2010-01-01 06:01:01','theme/lpf3/img/suburban.png','[This document is a straw man example, based on a reworking of the HIPAA policy statement at www.hhs.gov/hipaafaq/providers/hipaa-1068.html.  It has not been reviewed by legal council, nor reviewed or approved by Suburban Hospital or any other BHEPP member.]\r\n\r\n[The following is an example statement for attachment to a full record generated by \"TriagePic\".  Partial records (e.g., de-identified) have different statements.]\r\n\r\nNotice of Privacy Practices and Information Distribution\r\n========================================================\r\nSuburban Hospital is covered by HIPAA, so the provided disaster victim information (the \"record\", which may include a photo) is governed by provisions of the HIPAA Privacy Rule.\r\n\r\nDuring a disaster, HIPPA permits disclosures for treatment purposes, and certain disclosures to disaster relief organizations (like the American Red Cross) so that family members can be notified of the patient\'s location.  (See CFR 164.510(b)(4).\r\n\r\nThe primary of purpose of the record is for family reunification.  Secondary usages may include in-hospital patient tracking, treatment/continuity-of-care on patient transfer, disaster situational awareness and resource management, and feedback to emergency medical service providers who provide pre-hospital treatment.  The record (in various forms) will be distributed within Suburban Hospital, and to and within the institutions with which Suburban Hospital partners through the Bethesda Hospital Emergency Preparedness Partnership.  These are the NIH Clinical Center, Walter Reed National Military Medical Hospital, and National Library of Medicine.  In particular, the record is sent to NLM\'s Lost Person Finder database for exposure through the web site, with appropriate filtering and verification.  HIPAA allows patients to be listed in the facility directory, and some aspects of Lost Person Finder are analogous to that.  For more, see the Notice of Privacy Practices associated with the LPF web site.  \r\n\r\nThe record was generated by a \"TriagePic\" application, operated by Suburban Hospital personnel.  The application includes the ability to express the following [TO DO]:\r\n\r\n- patient agrees to let hospital personnel speak with family members or friends involved in the patient\'s care (45 CFR 164.510(b));\r\n- patient wishes to opt out of the facility directory (45 CFR 164.510(a));  [THIS MIGHT BE INTERPRETED AS OPTING OUT OF LPF.  MORE TO UNDERSTAND.]\r\n- patient requests privacy restrictions (45 CFR 164.522(a))\r\n- patient requests confidential communications (45 CFR 164.522(b))\r\n\r\n[IMPLICATIONS OF THESE CHOICES ON RECORD GENERATION NOT YET KNOWN.]\r\n\r\nIn addition, there is a requirement to distribute a notice of privacy practices, addressed by this attachment and the LPF Notice of Privacy Practices.\r\n\r\nPenalties for non-compliance with the above five rules may be waived, for a limited time in a limited geographical area, if the President declares an emergency or disaster AND the Health and Human Services Secretary declares a public health emergency.  Within that declared timespan, a hospital may rely on the waiver (which covers all patients present) only from the moment of instituting its disaster protocol to at most 72 hours later.  For more, see www.hhs.gov/hipaafaq/providers/hipaa-1068.html.  The waiver is authorized under the Project Bioshield Act of 2004 (PL 108-276) and section 1135(b) of the Social Security Act.\r\n\r\nPhoto Copyright\r\n===============\r\nThe attached photo if any was taken by an employee or volunteer of Suburban Hospital and is copyright Suburban Hospital in the year given by the reporting date.  Reproduction and distribution is permitted to the extent governed by policy for the overall record.  Please credit Suburban Hospital and the employee(s)/volunteer(s) listed.','two',NULL,NULL,0,0,0),(2,'National Naval Medical Center','NNMC','8901 Rockville Pike','','Bethesda','Montgomery','MD','20889-0001','USA',39.00204,-77.0945,'301-295-4611','','','www.bethesda.med.navy.mil','1356317069','1000-',0,16,'2010-09-22 22:49:34','theme/lpf3/img/nnmc.png','','',NULL,NULL,0,0,0),(3,'NLM (testing)','NLM','9000 Rockville Pike','','Bethesda','Montgomery','MD','20892','USA',38.995523,-77.096597,'','','','www.nlm.nih.gov','1234567890','911-',1,-1,'2011-05-02 17:35:40','theme/lpf3/img/nlm.gif','','',NULL,NULL,0,0,0);
/*!40000 ALTER TABLE `hospital` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `image`
--

DROP TABLE IF EXISTS `image`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `image` (
  `image_id` bigint(20) NOT NULL,
  `p_uuid` varchar(128) NOT NULL,
  `note_record_id` varchar(128) DEFAULT NULL,
  `image_type` varchar(100) NOT NULL,
  `image_height` int(11) NOT NULL DEFAULT '1',
  `image_width` int(11) NOT NULL DEFAULT '1',
  `created` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `url` varchar(512) DEFAULT NULL,
  `url_thumb` varchar(512) DEFAULT NULL,
  `original_filename` varchar(64) DEFAULT NULL,
  `principal` tinyint(1) NOT NULL DEFAULT '1',
  `sha1original` varchar(40) DEFAULT NULL COMMENT 'holds the sha1 of the original image from which this image and thumbnail are derived',
  `color_channels` int(11) DEFAULT '3' COMMENT 'number of color channels, ie. 3 for RGB',
  `note_id` int(11) NOT NULL DEFAULT '0' COMMENT 'if this image accompanies a note, then this id refers to which specific note it originated from',
  PRIMARY KEY (`image_id`),
  UNIQUE KEY `note_record_id` (`note_record_id`),
  UNIQUE KEY `p_uuid` (`p_uuid`,`note_record_id`),
  KEY `principal` (`principal`),
  KEY `note_index` (`note_id`),
  CONSTRAINT `image_ibfk_1` FOREIGN KEY (`p_uuid`) REFERENCES `person_uuid` (`p_uuid`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `image_ibfk_2` FOREIGN KEY (`note_id`) REFERENCES `person_notes` (`note_id`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `image`
--

LOCK TABLES `image` WRITE;
/*!40000 ALTER TABLE `image` DISABLE KEYS */;
/*!40000 ALTER TABLE `image` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `image_seq`
--

DROP TABLE IF EXISTS `image_seq`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `image_seq` (
  `id` bigint(20) NOT NULL AUTO_INCREMENT COMMENT 'stores next id in sequence for the image table',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=2703 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `image_seq`
--

LOCK TABLES `image_seq` WRITE;
/*!40000 ALTER TABLE `image_seq` DISABLE KEYS */;
INSERT INTO `image_seq` (`id`) VALUES (2702);
/*!40000 ALTER TABLE `image_seq` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `image_tag`
--

DROP TABLE IF EXISTS `image_tag`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `image_tag` (
  `tag_id` int(12) NOT NULL AUTO_INCREMENT,
  `image_id` bigint(20) NOT NULL,
  `tag_x` int(12) NOT NULL,
  `tag_y` int(12) NOT NULL,
  `tag_w` int(12) NOT NULL,
  `tag_h` int(12) NOT NULL,
  `tag_text` varchar(128) DEFAULT NULL,
  PRIMARY KEY (`tag_id`),
  KEY `tag_id` (`tag_id`,`image_id`),
  KEY `image_id` (`image_id`),
  CONSTRAINT `image_tag_ibfk_1` FOREIGN KEY (`image_id`) REFERENCES `image` (`image_id`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `image_tag`
--

LOCK TABLES `image_tag` WRITE;
/*!40000 ALTER TABLE `image_tag` DISABLE KEYS */;
/*!40000 ALTER TABLE `image_tag` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `image_tag_seq`
--

DROP TABLE IF EXISTS `image_tag_seq`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `image_tag_seq` (
  `id` bigint(20) NOT NULL AUTO_INCREMENT COMMENT 'stores next id in sequence for the image_tag table',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=4615 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `image_tag_seq`
--

LOCK TABLES `image_tag_seq` WRITE;
/*!40000 ALTER TABLE `image_tag_seq` DISABLE KEYS */;
INSERT INTO `image_tag_seq` (`id`) VALUES (4614);
/*!40000 ALTER TABLE `image_tag_seq` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `imagestats`
--

DROP TABLE IF EXISTS `imagestats`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `imagestats` (
  `image_id` bigint(20) NOT NULL,
  `regions` varchar(512) CHARACTER SET utf8 COLLATE utf8_unicode_ci NOT NULL,
  `groundTruthStatus` smallint(6) NOT NULL,
  PRIMARY KEY (`image_id`),
  CONSTRAINT `imagestats_ibfk_1` FOREIGN KEY (`image_id`) REFERENCES `image` (`image_id`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `imagestats`
--

LOCK TABLES `imagestats` WRITE;
/*!40000 ALTER TABLE `imagestats` DISABLE KEYS */;
/*!40000 ALTER TABLE `imagestats` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `incident`
--

DROP TABLE IF EXISTS `incident`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `incident` (
  `incident_id` bigint(20) NOT NULL AUTO_INCREMENT,
  `parent_id` bigint(20) DEFAULT NULL,
  `search_id` varchar(60) DEFAULT NULL,
  `name` varchar(60) DEFAULT NULL,
  `shortname` varchar(16) DEFAULT NULL,
  `date` date DEFAULT NULL,
  `updated` datetime DEFAULT NULL,
  `type` varchar(32) DEFAULT NULL,
  `latitude` double DEFAULT NULL,
  `longitude` double DEFAULT NULL,
  `default` tinyint(1) DEFAULT NULL,
  `private_group` int(11) DEFAULT NULL,
  `closed` tinyint(1) NOT NULL DEFAULT '0',
  `description` varchar(1024) DEFAULT NULL,
  `street` varchar(256) DEFAULT NULL,
  `external_report` varchar(8192) DEFAULT NULL,
  `pinned` tinyint(1) DEFAULT NULL,
  `article` text COMMENT 'holds the article text body for the new home page',
  `images` text COMMENT 'hold a json array of images to use in the article',
  `caption` text COMMENT 'caption for the image',
  `tags` text,
  `record_tally_unexpired` int(11) DEFAULT NULL COMMENT 'holds a tally of person records that haven''t yet expired',
  `record_tally_expired` int(11) DEFAULT NULL COMMENT 'holds a tally of person records that have already expired',
  PRIMARY KEY (`incident_id`),
  UNIQUE KEY `shortname_idx` (`shortname`),
  KEY `parent_id` (`parent_id`),
  KEY `private_group` (`private_group`),
  CONSTRAINT `incident_ibfk_1` FOREIGN KEY (`private_group`) REFERENCES `sys_user_groups` (`group_id`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB AUTO_INCREMENT=114 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `incident`
--

LOCK TABLES `incident` WRITE;
/*!40000 ALTER TABLE `incident` DISABLE KEYS */;
INSERT INTO `incident` (`incident_id`, `parent_id`, `search_id`, `name`, `shortname`, `date`, `updated`, `type`, `latitude`, `longitude`, `default`, `private_group`, `closed`, `description`, `street`, `external_report`, `pinned`, `article`, `images`, `caption`, `tags`, `record_tally_unexpired`, `record_tally_expired`) VALUES (1,NULL,NULL,'Test Exercise','test','2013-04-16',NULL,'TEST',29.980537,103.013261,1,NULL,0,'for the Test Exercise','Yaan, Sichuan, China','',NULL,'<div><span class=\"wysiwyg-font-size-large\"><b>Headline for this Event is Massive</b></span></div><div><span class=\"wysiwyg-font-size-small\">source: cnn.com</span><br><br></div><div>Lorem ipsum dolor sit amet, consectetuer adipiscing elit. Nam cursus. Morbi ut mi. Nullam enim leo, egestas id, condimentum at, laoreet mattis, massa. Sed eleifend nonummy diam. Praesent mauris ante, elementum et, bibendum at, posuere sit amet, nibh. Duis tincidunt lectus quis dui viverra vestibulum. Suspendisse vulputate aliquam dui. Nulla elementum dui ut augue. Aliquam vehicula mi at mauris. Maecenas placerat, nisl at consequat rhoncus, sem nunc gravida justo, quis eleifend arcu velit quis lacus. Morbi magna magna, tincidunt a, mattis non, imperdiet vitae, tellus. Sed odio est, auctor ac, sollicitudin in, consequat vitae, orci. Fusce id felis. Vivamus sollicitudin metus eget eros.</div><div><br></div><div>Pellentesque habitant morbi tristique senectus et netus et malesuada fames ac turpis egestas. In posuere felis nec tortor. Pellentesque faucibus. Ut accumsan ultricies elit. Maecenas at justo id velit placerat molestie. Donec dictum lectus non odio. Cras a ante vitae enim iaculis aliquam. Mauris nunc quam, venenatis nec, euismod sit amet, egestas placerat, est. Pellentesque habitant morbi tristique senectus et netus et malesuada fames ac turpis egestas. Cras id elit. Integer quis urna. Ut ante enim, dapibus malesuada, fringilla eu, condimentum quis, tellus. Aenean porttitor eros vel dolor. Donec convallis pede venenatis nibh. Duis quam. Nam eget lacus. Aliquam erat volutpat. Quisque dignissim congue leo.</div><div><br></div>','tmp/plus_cache/eventArticleImage_2680_thumb.jpg','google.com','test,event,demo',0,0);
/*!40000 ALTER TABLE `incident` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `incident_seq`
--

DROP TABLE IF EXISTS `incident_seq`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `incident_seq` (
  `id` bigint(20) NOT NULL AUTO_INCREMENT COMMENT 'stores next id in sequence for the incident table',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=113 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `incident_seq`
--

LOCK TABLES `incident_seq` WRITE;
/*!40000 ALTER TABLE `incident_seq` DISABLE KEYS */;
INSERT INTO `incident_seq` (`id`) VALUES (112);
/*!40000 ALTER TABLE `incident_seq` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `ip_info`
--

DROP TABLE IF EXISTS `ip_info`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `ip_info` (
  `city` varchar(32) DEFAULT NULL,
  `region_code` varchar(8) DEFAULT NULL,
  `region_name` varchar(32) DEFAULT NULL,
  `areacode` varchar(8) DEFAULT NULL,
  `ip` varchar(16) NOT NULL,
  `zipcode` varchar(16) DEFAULT NULL,
  `longitude` double DEFAULT NULL,
  `country_name` varchar(32) DEFAULT NULL,
  `country_code` varchar(8) DEFAULT NULL,
  `metrocode` varchar(8) DEFAULT NULL,
  `latitude` double DEFAULT NULL,
  PRIMARY KEY (`ip`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='we store info obtained from freegeoip.net here';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `ip_info`
--

LOCK TABLES `ip_info` WRITE;
/*!40000 ALTER TABLE `ip_info` DISABLE KEYS */;
/*!40000 ALTER TABLE `ip_info` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `mpres_log`
--

DROP TABLE IF EXISTS `mpres_log`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `mpres_log` (
  `p_uuid` varchar(128) NOT NULL,
  `email_subject` varchar(256) NOT NULL,
  `email_from` varchar(128) NOT NULL,
  `email_date` varchar(64) NOT NULL,
  `update_time` datetime NOT NULL,
  `xml_format` varchar(16) DEFAULT NULL COMMENT 'MPRES (unstructured) or XML Format of Incoming Email'
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `mpres_log`
--

LOCK TABLES `mpres_log` WRITE;
/*!40000 ALTER TABLE `mpres_log` DISABLE KEYS */;
/*!40000 ALTER TABLE `mpres_log` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `mpres_messages`
--

DROP TABLE IF EXISTS `mpres_messages`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `mpres_messages` (
  `when` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP COMMENT 'when the script was last executed',
  `messages` text COMMENT 'the message log from the execution',
  `error_code` int(12) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='stores the message log from mpres module';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `mpres_messages`
--

LOCK TABLES `mpres_messages` WRITE;
/*!40000 ALTER TABLE `mpres_messages` DISABLE KEYS */;
/*!40000 ALTER TABLE `mpres_messages` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `ntfy_config`
--

DROP TABLE IF EXISTS `ntfy_config`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `ntfy_config` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `outType` varchar(25) NOT NULL,
  `nckey` varchar(15) NOT NULL,
  `icon` varchar(50) NOT NULL,
  `value` varchar(500) NOT NULL,
  `lastUpdate` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  `seq` int(4) NOT NULL,
  PRIMARY KEY (`id`),
  KEY `key` (`nckey`) USING BTREE,
  KEY `idx_seq` (`seq`) USING BTREE
) ENGINE=InnoDB AUTO_INCREMENT=51 DEFAULT CHARSET=latin1 COMMENT='Stores targetted destination for Social Media, Email and RSS';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `ntfy_config`
--

LOCK TABLES `ntfy_config` WRITE;
/*!40000 ALTER TABLE `ntfy_config` DISABLE KEYS */;
INSERT INTO `ntfy_config` (`id`, `outType`, `nckey`, `icon`, `value`, `lastUpdate`, `seq`) VALUES (10,'Unlimited Length','Facebook','facebook.png','345209672191890|6aece49063c0a6c2f1239b21213968f0|NLM.LPF','2012-03-30 22:02:05',1),(20,'Limited Length','Twitter','twitter.png','3C3kKcoMnnnzACkVcfQ|vlg5Zp4xwIKI9LKBUc9MMF38bqymGFmyRKdkpM|297479173-MSuh9CQCtC2m7YvN9SrN0yrUldBTfm5iIBxmL4we|MkjASli5ukEhdQtiwnZiv45yMUkbPspvjNiQClU9I','2012-03-30 21:58:35',2),(30,'Unlimited Length','Email','government-email.png','Mike Gill|mgill@mail.nih.gov','2012-03-29 23:05:49',2),(40,'Unlimited Length','Email','government-email.png','PL Dev ListServ|pl-dev-l@list.nih.gov','2012-04-17 17:09:25',3),(50,'Limited Length','SMS','sms.png','Mike Gill|3015231784|nlmlhccebdisaster|N0thingn0thing','2012-04-19 00:26:44',5);
/*!40000 ALTER TABLE `ntfy_config` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `ntfy_error`
--

DROP TABLE IF EXISTS `ntfy_error`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `ntfy_error` (
  `configid` int(11) NOT NULL,
  `username` varchar(25) NOT NULL,
  `error` varchar(2048) NOT NULL,
  `creationdate` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `ntfy_error`
--

LOCK TABLES `ntfy_error` WRITE;
/*!40000 ALTER TABLE `ntfy_error` DISABLE KEYS */;
/*!40000 ALTER TABLE `ntfy_error` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `ntfy_messages`
--

DROP TABLE IF EXISTS `ntfy_messages`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `ntfy_messages` (
  `configid` int(11) NOT NULL,
  `sysgenid` int(11) NOT NULL AUTO_INCREMENT,
  `message` varchar(500) NOT NULL,
  `user_name` varchar(128) NOT NULL,
  `sent` datetime NOT NULL,
  `created` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `status` varchar(50) NOT NULL,
  PRIMARY KEY (`sysgenid`),
  KEY `configid` (`configid`) USING BTREE,
  KEY `idx_sent` (`sent`) USING BTREE,
  CONSTRAINT `ntfy_messages_ibfk_1` FOREIGN KEY (`configid`) REFERENCES `ntfy_config` (`id`) ON DELETE NO ACTION ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=latin1 COMMENT='InnoDB free: 1446912 kB; (`configid`) REFER `plstage/ntfy_co';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `ntfy_messages`
--

LOCK TABLES `ntfy_messages` WRITE;
/*!40000 ALTER TABLE `ntfy_messages` DISABLE KEYS */;
/*!40000 ALTER TABLE `ntfy_messages` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `ntfy_short_urls`
--

DROP TABLE IF EXISTS `ntfy_short_urls`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `ntfy_short_urls` (
  `long_url` varchar(256) NOT NULL,
  `short_url` varchar(50) NOT NULL,
  PRIMARY KEY (`long_url`),
  UNIQUE KEY `short_url` (`short_url`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `ntfy_short_urls`
--

LOCK TABLES `ntfy_short_urls` WRITE;
/*!40000 ALTER TABLE `ntfy_short_urls` DISABLE KEYS */;
/*!40000 ALTER TABLE `ntfy_short_urls` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `ntfy_sitename`
--

DROP TABLE IF EXISTS `ntfy_sitename`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `ntfy_sitename` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `site` varchar(50) NOT NULL,
  PRIMARY KEY (`id`),
  UNIQUE KEY `site` (`site`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `ntfy_sitename`
--

LOCK TABLES `ntfy_sitename` WRITE;
/*!40000 ALTER TABLE `ntfy_sitename` DISABLE KEYS */;
/*!40000 ALTER TABLE `ntfy_sitename` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `ntfy_word_list`
--

DROP TABLE IF EXISTS `ntfy_word_list`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `ntfy_word_list` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `category` varchar(25) NOT NULL,
  `words` varchar(120) NOT NULL,
  PRIMARY KEY (`id`),
  KEY `category` (`category`,`words`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `ntfy_word_list`
--

LOCK TABLES `ntfy_word_list` WRITE;
/*!40000 ALTER TABLE `ntfy_word_list` DISABLE KEYS */;
/*!40000 ALTER TABLE `ntfy_word_list` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `ntfy_word_template`
--

DROP TABLE IF EXISTS `ntfy_word_template`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `ntfy_word_template` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `list` varchar(120) NOT NULL,
  `last_update` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`),
  UNIQUE KEY `list` (`list`) USING BTREE,
  KEY `last_update` (`last_update`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `ntfy_word_template`
--

LOCK TABLES `ntfy_word_template` WRITE;
/*!40000 ALTER TABLE `ntfy_word_template` DISABLE KEYS */;
/*!40000 ALTER TABLE `ntfy_word_template` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `old_passwords`
--

DROP TABLE IF EXISTS `old_passwords`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `old_passwords` (
  `p_uuid` varchar(60) NOT NULL,
  `password` varchar(100) NOT NULL DEFAULT '',
  `changed_timestamp` bigint(20) NOT NULL,
  PRIMARY KEY (`p_uuid`,`password`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `old_passwords`
--

LOCK TABLES `old_passwords` WRITE;
/*!40000 ALTER TABLE `old_passwords` DISABLE KEYS */;
INSERT INTO `old_passwords` (`p_uuid`, `password`, `changed_timestamp`) VALUES ('1','5654f930611ede368f6a148c9ff9b39d',1370980603);
/*!40000 ALTER TABLE `old_passwords` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `password_event_log`
--

DROP TABLE IF EXISTS `password_event_log`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `password_event_log` (
  `log_id` bigint(20) NOT NULL AUTO_INCREMENT,
  `changed_timestamp` timestamp NULL DEFAULT CURRENT_TIMESTAMP,
  `p_uuid` varchar(128) NOT NULL,
  `user_name` varchar(100) NOT NULL,
  `comment` varchar(100) NOT NULL,
  `event_type` varchar(32) DEFAULT NULL,
  PRIMARY KEY (`log_id`),
  KEY `p_uuid` (`p_uuid`),
  CONSTRAINT `password_event_log_ibfk_1` FOREIGN KEY (`p_uuid`) REFERENCES `person_uuid` (`p_uuid`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB AUTO_INCREMENT=4 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `password_event_log`
--

LOCK TABLES `password_event_log` WRITE;
/*!40000 ALTER TABLE `password_event_log` DISABLE KEYS */;
INSERT INTO `password_event_log` (`log_id`, `changed_timestamp`, `p_uuid`, `user_name`, `comment`, `event_type`) VALUES (1,'2013-06-11 19:51:11','3','root','::1 :: Invalid password','AUTHENTICATION FAULURE'),(2,'2013-06-11 19:51:25','3','root','::1 :: Invalid password','AUTHENTICATION FAULURE'),(3,'2013-06-11 19:51:38','3','root','::1 :: Invalid password','AUTHENTICATION FAULURE');
/*!40000 ALTER TABLE `password_event_log` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `person_deceased`
--

DROP TABLE IF EXISTS `person_deceased`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `person_deceased` (
  `deceased_id` int(11) NOT NULL AUTO_INCREMENT,
  `p_uuid` varchar(128) NOT NULL,
  `details` text,
  `date_of_death` date DEFAULT NULL,
  `location` varchar(20) DEFAULT NULL,
  `place_of_death` text,
  `comments` text,
  PRIMARY KEY (`deceased_id`),
  UNIQUE KEY `p_uuid` (`p_uuid`),
  CONSTRAINT `person_deceased_ibfk_1` FOREIGN KEY (`p_uuid`) REFERENCES `person_uuid` (`p_uuid`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `person_deceased_ibfk_2` FOREIGN KEY (`p_uuid`) REFERENCES `person_uuid` (`p_uuid`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `person_deceased`
--

LOCK TABLES `person_deceased` WRITE;
/*!40000 ALTER TABLE `person_deceased` DISABLE KEYS */;
/*!40000 ALTER TABLE `person_deceased` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `person_details`
--

DROP TABLE IF EXISTS `person_details`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `person_details` (
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
  KEY `years_old` (`years_old`),
  CONSTRAINT `person_details_ibfk_1` FOREIGN KEY (`p_uuid`) REFERENCES `person_uuid` (`p_uuid`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB AUTO_INCREMENT=2992233 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `person_details`
--

LOCK TABLES `person_details` WRITE;
/*!40000 ALTER TABLE `person_details` DISABLE KEYS */;
INSERT INTO `person_details` (`details_id`, `p_uuid`, `birth_date`, `opt_race`, `opt_religion`, `opt_gender`, `years_old`, `minAge`, `maxAge`, `last_seen`, `last_clothing`, `other_comments`) VALUES (2989677,'3',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL),(2990111,'4',NULL,NULL,NULL,NULL,NULL,18,150,'NLM (testing) Hospital',NULL,'LPF notification - disaster victim arrives at hospital triage station');
/*!40000 ALTER TABLE `person_details` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `person_followers`
--

DROP TABLE IF EXISTS `person_followers`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `person_followers` (
  `id` int(16) NOT NULL AUTO_INCREMENT,
  `p_uuid` varchar(128) NOT NULL,
  `follower_p_uuid` varchar(128) NOT NULL,
  PRIMARY KEY (`id`),
  KEY `p_uuid` (`p_uuid`),
  KEY `follower_p_uuid` (`follower_p_uuid`),
  CONSTRAINT `person_followers_ibfk_1` FOREIGN KEY (`p_uuid`) REFERENCES `person_uuid` (`p_uuid`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `person_followers_ibfk_2` FOREIGN KEY (`follower_p_uuid`) REFERENCES `person_uuid` (`p_uuid`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `person_followers`
--

LOCK TABLES `person_followers` WRITE;
/*!40000 ALTER TABLE `person_followers` DISABLE KEYS */;
/*!40000 ALTER TABLE `person_followers` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `person_notes`
--

DROP TABLE IF EXISTS `person_notes`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `person_notes` (
  `note_id` int(11) NOT NULL AUTO_INCREMENT,
  `note_about_p_uuid` varchar(128) NOT NULL,
  `note_written_by_p_uuid` varchar(128) NOT NULL,
  `note` varchar(1024) DEFAULT NULL,
  `when` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `suggested_status` varchar(3) DEFAULT NULL COMMENT 'the status of the person as suggested by the note maker',
  `suggested_location` varchar(1024) DEFAULT NULL,
  PRIMARY KEY (`note_id`),
  KEY `note_about_p_uuid` (`note_about_p_uuid`),
  KEY `note_written_by_p_uuid` (`note_written_by_p_uuid`),
  CONSTRAINT `person_notes_ibfk_1` FOREIGN KEY (`note_about_p_uuid`) REFERENCES `person_uuid` (`p_uuid`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `person_notes_ibfk_2` FOREIGN KEY (`note_written_by_p_uuid`) REFERENCES `person_uuid` (`p_uuid`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB AUTO_INCREMENT=1018 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `person_notes`
--

LOCK TABLES `person_notes` WRITE;
/*!40000 ALTER TABLE `person_notes` DISABLE KEYS */;
/*!40000 ALTER TABLE `person_notes` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `person_notes_seq`
--

DROP TABLE IF EXISTS `person_notes_seq`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `person_notes_seq` (
  `id` bigint(20) NOT NULL AUTO_INCREMENT COMMENT 'stores next id in sequence for the person_notes table',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=1017 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `person_notes_seq`
--

LOCK TABLES `person_notes_seq` WRITE;
/*!40000 ALTER TABLE `person_notes_seq` DISABLE KEYS */;
INSERT INTO `person_notes_seq` (`id`) VALUES (1016);
/*!40000 ALTER TABLE `person_notes_seq` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `person_physical`
--

DROP TABLE IF EXISTS `person_physical`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `person_physical` (
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
  UNIQUE KEY `p_uuid` (`p_uuid`),
  CONSTRAINT `person_physical_ibfk_1` FOREIGN KEY (`p_uuid`) REFERENCES `person_uuid` (`p_uuid`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `person_physical`
--

LOCK TABLES `person_physical` WRITE;
/*!40000 ALTER TABLE `person_physical` DISABLE KEYS */;
/*!40000 ALTER TABLE `person_physical` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Temporary table structure for view `person_search`
--

DROP TABLE IF EXISTS `person_search`;
/*!50001 DROP VIEW IF EXISTS `person_search`*/;
SET @saved_cs_client     = @@character_set_client;
SET character_set_client = utf8;
/*!50001 CREATE TABLE `person_search` (
  `p_uuid` tinyint NOT NULL,
  `full_name` tinyint NOT NULL,
  `given_name` tinyint NOT NULL,
  `family_name` tinyint NOT NULL,
  `alternate_names` tinyint NOT NULL,
  `expiry_date` tinyint NOT NULL,
  `updated` tinyint NOT NULL,
  `updated_db` tinyint NOT NULL,
  `opt_status` tinyint NOT NULL,
  `opt_gender` tinyint NOT NULL,
  `years_old` tinyint NOT NULL,
  `minAge` tinyint NOT NULL,
  `maxAge` tinyint NOT NULL,
  `ageGroup` tinyint NOT NULL,
  `image_height` tinyint NOT NULL,
  `image_width` tinyint NOT NULL,
  `url_thumb` tinyint NOT NULL,
  `url` tinyint NOT NULL,
  `color_channels` tinyint NOT NULL,
  `original_filename` tinyint NOT NULL,
  `icon_url` tinyint NOT NULL,
  `shortname` tinyint NOT NULL,
  `name` tinyint NOT NULL,
  `hospital` tinyint NOT NULL,
  `comments` tinyint NOT NULL,
  `last_seen` tinyint NOT NULL,
  `mass_casualty_id` tinyint NOT NULL,
  `triage_category` tinyint NOT NULL
) ENGINE=MyISAM */;
SET character_set_client = @saved_cs_client;

--
-- Table structure for table `person_seq`
--

DROP TABLE IF EXISTS `person_seq`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `person_seq` (
  `id` int(11) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `person_seq`
--

LOCK TABLES `person_seq` WRITE;
/*!40000 ALTER TABLE `person_seq` DISABLE KEYS */;
INSERT INTO `person_seq` (`id`) VALUES (4005025);
/*!40000 ALTER TABLE `person_seq` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `person_status`
--

DROP TABLE IF EXISTS `person_status`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `person_status` (
  `status_id` int(11) NOT NULL AUTO_INCREMENT,
  `p_uuid` varchar(128) NOT NULL,
  `opt_status` varchar(3) NOT NULL DEFAULT 'unk',
  `last_updated` datetime DEFAULT NULL,
  `creation_time` datetime DEFAULT NULL,
  `last_updated_db` datetime DEFAULT NULL COMMENT 'Last DB update. (For SOLR indexing.)',
  `street1` varchar(128) DEFAULT NULL COMMENT 'street address 1',
  `street2` varchar(128) DEFAULT NULL COMMENT 'street address 2',
  `neighborhood` varchar(64) DEFAULT NULL COMMENT 'neighborhood',
  `city` varchar(64) DEFAULT NULL COMMENT 'city name',
  `region` varchar(32) DEFAULT NULL COMMENT 'name of region',
  `postal_code` varchar(16) DEFAULT NULL COMMENT 'postal code',
  `country` varchar(32) DEFAULT NULL COMMENT 'name of country',
  `latitude` double DEFAULT NULL COMMENT 'gps latitude',
  `longitude` double DEFAULT NULL COMMENT 'gps longitude',
  PRIMARY KEY (`status_id`),
  UNIQUE KEY `p_uuid` (`p_uuid`),
  KEY `last_updated_db` (`last_updated_db`),
  KEY `last_updated` (`last_updated`),
  KEY `opt_status` (`opt_status`),
  CONSTRAINT `person_status_ibfk_1` FOREIGN KEY (`p_uuid`) REFERENCES `person_uuid` (`p_uuid`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB AUTO_INCREMENT=5859913 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `person_status`
--

LOCK TABLES `person_status` WRITE;
/*!40000 ALTER TABLE `person_status` DISABLE KEYS */;
INSERT INTO `person_status` (`status_id`, `p_uuid`, `opt_status`, `last_updated`, `creation_time`, `last_updated_db`, `street1`, `street2`, `neighborhood`, `city`, `region`, `postal_code`, `country`, `latitude`, `longitude`) VALUES (3297,'3','','2011-12-31 02:13:00',NULL,'2012-01-09 21:41:59',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL),(3730,'4','dec','2013-01-07 13:01:11','2011-08-08 12:20:00','2013-01-07 13:01:11',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
/*!40000 ALTER TABLE `person_status` ENABLE KEYS */;
UNLOCK TABLES;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
/*!50003 CREATE*/ /*!50017 DEFINER=`root`@`localhost`*/ /*!50003 TRIGGER `person_status_after_insert` AFTER INSERT
ON `person_status`
FOR EACH ROW BEGIN
   INSERT INTO person_update_log (p_uuid, update_time) VALUES (NEW.p_uuid, NEW.last_updated_db);
END */;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
/*!50003 CREATE*/ /*!50017 DEFINER=`root`@`localhost`*/ /*!50003 TRIGGER `person_status_after_update` AFTER UPDATE
ON `person_status`
FOR EACH ROW BEGIN
   INSERT INTO person_update_log (p_uuid, update_time) VALUES (NEW.p_uuid, NEW.last_updated_db);
END */;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;

--
-- Table structure for table `person_to_report`
--

DROP TABLE IF EXISTS `person_to_report`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `person_to_report` (
  `p_uuid` varchar(128) NOT NULL,
  `rep_uuid` varchar(128) NOT NULL,
  PRIMARY KEY (`p_uuid`,`rep_uuid`),
  KEY `rep_uuid` (`rep_uuid`),
  CONSTRAINT `person_to_report_ibfk_1` FOREIGN KEY (`p_uuid`) REFERENCES `person_uuid` (`p_uuid`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `person_to_report`
--

LOCK TABLES `person_to_report` WRITE;
/*!40000 ALTER TABLE `person_to_report` DISABLE KEYS */;
INSERT INTO `person_to_report` (`p_uuid`, `rep_uuid`) VALUES ('3',''),('4','ceb-stage-lx.nlm.nih.gov/~miernickig/vesuvius/vesuvius/www/person.4002054');
/*!40000 ALTER TABLE `person_to_report` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `person_update_log`
--

DROP TABLE IF EXISTS `person_update_log`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `person_update_log` (
  `p_uuid` varchar(128) NOT NULL,
  `update_time` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  KEY `update_time` (`update_time`),
  KEY `p_uuid` (`p_uuid`),
  CONSTRAINT `person_update_log_ibfk_1` FOREIGN KEY (`p_uuid`) REFERENCES `person_uuid` (`p_uuid`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='Full update history for accurate exports';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `person_update_log`
--

LOCK TABLES `person_update_log` WRITE;
/*!40000 ALTER TABLE `person_update_log` DISABLE KEYS */;
INSERT INTO `person_update_log` (`p_uuid`, `update_time`) VALUES ('3','2012-01-10 02:41:59'),('4','2012-04-10 18:25:01'),('4','2013-01-07 18:01:11');
/*!40000 ALTER TABLE `person_update_log` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `person_updates`
--

DROP TABLE IF EXISTS `person_updates`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `person_updates` (
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
  KEY `updated_by_p_uuid` (`updated_by_p_uuid`),
  CONSTRAINT `person_updates_ibfk_1` FOREIGN KEY (`p_uuid`) REFERENCES `person_uuid` (`p_uuid`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `person_updates_ibfk_2` FOREIGN KEY (`updated_by_p_uuid`) REFERENCES `person_uuid` (`p_uuid`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB AUTO_INCREMENT=23 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `person_updates`
--

LOCK TABLES `person_updates` WRITE;
/*!40000 ALTER TABLE `person_updates` DISABLE KEYS */;
/*!40000 ALTER TABLE `person_updates` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `person_uuid`
--

DROP TABLE IF EXISTS `person_uuid`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `person_uuid` (
  `p_uuid` varchar(128) NOT NULL,
  `full_name` varchar(100) DEFAULT NULL,
  `family_name` varchar(50) DEFAULT NULL,
  `given_name` varchar(50) DEFAULT NULL,
  `alternate_names` varchar(50) DEFAULT NULL,
  `profile_urls` text,
  `incident_id` bigint(20) DEFAULT NULL,
  `hospital_uuid` int(32) DEFAULT NULL,
  `expiry_date` datetime DEFAULT NULL,
  PRIMARY KEY (`p_uuid`),
  KEY `full_name_idx` (`full_name`),
  KEY `incident_id_index` (`incident_id`),
  KEY `hospital_index` (`hospital_uuid`),
  CONSTRAINT `person_uuid_ibfk_1` FOREIGN KEY (`incident_id`) REFERENCES `incident` (`incident_id`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `person_uuid_ibfk_2` FOREIGN KEY (`hospital_uuid`) REFERENCES `hospital` (`hospital_uuid`) ON DELETE SET NULL ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `person_uuid`
--

LOCK TABLES `person_uuid` WRITE;
/*!40000 ALTER TABLE `person_uuid` DISABLE KEYS */;
INSERT INTO `person_uuid` (`p_uuid`, `full_name`, `family_name`, `given_name`, `alternate_names`, `profile_urls`, `incident_id`, `hospital_uuid`, `expiry_date`) VALUES ('1','Root /','/','Root',NULL,NULL,NULL,NULL,NULL),('2','Email System','System','Email',NULL,NULL,NULL,NULL,NULL),('3','Anonymous User','User','Anonymous',NULL,NULL,NULL,NULL,'2012-12-29 02:13:00'),('4','TestFrom WebServices','WebServices','TestFrom',NULL,NULL,NULL,NULL,'2013-01-07 13:01:11'),('5','test dontDelete','dontDelete','test',NULL,NULL,NULL,NULL,NULL);
/*!40000 ALTER TABLE `person_uuid` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `pfif_export_log`
--

DROP TABLE IF EXISTS `pfif_export_log`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `pfif_export_log` (
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
  PRIMARY KEY (`log_index`),
  KEY `repository_id` (`repository_id`),
  CONSTRAINT `pfif_export_log_ibfk_1` FOREIGN KEY (`repository_id`) REFERENCES `pfif_repository` (`id`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `pfif_export_log_ibfk_2` FOREIGN KEY (`repository_id`) REFERENCES `pfif_repository` (`id`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `pfif_export_log`
--

LOCK TABLES `pfif_export_log` WRITE;
/*!40000 ALTER TABLE `pfif_export_log` DISABLE KEYS */;
/*!40000 ALTER TABLE `pfif_export_log` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `pfif_harvest_note_log`
--

DROP TABLE IF EXISTS `pfif_harvest_note_log`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `pfif_harvest_note_log` (
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
  KEY `repository_id` (`repository_id`),
  CONSTRAINT `pfif_harvest_note_log_ibfk_1` FOREIGN KEY (`repository_id`) REFERENCES `pfif_repository` (`id`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `pfif_harvest_note_log_ibfk_2` FOREIGN KEY (`repository_id`) REFERENCES `pfif_repository` (`id`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `pfif_harvest_note_log`
--

LOCK TABLES `pfif_harvest_note_log` WRITE;
/*!40000 ALTER TABLE `pfif_harvest_note_log` DISABLE KEYS */;
/*!40000 ALTER TABLE `pfif_harvest_note_log` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `pfif_harvest_person_log`
--

DROP TABLE IF EXISTS `pfif_harvest_person_log`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `pfif_harvest_person_log` (
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
  KEY `repository_id` (`repository_id`),
  CONSTRAINT `pfif_harvest_person_log_ibfk_1` FOREIGN KEY (`repository_id`) REFERENCES `pfif_repository` (`id`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `pfif_harvest_person_log_ibfk_2` FOREIGN KEY (`repository_id`) REFERENCES `pfif_repository` (`id`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `pfif_harvest_person_log`
--

LOCK TABLES `pfif_harvest_person_log` WRITE;
/*!40000 ALTER TABLE `pfif_harvest_person_log` DISABLE KEYS */;
/*!40000 ALTER TABLE `pfif_harvest_person_log` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `pfif_note`
--

DROP TABLE IF EXISTS `pfif_note`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `pfif_note` (
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
  `author_made_contact` varchar(5) DEFAULT NULL,
  `status` varchar(20) DEFAULT NULL,
  `email_of_found_person` varchar(100) DEFAULT NULL,
  `phone_of_found_person` varchar(100) DEFAULT NULL,
  `last_known_location` text,
  `text` text,
  `photo_url` varchar(512) DEFAULT NULL,
  PRIMARY KEY (`note_record_id`),
  KEY `p_uuid` (`p_uuid`),
  KEY `source_repository_id` (`source_repository_id`),
  KEY `linked_person_record_id` (`linked_person_record_id`),
  KEY `source_date` (`source_date`),
  CONSTRAINT `pfif_note_ibfk_2` FOREIGN KEY (`source_repository_id`) REFERENCES `pfif_repository` (`id`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='IMPORT WILL FAIL if you add foreign key constraints.';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `pfif_note`
--

LOCK TABLES `pfif_note` WRITE;
/*!40000 ALTER TABLE `pfif_note` DISABLE KEYS */;
INSERT INTO `pfif_note` (`note_record_id`, `p_uuid`, `source_version`, `source_repository_id`, `linked_person_record_id`, `entry_date`, `author_name`, `author_email`, `author_phone`, `source_date`, `author_made_contact`, `status`, `email_of_found_person`, `phone_of_found_person`, `last_known_location`, `text`, `photo_url`) VALUES ('ceb-stage-lx.nlm.nih.gov/~miernickig/vesuvius/vesuvius/www/note.1982','NULL','NULL',NULL,NULL,'2012-12-04 22:58:57','hs hs','hs@hs.com',NULL,'0000-00-00 00:00:00',NULL,NULL,NULL,NULL,NULL,NULL,NULL),('ceb-stage-lx.nlm.nih.gov/~miernickig/vesuvius/vesuvius/www/note.1983','NULL','NULL',NULL,NULL,'2012-12-04 22:59:16','hs hs','hs@hs.com',NULL,'0000-00-00 00:00:00',NULL,NULL,NULL,NULL,NULL,NULL,NULL),('ceb-stage-lx.nlm.nih.gov/~miernickig/vesuvius/vesuvius/www/note.1984','NULL','NULL',NULL,NULL,'2012-12-04 23:00:00','hs hs','hs@hs.com',NULL,'0000-00-00 00:00:00',NULL,NULL,NULL,NULL,NULL,NULL,NULL),('ceb-stage-lx.nlm.nih.gov/~miernickig/vesuvius/vesuvius/www/note.1985','NULL','NULL',NULL,NULL,'2012-12-04 23:01:58','hs hs','hs@hs.com',NULL,'0000-00-00 00:00:00',NULL,'believed_missing',NULL,NULL,NULL,NULL,NULL);
/*!40000 ALTER TABLE `pfif_note` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `pfif_note_seq`
--

DROP TABLE IF EXISTS `pfif_note_seq`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `pfif_note_seq` (
  `id` int(11) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `pfif_note_seq`
--

LOCK TABLES `pfif_note_seq` WRITE;
/*!40000 ALTER TABLE `pfif_note_seq` DISABLE KEYS */;
INSERT INTO `pfif_note_seq` (`id`) VALUES (3516);
/*!40000 ALTER TABLE `pfif_note_seq` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `pfif_person`
--

DROP TABLE IF EXISTS `pfif_person`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `pfif_person` (
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
  `given_name` varchar(100) DEFAULT NULL,
  `family_name` varchar(100) DEFAULT NULL,
  `alternate_names` varchar(100) DEFAULT NULL,
  `profile_urls` text,
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
  `description` text,
  PRIMARY KEY (`p_uuid`),
  KEY `source_repository_id` (`source_repository_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `pfif_person`
--

LOCK TABLES `pfif_person` WRITE;
/*!40000 ALTER TABLE `pfif_person` DISABLE KEYS */;
/*!40000 ALTER TABLE `pfif_person` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `pfif_repository`
--

DROP TABLE IF EXISTS `pfif_repository`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `pfif_repository` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `name` varchar(100) NOT NULL,
  `incident_id` bigint(11) DEFAULT NULL,
  `base_url` varchar(512) NOT NULL,
  `resource_type` varchar(6) NOT NULL DEFAULT 'person',
  `role` varchar(6) NOT NULL,
  `granularity` varchar(20) NOT NULL DEFAULT 'YYYY-MM-DDThh:mm:ssZ',
  `deleted_record` varchar(10) NOT NULL DEFAULT 'no',
  `sched_interval_minutes` int(11) NOT NULL DEFAULT '0',
  `log_granularity` varchar(20) NOT NULL DEFAULT '24:00:00',
  `first_entry` datetime DEFAULT NULL,
  `last_entry` datetime DEFAULT NULL,
  `total_persons` int(11) DEFAULT '0',
  `total_notes` int(11) DEFAULT '0',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=175 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `pfif_repository`
--

LOCK TABLES `pfif_repository` WRITE;
/*!40000 ALTER TABLE `pfif_repository` DISABLE KEYS */;
/*!40000 ALTER TABLE `pfif_repository` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `phonetic_word`
--

DROP TABLE IF EXISTS `phonetic_word`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `phonetic_word` (
  `encode1` varchar(50) DEFAULT NULL,
  `encode2` varchar(50) DEFAULT NULL,
  `pgl_uuid` varchar(128) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `phonetic_word`
--

LOCK TABLES `phonetic_word` WRITE;
/*!40000 ALTER TABLE `phonetic_word` DISABLE KEYS */;
/*!40000 ALTER TABLE `phonetic_word` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `plus_access_log`
--

DROP TABLE IF EXISTS `plus_access_log`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `plus_access_log` (
  `access_time` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `application` varchar(32) DEFAULT NULL,
  `version` varchar(16) DEFAULT NULL,
  `ip` varchar(16) DEFAULT NULL,
  `call` varchar(64) DEFAULT NULL,
  `api_version` varchar(8) DEFAULT NULL,
  `user_name` varchar(100) DEFAULT NULL COMMENT 'users.user_name that make the call'
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `plus_access_log`
--

LOCK TABLES `plus_access_log` WRITE;
/*!40000 ALTER TABLE `plus_access_log` DISABLE KEYS */;
/*!40000 ALTER TABLE `plus_access_log` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `plus_ping`
--

DROP TABLE IF EXISTS `plus_ping`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `plus_ping` (
  `machine_name` varchar(64) NOT NULL COMMENT 'name of the machine pinging',
  `milliseconds` int(11) NOT NULL COMMENT 'ping time',
  `time_stamp` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP COMMENT 'when pinged',
  `ip` varchar(16) DEFAULT NULL,
  `device_id` varchar(64) DEFAULT NULL,
  `app_name` varchar(16) DEFAULT NULL,
  `app_version` varchar(16) DEFAULT NULL,
  `operating_system` varchar(16) DEFAULT NULL,
  `device_username` varchar(16) DEFAULT NULL,
  `pl_username` varchar(16) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `plus_ping`
--

LOCK TABLES `plus_ping` WRITE;
/*!40000 ALTER TABLE `plus_ping` DISABLE KEYS */;
/*!40000 ALTER TABLE `plus_ping` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `plus_report_log`
--

DROP TABLE IF EXISTS `plus_report_log`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `plus_report_log` (
  `report_id` int(16) NOT NULL AUTO_INCREMENT,
  `p_uuid` varchar(128) NOT NULL,
  `report_time` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `enum` varchar(16) DEFAULT NULL,
  PRIMARY KEY (`report_id`),
  KEY `p_uuid` (`p_uuid`),
  CONSTRAINT `plus_report_log_ibfk_1` FOREIGN KEY (`p_uuid`) REFERENCES `person_uuid` (`p_uuid`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB AUTO_INCREMENT=31 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `plus_report_log`
--

LOCK TABLES `plus_report_log` WRITE;
/*!40000 ALTER TABLE `plus_report_log` DISABLE KEYS */;
/*!40000 ALTER TABLE `plus_report_log` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `pop_outlog`
--

DROP TABLE IF EXISTS `pop_outlog`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `pop_outlog` (
  `outlog_index` int(11) NOT NULL AUTO_INCREMENT,
  `mod_accessed` varchar(8) NOT NULL,
  `time_sent` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  `send_status` varchar(8) NOT NULL,
  `error_message` varchar(512) NOT NULL,
  `email_subject` varchar(256) NOT NULL,
  `email_from` varchar(128) NOT NULL,
  `email_recipients` varchar(256) NOT NULL,
  PRIMARY KEY (`outlog_index`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `pop_outlog`
--

LOCK TABLES `pop_outlog` WRITE;
/*!40000 ALTER TABLE `pop_outlog` DISABLE KEYS */;
/*!40000 ALTER TABLE `pop_outlog` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `rap_log`
--

DROP TABLE IF EXISTS `rap_log`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `rap_log` (
  `rap_id` int(16) NOT NULL AUTO_INCREMENT,
  `p_uuid` varchar(128) NOT NULL,
  `report_time` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  PRIMARY KEY (`rap_id`),
  KEY `p_uuid` (`p_uuid`),
  CONSTRAINT `rap_log_ibfk_1` FOREIGN KEY (`p_uuid`) REFERENCES `person_uuid` (`p_uuid`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB AUTO_INCREMENT=1977 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `rap_log`
--

LOCK TABLES `rap_log` WRITE;
/*!40000 ALTER TABLE `rap_log` DISABLE KEYS */;
/*!40000 ALTER TABLE `rap_log` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `rez_pages`
--

DROP TABLE IF EXISTS `rez_pages`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `rez_pages` (
  `rez_page_id` int(11) NOT NULL AUTO_INCREMENT,
  `rez_menu_title` varchar(64) NOT NULL,
  `rez_menu_order` int(11) NOT NULL,
  `rez_content` mediumtext NOT NULL,
  `rez_timestamp` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  `rez_visibility` varchar(16) NOT NULL,
  PRIMARY KEY (`rez_page_id`)
) ENGINE=InnoDB AUTO_INCREMENT=3 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `rez_pages`
--

LOCK TABLES `rez_pages` WRITE;
/*!40000 ALTER TABLE `rez_pages` DISABLE KEYS */;
INSERT INTO `rez_pages` (`rez_page_id`, `rez_menu_title`, `rez_menu_order`, `rez_content`, `rez_timestamp`, `rez_visibility`) VALUES (-102,'TriagePic Release Notes 2012',-52,'<div style=\"\">\n<script type=\"text/javascript\" src=\"res/js/jquery.js\"></script>\n<script type=\"text/javascript\" src=\"res/js/animatedcollapse.js\"></script>\n<script>\nanimatedcollapse.addDiv(\'v151\', \'fade=1,hide=1\');\nanimatedcollapse.addDiv(\'v150\', \'fade=1,hide=1\');\nanimatedcollapse.addDiv(\'v149\', \'fade=1,hide=1\');\nanimatedcollapse.addDiv(\'v148\', \'fade=1,hide=1\');\nanimatedcollapse.addDiv(\'v147\', \'fade=1,hide=1\');\nanimatedcollapse.addDiv(\'v146\', \'fade=1,hide=1\');\nanimatedcollapse.addDiv(\'v145\', \'fade=1,hide=1\');\nanimatedcollapse.addDiv(\'v143\', \'fade=1,hide=1\');\nanimatedcollapse.addDiv(\'v142\', \'fade=1,hide=1\');\nanimatedcollapse.init();\nanimatedcollapse.ontoggle=function($, divobj, state){}\n</script>\n\n<h2>May, 2012</h2>\n\n<h3>Version 1.51<br></h3>\n\n<ul>\n<li>Log in now requires PL credentials that are more restrictive, namely,\nhospital staff (hs) or hospital staff administration (hsa), not just general PL user.</li>\n<li>Selecting a Patient ID number already used by a different casualty of a particular disaster is now actively discouraged.\nIn addition to the conflicted number appearing with an orange background as before:<br>\n<ul>\n<li>Other text fields are also tinted light orange, and text entry blocked; and</li>\n<li>Clicks on button, including those that send a report, are intercepted.</li>\n</ul>\n</li><li>Uniqueness of Patient ID is now being enforced per-disaster-event, instead of across events.</li>\n<li>A few problems with practice mode were fixed, and the app title bar made terse.</li>\n</ul>\n\n&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href=\"javascript:animatedcollapse.toggle(\'v151\')\">Show/Hide Details...</a>\n<br>\n<br>\n<div id=\"v151\" display=\"none;\">\n\n<ul>\n<li>The new requirement for hs/hsa credentials is verified by a new web service.</li>\n<li>This should avoid these problems that occurred if non-hs/non-hsa credentials were entered on startup\n(which was particularly likely when PL credentials were newly entered after a fresh install or wipe of c:\\Shared TriagePic Data):<br>\n<br>\n<ul>\n<li>Patient ID conflicts were not properly recognized, and so not rendered in orange.</li>\n<li>When a report was sent, it would fail due to bad credentials.\n(An attempt was made to recover and ask for new credentials, but that didn’t ultimately work.)</li>\n</ul>\n</li>\n<li>When the Patient ID is orange on the Main Info tab (of the main window,\nor the Outbox’s View and Edit window when in edit mode), data entry and sending is disallowed.\nOn the main form, if you try to send or type in a field, then you get a message:<br>\n<br>\n                This Patient ID has already been used for this disaster event,<br>\n                so you can\'t use it here for another patient.<br>\n                Can you assign a different ID?<br>\n                <br>\n                (Select \'No\' for info on changing the record and other options.)<br>\n<br>\nThe foregoing dialogs to the user are similar but not identical for the View and Edit window.</li>\n<li>Preventing spurious Patient ID reuse is beneficial.\nIt prevents generation of new files that may have name-conflicts with pre-existing files in the ‘sent’ folder.\n(Such conflicts are handled by complaints to the user and renaming, but the renaming can cause its own problems.)</li>\n<li>To support a model where Patient ID must be unique within a disaster event, but not across events:<br>\n<br>\n<ul>\n<li>Adjustments were made to the “orange” processing.</li>\n<li>In Outbox, when opening to edit, an Outbox record and Patient Report record are matched.\nBefore, this matching was based on Patient ID and zone.  This now includes event as well.</li>\n</ul>\n</li><li>When an attempt to send a report to PL by web service fails, the subsequent logic is simplified.  A revised dialog appears:<br>\n<br>\n                    This report couldn\'t be sent to PL by web services.<br>\n                    Try by e-mail instead?<br>\n                    <br>\n                     (If you choose \'No\', no email send will be attempted,<br>\n                     including to your requested email recipients.<br>\n                     You can later try to resend this report from the Outbox.)<br>\n<br>\nPreviously, if you chose ‘No’, it would still send to the recipients but not PL, which was confusing.</li>\n<li>On startup, if the path to an .lp2 file is missing from PatientReports.xml\n(which was the case for any patient processed before March), this is now repaired,\ninstead of resulting in an exception.\n(A test with patient data starting mid-December, 2011 seemed compatible with TriagePic 1.51.)</li>\n<li>Rows in the Outbox having a Patient ID with short-form prefix “Prac-” were causing an exception when double-clicked.\nFixed.   Also, these now show the full prefix “Practice-” in the View and Edit window without truncation; a narrower font makes this possible.</li>\n<li>The lengthy text in the app title bar has been shortened to show,\nfor example, “TriagePic 1.51”, which is better when the app is minimized.\nSome of the longer text, particularly the build date, has been moved to the title of the “About” window\n(accessed by right-clicking on the app title bar).</li>\n<li>Preliminary work has been done on soliciting more exact email settings data during fresh installs\n(or wipes of Shared TriagePic Data), namely choice of email profile and custom ‘From’ entry.</li>\n</ul>\n</div>\n\n\n<h3>Version 1.50<br></h3>\n\n<ul>\n<li>It is highly recommended to manually delete the contents of “C:\\Shared TriagePic Data\\” before testing v 1.50,\nsince file naming conventions have changed.</li>\n<li>A TriagePic-originated patient record can now be edited and sent to PL and other recipients multiple times.\nEach such time increments a “message number”.</li>\n<li>Consistency checking and repair during startup is further improved.</li>\n<li>Just-reported bugs involving email attachments, file moving, and startup were fixed.</li>\n</ul>\n\n&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href=\"javascript:animatedcollapse.toggle(\'v150\')\">Show/Hide Details...</a>\n<br>\n<br>\n<div id=\"v150\" display=\"none;\">\n\n<ul>\n<li>Another status code is added: “QN”.\nIt means that the first attempt to send failed (by both web services and email),\nresulting in the “N” code, and the user then queued up another attempt.\n(Because this is a subsequent attempt, data files already exists and don’t have to be generated.)</li>\n<li>The filename format of patient-specific files that TriagePic generates is now changed,\nand should greatly reduce problems due to filename collisions.\nAs an example, consider a patient whose single image is first reported to PL and email recipients as:<br>\n<br>\n911-149 Red.jpg<br>\n<br>\nIf this report is later edited at TriagePic and resent, the image file will now be called (assuming no change in zone):<br>\n<br>\n911-149 Msg #2 Red.jpg<br>\n<br>\nwhere Msg denotes the “message” or version number.\nAssociated files with other extensions are treated similarly.\nFurther edits and resends will continue to increment the Msg #.</li>\n<li>The Msg # will also appear:\n<ul>\n<li>In abbreviated form within the Outbox’s “Sent” column.  (e.g., as a “#2” suffix)</li>\n<li>In the email subject line, such as:<br>\n<br>\nNew Disaster Patient #911-149 Msg #2 Arrived at Suburban<br>\n<br>\n</li><li>In some of the transient status line messages.</li>\n</ul>\n</li><li>Architecture changes during the recent queue implementation caused incomplete clearing of the list of email attachments.\nThis caused multiple problems, now fixed, among them:\n<ul>\n<li>Image file attachments from previous patients being added to email about a new patient.</li>\n<li>The message “Unable to locate source file located at C:\\Shared TriagePic Data/processed/911-913 Yellow.jpg Report this to developer.”,\nthen eventual fatal lockup with message: “uncaught thread exception: Could not find file…” (same).</li>\n</ul>\n</li><li>More generally “Uncaught thread exception: Could not find file..”, is now caught in the above circumstance,\ngives a nicer error message, and lets TriagePic continue.</li>\n<li>Similarly with “Uncaught thread exception: The process cannot access the file because it is in use by another process”.\n(But underlying causes of this remain to be further addressed.)</li>\n<li>During startup consistency checking:\n<ul>\n<li>If there’s an entry in PatientReports.xml that’s missing an Outbox row (from outbox.xml),\nthe latter is now added, and noted to the user and in the RepairLog.txt.</li>\n<li>For any current report appearing in the Outbox as entirely sent (e.g., code begins with “Y” or “y”, with no “-“ indicators),\nif there are any files (e.g., .lpf, .lp2, .pfif, .jpg, .kml) that were not properly moved to the “sent” folder,\nthis is fixed and similarly reported.  (This checking does not include deleted or superseded files at this time.)</li>\n</ul>\n</li><li>On a new install, or if “C:\\Shared TriagePic Data\\” is manually wiped and then regenerated during TriagePic startup,\nthe default disaster event (and its type) has been changed to “Test with TriagePic - TEST/DEMO/DRILL”\nfrom old-style “Unnamed TEST or DEMO”.  This removes an unhelpful warning message on first startup.</li>\n\n</ul>\n</div>\n\n\n<h3>Version 1.49<br></h3>\n\n<ul>\n  <li>Besides sporting the new TriagePic logo, seen on the splash screen and elsewhere,\n  this is the first version that uses the send-report queue and its parallel processing for edit and resend.\n  Also, if an earlier send attempt failed, it may be repeated (by manual request only, in this release).\n  Some bugs introduced last time with queuing were fixed.</li>\n</ul>\n\n&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href=\"javascript:animatedcollapse.toggle(\'v149\')\">Show/Hide Details...</a>\n<br>\n<br>\n<div id=\"v149\" display=\"none;\">\n<ul>\n<li>Queuing introduced in the last release has multiple patient reports, and sometimes the wrong one was referenced.\nAs a result, some field values reported by email were empty or otherwise misreported,\n in both the body of the email and certain XML attachments.  Fixed.</li>\n<li>The re-sending process should work as expected with “good connectivity”.\nFor “adverse/lost connectivity”, testing so far as been limited to:\n<ul>\n<li>Removing the network cable immediately before a send attempt;</li>\n<li>Using the wrong email service profile immediately before a send attempt; then correcting that and manually resending.</li>\n</ul>\nThe next release will get further stress-testing and adjustments as needed.</li>\n<li>The connectivity traffic light will now show red if error due to total connection failure, not just delay over 3 seconds.</li>\n<li>Be aware that at this time:\n<ul>\n<li>If there is no connectivity on TriagePic startup,\nthe user can’t get past the dialog asking for PL credentials.</li>\n<li>While a record update should work, an “update of an update” may misbehave,\ndue to filename collision issues.  (A better file-versioning architecture is planned).</li>\n<li>PL so far implements a TriagePic update suboptimally, as “delete old, then create new”.</li>\n</ul>\n</li><li>Under the Outbox tab, there is now a “Show ‘Sent’ Codes” link,\nthat brings up a brief summary of what the codes in the Sent column mean:<br>\n<br>\n<p>Codes for sending of updated reports will have “#2” as a suffix<br>\n<br>\n    Code = Meaning<br>\n<br>\n	Q = report is queued for sending<br>\n	Y = sent successfully to PL by web service…<br>\n	&nbsp;&nbsp;Y+ = ...and email sent to others OK<br>\n	&nbsp;&nbsp;Y- = ...but email to others failed<br>\n<br>\n	When send to PL by web services fails:<br>\n	y = send to PL succeeded by email...<br>\n	&nbsp;&nbsp;y+ = ...and email sent to others OK<br>\n	N = all email sends failed, including to PL<br>\n<br>\n	If original send got to PL, but not email recipients:<br>\n	&nbsp;&nbsp;QE = report is queued, to try email again<br>\n<br>\n	Internal or configuration error codes:<br>\n	X1 = missing path to .lp2 file<br>\n	X2 = missing .lp2 file<br>\n	X3 = unreadable .lp2 file<br>\n	X4 = unrecognized Sent code in queue (should begin with Q or QE)</p>\nThese are subject to change, e.g., to make them less geeky.</li>\n<li>The code “QE” and code suffix “#2” are new, as is the corresponding functionality.\n(Treatment of “#2” is an incomplete placeholder, and will be generalized to “#n”,\nwhere n is an update number, in the next release.)</li>\n<li>At this time, a report that is in the queue or in the process of being sent cannot be edited.\nAttempting to unlock a report for editing whose Sent code starts with “Q” will complain:<br>\n<br>\n<p>Current Limitation:<br>\nThis report awaits sending in the queue, so can\'t be unlocked to edit.<br>\nClose this view, establish a network connection, and wait for sending to succeed or fail.</p>\n</li>\n<li>For email, there’s a new “Distribution” tab on the View and Edit form,\nsimilar to “Distribution” on the main form.\nThe content shows the email distribution lists for the current record, as well as, for reference,\nthe distribution lists for new records.  The former can be unlocked to edit.</li>\n<li>In the Outbox, when the user opens a record that didn’t send successfully before,\nwith Sent code that starts with “N”,  there is first a message that begins:<br>\n<br>\n<p>Since the previous attempt to send this report failed,<br>\nwould you just like to try to send it again, unchanged?</p>\nOr if the Sent code starts with “Y-”, the message begins:<br>\n<br>\n<p>The previous attempt to send this report to PL succeeded,<br>\nbut failed to send to email recipients.  Would you like to<br>\ntry to send it again, unchanged, just to the email recipients?If so, once the \'View and Edit Patient Report\' opens, hit the<br>\nsame zone button used earlier (the only one of full height),<br>\nthat has been temporarily enabled for this purpose.</p>\nAnd there may be this additional note:<br>\n<br>\n<p>CAUTION:  Doing so will send to the previously-specified email<br>\nrecipients for this record, which DIFFERS from the current email<br>\nrecipients for new records.  The \'Distribution\' tab of the<br>\n\'View and Edit Patient Report\' has details.  Consider unlocking to<br>\nedit this record\'s Distribution lists before sending.</p>\n</li>\n<li>If a send attempt did not succeed 100%, files are now by design left in the “processed” folder (to try again),\nnot in “sent”.  Supportive of this, on startup, the “processed” folder is no longer purged.</li>\n<li>On startup, The user may see:<br>\n<br>\n<p>WARNING: Inconsistency was found in local patients records.<br>\nThis is usually due to a previous abnormal program termination.<br>\nRepairs were done to the local records of some patients.<br>\nSee c:\\\\Shared TriagePic Settings\\\\RepairLog.txt for details.</p>\nConsistency check and repair is so far limited to the Sent code of each patient between Outbox.xml\nand PatientReportData.xml.  This will be further extended.</li>\n<li>An additional status-line message, “Trying to Send Email…”, now appears, so the predecessor message,\n“Preparing Email Message…” will be appear far more briefly.\nThe new message spans both the email server connection attempt and subsequent transmittal.</li>\n</ul>\n</div>\n\n\n\n<h2>March, 2012</h2>\n\n<h3>Version 1.48<br></h3>\n<ul>\n<li>A problem affecting startup after an upgrade is addressed.\nAlso, this is the first version that uses the job queue and its parallel processing for the send process (though not yet edit and resend).</li>\n</ul>\n\n&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href=\"javascript:animatedcollapse.toggle(\'v148\')\">Show/Hide Details...</a>\n<br>\n<br>\n<div id=\"v148\" display=\"none;\">\n\n<ul>\n<li>The sending process should work as expected with “good connectivity”.\nIt has not yet been tested with “adverse/lost connectivity”, for which further adjustments are anticipated.</li>\n<li>When a zone button is pressed for a fresh report, the status line will read “Posting to send <subject>…” .\nSubsequently, the Outbox will show this with “Sent” column = “Q”, for “queued new report”.\nAs usual, the timestamp is when the zone button was pressed, not when sending starts or ends.\nOnce sending has occurred, the Outbox Sent code will change.</subject></li>\n<li>If an unrecoverable error is detected during dequeuing a job and fetching its content from the .lp2 file,\nthe Outbox Sent code may show an “Xn” value:\n<ul>\n<li>X1 = missing path to .lp2 file;</li>\n<li>X2 = missing .lp2 file;</li>\n<li>X3 = unreadable .lp2 file;</li>\n<li>X4 = unrecognized Send code (should be “Q” or [when edit/resend is possible] “q”).</li>\n</ul>\nThese are expected to be rare.</li>\n<li>On startup, if a problem occurred during decryption of PL user name or password,\nit could manifest itself unhelpfully as a dialog saying “Unhandled exception, array cannot be null”, followed by exit.\nIt now will say (for instance):<br>\n<br>\n<code>  \n<font size=\"3\">The saved value of your PL user name could not be decrypted.<br>\n(This can happen if certain TriagePic settings were hand-edited<br>\nor copied from another Windows machine.)</font><br>\n</code>\n<br>\nfollowed by a resolicitation of PL user name and password.\nError detection and recovery during this phase of startup are now more robust.</li>\n<li>More effort is made to detect and correct certain problems in the file UsersAndVersions.xml,\nnamely duplicate entries for a user or bad version number.\nOn new creation of this file, LastCompatibleVersion is now given as 1.47, instead of 1.07.</li>\n</ul>\n</div>\n\n<h2>February, 2012</h2>\n\n<h3>Version 1.47<br></h3>\n<ul>\n<li>This is minor maintenance release.</li>\n</ul>\n\n&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href=\"javascript:animatedcollapse.toggle(\'v147\')\">Show/Hide Details...</a>\n<br>\n<br>\n<div id=\"v147\" display=\"none;\">\n\n<ul>\n<li>Versioning numeration of PL web services has changed. The successor to “2.3” is “24”.\nTriagePic supports this.</li>\n\n<li>When logging on as administrator (to install the full .NET 4 prerequisite) and running the TriagePic update,\nan “Array cannot be null” error could occur when trying to decrypt the PL user name.\nThis could be overcome by retries/reinstalls (but circumstances are murky).\nA possible code fix is now applied.</li>\n</ul>\n</div>\n\n<h3>Version 1.46<br></h3>\n<hr style=\"height: 1px; background-color: rgb(255, 255, 255); border-width: 1px medium medium; border-style: solid none none; border-color: rgb(229, 234, 239) -moz-use-text-color -moz-use-text-color; margin-bottom: 15px;\">\n\n<ul>\n<li>A traffic light symbol at the lower left edge indicates if TriagePic is in contact with PL.\nEvery 3 seconds, it blinks as it tests the round-trip lag time.\nNormal responsiveness (under ½ second) is shown in green, effective connectivity loss (over 3 seconds) is red, and in-between is yellow.</li>\n<li>If there is a persistent red traffic light or other signs of connectivity problems, the user should consider corrective measures.\nFor mobile deployments, these include checking that WiFi is on and logged in, moving closer to the nearest hotspot, and boosting battery charge.\nAlso, confirm with the administrator that PL is running.</li>\n<li>TriagePic now requires .Net 4 instead of 3.5, and will install .Net 4 during update.</li>\n</ul>\n\n&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href=\"javascript:animatedcollapse.toggle(\'v146\')\">Show/Hide Details...</a>\n<br>\n<br>\n<div id=\"v146\" display=\"none;\">\n\n<ul>\n<li>This release begins the long process to implement a send queue with parallel processing.\nThis will eventually allow a more responsive user interface with faster patient throughput, smarter error recovery,\nand automatic retries after wireless (and other) network blockages.</li>\n<li>The traffic light, using PL’s new “ping” web service, allows a simple lag-time test.\nThis supplements other connection-quality indicators, such as wireless signal strength that Windows provides, or bandwidth-testing tools.\nThe thresholds of ½ second and 3 seconds are arbitrary.\nWhile being at the edge of your wireless range can increase lag time, there are many other factors.\nFor example, a satellite hop in the path between TriagePic and PL can push roundtrip time over ½ second.\nCurrently, TriagePic becomes unusable in practice when connectivity degrades substantially.\nThis will be relieved as the send queue becomes real.</li>\n<li>.Net 4 provides a suite of new C# programming tools for threading and parallelism.\nSome of these are used for the traffic light.\nDeploying these tools to build the send queue mechanisms has begun, but should not impact this release.</li>\n</ul>\n</div>\n\n<h3>Version 1.45<br></h3>\n<hr style=\"height: 1px; background-color: rgb(255, 255, 255); border-width: 1px medium medium; border-style: solid none none; border-color: rgb(229, 234, 239) -moz-use-text-color -moz-use-text-color; margin-bottom: 15px;\">\n\n<ul>\n<li>To save WiFi bandwidth and improve throughput during resends, this version suppresses photo re-transmissions.  A bug during failed email sends is also fixed.</li>\n</ul>\n\n&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href=\"javascript:animatedcollapse.toggle(\'v145\')\">Show/Hide Details...</a>\n<br>\n<br>\n<div id=\"v145\" display=\"none;\">\n\n<ul>\n<li>Consider a patient record with one or more photos that was successfully sent to PL, then edited and resent (by the normal web services).  For every photo that was part of the original send, only the associated text data (e.g., filename, caption) is resent, not the image itself.</li>\n<li>To accomplish this, when first sending an embedded photo, the standard EDXL wrapper now includes the jpeg filesize in bytes and the FIPS140-2 -compliant SHA-1 hash over the file data.  When resending the same photo (as determined by hash matching), the filesize and hash (as well as filename with optional caption and primary/secondary flag) are re-reported, but the embedded image itself is not.  PL will use the hash value to locate its copy of the image.</li>\n<li>If also distributing by email, and the email fails (e.g., due to selecting an inappropriate email profile), previously an endless fatal loop of errors would ensue (reporting email failure and file locking).  A short-term fix is now in place.</li>\n<li>The dialog box reporting such an email failure now includes the line:<br>\n<code>Often this is due to an email profile problem under the \'Email Setup\' tab.</code>\n</li>\n</ul>\n</div>\n\n<h2>January, 2012</h2>\n\n<h3>Version 1.44<br></h3>\n<hr style=\"height: 1px; background-color: rgb(255, 255, 255); border-width: 1px medium medium; border-style: solid none none; border-color: rgb(229, 234, 239) -moz-use-text-color -moz-use-text-color; margin-bottom: 15px;\">\n\n<ul>\n<li>On resend, adding additional photos should no longer cause a fatal error.</li>\n<li>Verification of photo-related policies now applies to resends, and solicitation of reasons for overriding such policies is more in tune with the resend context.</li>\n</ul>\n\n<h3>Version 1.43<br></h3>\n<hr style=\"height: 1px; background-color: rgb(255, 255, 255); border-width: 1px medium medium; border-style: solid none none; border-color: rgb(229, 234, 239) -moz-use-text-color -moz-use-text-color; margin-bottom: 15px;\">\n\n<ul>\n<li>In the “View and Edit Patient Report” form, editing the Patient ID prior to resending is now possible.</li>\n</ul>\n\n&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href=\"javascript:animatedcollapse.toggle(\'v143\')\">Show/Hide Details...</a>\n<br>\n<br>\n<div id=\"v143\" display=\"none;\">\n\n<ul>\n<li>When the form is unlocked for editing, the associated controls are now enabled:\n<ul>\n<li>The Patient ID field is directly editable, and will have a faint blue tint (web color “azure”) to indicate the shown value is unchanged from the pre-unlocking value.  If the value is then changed to something else already used, an orange warning color appears, as with the main form.\n</li><li>The + or – buttons can be tapped to increment/decrement Patient ID by 1, or held down to roll its value.\n</li><li>Constraints on the format of Patient ID here are the same as in the main form, as determined by hospital-specific policy\n(By design, only the mutable part of the Patient ID can be edited, not the fixed prefix.)</li>\n<li>The Practice Mode checkbox is active.</li>\n<li>The “Delete Record…” button, instead of disappearing upon unlock, becomes a “Patient ID Tools…” button.\nIf the persistent Patient ID Tools form was already open on-screen, it will remain so.\nRecall that this form allows entry by on-screen touchpad and by barcode scan.</li>\n</ul>\n</li>\n<li>In order to edit and resend a report with photos, certain aspects of policy related to photos are not yet enforced.\nInternal work was started to address this, but will not be apparent until a subsequent release.</li>\n<li>A report can be edited and resent more than once.</li>\n</ul>\n</div>\n\n<h3>Version 1.42<br></h3>\n<hr style=\"height: 1px; background-color: rgb(255, 255, 255); border-width: 1px medium medium; border-style: solid none none; border-color: rgb(229, 234, 239) -moz-use-text-color -moz-use-text-color; margin-bottom: 15px;\">\n\n<ul>\n<li>This version allows editing (with some exceptions) and resending of a basic patient report to PL by web services.\nAs before, this process begins by selecting a row of interest in the Outbox, and bringing up the “View and Edit Patient Report” window.</li>\n<li>View and Edit’s photo-related controls are now active, and become all visible and enabled when unlocked for editing.\nThe unlock button and associated icons and dialogs have changed.</li>\n<li>This release still does not allow editing of Patient ID and those controls remain disabled.  It also doesn’t resend via email.</li>\n<li>On the server side, be aware that the initial implementation of resend at PL merely deletes the original record and creates a new record.\nThis is not optimal with respect to record creation dates and audit trails.\nAlso, PL support for multiple images, image roles, and image captions remains incomplete.</li>\n<li>Under TriagePic\'s main tab, the Patient ID field will turn orange to indicate a number has already been used.\nPreviously yellow was used, but that might cause confusion with a zone color.</li>\n</ul>\n\n&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href=\"javascript:animatedcollapse.toggle(\'v142\')\">Show/Hide Details...</a>\n<br>\n<br>\n<div id=\"v142\" display=\"none;\">\n\n<ul>\n<li>Misnamed field “TriagePhysicans[sic]OrRNs” is now fixed, including as XML element in HospitalStaffing.xml file.\nFix of latter happens when TriagePic starts.</li>\n<li>Transitional support for migration of old Options.xml file and corresponding internal data object has been dropped.</li>\n<li>Date and time in several formats (for Outbox display &amp; sort, EDXL send) are now stored in PatientReports.xml.\nThis simplifies processing and audit trail understanding.\nAlso added are the hospital/organization’s full name, NPI 10-digit number, phone, and email contacts,\nas well as EDXL message distribution ID and sender ID.\nHaving this information at the individual patient report level means fewer assumptions about these values being unchanging.</li>\n<li>Regarding the status message at the bottom of View and Edit Patient Record:\nIn 1.41, a change to processing accidentally emptied the status message when in View mode.  Fixed.</li>\n<li>When resending a message, the XML content of the message is slightly different:\n<ul>\n<li>The EDXL distribution type is “Update” instead of “Report”.</li>\n<li>The relevant preceding message is referenced, using the EDXL-DE standard comma-separated format of <i>distributionID,senderID,datetimeSent</i>.\n(At this time only the immediately preceding message is referenced, though the standard would allow more in separate XML elements.)</li>\n</ul>\n</li><li>When sending or resending a message, the TriagePic convention is currently:\n<ul>\n<li>distributionID is “NPI ” followed by the org’s 10-digit NPI identifier, then a space and datetimeSent</li>\n<li>senderID is\n<ul>\n<li>org’s email address if provided;</li>\n<li>otherwise, org full name and (if provided) phone number preceded by space.\n(Previously, this was phone number alone if provided, or “301.301.TEST” if not.)</li>\n<li>Any commas are replaced by hyphens.</li>\n</ul>\n</li><li>datetimeSent is EDXL-specified UTC format, 24-hour clock, “Z” time zone offset.</li>\n\n</ul>\n</li>\n</ul>\n</div>\n\n\n<br>\n<br>\n<br>\n</div>','2012-06-01 02:31:17','Hidden'),(-101,'TriagePic Release Notes 2011',-51,'<div style=\"\">\n<script type=\"text/javascript\" src=\"res/js/jquery.js\"></script>\n<script type=\"text/javascript\" src=\"res/js/animatedcollapse.js\"></script>\n<script>\nanimatedcollapse.addDiv(\'v141\', \'fade=1,hide=1\');\nanimatedcollapse.addDiv(\'v140\', \'fade=1,hide=1\');\nanimatedcollapse.addDiv(\'v139\', \'fade=1,hide=1\');\nanimatedcollapse.addDiv(\'v138\', \'fade=1,hide=1\');\nanimatedcollapse.addDiv(\'v136\', \'fade=1,hide=1\');\nanimatedcollapse.addDiv(\'v135\', \'fade=1,hide=1\');\nanimatedcollapse.addDiv(\'v132\', \'fade=1,hide=1\');\nanimatedcollapse.addDiv(\'v131\', \'fade=1,hide=1\');\nanimatedcollapse.addDiv(\'v130\', \'fade=1,hide=1\');\nanimatedcollapse.addDiv(\'v129\', \'fade=1,hide=1\');\nanimatedcollapse.addDiv(\'v128\', \'fade=1,hide=1\');\nanimatedcollapse.addDiv(\'v127\', \'fade=1,hide=1\');\nanimatedcollapse.addDiv(\'v124\', \'fade=1,hide=1\');\nanimatedcollapse.addDiv(\'v123\', \'fade=1,hide=1\');\nanimatedcollapse.addDiv(\'v122\', \'fade=1,hide=1\');\nanimatedcollapse.addDiv(\'v119\', \'fade=1,hide=1\');\nanimatedcollapse.addDiv(\'v117\', \'fade=1,hide=1\');\nanimatedcollapse.addDiv(\'v116\', \'fade=1,hide=1\');\nanimatedcollapse.addDiv(\'v115\', \'fade=1,hide=1\');\nanimatedcollapse.addDiv(\'v114\', \'fade=1,hide=1\');\nanimatedcollapse.init();\nanimatedcollapse.ontoggle=function($, divobj, state){}\n</script>\n\n\n<h2>December, 2011</h2>\n\n<h3>Version 1.41<br></h3>\n<hr style=\"height: 1px; background-color: rgb(255, 255, 255); border-width: 1px medium medium; border-style: solid none none; border-color: rgb(229, 234, 239) -moz-use-text-color -moz-use-text-color; margin-bottom: 15px;\">\n<ul>\n<li>This release is of interest only to developers and testers, except for a minor bug fix involving Outbox deletion of a record with multiple photos (which wasn’t archiving all local image files correctly).</li>\n<li>This release provides a preview mockup, with limited functionality, of the edit and resend feature that will be more fully released in subsequent releases.  From a previously-sent record in the Outbox, the “View and Edit Patient Report” has more functionality in its “Unlock to Edit” button.  It now allows editing of most text and checkbox items on the two tab pages, and resend by hitting one of the zone buttons.  However, consumption of this data by PL is not yet available (as of the 1.41 release date).  Note that there are various warning dialogs that will be removed or refined in subsequent releases.  See the details for more specifics and limitations.</li>\n</ul>\n&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href=\"javascript:animatedcollapse.toggle(\'v141\')\">Show/Hide Details...</a>\n<br>\n<br>\n<div id=\"v141\" display=\"none;\">\n<ul>\n<li>When a resend is attempted, the original row is deleted from the Outbox, its files are moved from the “sent” directory to a new “superseded” directory, and a new Outbox row created.</li>\n<li>Also, the resent photos and text data are cleared.  (Whether this behavior is best can be argued, but it is consistent with original-send behavior, and also avoids a file-locking problem with photos in the film-strip controls, when trying to move .jpg files.)  The empty “View and Edit…” form stays up to show status messages as the send progresses, until closed by the user.</li>\n<li>Resend is limited at this time to PL’s web service.  Resend of email (both as failover from web service and as supplemental sends) is not yet available.</li>\n<li>Resend should be able to handle records with any number of photos.</li>\n<li>The web service call was updated to PLUS v 2.2, to use the new reReportPerson stub call.</li>\n<li>Recovery from failure-to-send-successfully (because of stub and no email send) is not sufficient to try again.  It is recommended to return to the Outbox and delete the record.</li>\n<li>Editing changes to the following items are allowed:</li>\n<ul>\n<li>Under “Main Info” tab, first and last name, gender, adult/peds, and (when resending) zone.</li>\n<li>Under “Checklist” tab, the 4 station staff fields.  Also, as shown under this tab, a resend will include current credentials for Windows login name and PL user name, which could differ from original credentials.  (Changes to the staff values affect only the current record, don’t propagate to the normal-send Checklist fields… maybe in the future.)</li>\n</ul>\n<li>Changes to the following items are not yet allowed, and controls to do so are either disabled (grayed) or hidden:</li>\n<ul>\n<li>Patient ID (including +/- buttons, practice mode checkbox).</li>\n<li>Photos (and their sources) and photo metadata (roles, captions).</li>\n<li>Event choice.</li>\n<li>Organization/hospital choice (no tab or control for this provided).</li>\n</ul>\n<li>Also, there is no current method to retrieve and edit records sent from a station (machine) other than current one.  This requires a search presentation, rather than Outbox.</li>\n<li>In the “C:\\Shared TriagePic Settings” folder, these file movements currently occur in sequence, when a zone button is pressed for a resend:</li>\n</ul>\n<ol>\n<li>Files associated with the original record (of types .jpg, .pfif, .lp2) are moved from the “sent” to “superseded” subfolder. In addition, the original record’s data in the “outbox queue” files outbox.xml and PatientRecords.xml are removed, and moved as two appropriately-named xml fragment files to “superceded” (with naming the same as used for deletion).  This activity is part of the process of deleting the original-record Outbox row.</li>\n<li>Files associated with the resent record (of types .jpg, .pfif., lp2) are generated in the “processed” folder.  (These may or may not have the same names as the original files, depending on, e.g., whether the zone was changed).  The resend Outbox row is created.</li>\n<li>After 10 seconds, the “processed” files are moved to the “sent” folder.  (This migration from processed to sent is the largely the same as used for normal sends.)</li>\n</ol>\n</div>\n\n<h3>Version 1.40<br></h3>\n<hr style=\"height: 1px; background-color: rgb(255, 255, 255); border-width: 1px medium medium; border-style: solid none none; border-color: rgb(229, 234, 239) -moz-use-text-color -moz-use-text-color; margin-bottom: 15px;\">\n<ul>\n<li>MANUAL CLEANUP STEP BEFORE UPDATING:  In “C:\\Shared TriagePic Data\\outbox queue\\”, please either rename or delete outbox.xml and PatientReports.xml.</li>\n<li>This release improves data and flow handling and provides minor visible improvements.</li>\n</ul>\n&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href=\"javascript:animatedcollapse.toggle(\'v140\')\">Show/Hide Details...</a>\n<br>\n<br>\n<div id=\"v140\" display=\"none;\">\n<ul>\n<li>A bug in the PatientReport.xml file (introduced in the previous version) that wrote the same patient record multiple times is fixed.</li>\n<li>Selecting a row from the Outbox now relies upon the PatientReport.xml entry (unless it’s unavailable) and populates the internal object from it.</li>\n<li>As a result, the “View and Edit Patient Report” window now has better information to populate fields under the “Checklist” tab, e.g., values for staff and credentials when sent originally.</li>\n<li>If the user sends a record then immediately goes to the Outbox to change it, the associated files may not yet be in the “sent” folder.  Handling of this case is now better.</li>\n<li>If an Outbox row is deleted, the block of associated XML data is removed from Patient\nReport.xml (not just copied to the deleted directory as in v 1.39).</li>\n<li>Deletion of an Outbox record with no associated KML file now works correctly.</li>\n</ul>\n</div>\n\n<h2>November, 2011</h2>\n<h3>Version 1.39<br></h3>\n<hr style=\"height: 1px; background-color: rgb(255, 255, 255); border-width: 1px medium medium; border-style: solid none none; border-color: rgb(229, 234, 239) -moz-use-text-color -moz-use-text-color; margin-bottom: 15px;\">\n<ul>\n<li>This release allows the TriagePic user to delete individual patient records not just locally, but at PL.  It fixes a number of bugs, e.g., during fresh installs, and in tagging secondary photos.  Internally, more progress was made in handling patient reports as unified data objects, savable to an external file.</li>\n</ul>\n&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href=\"javascript:animatedcollapse.toggle(\'v139\')\">Show/Hide Details...</a>\n<br>\n<br>\n<div id=\"v139\" display=\"none;\">\n<ul>\n<li>When a particular patient record is selected from the Outbox into the “View and Edit Patient Report” window, the “Delete Record…” button brings up a new dialog.  One can now optionally ask to delete the patient record from PL, not just the Outbox (and give a reason for the PL log). The response will depend on the user’s PL credentials as logged into TriagePic.  Deletion (in the form of expiration) will happen immediately if the requestor is a PL hospital staff administrator, a PL administrator, or the original reporter and a member of hospital staff.  Otherwise, the request will be queued for someone with those credentials.</li>\n<li>On a deletion, local files moved from the “sent” to “deleted” folder now includes the appropriate .kml file if present.  This file is generated only when emails are sent.</li>\n<li>Internally, a patient report now includes user-supplied names associated with the four staff roles, and lists of associated files (e.g., images, other email attachments).</li>\n<li>A new file, “PatientReports.xml”, appears in the “outbox queue” directory.  This saves more information than was done previously, such as to whom emails were sent to and the outbox sent-codes.  Like outbox.xml, this file is altered as patient reports are added or (through the outbox) deleted.  (Full effective use of this information by “View and Edit Patient Report” will await a forthcoming release.)</li>\n<li>A bug introduced in the previous version that would accidently clear email distribution lists has been fixed.</li>\n<li>If there are photos, exactly one must be given the role of “primary”.  The enforcement of this had a bug when there was a single photo, allowing it to be sent as “secondary”.  Fixed.</li>\n<li>As part of technical maintenance or a fresh install, the user may purge the contents of c:\\Shared TriagePic Settings\\, after which TriagePic will create fresh content on first startup.  There have been several improvements that affect first startup in this case, given in the remaining bullet points.</li>\n<li>PL name and password are solicited as always.  There was a bug that forgot to pass these immediately to the routine that got the event list, leading to “Could not connect to web service to get disaster event list.  Using local cached version instead.”  Fixed.</li>\n<li>One would see the message:<br>\n<br>\n<div style=\"margin-left: 40px;\">\nUnable to automatically determine the default disaster event in the pulldown control.<br>\nPlease make your choice now.<br>\n</div>\n<br>\nHowever, if the user liked the first choice, and didn’t change it, that wasn’t properly noted for subsequent sessions.  Now it is.  Also, the message is now:<br>\n<br>\n<div style=\"margin-left: 40px;\">\n<i>\'event name\'</i> has been selected as the default disaster event,<br>\nbecause it is the first one listed in the pulldown control.  Please review this choice.\n</div>\n</li>\n<li>The new-install starting patient ID value for all hospitals, for both normal and practice modes, is now “1” (instead of six zeroes).  For normal mode, the 1 will be padded with the appropriate number of leading zeroes if that is the hospital policy.  This will get rid of various error messages about illegal value or leading zeroes.  However, there will sometimes now be a warning about that number having already been used on PL for the given event.</li>\n<li>The organization-specific embedded files used to initialized email settings have been changed, so that “email to” is now left blank, instead of being filled in with now-obsolete disaster4@mail.nih.gov.  The latter was giving a warning message as it was subsequently programmatically removed.  (Exception: as before, “NLM” is treated differently, having the lead developer under “email to”.)</li>\n</ul>\n</div>\n\n\n<h3>Version 1.38<br></h3>\n<hr style=\"height: 1px; background-color: rgb(255, 255, 255); border-width: 1px medium medium; border-style: solid none none; border-color: rgb(229, 234, 239) -moz-use-text-color -moz-use-text-color; margin-bottom: 15px;\">\n<ul>\n<li>This release further develops internal restructuring, particularly with respect to email.  There are visible changes in the status messages during a send, and in the Outbox “Sent” column.</li>\n</ul>\n&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href=\"javascript:animatedcollapse.toggle(\'v138\')\">Show/Hide Details...</a>\n<br>\n<br>\n<div id=\"v138\" display=\"none;\">\n<ul>\n<li>In the Outbox “View and Edit Patient”, the too-long status message at bottom was shortened by removing event type (e.g., “ - TEST/DEMO/DRILL”).</li>\n<li>After a send attempt, what the status line momentarily shows, and what the corresponding Outbox row has for “Sent”, and the Outbox “View and Edit Patient” status line, is now finer-grained and more consistent:</li>\n</ul>\n<table rule=\"all\" border=\"1\">\n<thead>\n<tr>\n<td>Any email recipients specified?</td>\n<td>Send to PL by web service succeeded?</td>\n<td>Send to all email recipients succeeded?</td>\n<td>Under “Main Info” tab, status line will begin (at send time, or in Outbox “View and Edit..” for previous sends)</td>\n<td>Outbox “Sent” column will show</td>\n</tr>\n</thead>\n<tbody>\n<tr>\n  <td>N</td>	<td>Y</td>	<td>n/a</td>	<td>Sent to PL:</td>	<td>Y</td>\n</tr>\n<tr>\n  <td>Y</td>	<td>Y</td>	<td>Y</td>	<td>Sent to PL, emailed others:</td>	<td>Y+</td>\n</tr>\n<tr>\n  <td>Y</td>	<td>Y</td>	<td>N</td>	<td>Sent to PL OK;  Some/all emails to others failed:</td> 	<td>Y-</td>\n</tr>\n<tr>\n  <td>N</td>	<td>N</td>	<td>Y*</td>	<td>Emailed PL:</td>	<td>y</td>\n</tr>\n<tr>\n  <td>N</td>	<td>N</td>	<td>N*</td>	<td>Emailed PL but failed: **</td>	<td>N</td>\n</tr>\n<tr>\n  <td>Y</td>	<td>N</td>	<td>Y*</td>	<td>Emailed PL &amp; others:</td>	<td>y+</td>\n</tr>\n<tr>\n  <td>Y</td>	<td>N</td>	<td>N*</td>	<td>Emailed PL &amp; others, but some/all failed:  **</td>	<td>??</td>\n</tr>\n</tbody>\n</table>\n* including PL-send failover to email<br>\n** At send time, severe failure is reported to user by dialog box.  Note that currently handling is unsophisticated with respect to causes of email failure, in particular not differentiating between partial and complete failure.  So “??” is indeterminate as to whether PL received the report.<br><br>\n<ul>\n<li>This finer coding will only be generated for new sends.  In the Outbox “Sent” column, earlier sends will remain simply “Y” or “N”.  The “View and Edit” pane, when displaying status, will use the new phrases shown in the table, which may not be historically accurate.  Prior to this release, N meant what is now N and ??, and all other cases were Y.</li>\n<li>Some of the status line messages are longer than they used to be.  To fit the total message in the screen line, for all messages whose prompts (column 4 above) are longer than 28 characters, the next part of the message is shortened from “New Disaster Patient #” to just “#”.</li>\n<li>KML statistics file was supposed to be sent out with both anonymized and non-anonymized emails, but was only actually sent with latter.  Fixed.</li>\n<li>Unhelpful maximization button of View and Edit Patient form is now disabled.</li>\n</ul>\n</div>\n\n\n<h3>Version 1.37<br></h3>\n<hr style=\"height: 1px; background-color: rgb(255, 255, 255); border-width: 1px medium medium; border-style: solid none none; border-color: rgb(229, 234, 239) -moz-use-text-color -moz-use-text-color; margin-bottom: 15px;\">\n<ul>\n<li>A bug when reopening the Patient ID Tools window was fixed.</li>\n<li>Email addresses of the form “disaster<span style=\"font-style: italic;\">N</span>@mail.nih.gov”, where <span style=\"font-style: italic;\">N</span> = 1-10, as well as “disaster@mail.nih.gov”, are purged from all email distribution lists. (Before, only the latter and “disaster4...” were being purged.)</li>\n<li>Work has started on infrastructure objects to support edits, resend, and queuing.\nIn this release, there are re-implementations of gender, peds/adults, and practice choice in the Main tab and Outbox “View and Edit Patient Report”.<br></li>\n<li>In the Outbox, display of “peds” values was limited to “Y” and “N”, now includes further choices “” (unknown) and “P+A” (peds+adult).\nIn the Outbox “View and Edit Patient Report”, checkbox handling is changed to reflect this (and fix an analogous bug in display of unknown or complex gender).</li>\n<li>The list of Outbox rows is now by default sorted with the latest send at top, instead of at bottom.\nOther aspects are as before (though implemented differently):\n<ul>\n<li>The user can click on a column heading to change the sort column and ordering.  This is remembered across sends but not across sessions.</li>\n<li>The ordering in the “outbox queue” XML file is unchanged, with latest at bottom.</li>\n</ul>\n</li></ul>\n<h2>October, 2011</h2>\n<h3>Version 1.36<br></h3>\n<hr style=\"height: 1px; background-color: rgb(255, 255, 255); border-width: 1px medium medium; border-style: solid none none; border-color: rgb(229, 234, 239) -moz-use-text-color -moz-use-text-color; margin-bottom: 15px;\">\n<ul>\n<li>For the Patient ID field, this release provides a virtual numeric keypad, plus additional verifications.</li>\n</ul>\n&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href=\"javascript:animatedcollapse.toggle(\'v136\')\">Show/Hide Details...</a>\n<br>\n<br>\n<div id=\"v136\" display=\"none;\">\n<ul>\n<li>The former “Barcode Patient ID” button on the main tab is now “Patient ID Tools…”  It brings up a separate persistent window (sized to allow concurrent display with the web cam window) with two tabs:\n<ul>\n<li>“Number Pad” to speed finger-touch entry of patient ID, with “Clear” and backspace buttons.</li>\n<li>“Barcode Scan”, which exposes the previous functionality (with minor changes) for input by an external scanner.</li>\n</ul>\n</li><li>When editing the Patient ID field, the Del/Delete keys (on laptop or virtual keyboards) will be interpreted as “Clear”.</li>\n<li>Previously, whenever the patient ID changes, TriagePic would check its Outbox and local file system to see if it had been sent earlier.  Now it also checks PL, to see if any station or hospital had previously reported this number for the current disaster event.  In doing this check against PL, it is looking for an exact match, including the prefix and leading zeroes.</li>\n<li>Previous, when an earlier-sent patient ID was detected, a dialog was put up.  This is still the case on startup, but during normal patient ID incrementing or adjustment, earlier-sent patient IDs are indicated by making the field background yellow and putting a transient message at window bottom (unless there’s a sending message in progress).  This is less disruptive.</li>\n<li>When the patient ID field is empty (which is possible when the format is variable-number-of-digits), hitting the “+” button now gives “1” instead of an error.</li>\n<li>Unrelated to the foregoing, all dialogs that show an upper-left icon now use the TriagePic icon.</li>\n</ul>\n</div>\n<h3>Version 1.35<br></h3>\n<hr style=\"height: 1px; background-color: rgb(255, 255, 255); border-width: 1px medium medium; border-style: solid none none; border-color: rgb(229, 234, 239) -moz-use-text-color -moz-use-text-color; margin-bottom: 15px;\">\n<ul>\n<li>This release follows the Capital Shield 2012 drill, and begins incorporating findings from that exercise.  In particular, many of the problems with Patient ID input are addressed.  Other changes pre-date the drill.</li>\n</ul>\n&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href=\"javascript:animatedcollapse.toggle(\'v135\')\">Show/Hide Details...</a>\n<br>\n<br>\n<div id=\"v135\" display=\"none;\">\n<ul>\n<li>For Patient ID, the font and field height is larger.  The fixed-number-of-digits mode now is enforced when requested, replacing the troublesome use of variable-number-of-digits mode, which mishandled leading zeroes.</li>\n<li>Fixed-number-of-digits mode now uses a more-understandable “calculator style” editing model, that:\n<ul>\n    <li>Automatically supplies leading zeroes to maintain the defined digit count;</li>\n    <li>Pins the input cursor to the right;</li>\n    <li>Rolls entered numbers in from right, and (with backspace) deletes by shifting the other way.  (Keyboard “Delete” will clear field to all zeroes in this mode.)</li>\n</ul>\n</li>\n<li>Variable-number-of-digits mode now consistently forbids leading zeroes.</li>\n<li>Drill participants did not immediately see how to delete a photo.  As a quick visual cue, the background of the “Selected Photo” group box (with its “Delete Bad” button) is given a color similar to the photo image background graphic.</li>\n<li>Display of additional information of use to developers, such as stack trace, was added to app-level unhandled-exception handlers.</li>\n<li>During startup, a wait cursor is shown when nothing else is on the screen.</li>\n<li>Trying to delete from the Outbox when there was the only one row no longer causes a crash.</li>\n<li>Transient messages during sending are shortened, ellipses moved to message end, and refreshing done so that the added dots-per-sec during slow sends are actually shown.</li>\n<li>The ClickOnce deployment process is requested to create a valid, always-up-to-date TriagePic desktop shortcut.  (But it\'s not being seen in practice; may be permission issues on some desktops.)</li>\n</ul>\n</div>\n\n<h3>Version 1.34<br></h3>\n<hr style=\"height: 1px; background-color: rgb(255, 255, 255); border-width: 1px medium medium; border-style: solid none none; border-color: rgb(229, 234, 239) -moz-use-text-color -moz-use-text-color; margin-bottom: 15px;\">\n<ul>\n<li>In viewing a record retrieved from the Outbox:</li>\n<ul>\n<li>A link to the active PL copy (if it exists) is provided, courtesy of a new PL web service.</li>\n<li>A “Edit Using Browser” button brings up a dialog with some guidance and the same link, encouraging the user to jump there.  This is an interim solution, until editing and/or resending within TriagePic are possible.</li>\n<li>The “Delete Record…” button as before will delete the local Outbox copy, but if there’s also a PL copy, it will also let you know that it is not affected.  Again, an interim state until deletion via TriagePic is possible.</li>\n</ul>\n<li>When sending a message from the main tab, there are additional status messages “Prepping…” and “Sending to PL…”.  These may be seen during slow connections.</li>\n<li>These messages (as well as existing \"Preparing Email Message...\" and \"Authenticating...”) are all messages that end with ellipse.  To indicate aliveness, they get an additional dot added to them every second.</li>\n</ul>\n\n<h3>Version 1.33<br></h3>\n<hr style=\"height: 1px; background-color: rgb(255, 255, 255); border-width: 1px medium medium; border-style: solid none none; border-color: rgb(229, 234, 239) -moz-use-text-color -moz-use-text-color; margin-bottom: 15px;\">\n<ul>\n<li>The patient ID +/- buttons are widened.  Recall that:\n<ul>\n<li>holding down either button eventually starts rolling the ID value;</li>\n<li>if the changed patient ID value matches one that was previously used at this TriagePic station, a warning dialog appears.</li>\n</ul>\nPreviously, if either button was held down by touch (and not mouse), and the match dialog appeared, the rolling would refuse to stop. This is resolved.  (Note that by touch you’ll see a circle denoting right-click appear after a few seconds, but rolling still works as expected.)</li>\n<li>On the webcam screen, the caption was made larger, an enlarged target was provided for the “Hide after capture” checkbox, and the code was modified once again to try to minimize lockups.  If capture fails in a certain way, there is now a new “Sorry, not ready.  Please try again” message.</li>\n<li>To ease bandwidth problems when transmitting pictures, images captured using the 3 megapixel rear-facing camera (“USB 3M DocCam”) of the Motion Computing CL900 tablet are jpeg-compressed to 50%.  Images from other cameras (assumed lower-res) are unchanged (for now).</li>\n</ul>\n\n<h3>Version 1.32<br></h3>\n<hr style=\"height: 1px; background-color: rgb(255, 255, 255); border-width: 1px medium medium; border-style: solid none none; border-color: rgb(229, 234, 239) -moz-use-text-color -moz-use-text-color; margin-bottom: 15px;\">\n<ul>\n<li>Overall, this release continues improvements to touch enabling and reliability, specifically related to web cam operation, photo metadata and policies, and event list management.</li>\n</ul>\n&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href=\"javascript:animatedcollapse.toggle(\'v132\')\">Show/Hide Details...</a>\n<br>\n<br>\n<div id=\"v132\" display=\"none;\">\n<ul>\n<li>Some patient reports were rejected by PL, because their distribution status (which should be “exercise”, “test”, or “actual”) was null.  This is no longer possible.  The logic is now:</li>\n<ul>\n<li>“exercise” is the default.</li>\n<li>“test” is assigned only for events that TriagePic shows as TEST/DEMO/DRILL and with “Test” or “test” in their PL names.</li>\n<li>“actual” is assigned for REAL – NOT A DRILL events.</li>\n</ul>\n<li>The root cause of the foregoing was bugs when the first event in the event pulldown list had been chosen.  On TriagePic startup, this would show itself as a warning message complaining that no default event was provided.  Fixed.</li>\n<li>The enlarged overlay button to Select Known Events is repositioned to better hide its underlying pulldown thumb on the Motion Computing tablets, where the custom Win 7 theme widens such thumbs.</li>\n<li>From the Outbox tab, the dialog to see details about a sent message is renamed “View and Edit Patient Report”, from “Edit Patient Report Information”</li>\n<li>In that dialog, a report sent in practice mode is now reported correctly, and the practice mode checkbox is enlarged for touch.</li>\n<li>Since edit and resend will not be ready for Capital Shield, the preliminary code to enable editing is suppressed for now.  Instead, an attempt will be made shortly to provide a link to PL’s “edit a person” for that specific report.</li>\n<li>When the web cam window is invoked from the main part of the application, image capture is now done by tapping on the video, rather than a separate button.  There’s a text caption about this.  This is better for finger touch.</li>\n<li>If there are multiple cameras available on the device, the most-recently chosen one is remembered, in OtherSettings.xml, and used by default next time.</li>\n<li>Yet another change was made to try to make webcam image capture more reliable.</li>\n<li>Radios for the “Why No Photo” dialog have enlarged targets, shown in light color (as was done with other radios earlier).</li>\n<li>Under the “Policies” tab, three hospital-specific photo-related policies are now gathered from the PL web site (but not yet cached into XML), and affect behavior:</li>\n<ul>\n<li>“Photo required, or ask why not” will suppress warning if no photo, and invocation of Why No Photo dialog.</li>\n<li>“Honor patient request for no photo” determines whether that choice is offered in the “Why No Photo” dialog.</li>\n<li>“Require photographer names” will prevent sending a report with a photo until that field (under the Checklist tab) is filled in.</li>\n</ul>\n</ul></div>\n\n<h3>Version 1.31<br></h3>\n<hr style=\"height: 1px; background-color: rgb(255, 255, 255); border-width: 1px medium medium; border-style: solid none none; border-color: rgb(229, 234, 239) -moz-use-text-color -moz-use-text-color; margin-bottom: 15px;\">\n<ul>\n<li>This release improves some aspects of finger-touch, adds additional flexibility when entering patient ID, and clarifies the report-sent message at screen bottom.  It also fixes a recent startup bug.</li>\n</ul>\n&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href=\"javascript:animatedcollapse.toggle(\'v131\')\">Show/Hide Details...</a>\n<br>\n<br>\n<div id=\"v131\" display=\"none;\">\n<ul>\n<li>A “Barcode Patient ID” button invokes a separate dialog to enter a patient ID with a keyboard-emulating barcode scanner (typically attached by USB).  If differs from the normal patient ID field in that the barcode is assumed to include the fixed patient ID prefix (if one is defined).  The entry must begin with that prefix, then the rest, the numerically-variable portion, is placed into the normal patient ID field.</li>\n<li>If either the “+” or “-” button next to Patient ID is held down 1 second, the numbers increment/decrement continuously (5 times a second).</li>\n<li>The “Practice Mode” checkbox is enlarged.</li>\n<li>As before, if no gender choice is made on the main page, a reminder dialog appears.  Likewise with adult versus peds choice.  These reminders now have radio buttons with enlarged finger-target areas, shown as a light-colored rectangle behind each radio button.  Similarly with the extra-choice dialog that appears when a send is done in Practice Mode.</li>\n<li>When successfully sending a report, expect to see at the bottom of main page (after a few “eyeblink” messages) a short message, left up about 10 seconds, that begins with...</li>\n<ul>\n<li>“Sent to PL: ” -  meaning sent by web services to PL, and no email recipients specified.</li>\n<li>“Sent to PL, Others: ” - meaning set by web services to PL, by email to others.</li>\n<li>“Emailed PL, Others: ” - meaning sent by email to PL and others.</li>\n</ul>\n<li>A recent code change introduced a bug, causing a complaint when trying to get the list of hospitals from the web service on TriagePic startup.  Fixed.</li>\n</ul></div>\n<h3>Version 1.30<br></h3>\n<hr style=\"height: 1px; background-color: rgb(255, 255, 255); border-width: 1px medium medium; border-style: solid none none; border-color: rgb(229, 234, 239) -moz-use-text-color -moz-use-text-color; margin-bottom: 15px;\">\n<ul>\n<li>The release was mainly about improvements to the appearance, behavior, and contents of the “Select Known Events” control under the “Checklist” tab.</li>\n</ul>\n&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href=\"javascript:animatedcollapse.toggle(\'v130\')\">Show/Hide Details...</a>\n<br>\n<br>\n<div id=\"v130\" display=\"none;\">\n<ul>\n<li>The control was made finger-touchable, by enlarging the font and overlaying a bigger button to cause dropdown.</li>\n<li>The event dropdown list now shows only open-to-reporting events (that are either public or viewable by the group given by the PL credentials).  This was made possible by an upgrade to PL web services (now 1.9.9), which also lays the groundwork for further improvements.</li>\n<li>When TriagePic starts, the event selected by default will be the same one selected last time on this machine if possible (or the user is warned to select a new choice).  This is NOT the same as the event listed as the default for the web site.  The remembered event is saved in OtherSettings.xml.</li>\n<li>If the user hits the “Re-enter PL Credentials” button and changes their user PL name, the event list is refetched.</li>\n<li>If the event list changes (on fresh TriagePic startup or by a refetch), and the previous default event is no longer available, the user is warned to select an event manually.</li>\n<li>If the event list is empty, the user is warned to take steps to change that situation.</li>\n</ul>\n</div>\n\n<h3>Version 1.29<br></h3>\n<hr style=\"height: 1px; background-color: rgb(255, 255, 255); border-width: 1px medium medium; border-style: solid none none; border-color: rgb(229, 234, 239) -moz-use-text-color -moz-use-text-color; margin-bottom: 15px;\">\n<ul>\n<li>Overall, this release improves flexibility and clarity regarding access to PL web services and email for reporting.</li>\n</ul>\n&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href=\"javascript:animatedcollapse.toggle(\'v129\')\">Show/Hide Details...</a>\n<br>\n<br>\n<div id=\"v129\" display=\"none;\">\n<ul>\n<li>Under the “Checklist” tab, there is now a “Re-enter PL Credentials” button.</li>\n<li>If you try to report a person and fail, there is also more fulsome reporting to the user of the error encountered.  If the failure is due to credentials, you are re-asked for credentials.</li>\n<li>You can try to enter credentials more than once before it quits.</li>\n<li>If you explicitly cancel the entering of PL credentials, you are first warned that will end TriagePic, and now it actually does.</li>\n<li>Further improvements are made to handling PL API versioning.  If the cached version number is outdated, it is updated to the latest “production” version, with notification.</li>\n<li>Events seen in the event list are now limited to those viewable with the current PL user credentials.</li>\n<li>Depending on the type of web service error, fall-over to email may or may not make sense.  Pending better programmatic analysis of error cases, automatic fall-over is replaced by a query to the user about it.</li>\n<li>Some further work was done to disable the second email service.  In particular, a leftover, troublemaking verification check for different values of EmailProfileSelected field in EmailSettings file was disabled.</li>\n</ul>\n</div>\n\n<h2>September, 2011</h2>\n<h3>Version 1.28<br></h3>\n<hr style=\"height: 1px; background-color: rgb(255, 255, 255); border-width: 1px medium medium; border-style: solid none none; border-color: rgb(229, 234, 239) -moz-use-text-color -moz-use-text-color; margin-bottom: 15px;\">\n<ul>\n<li>In the email distribution fields, checks are also made to remove “disaster@mail.nih.gov” (not just disaster4), and also to prevent email sends to PL of anonymous-format email; the user is informed in either case.  Also, if a web service failure causes failover to email, the disaster4 recipient that is added no longer persists after the send.</li>\n<li>To aid editing by touch, there are now +/- buttons on patient ID.</li>\n<li>Local deletion of multiple Outbox rows was broken but should now work.</li>\n<li>New code is added to better support viewing (and eventually resend) of Outbox items, as described in the Details.</li>\n</ul>\n&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href=\"javascript:animatedcollapse.toggle(\'v128\')\">Show/Hide Details...</a>\n<br>\n<br>\n<div id=\"v128\" display=\"none;\">\n<ul>\n<li>Some information about each Outbox items is stored in its PFIF file.  The code was changed to always generate a PFIF file (even if only sending by web services), with richer semi-structured content (and upgrade from PFIF 1.1 to 1.2).  Also, a PFIF file is generated even if no required patient name is available, by filling in “N/A”.</li>\n<li>TriagePic can now read PFIF files, not just write them, to retrieve staffing and other supplemental information.</li>\n<li>Outbox details on a particular sent report now shows more meaningful information under the “Events/Staffing” tab.</li>\n<li>The actual patient ID prefix is now shown in the Outbox, instead of fixed text.</li>\n</ul>\n</div>\n\n<h3>Version 1.27<br></h3>\n<hr style=\"height: 1px; background-color: rgb(255, 255, 255); border-width: 1px medium medium; border-style: solid none none; border-color: rgb(229, 234, 239) -moz-use-text-color -moz-use-text-color; margin-bottom: 15px;\">\n<ul>\n<li>There is more reliance on web services as the preferred way to convey most information, instead of email.  The email service offering has been simplified.  Previously, one specified the main email service, and a second email service for automatic-failover.  The latter has been suppressed.</li>\n<li>The requirement that one of the “To” email distribution fields must be filled in has been dropped.</li>\n</ul>\n&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href=\"javascript:animatedcollapse.toggle(\'v127\')\">Show/Hide Details...</a>\n<br>\n<br>\n<div id=\"v127\" display=\"none;\">\n<ul>\n<li>Related to dropping the “To” email requirement:</li>\n<ul>\n<li>Comparable restrictions affecting the EmailSetting.xml file were removed.</li>\n<li>When a report with photos is successfully sent only by web service, photo files are now moved correctly to the “sent” folder.  This was needed to make Outbox viewing work.</li>\n</ul>\n<li>If viewing multiple photos in the Outbox, the roles (primary or secondary) and captions are now correct.</li>\n<li>The PL username &amp; password are being correctly secure-stored on the local machine.</li>\n<li>It is desirable for TriagePic to request a specific version of the PL web services, but that wasn’t happening if some recent version number wasn’t already contained in OtherSettings file.  Fixed.</li>\n<li>Trying to delete multiple rows of the Outbox raised an uncaught exception, now handled.</li>\n</ul>\n</div>\n\n<h3>Version 1.26<br></h3>\n<hr style=\"height: 1px; background-color: rgb(255, 255, 255); border-width: 1px medium medium; border-style: solid none none; border-color: rgb(229, 234, 239) -moz-use-text-color -moz-use-text-color; margin-bottom: 15px;\">\n<ul>\n<li>\nThe old design for seeing details of records in the Outbox is being replaced by a new design, with layout more like the Main Info tab. The replacement design, which continues to rely on the traditional local data file archiving, largely works for viewing reports sent by email. Further refinements are planned.  In particular, additional effort is expected to handle reports sent by web services, where the archived data is in different form, or local archiving is replaced or supplemented by fetching from PL. The design also does some anticipatory work towards supporting editing and resending.</li>\n</ul>\n<h3>Version 1.25<br></h3>\n<hr style=\"height: 1px; background-color: rgb(255, 255, 255); border-width: 1px medium medium; border-style: solid none none; border-color: rgb(229, 234, 239) -moz-use-text-color -moz-use-text-color; margin-bottom: 15px;\">\n<ul>\n<li>Fixed: fresh installs failed because the selection box for hospital choice was empty.</li>\n</ul>\n<br>\n<h3>Version 1.24<br></h3>\n<hr style=\"height: 1px; background-color: rgb(255, 255, 255); border-width: 1px medium medium; border-style: solid none none; border-color: rgb(229, 234, 239) -moz-use-text-color -moz-use-text-color; margin-bottom: 15px;\">\n<ul>\n<li>The overall purpose of this release is to fix a number of the remaining problems with reporting a missing person by the new web service, while continuing to support traditional email sending.</li>\n</ul>\n&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href=\"javascript:animatedcollapse.toggle(\'v124\')\">Show/Hide Details...</a>\n<br>\n<br>\n<div id=\"v124\" display=\"none;\">\n<ul>\n<li>When a report is successfully sent by web service, the confirming message at the bottom of the main page now remains long enough to actually see.</li>\n<li>If the web service fails, failover to email now works.  A dialog alerts the user when this occurs.</li>\n<li>Email sending can supplement reporting by web service, if email addresses are specified on the “Distribution” tab.  This was broken in 1.23, is fixed in 1.24.  It is no longer recommended to specify “disaster4” as a “To” recipient, and it will be ignored if it does appear (though that address will be used in case of failover, as mentioned above).</li>\n<li>When sending by email, the draft “TEP” attachment, introduced in release 1.23, was seen to cause problems for PL parsing.  Further TEP generation is being suppressed for now.</li>\n<li>The generated KML attachment with statistics is now archived like other attachments.</li>\n<li>When a report with a photo was successfully sent, deletion of the photo from the GUI should occur, but did not.  Fixed.</li>\n<li>If there were multiple photos associated with a patient, only the last one was included in the XML sent to the web service.  Now all are sent, and, to ease PL processing, the photo tagged as primary appears first in the XML.  (There are additional issues to overcome before PL can handle multiple photos appropriately.)</li>\n<li>The prefix for the patient ID aka mass casualty ID, such as “911-”, was not being added to filenames of email attachments, nor to patient ID fields of the XML sent by email or web service.  Consequently, the prefix did not appear in PL.  This evidently got broken during an earlier code restructuring. Fixed.</li>\n</ul>\n</div>\n\n<h3>Version 1.23<br></h3>\n<hr style=\"height: 1px; background-color: rgb(255, 255, 255); border-width: 1px medium medium; border-style: solid none none; border-color: rgb(229, 234, 239) -moz-use-text-color -moz-use-text-color; margin-bottom: 15px;\">\n<ul>\n<li>National Naval Medical Center is newly named Walter Reed National Military Medical Center.&nbsp; TriagePic and PL reflect that change, e.g., \"NNMC\" is replaced by \"WRMMC\".</li><li>Work has begun to better support finger-based touch.&nbsp; So far, the 4 checkboxes for gender and peds/adult choices on the main tab are enlarged, as is the pulldown choice of webcams.</li><li>Another change was made to prevent the occasional lockup during webcam photo capture.<br></li><li>Sending a patient report, including photos, to the PL web site is now done by authenticated web services (unless the service is down, in which case email is used).</li>\n<li>The main change the user will see is a new dialog that asks for PL user name and password.  This information will be requested once, then securely remembered.  It will only be asked again if a web service fails authentication, say, due to changing a password at the PL site.</li>\n<li>Email addresses for additional recipients may still be specified under the \"Distribution\" tab (and there must be at least one currently), but please removed \"disaster4@nlm.nih.gov\" from the list.</li>\n</ul>\n&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href=\"javascript:animatedcollapse.toggle(\'v123\')\">Show/Hide Details...</a>\n<br>\n<br>\n<div id=\"v123\" display=\"none;\">\n<ul>\n<li>Regarding recipient email addresses, the restriction that at least one must be supplied (and it should be a full record to avoid a bug) will be lifted in the near future.  At this time, these emails continue to be sent directly from TriagePic, not by PL; this may change eventually.</li><li>Recipients of non-anonymized email will see an additional email attachment with file extension \"tep\".&nbsp; This is a first draft of a new XML format, eventually to be used for web services.<br></li>\n<li>To support write-to-PL web services, Windows code to securely store encrypted PL credentials was added.</li>\n<li>The hospital choice (e.g., Suburban, WRMMC, or NLM) is solicited just once and remembered.  This fixes the problem in 1.22 that re-solicited during every TriagePic start-up.</li>\n</ul>\n</div>\n\n\n<h2>June, 2011</h2>\n<h3>Version 1.22</h3>\n<hr style=\"height: 1px; background-color: rgb(255, 255, 255); border-width: 1px medium medium; border-style: solid none none; border-color: rgb(229, 234, 239) -moz-use-text-color -moz-use-text-color; margin-bottom: 15px;\">\n<ul>\n<li>This version gets more information from PL web services, associated with the central NLM People Locator web site (pl.nlm.nih.gov).  So far, these are read-only web services.</li>\n<li>TriagePic has been made less tall, and the photo viewer improved.</li>\n<li>These changes triggered improvements to the layout and functionality.</li>\n</ul>\n&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href=\"javascript:animatedcollapse.toggle(\'v122\')\">Show/Hide Details...</a>\n<br>\n<br>\n<div id=\"v122\" display=\"none;\">\n<ul>\n<li>Specifically regarding the web services, the source of information under the â€œHospitalâ€ and â€œPolicyâ€ tabs has changed.  Previously, this was stored only on the local machine.  It now comes instead from PL web services.  This information comprises the organizationâ€™s contact information and the format of the mass casualty patient ID (i.e., prefix, count of numeric digits if fixed).</li>\n<li>The foregoing â€œHospitalâ€ and â€œPolicyâ€ data is still cached locally for use when web services are unavailable.  The nature of the caching has changed.  Information previously in â€œHospitalSettings.xmlâ€ has been split into â€œHospitalContactInfo.xmlâ€, â€œHospitalPolicy.xmlâ€, and â€œHospitalStaffing.xmlâ€.  The last of these is not drawn from a web service.<br></li>\n<li>After upgrade (but not fresh install), as the app starts for the first time, a dialog box will say:<br>\n<br>\nUpdating data files in directory \'C:\\Shared TriagePic Settings\':</li>\n<ul>\n<li>HospitalSettings.xml is renamed to HospitalSettings[obsolete].xml.</li>\n<li>Info about staffing of TriagePic station will now be stored in HospitalStaffing.xml</li>\n<li>Info under \'Hospital\' tab will be now gathered from a web service, and cached in HospitalContactInfo.xml</li>\n<li>Info under \'Policies\' tab, like Patient ID format, is also from a web service, &amp; cached in HospitalPolicy.xml</li>\n</ul>\n<li>After upgrade (or on fresh install), as the app starts up for the first time, a small dialog box will ask for the hospital.  There are currently only 3 choices [Suburban, NNMC, NLM (testing)], but this number can vary in the future.  Unlike before, these choices come from a PL web service, as discussed.  Once selected, the value will be retained.</li>\n<li>The height of TriagePicâ€™s window is now reduced slightly to work better on some Windows platforms, making it more square.  The layout of the contents of tab pages have been changed somewhat to accommodate this (as mentioned further below).  Also, the unhelpful Maximize button in the title bar is disabled.</li>\n<li>Under the â€œMain Tabâ€, individual fields for first name, middle names, nicknames, last names and suffix are simplified to two fields, with examples.  The gender and peds/adult settings have been moved.  These changes facilitate reducing TriagePicâ€™s window height.</li>\n<li>The filmstrip control that shows photos has been modified.  The main photo was previously being undesirably clipped on its bottom and right side.  It is now shown in its entirety (though at the expense of giving up on centering; left/top is now abutted).  The thumbnail strip at bottom is now less high, so thereâ€™s more room for the main photo.</li>\n<li>Under the â€œChecklist (Event, Staff)â€ tab, contents have been simplified, and the â€œSelect Known Eventâ€ combo box made wider.   (The map and inactive controls that were mockups for FEMA-network distribution have been moved to the â€œ[TO DO] Define New Eventâ€ popup.)</li>\n<li>Checklist staff names entered are remembered locally as before.  These entries are not hospital-specific (also as before, but now more obvious).  Upon upgrade from version 1.20, the staff fields are blank (i.e., not carried over from HospitalSettings.xml).</li>\n<li>Under the â€œDistributionâ€ tab, leftover â€œ[TO DO]â€¦â€ comments are now gone.  Instead, the â€œWeb Servicesâ€ setting has been moved here from the â€œHospitalâ€ tab, and its URL changed as described next.</li>\n<li>Previously, the web services were now found through a URL stored in OtherSettings.xml.  It is desirable to add to the URL a parameter for a specific version of the web services.  This would guarantee selection by a particular version of TriagePic of the correct version of the web services, out of many simultaneously offered.  Currently (and provisionally), the OtherSettings value is ignored, and the URL with version parameter comes from app.config.</li>\n<li>Before, the TriagePic user could only easily specify the deployment hospital at install time (or by XML file manipulation).  One can now specify the hospital any time, using a new combo control under the â€œHospitalâ€ tab.  The choices presented come from a PL web service, and currently are Suburban, NNMC, or NLM (testing).  The choice once made is remembered (in the cache file OtherSettings.xml).  Changing the choice will be reflected throughout, including in the title barâ€™s â€œâ€¦Deployed atâ€¦â€ designation.</li>\n<li>After changing the hospital, you get a reminder:<br>\n<br>\nYou\'ve selected a new hospital.  Be sure to:\n<ul>\n<li>Adjust the next Patient ID number on the \'Main\' tab.</li>\n<li> Make it compatible with the format of the new hospital,\n    as expressed under the \'Policies\' tab.</li>\n<li>Change the staffing under the \'Checklist\' tab.</li>\n</ul>\n</li><li>An attempt to change the hospital may not succeed when certain web services are unavailable.  Instead, a warning message appears and the hospital remains unchanged.</li>\n<li>The â€œNLM (testing)â€ choice now has more information.</li>\n<li>While a particular hospital can be chosen, the detailed information about it is now treated as read-only within TriagePic.  (It may be edited by someone logged into PL with â€œhospital administratorâ€ credentials, from PLâ€™s â€œHospital Administrationâ€ page.)</li>\n</ul>\n</div>\n\n<h2>March, 2011</h2>\n<h3>Version 1.21</h3>\n<hr style=\"height: 1px; background-color: rgb(255, 255, 255); border-width: 1px medium medium; border-style: solid none none; border-color: rgb(229, 234, 239) -moz-use-text-color -moz-use-text-color; margin-bottom: 15px;\">\n<ul>\n<li>This fixes incorrect UTC timestamps within .lpf and .pfif email attachments.</li>\n<li>TriagePic testers inside NLM only: If the update to 1.21 doesn\'t occur, due to a change in distribution method, you\'ll need to uninstall and reinstall TriagePic.</li>\n</ul>\n\n<h3>Version 1.20</h3>\n<hr style=\"height: 1px; background-color: rgb(255, 255, 255); border-width: 1px medium medium; border-style: solid none none; border-color: rgb(229, 234, 239) -moz-use-text-color -moz-use-text-color; margin-bottom: 15px;\">\n<ul>\n<li>The NLM certificate used to sign TriagePic updates has itself been updated for 2011.  You may be asked to take extra steps to accept the new certificate when upgrading to 1.20.</li>\n<li>The name of the selected disaster event is sent to PL in both a short and long form within the .lpf file.  This allows better compatibility, and was first shown at the DIMRC Disaster Information Outreach Symposium at NLM.</li>\n</ul>\n\n<h2>February, 2011</h2>\n<h3>Version 1.19</h3>\n<hr style=\"height: 1px; background-color: rgb(255, 255, 255); border-width: 1px medium medium; border-style: solid none none; border-color: rgb(229, 234, 239) -moz-use-text-color -moz-use-text-color; margin-bottom: 15px;\">\n<ul>\n<li>As each patient\'s arrival is reported, another file attachment is now sent out, summarizing all the previous patients who have passed through the triage station for the current disaster.  This is a .kml file, that would appear on Google Earth as a pushpin at the hospital location.  When clicked, its balloon shows brief identifying data and a compact, color-coded table with the statistics seen in TriagePic\'s \"Outbox\" tab.</li>\n</ul>\n&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href=\"javascript:animatedcollapse.toggle(\'v119\')\">Show/Hide Details...</a>\n<br>\n<br>\n<div id=\"v119\" display=\"none;\">\n<ul>\n<li>The same .kml file is included with both anonymized and normal distributions</li>\n<li>The balloon contents are in html form, and could be easily extracted for other purposes.</li>\n<li>Be aware that some email systems will empty .kml attachments, causing errors when attempting to view them on Google Earth.</li>\n<li>This feature was motivated by, and first shown at, the Camp Roberts East/RELIEF exercise at VA Tech/Naval Postgraduate School, Balston, VA.</li>\n</ul>\n</div>\n\n<h3>Version 1.18</h3>\n<hr style=\"height: 1px; background-color: rgb(255, 255, 255); border-width: 1px medium medium; border-style: solid none none; border-color: rgb(229, 234, 239) -moz-use-text-color -moz-use-text-color; margin-bottom: 15px;\">\n<ul>\n<li>Quick, temporary fix to event name reported in the .lpf file attachment, to address a problem with parsing when received at the PL site.</li>\n</ul>\n\n<h3>Version 1.17</h3>\n<hr style=\"height: 1px; background-color: rgb(255, 255, 255); border-width: 1px medium medium; border-style: solid none none; border-color: rgb(229, 234, 239) -moz-use-text-color -moz-use-text-color; margin-bottom: 15px;\">\n<ul>\n<li>Latitude and longitude fields are added to the Hospital tab.</li>\n<li>There is now flexibility in determining where TriagePic should find its web services.</li>\n<li>The event list, that populates the combo box on the Checklist tab, is now read from the PL web service, instead of relying exclusively on the cached eventList.xml file.</li>\n</ul>\n&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href=\"javascript:animatedcollapse.toggle(\'v117\')\">Show/Hide Details...</a>\n<br>\n<br>\n<div id=\"v117\" display=\"none;\">\n<ul>\n<li>The latitude and longitude are persisted in the HospitalSettings.xml file.  New and existing installs will get default coordinates as appropriate for NLM (and Generic), NNMC, and Suburban Hospital.  Lat/Long will eventually be tied into web services, and in the immediate term allows geolocation specification for the Camp Roberts East/RELIEF exercise.</li>\n<li>The web services end point address is persisted in OtherSettings.xml and shown (perhaps temporarily) under the Hospital Settings tab, under a heading \"Web Service Settings\".  New and existing installs will get the default: \"https://pl.nlm.nih.gov/?wsdl\".</li>\n</ul>\n</div>\n\n<h3>Version 1.16</h3>\n<hr style=\"height: 1px; background-color: rgb(255, 255, 255); border-width: 1px medium medium; border-style: solid none none; border-color: rgb(229, 234, 239) -moz-use-text-color -moz-use-text-color; margin-bottom: 15px;\">\n<ul>\n<li>Fixes two problems when saving email settings and outbox entries.</li>\n<li>Allows local outbox history deletions.</li>\n<li>Aligns disaster event categories better with PL.</li>\n</ul>\n&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href=\"javascript:animatedcollapse.toggle(\'v116\')\">Show/Hide Details...</a>\n<br>\n<br>\n<div id=\"v116\" display=\"none;\">\n<ul>\n<li>A recently introduced bug, where email setting changes were not saved, is fixed.</li>\n<li>In the Outbox, the â€œPicâ€ column was earlier converted to counts (instead of Y/N), but this conversion wasnâ€™t getting saved in the outbox.xml file except for new entries.  Now it is.</li>\n<li>Version 1.16 allows the user to go into the Outbox and delete one or more individual items from the outbox history.  These are local deletes for now - they are not reported to the central LPF database.</li>\n<li>There are two ways to delete a particular selected Outbox row:  hit the â€œDeleteâ€ key (or DEL or Backspace) or open the Outbox Details window and hit the new â€œDeleteâ€ button.  The first method also allows multiple rows to be selected and deleted.  To facilitate this functionality, a single click on an Outbox row now selects it, instead of opening Outbox Details (which now takes a double click).</li>\n<li>A delete is persisted by changing the contents of outbox.xml.  The contents removed from outbox.xml is moved to a separate file under the â€œdeletedâ€ directory, e.g., â€œ911-1234 Green [from outbox].xmlâ€.  Also moved are the associated *.lpf, *.pfif, and *.jpg files.</li>\n<li>Deletion of one or more rows normally concludes with a dialog box saying:</li>\n\"Deleted patient data moved to the C:/Shared TriagePic Data/deleted/ folder:\"<br>\nfollowed by, for each deleted row, patient number, zone, and number of deleted files, e.g.:<br>\n\"2045 Green (4 files)\"<br>\nAll the filenames will begin, in this case, with â€œ2045 Greenâ€¦â€ . There are typically 4 files, consisting of an outbox fragment .xml, an .lpf file, a .pfif file, and one .jpg file.  If problems arise during the deletion process, additional error dialog boxes may precede the normal one.<br>\n<li>Specifically, when deleting a row, possible error messages are:</li>\n<ol>\n<li>\"Could not open file C:/Shared TriagePic Data/sent/<some file=\"\">\"</some></li>\n<li>For .lpf and .pfif attachments:</li>\n\"More than 1 file was found with the pattern C:/Shared TriagePic Data/sent/*.{some extension}<br>\nAll will be moved to the folder C:/Shared TriagePic Data/deleted/.\"<br>\n<li>\"It was planned to delete the file C:/Shared TriagePic Data/sent/{somefile.ext}<br>\nby moving it to the archive C:/Shared TriagePic Data/deleted/,<br>\nbut a file of that name already exists there, which was renamed to <somefile (1).ext=\"\">\"</somefile></li>\n<li>\"There were no files to delete.\"  This might occur after an abnormal termination due to crash or debugging.</li>\n</ol>\n<li>If deleting the last row from history, a dummy blank row is automatically inserted into outbox.xml.  This prevents problems later.  The first real data replaces that row.</li>\n<li>TriagePic previously had a 3-way categorization of events:  â€œTEST or DEMOâ€, â€œDRILLâ€, or â€œREAL â€“ NOT A DRILLâ€.  In the first category, a default instance was â€œUnnamed TEST or DEMOâ€.  PL has adopted a 2-way categorization:  â€œTESTâ€ or â€œREALâ€, which it is exposing through the web service (and cached by TriagePic); the default instance is â€œTest Eventâ€.  To align closer to PL, the TriagePic UI is now showing the two categories â€œTEST/DEMO/DRILLâ€ and â€œREAL â€“ NOT A DRILLâ€.  Additionl cleanups to support this re-categorization are discussed next.</li>\n<li>If it is necessary to distinguish between a test and a drill (for instance, in filling out EDXL headers for the .lpf files), the mapping will be that â€œTest Eventâ€ is considered a test and all other events of PL type TEST are considered a drill.</li>\n<li>Historical outbox entries where Event = â€œUnnamed TEST or DEMOâ€ are changed to Event = â€œTest Eventâ€.  This (and correction of related bugs) will allow the stat boxes for Current Event to show useful results when the current event for TriagePic is Test Event.</li>\n<li>If the cached event list includes event â€œnew eventâ€, which PL once exposed, it is deleted.</li>\n</ul>\n</div>\n\n<h2>January, 2011</h2>\n<h3>Version 1.15</h3>\n<hr style=\"height: 1px; background-color: rgb(255, 255, 255); border-width: 1px medium medium; border-style: solid none none; border-color: rgb(229, 234, 239) -moz-use-text-color -moz-use-text-color; margin-bottom: 15px;\">\n<ul>\n<li>Fixes a few email settings bugs.</li>\n</ul>\n\n&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href=\"javascript:animatedcollapse.toggle(\'v115\')\">Show/Hide Details...</a>\n<br>\n<br>\n<div id=\"v115\" display=\"none;\">\n<ul>\n<li>Version 1.15 fixes several problems with the newly introduced EmailSettings.xml file.  (And for those who upgraded to v 1.14 and had problems, a workaround replacement EmailSettings.xml file was distributed.)\n</li><li>If migrating from TriagePic v 1.13 or earlier, email settings previously in SharedTriagePic.xml now migrate correctly to EmailSettings.xml.  In 1.14, only the default (new-install) contents for EmailSettings.xml appear.\n</li><li>Related to that, if â€œNLM (Testing)â€ was in the profile in use (rather than Generic, Suburban Hospital, or NNMC), the default contents of the EmailSettings.xml file was incorrect (e.g., first line begins as \"<options â€¦â€=\"\" instead=\"\" of=\"\" â€œ=\"\"><emailsettings â€¦â€)=\"\"></emailsettings></options></li>\n<li>The preceding problem caused the app to silently quit during startup, right after the â€œCould not connect to web serviceâ€¦â€ dialog.  Changed to be noisy, identifying the bad xml file.  Similar noisy reads are also now done for HospitalSettings.xml, OtherSettings.xml, and UsersAndVersions.xml.</li>\n</ul>\n</div>\n\n<h3>Version 1.14</h3>\n<hr style=\"height: 1px; background-color: rgb(255, 255, 255); border-width: 1px medium medium; border-style: solid none none; border-color: rgb(229, 234, 239) -moz-use-text-color -moz-use-text-color; margin-bottom: 15px;\">\n<ul>\n<li>Improves the Outbox - the listing of sent items - particularly the information shown when clicking on a particular entry and seeing the dialog box now called â€œOutbox Detailsâ€.</li>\n<li>Has some preliminary work on deletion capabilities, most of which is not exposed in this release.</li>\n</ul>\n&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href=\"javascript:animatedcollapse.toggle(\'v114\')\">Show/Hide Details...</a>\n<br>\n<br>\n<div id=\"v114\" display=\"none;\">\n<ul>\n<li>The Outbox â€œSentâ€ column before always showed â€œYâ€, whether the email send was successful or not.  It now should show â€œNâ€ if email send was not 100% successful to all recipients.  (Error cases have not been extensively tested.)  In code, the function myEmail.MySendMail now returns a Boolean value to indicate success, the two values of which (for anonymized and regular sends) determine the Outbox â€œSentâ€ value.  To achieve that, the function body, particularly inside the catch handlers, has been moderately modified.</li>\n<li>The Outbox listing of sent items now includes nicknames/aliases in the â€œFirstâ€ name column, and avoids trailing spaces in the â€œFirstâ€ and â€œLastâ€ columns.  The format is now:</li>\n&nbsp;&nbsp;&nbsp;First: William \"Bill\" A.<br>\n&nbsp;&nbsp;&nbsp;Last: Stockton Jr.<br>\n<li>The title bar of Outbox Details now includes the form name and an alias if present, and handles a missing patient name specially. Two examples:</li>\n&nbsp;&nbsp;&nbsp;Outbox Details - [No Patient Name Given]<br>\n&nbsp;&nbsp;&nbsp;Outbox Details - William \"Bill\" A. Stockton Jr.<br>\n<li>Outbox Details is larger.  It now reveals both left and right filmstrip controls.  A new read-only area in the lower-left shows details of the selected record.  This is largely the same information thatâ€™s in the selected Outbox data row.  Since the Outbox data row items are sometimes truncated or scrolled from visibility, this gives a more complete picture at a glance.  Minor differences from Outbox, seen in Outbox Details, are:\n</li><li>The â€œwhen sentâ€ timestamp is in a different format which includes the year, and shows the month as a number.  (There is a separate alternative-format timestamp field that could be shown, but we chose not to.)</li>\n<li>The count of pictures is not needed, given that you can see them.</li>\n<li>The read-only role (primary or secondary) &amp; optional caption for a selected photo is shown.  By default, the primary photo is selected when Outbox Details opens.  The selection can be changed just as under the Main Info tab.\n</li><li>When opening an Outbox Details, you may get the warning message:<br>\n\"There is a mismatch between the \'Pic\' count and the number of images found in the folder<br>\nC:\\TriagePicSharedData\\sent<br>\nIf you just sent this email, wait a few seconds for background processing to complete and try again.\"</li>\n<li>On startup, new folder C:/TriagePic Shared Data/deleted is created if not present.</li>\n<li>The form, class, and .cs file â€œFormSentImagesâ€ has been renamed to â€œFormOutboxDetailsâ€.</li>\n<li>For version 1.13, an â€œAboutâ€ entry is added to the System menu (upper left hand corner).  â€œAboutâ€ content  design is very much like the splash screen, but slightly bigger, with title bar, OK button, and mention of the LPF project.  The wordings on the splash screen and main title bar were slightly revised.\n</li><li>During startup, a missing outbox.xml file is always checked for, not just when a new â€œoutbox queueâ€ directory is created.  If missing, a file defining an empty queue is substituted.</li>\n<li>The file TriagePicSharedSettings.xml is split into 3 files:  EmailSettings.xml, HospitalSettings.xml, and OtherSettings.xml.  Internally, new I/O classes are created for them.  Startup code is added to do migration from the old to new structure, and to use the new structure for fresh installs (with Generic, NLM, NNMC, Suburban internal file resources).  For existing installs, after migration, the old settings file is retained but renamed TriagePicSharedSettings[obsolete].xml.\n</li><li>The PatientID format setting is now retained, specifically in the HospitalSettings.xml file.  The representation is the fixed number of digits (not counting alphanumeric prefix); a variable number of digits is represented by -1.  For new installs, the default patient ID format for Suburban and NNMC is â€œfixed 4 digitâ€; for NLM and Generic, itâ€™s â€œvariableâ€ format.\n</li><li>Under â€œHospitalâ€ tab, the leftover subheading â€œPolicies and Other Settingsâ€ was removedâ€¦ (See the separate â€œPoliciesâ€ tab instead, created in a prior version.)\n</li><li>Edits to any field under the â€œHospitalâ€ tab now takes effect immediately, not just when TriagePic exits.  Likewise the Patient Prefix on the â€œPoliciesâ€ tab.\n</li><li>For programmers, the TriagePic VS project now contains a set of Class Diagrams.</li></ul>\n</div>\n<h2>Before January, 2011</h2>\nEarlier notes are available off-line.<br>\n\n<br>\n<br>\n<br>\n</div>','2012-06-01 02:31:22','Hidden'),(-100,'TriagePic Overview',-50,'TriagePic, a hospital-based Windows application, helps quickly gather photos and minimal information about disaster victims arriving at a perimeter triage station, particularly to assist with family reunification.  Photos can be gathered using a webcam or paired Bluetooth camera.  TriagePic can be used with a hospital\'s preprinted triage forms, auto-incrementing mass casualty ID numbers.   As each patient is routed to a color-coded zone for treatment, information about that patient is sent to and read in by this web site, where it may be accessed by hospital reunification counselors and emergency managers.  If desired (and available bandwidth allows), TriagePic can also email information directly to designated recipients\n<br>\n<br>\nFor more information, see the <a href=\"http://archive.nlm.nih.gov/proj/lpf.php\" target=\"_blank\">NLM project website</a>.<br>\n<br>\n<b>NEW - Using TriagePic on Tablets - Guidance for Capital Shield 12 Participants</b><br>\n<br>\nTriagePic was tested during the October, 2011 \"Capital Shield 12\" exercise.  Two guidance documents prepared in advance for hospital participants about the effective use of PL, and of TriagePic on Windows 7 tablets, can be found can be found at the above project web site.','2012-06-01 02:31:26','Hidden'),(-4,'Links',35,'<h2>Find Family and Friends</h2>\n<a href=\"https://safeandwell.communityos.org/cms//\" title=\"Red Cross Safe and Well List\">Red Cross Safe and Well List</a><br>\n<a href=\"http://www.nokr.org/nok/restricted/home.htm\" title=\"Next of Kin National Registry\">Next of Kin National Registry</a><br>\n<a href=\"http://www.lrcf.net/create-flyer/\" title=\"Missing Person Flier Creation Tools\">Missing Person Flier Creation Tools</a><br>\n\n<br><hr><br>\n\n<h2>Disaster Resources - General</h2>\n<a target=\"\" title=\"Disaster Assistance\" href=\"http://www.disasterassistance.gov/\">Disaster Assistance</a><br>\n<a href=\"http://app.redcross.org/nss-app/\" title=\"Red Cross Provides Shelters and Food\">Red Cross Provides Shelters and Food</a><br>\n<a target=\"\" title=\"NLM\'s Disaster Information Management Resource Center\" href=\"http://disasterinfo.nlm.nih.gov\">NLM\'s Disaster Information Management Resource Center</a><br> \n\n<br><hr><br>\n\n<h2>Disaster Resources - Tornadoes</h2>\n<a target=\"\" title=\"Tornado Information from the Disaster Information Management Resource Center\" href=\"http://disaster.nlm.nih.gov/enviro/tornados.html\">From the Disaster Information Management Resource Center</a><br>\n<a target=\"\" title=\"Tornado Information  from MedlinePlus\" href=\"http://www.nlm.nih.gov/medlineplus/tornadoes.html\">From MedlinePlus</a><br>\n<a target=\"\" title=\"NOAA 2011 Tornado Information\" href=\"http://www.noaanews.noaa.gov/2011_tornado_information.html\">From NOAA 2011</a><br>\n','2012-06-04 22:54:45','Hidden'),(-3,'Contact',31,'<h2>Contact Us</h2>\n<li>\n<script>\nvar kontakt = \"Contact us via email\"\nvar uzer = \"lpfsupport\"\nvar dohmain = \"mail.nih.gov\"\ndocument.write(\'<a href=\"\'+\'m\'+\'ai\'+\'l\'+\'to:\'+uzer+\'@\'+dohmain+\'?subject=Feedback for People Locator\">Click here to email the People Locator support staff</a>\')\n</script>.\n</li>\n<br><hr><br>\n<h2>Other Support</h2>\n<li>You may also be interested in the <a href=\"http://apps2.nlm.nih.gov/mainweb/siebel/nlm/index.cfm/\">the National Library of Medicine\'s help desk support page</a>.','2012-06-04 22:36:35','Hidden'),(-2,'Privacy',22,'<h2>Privacy with Publicly-Reported Community-Based Disaster Events</h2>\n<br>\nUnless otherwise indicated, community-based data contained on this Web site are available to the public and are searchable by all visitors to the Web site.\nData may be received from various sources, as indicated on individual records:<br>\n<br>\n<ol>\n<li>By direct reporting to NLM through...<br>\n<ul><li>the PL \"Report a Person\" page</li>\n<li>the \"ReUnite\" application</li>\n<li>email</li>\n</ul></li>\n<li>From instances of Google Person Finder</li>\n<li>From other organizations</li>\n\n</ol>\nSubmissions to the PL site are not moderated, and NLM and other contributing organizations should not be expected to review or verify the accuracy of any of the data presented on this Web site.<br>\nNote that data received by direct submission to NLM may be disseminated to other agencies, institutions, and organizations assisting the effort to locate missing people, including the appropriate instance of Google Person Finder.<br>\n<br>\nFor direct submissions to NLM, the sender\'s email address and iPhone device ID will be retained by NLM, but will not be displayed on the Web site.\nContact email addresses that are explicitly provided by the submitter may be displayed, however, regardless of the manner in which the data were received.<br>\n<br><hr><br>\n<h2>Privacy with Hospital-Based Disaster Events</h2>\n<br>\nUnless otherwise indicated, hospital-based data contained on this Web site are input by hospital personnel (i.e., registered PL users with hospital staff privileges),\nand available to and searchable by only hospital personnel and administrators.\nData may be received from multiple sources, as indicated on individual records:<br>\n<br>\n<ol>\n<li>By direct reporting to NLM through...<br>\n<ul><li>the PL \"Report a Person\" page</li>\n<li>the \"TriagePic\" application</li>\n<li>the \"ReUnite\" application</li>\n</ul>\n</li>\n<li>From other hospitals and Emergency Management organizations.</li>\n</ol>\nSubmissions to the PL site are not moderated,\nand NLM and other contributing organizations should not be expected to review or verify the accuracy of any of the data presented on this Web site.<br>\nNote that hospital-based data received by direct submission to NLM may be disseminated,\nwith permission, to other agencies, institutions, and organizations assisting the effort to locate missing people.<br>\n<br>For direct submissions to NLM, the sender\'s email address and iPhone device ID will be retained by NLM, but will not be displayed on the Web site.\nContact email addresses that are explicitly provided by the submitter may be displayed, however, regardless of the manner in which the data were received.<br>\n<br><hr><br>\n<h2>For More</h2>\n<ul>\n<li><a href=\"http://www.nlm.nih.gov/privacy.html\">NLM\'s Overall Privacy Statement</a></li>\n</ul>\n<br>','2012-06-04 22:31:01','Hidden'),(-1,'About',21,'<h2>About</h2>This site is provided by the <a href=\"http://www.nlm.nih.gov/\">United States National Library of Medicine (NLM)</a> in Bethesda, Maryland.NLM is part of the <a href=\"http://www.nih.gov/\">National Institutes of Health</a>, U.S. <a href=\"http://www.dhhs.gov/\">Department of Health and Human Services</a>, and has in its mission the development and coordination of communication technology to improve the delivery of health services. This site is provided for the purpose of studying the utility of such tools in responding to disasters. The underlying technology was developed through NLM\'s <a href=\"http://archive.nlm.nih.gov/proj/lpf.php\">Lost Person Finder (LPF) project</a>, part of NLM\'s contribution to the <a href=\"http://www.bethesdahospitalsemergencypartnership.org/\">Bethesda Hospitals\' Emergency Preparedness Partnership (BHEPP)</a>.&nbsp;The partnership (BHEPP) received initial federal funding for LPF and other NLM IT projects in 2008-9.&nbsp;The LPF project is currently supported by the Intramural Research Program of the NIH, through NLM\'s Lister Hill National Center for Biomedical Communications (LHNCBC). Software development is headed by LHNCBC\'s Communication Engineering Branch (CEB), with additional content from LHNCBC\'s Computer Science Branch (CSB) and the Disaster Information Management Research Center (DIMRC), part of NLM\'s Specialized Information Services.\n\n<br><br>\n<div style=\"clear:both;\"></div>\n<hr>\n<br>\n\n<h2>Browser Support</h2>\nThis site should work with the most recent versions of Firefox, Chrome, Safari, Internet Explorer, and any other modern HTML5 capable browsers.\n\n<br><br>\n<div style=\"clear:both;\"></div>\n<hr>\n<br>\n\n<h2>Attribution</h2>\n\nThe Lost Person Finder project and the People Locator are developed and hosted<br><br>\n\n<a href=\"http://www.nlm.nih.gov\"><img class=\"aboutIMG\" src=\"theme/lpf3/img/nlm.png\" alt=\"United States National Library of Medicine Logo\"></a> by the <a href=\"http://www.nlm.nih.gov\">US National Library of Medicine</a><br>\n<a href=\"http://www.nih.gov\"><img class=\"aboutIMG\" src=\"theme/lpf3/img/nih.png\" alt=\"National Institutes of Health Logo\"></a> which is a part of the <a href=\"http://www.nih.gov\">National Institutes of Health</a><br>\n<a href=\"http://www.hhs.gov\"><img class=\"aboutIMG\" src=\"theme/lpf3/img/hhs.png\" alt=\"Department of Health and Human Services Logo\"></a> under the <a href=\"http://www.hhs.gov\">Department of Health and Human Services</a><br>\n\n<a href=\"http://www.bethesdahospitalsemergencypartnership.org/\"><img class=\"aboutIMG\" src=\"theme/lpf3/img/bhepp.png\" alt=\"Bethesda Hospitals\' Emergency Preparedness Partnership Logo\"></a> in Collaboration with the <a href=\"http://www.bethesdahospitalsemergencypartnership.org/\">Bethesda Hospitals\' Emergency Preparedness Partnership</a> of which<br>\n\n<a href=\"http://www.suburbanhospital.org/\"><img class=\"aboutIMG\" src=\"theme/lpf3/img/suburban.png\" alt=\"Suburban Hospital a part of Johns Hopkins Medicine Logo\"></a> <a href=\"http://www.suburbanhospital.org/\">Suburban Hospital</a>, <br>\n\n<a href=\"http://www.bethesda.med.navy.mil/\"><img class=\"aboutIMG\" src=\"theme/lpf3/img/nnmc.png\" alt=\"Walter Reed National Military Medical Center Logo\"></a> <a href=\"http://www.bethesda.med.navy.mil/\"> Walter Reed National Military Medical Center</a>,<br>\n\n<a href=\"http://clinicalcenter.nih.gov/\"><img class=\"aboutIMG\" src=\"theme/lpf3/img/clinicalcenter.png\" alt=\"National Institutes of Health Clinical Center Logo\"></a> and the <a href=\"http://clinicalcenter.nih.gov/\">National Institutes of Health Clinical Center</a> are members<br>\n\n<a href=\"http://sahanafoundation.org\"><img class=\"aboutIMG\" src=\"theme/lpf3/img/sahana.png\" alt=\"Sahana Software Foundation Logo\"></a> with some portions of code contributed by the <a href=\"http://sahanafoundation.org\">Sahana Software Foundation</a>.<br><br>\n\n\n\n\n','2012-06-04 22:29:48','Hidden'),(1,'How do I search for a person?',14,'<h2>Searching</h2>\n1) Enter a name in the search box<br>\n2) Click on the \"Search\" button, or hit Enter <br>\n<br>\n<i>Examples:</i><br>\n<br>\n Joseph Doe<br>\n Doe, Jane<br>\n Joseph Joe Joey<br>\n<br>\nIt is recommended to leave off titles (\"Dr.\", \"Mrs.\") and suffixes (\"Jr\") at this time.<br>\n\n<br><hr><br>\n\n<h2>Search Options</h2>\nOnce you have performed a search, you may also limit your results by status, gender, and age.<br>\n<br>\nStatus choices are missing (blue), alive and well (green), injured (red), deceased (black), found (brown) or unknown (gray).<br>\n<br>\nGender choices are male, female, and other/unknown.<br>\n<br>\nAge choices are 0-17, 18+, or unknown.<br><br>If you want to see only records that have photos, include \"[image]\" in the search box.&nbsp; Use \"[-image]\" to see only records that have no photos.<br>\n\n<br><hr><br>\n\n<h2>Results</h2>\nResults include any of the search terms.&nbsp; To tolerate misspellings, results are not limited to exact matches.&nbsp; Matches may include names found in certain fields, like Notes, that will be visible only if you consult the record\'s details.<br>\n<br>\nUnder the search box is the number of records found that match your search, and the total number in the database (e.g., Found 2 out of 43).<br>\n<br>\nYou may sort your results by Time, Name, Age, or Status.&nbsp; By Name orders by last name, then within that, first.&nbsp; By Age will use a calculated midpoint age for each record with an age range. <br>\n<br>\nInteractive mode displays results by page.  The default is 25 per page.  You may change it to 50 or 100 per page via the pull down menu at the top of the results.<br>\n<br>\nHands Free mode displays results as several horizontally-scrolling rows of boxes with a photograph or text.  The boxes always distribute themselves evenly among the rows, starting at the right side and from top row to bottom.  If there are more boxes than can be shown at once, the rows will become animated to scroll horizontally with wrap-around.  There is no meaning to the ordering of the images at this time.<br>\n\n<br><hr><br>\n\n<h2>Getting Details about a Given Results<br></h2>\nClick on the results (photo or text) for more information.<br>\n\n<br><hr><br>\n\n<h2>Pause and Play Buttons</h2>\nIf horizontal scrolling is occurring, Pause will stop that, and Play will resume it.  Even while paused, the search will be repeated every minute to look for fresh content.<br>\n\n<br><hr><br>\n\n<h2>Search and Data Updates</h2>\nOnce a set of results for a search is loaded, the search will be quietly repeated every minute to see if there is new content.<br><br>New Information can be input via the Report a Person page, or sent to us directly by email or web service, e.g., from apps like ReUnite and TriagePic.\n<br>\n<br>\n<br>\n','2012-06-04 23:04:44','Public'),(2,'How do I report a missing or found person?',18,'<h2>Report a Missing or Found Person via this website</h2>\n\nWhen you click the link:<a href=\"report\">Report a Person</a>, a new empty person record is created.<br>\n<br>\nIt is up to you at this point to enter as much information as you can to help in the assistance of relocating this person. When you have finished entering textual data, click the save button to update the \"person record\" in our system. Please rememeber to do this before trying to upload a photograph or your textual information may be lost (not saved).<br>\n<br>\nAs soon as you have completed the above steps, you will automatically be registered to \"follow\" the record for the person you just entered. This equates to being notified via email (the address used it the one for your user account on PL) whenever a change or comment is added to the person\'s record. In this manner, you are kept up to date with any additional information others may provide about this person which may help you in your reunification process.<br>\n\n<br><hr><br>\n\n<h2>Report a Person using our iOS™ App, “ReUnite”</a></h2>\nOf particular interest to aid workers, we provide a free iPhone app called <a href=\"http://lpf.nlm.nih.gov/\" title=\"\">ReUnite</a> through the Apple Store.&nbsp;\nThis app creates structured content with associated photographs (limited to 1 per submission at the moment).&nbsp;\nMore information can be transmitted to us this way than using the unstructured email method detailed below.<br>\n<br>\nReUnite currently supports iPhone 3G and iPhone 4 with iPhone OS 3.0 or later.&nbsp; iPod Touch and iPad are also usable.<br>\n<br>\n<div id=\"more_reunite_en\">\nUsers can choose to take a new photo using their iPhone’s camera\nor use an existing image from their camera roll/photo library.&nbsp;\nThey are then able to tag certain information about the person in the photo.&nbsp;\nThe following fields, all optional, are available for editing:<br><br>\n    <ul>\n      <li>Given Name</li>\n      <li>Family Name</li>\n      <li>Health Status: (Alive &amp; Well / Injured / Deceased / Unknown)</li>\n      <li>Gender: (Male / Female / Unknown)</li>\n      <li>Age Group: (0-17 / 18+ / Unknown) <i>(or enter an estimated age instead)</i></li>\n      <li>Estimated Age, in years</li>\n      <li>Location Status: (Missing / Known)</li>\n      <li>Last Known Location <i>(if missing)</i> / Current Location <i>(otherwise)</i></li>\n      <li>Street</li>\n      <li>Town</li>\n      <li>ID Tag <i>Automatically generated by default. Aid workers can substitute organization’s triage number.</i></li>\n      <li>Voice Note</li>\n      <li>Text Note</li>\n    </ul>\n    <p>In addition, the following info is generated at the time the record is created:</p>\n    <ul>\n      <li>GPS Location <span style=\"font-style: italic;\">(if enabled)</span><br></li>\n      <li>Date and time</li>\n    </ul>\nThe image and corresponding information is then sent by web service to the People Locator servers automatically.&nbsp;\n    Some information is also embedded into the image’s EXIF tags.&nbsp;\n    The records (including photos) are stored locally on the iPhone in an SQLite database format.&nbsp;\n    This database can be sent by email to <a href=\"mailto:lpfsupport@mail.nih.gov\">lpfsupport@mail.nih.gov</a>,\n    where support personnel can arrange to have it included in our database.&nbsp;\n    Consequently, data can be collected when cell phone connectivity is not available,\n    and subsequently sent when connectivity becomes available.<br>\n</div>\n\n<br><hr><br>\n\n<h2>Report a Person by Email</a></h2>\n\nYou may also report a name and simple status directly to us by email. You can also attach a single photograph (optional) in one of the following formats: jpeg, gif, png. For now, content in the email\'s body is ignored.<br>\n<br>\nSend the email to: <a href=\"mailto:disaster@mail.nih.gov\">disaster@mail.nih.gov</a><br><br>\nWith a subject line in the format below:<br><br>\n<p><i><b>Name of Missing or Found PersonStatus</b></i></p>\nYou may use any of these terms in the subject to denote the status of the person being reported :<br><br>\n<ul>\n   <li>Missing</li>\n   <li>Alive and Well</li>\n   <li>Injured</li>\n   <li>Deceased</li>\n   <li>Found <i> (However, this is discouraged as it does not imply health status)</i></li>\n</ul>\nExample of subject line :<br>\n<br>\n<p><b>Jean Edwards alive and well</b></p>\nAny punctuation will be ignored or treated as whitespace. You should also avoid using the word \"not\".<br>\n<br>\n<p><b>Table of Status Words</b></p>\n <table border=\"1\" cellpadding=\"0\" cellspacing=\"0\">\n  <tbody><tr>\n   <td valign=\"top\" width=\"163\"><p><b>Status Assumed</b></p></td>\n   <td valign=\"top\" width=\"811\"><p><b>Recognized words in subject line (case doesn’t matter)</b></p></td>\n  </tr>\n  <tr>\n   <td valign=\"top\" width=\"163\"><p>Missing</p></td>\n   <td valign=\"top\" width=\"811\"><p>missing, lost, looking for, [to] find</p></td>\n  </tr>\n  <tr>\n   <td valign=\"top\" width=\"163\"></td>\n   <td valign=\"top\" width=\"811\"><p>French: disparu, perdu, trouver, a la recherche de, trouver [SUITE: à la recherche de]</p></td>\n  </tr>\n  <tr>\n   <td valign=\"top\" width=\"163\"><p>Alive and Well</p></td>\n   <td valign=\"top\" width=\"811\"><p>alive, well, okay, OK, good, healthy, recovered, fine</p></td>\n  </tr>\n  <tr>\n   <td valign=\"top\" width=\"163\"></td>\n   <td valign=\"top\" width=\"811\"> <p>French: en vie, vivant, ok, bien portant, en bonne sante, gueri [SUITE: en bonne santé, guéri]</p></td>\n  </tr>\n  <tr>\n   <td valign=\"top\" width=\"163\"><p>Injured</p></td>\n   <td valign=\"top\" width=\"811\"><p>injured, hurt, wounded, sick, treated, recovering</p></td>\n  </tr>\n  <tr>\n   <td valign=\"top\" width=\"163\"></td>\n   <td valign=\"top\" width=\"811\"><p>French: blesse, mal en point, malade, soigne, convalscent [SUITE: blessé, soigné]</p></td>\n  </tr>\n  <tr>\n   <td valign=\"top\" width=\"163\"><p>Deceased</p></td>\n   <td valign=\"top\" width=\"811\"><p>deceased, dead, died, buried</p></td>\n  </tr>\n  <tr>\n   <td valign=\"top\" width=\"163\"></td>\n   <td valign=\"top\" width=\"811\"><p>French: decede, mort, inhume [SUITE: décédé, inhumé ]</p></td>\n  </tr>\n  <tr>\n   <td valign=\"top\" width=\"163\"><p>Found</p></td>\n   <td valign=\"top\" width=\"811\"><p>found</p></td>\n  </tr>\n </tbody></table>\n\n\n<br><hr><br>\n\n<h2>Report a Person via NLM\'s TriagePIC hospital software</h2>\nPlease contact us via support or inquire within your organization\'s help staff to find out more information on how to obtain this software.<br>\n<br>\n<br>\n','2012-06-05 01:01:57','Public');
/*!40000 ALTER TABLE `rez_pages` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `sessions`
--

DROP TABLE IF EXISTS `sessions`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `sessions` (
  `session_id` varchar(64) NOT NULL,
  `sess_key` varchar(64) NOT NULL,
  `secret` varchar(64) NOT NULL,
  `inactive_expiry` bigint(20) NOT NULL,
  `expiry` bigint(20) NOT NULL,
  `data` text,
  PRIMARY KEY (`session_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `sessions`
--

LOCK TABLES `sessions` WRITE;
/*!40000 ALTER TABLE `sessions` DISABLE KEYS */;
INSERT INTO `sessions` (`session_id`, `sess_key`, `secret`, `inactive_expiry`, `expiry`, `data`) VALUES ('f5ca44e5a1c61e8d85bbfbf382ce23a6','5660a1a8a2fedc0b6d62459b147693f5','71e72c2f1790902ad2bfeb8d54d2ca7b',1370980618,1370980618,NULL);
/*!40000 ALTER TABLE `sessions` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `sys_group_to_module`
--

DROP TABLE IF EXISTS `sys_group_to_module`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `sys_group_to_module` (
  `group_id` int(11) NOT NULL,
  `module` varchar(60) NOT NULL,
  `status` varchar(60) NOT NULL,
  PRIMARY KEY (`group_id`,`module`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `sys_group_to_module`
--

LOCK TABLES `sys_group_to_module` WRITE;
/*!40000 ALTER TABLE `sys_group_to_module` DISABLE KEYS */;
INSERT INTO `sys_group_to_module` (`group_id`, `module`, `status`) VALUES (1,'admin','enabled'),(1,'appcheck','enabled'),(1,'arrive','enabled'),(1,'biu','enabled'),(1,'eap','enabled'),(1,'em','enabled'),(1,'ha','enabled'),(1,'home','enabled'),(1,'inw','enabled'),(1,'mpres','enabled'),(1,'person','enabled'),(1,'pfif','enabled'),(1,'plus','enabled'),(1,'pop','enabled'),(1,'pref','enabled'),(1,'print','enabled'),(1,'queue','enabled'),(1,'rap','enabled'),(1,'report','enabled'),(1,'rez','enabled'),(1,'stat','enabled'),(1,'stat2','enabled'),(1,'tp','enabled'),(1,'xst','enabled'),(2,'eap','enabled'),(2,'home','enabled'),(2,'inw','enabled'),(2,'person','enabled'),(2,'pref','enabled'),(2,'print','enabled'),(2,'report','enabled'),(2,'rez','enabled'),(2,'stat2','enabled'),(2,'xst','enabled'),(3,'eap','enabled'),(3,'home','enabled'),(3,'inw','enabled'),(3,'person','enabled'),(3,'pref','enabled'),(3,'print','enabled'),(3,'report','enabled'),(3,'rez','enabled'),(3,'stat2','enabled'),(3,'xst','enabled'),(5,'arrive','enabled'),(5,'eap','enabled'),(5,'home','enabled'),(5,'inw','enabled'),(5,'person','enabled'),(5,'pref','enabled'),(5,'print','enabled'),(5,'report','enabled'),(5,'rez','enabled'),(5,'stat','enabled'),(5,'stat2','enabled'),(5,'tp','enabled'),(5,'xst','enabled'),(6,'arrive','enabled'),(6,'eap','enabled'),(6,'ha','enabled'),(6,'home','enabled'),(6,'inw','enabled'),(6,'person','enabled'),(6,'pref','enabled'),(6,'print','enabled'),(6,'report','enabled'),(6,'rez','enabled'),(6,'stat','enabled'),(6,'stat2','enabled'),(6,'tp','enabled'),(6,'xst','enabled'),(7,'eap','enabled'),(7,'home','enabled'),(7,'inw','enabled'),(7,'person','enabled'),(7,'pref','enabled'),(7,'print','enabled'),(7,'report','enabled'),(7,'rez','enabled'),(7,'stat','enabled'),(7,'stat2','enabled'),(7,'xst','enabled'),(1,'sync','enabled'),(2,'sync','enabled'),(4,'sync','enabled'),(5,'sync','enabled'),(6,'sync','enabled'),(7,'sync','enabled');
/*!40000 ALTER TABLE `sys_group_to_module` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `sys_user_groups`
--

DROP TABLE IF EXISTS `sys_user_groups`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `sys_user_groups` (
  `group_id` int(11) NOT NULL,
  `group_name` varchar(40) NOT NULL,
  PRIMARY KEY (`group_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `sys_user_groups`
--

LOCK TABLES `sys_user_groups` WRITE;
/*!40000 ALTER TABLE `sys_user_groups` DISABLE KEYS */;
INSERT INTO `sys_user_groups` (`group_id`, `group_name`) VALUES (1,'Administrator'),(2,'Registered User'),(3,'Anonymous User'),(5,'Hospital Staff'),(6,'Hospital Staff Admin'),(7,'Researchers');
/*!40000 ALTER TABLE `sys_user_groups` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `sys_user_to_group`
--

DROP TABLE IF EXISTS `sys_user_to_group`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `sys_user_to_group` (
  `group_id` int(11) NOT NULL,
  `p_uuid` varchar(128) NOT NULL,
  KEY `p_uuid` (`p_uuid`),
  KEY `group_id` (`group_id`),
  CONSTRAINT `sys_user_to_group_ibfk_1` FOREIGN KEY (`p_uuid`) REFERENCES `person_uuid` (`p_uuid`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `sys_user_to_group_ibfk_2` FOREIGN KEY (`group_id`) REFERENCES `sys_user_groups` (`group_id`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `sys_user_to_group_ibfk_4` FOREIGN KEY (`group_id`) REFERENCES `sys_user_groups` (`group_id`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `sys_user_to_group`
--

LOCK TABLES `sys_user_to_group` WRITE;
/*!40000 ALTER TABLE `sys_user_to_group` DISABLE KEYS */;
INSERT INTO `sys_user_to_group` (`group_id`, `p_uuid`) VALUES (1,'1'),(7,'5'),(3,'2');
/*!40000 ALTER TABLE `sys_user_to_group` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `user_preference`
--

DROP TABLE IF EXISTS `user_preference`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `user_preference` (
  `pref_id` int(16) NOT NULL AUTO_INCREMENT,
  `p_uuid` varchar(128) NOT NULL,
  `module_id` varchar(20) NOT NULL,
  `pref_key` varchar(60) NOT NULL,
  `value` varchar(100) DEFAULT NULL,
  PRIMARY KEY (`pref_id`),
  KEY `p_uuid` (`p_uuid`),
  CONSTRAINT `user_preference_ibfk_1` FOREIGN KEY (`p_uuid`) REFERENCES `person_uuid` (`p_uuid`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `user_preference_ibfk_2` FOREIGN KEY (`p_uuid`) REFERENCES `person_uuid` (`p_uuid`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `user_preference`
--

LOCK TABLES `user_preference` WRITE;
/*!40000 ALTER TABLE `user_preference` DISABLE KEYS */;
/*!40000 ALTER TABLE `user_preference` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `users`
--

DROP TABLE IF EXISTS `users`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `users` (
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
  KEY `p_uuid` (`p_uuid`),
  CONSTRAINT `users_ibfk_1` FOREIGN KEY (`p_uuid`) REFERENCES `person_uuid` (`p_uuid`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB AUTO_INCREMENT=199 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `users`
--

LOCK TABLES `users` WRITE;
/*!40000 ALTER TABLE `users` DISABLE KEYS */;
INSERT INTO `users` (`user_id`, `p_uuid`, `user_name`, `password`, `salt`, `changed_timestamp`, `status`, `confirmation`, `oauth_id`, `profile_link`, `profile_picture`, `locale`, `verified_email`) VALUES (1,'1','root','48a75258251572ee31678322913132dc','81f19585af15a0e84e',1370980603,'active','e3c0b3255617c3018f0d79933d95e869','1','http:google.com','','en_US',5),(2,'2','mpres',NULL,NULL,0,'active',NULL,NULL,NULL,NULL,NULL,NULL),(184,'5','testDontDelete','bc47e726664e859c27f66b50aeba50de','aab423ab34516eaa5a',1356722034,'banned','f4bf7ccd94499fea4745a55b3ae474fe',NULL,NULL,NULL,NULL,NULL);
/*!40000 ALTER TABLE `users` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `voice_note`
--

DROP TABLE IF EXISTS `voice_note`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `voice_note` (
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
  `sha1original` varchar(40) DEFAULT NULL COMMENT 'holds the sha1 of the original voicenote from which this voicenote was transcoded',
  PRIMARY KEY (`voice_note_id`),
  KEY `p_uuid` (`p_uuid`),
  CONSTRAINT `voice_note_ibfk_1` FOREIGN KEY (`p_uuid`) REFERENCES `person_uuid` (`p_uuid`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `voice_note`
--

LOCK TABLES `voice_note` WRITE;
/*!40000 ALTER TABLE `voice_note` DISABLE KEYS */;
/*!40000 ALTER TABLE `voice_note` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `voice_note_seq`
--

DROP TABLE IF EXISTS `voice_note_seq`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `voice_note_seq` (
  `id` bigint(20) NOT NULL AUTO_INCREMENT COMMENT 'stores next id in sequence for the voice_note table',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=1669 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `voice_note_seq`
--

LOCK TABLES `voice_note_seq` WRITE;
/*!40000 ALTER TABLE `voice_note_seq` DISABLE KEYS */;
INSERT INTO `voice_note_seq` (`id`) VALUES (1668);
/*!40000 ALTER TABLE `voice_note_seq` ENABLE KEYS */;
UNLOCK TABLES;



--
-- Table structure for table `sync_updates`
--

DROP TABLE IF EXISTS `sync_updates`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `sync_updates` (
  `sync_update_id` bigint(20) NOT NULL AUTO_INCREMENT,
  `update_time` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `incident_id` bigint(20) NOT NULL,
  `instance_uuid` varchar(64) NOT NULL,
  PRIMARY KEY (`sync_update_id`),
  CONSTRAINT `sync_updates_ibfk_1` FOREIGN KEY (`incident_id`) REFERENCES `incident` (`incident_id`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `sync_updates`
--

LOCK TABLES `sync_updates` WRITE;
/*!40000 ALTER TABLE `sync_updates` DISABLE KEYS */;
/*!40000 ALTER TABLE `sync_updates` ENABLE KEYS */;
UNLOCK TABLES;



--
-- Dumping routines for database 'tmp'
--
/*!50003 DROP PROCEDURE IF EXISTS `delete_reported_person` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = '' */ ;
DELIMITER ;;
/*!50003 CREATE*/ /*!50020 DEFINER=`root`@`localhost`*/ /*!50003 PROCEDURE `delete_reported_person`(IN id VARCHAR(128),IN deleteNotes BOOLEAN)
BEGIN


DELETE p.* FROM person_uuid p, person_to_report pr WHERE pr.rep_uuid = p.p_uuid AND pr.p_uuid = id AND pr.rep_uuid NOT IN (SELECT p_uuid FROM users);


DELETE person_uuid.* FROM person_uuid WHERE p_uuid = id;


DELETE pfif_person.* FROM pfif_person WHERE p_uuid = id;

IF deleteNotes THEN
  
  DELETE pfif_note.* FROM pfif_note WHERE p_uuid = id;

  
  UPDATE pfif_note SET linked_person_record_id = NULL WHERE linked_person_record_id = id;
END IF;

END */;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `PLSearch` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
/*!50003 CREATE*/ /*!50020 DEFINER=`root`@`localhost`*/ /*!50003 PROCEDURE `PLSearch`(IN `searchTerms` CHAR(255), IN `statusFilter` VARCHAR(100), IN `genderFilter` VARCHAR(100), IN `ageFilter` VARCHAR(100), IN `hospitalFilter` VARCHAR(100), IN `incidentName` VARCHAR(100), IN `sortBy` VARCHAR(100), IN `pageStart` INT, IN `perPage` INT)
BEGIN

       DROP TABLE IF EXISTS tmp_names;
   IF searchTerms = '' THEN
           CREATE TEMPORARY TABLE tmp_names AS (
           SELECT SQL_NO_CACHE pu.*
               FROM person_uuid pu
                  JOIN incident i  ON (pu.incident_id = i.incident_id AND i.shortname = incidentName)
               LIMIT 1000000
           );
   ELSEIF searchTerms = 'unknown' THEN
           CREATE TEMPORARY TABLE  tmp_names AS (
           SELECT SQL_NO_CACHE pu.*
               FROM person_uuid pu
                  JOIN incident i  ON (pu.incident_id = i.incident_id AND i.shortname = incidentName)
               WHERE (full_name = '' OR full_name IS NULL)
               LIMIT 1000000
           );
   ELSE
           CREATE TEMPORARY TABLE  tmp_names AS (
           SELECT SQL_NO_CACHE pu.*
               FROM person_uuid pu
                  JOIN incident i  ON (pu.incident_id = i.incident_id AND i.shortname = incidentName)
               WHERE full_name like CONCAT(searchTerms , '%')
               LIMIT 1000000
           );
    END IF;

   SET @sqlString = CONCAT("SELECT  SQL_NO_CACHE `tn`.`p_uuid`       AS `p_uuid`,
                               `tn`.`full_name`    AS `full_name`,
                               `tn`.`given_name`   AS `given_name`,
                               `tn`.`family_name`  AS `family_name`,
                               (CASE WHEN `ps`.`opt_status` NOT IN ('ali', 'mis', 'inj', 'dec', 'fnd') THEN 'unk' ELSE `ps`.`opt_status` END) AS `opt_status`,
                               CONVERT_TZ(ps.last_updated,'America/New_York','UTC') AS updated,
                               (CASE WHEN `pd`.`opt_gender` NOT IN ('mal', 'fml', 'cpx') OR `pd`.`opt_gender` IS NULL THEN 'unk' ELSE `pd`.`opt_gender` END) AS `opt_gender`,
                               `pd`.`years_old` as `years_old`,
                               `pd`.`minAge` as `minAge`,
                               `pd`.`maxAge` as `maxAge`,
                               `i`.`image_height` AS `image_height`,
                               `i`.`image_width`  AS `image_width`,
                               `i`.`url` AS `url`,
                               `i`.`url_thumb`    AS `url_thumb`,
                               `i`.`color_channels` AS `color_channels`,
                               (CASE WHEN `h`.`short_name` NOT IN ('nnmc', 'suburban') OR `h`.`short_name` IS NULL THEN 'other' ELSE `h`.`short_name` END)  AS `hospital`,
                               (CASE WHEN (`h`.`hospital_uuid` = -(1)) THEN NULL ELSE `h`.`icon_url` END) AS `icon_url`,
                               `pd`.last_seen,
                               `pd`.other_comments as comments,
                                ecl.person_id as mass_casualty_id
            FROM tmp_names tn
            JOIN person_status ps ON (tn.p_uuid = ps.p_uuid AND INSTR(?, (CASE WHEN ps.opt_status NOT IN ('ali', 'mis', 'inj', 'dec', 'fnd') OR ps.opt_status IS NULL THEN 'unk' ELSE  ps.opt_status END)))
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
            LEFT JOIN hospital h ON (tn.hospital_uuid = h.hospital_uuid AND INSTR(?, (CASE WHEN `h`.`short_name` NOT IN ('nnmc', 'suburban') OR `h`.`short_name` IS NULL THEN 'other' ELSE `h`.`short_name` END)))
            LEFT JOIN image i ON (tn.p_uuid = i.p_uuid AND i.principal = TRUE)
            LEFT JOIN edxl_co_lpf ecl ON tn.p_uuid = ecl.p_uuid
        WHERE (tn.expiry_date > NOW() OR tn.expiry_date IS NULL)
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
END */;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `PLSearch_Count` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = '' */ ;
DELIMITER ;;
/*!50003 CREATE*/ /*!50020 DEFINER=`root`@`localhost`*/ /*!50003 PROCEDURE `PLSearch_Count`(IN searchTerms CHAR(255),
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



END */;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;

--
-- Final view structure for view `person_search`
--

/*!50001 DROP TABLE IF EXISTS `person_search`*/;
/*!50001 DROP VIEW IF EXISTS `person_search`*/;
/*!50001 SET @saved_cs_client          = @@character_set_client */;
/*!50001 SET @saved_cs_results         = @@character_set_results */;
/*!50001 SET @saved_col_connection     = @@collation_connection */;
/*!50001 SET character_set_client      = utf8 */;
/*!50001 SET character_set_results     = utf8 */;
/*!50001 SET collation_connection      = utf8_general_ci */;
/*!50001 CREATE ALGORITHM=UNDEFINED */
/*!50013 DEFINER=`root`@`localhost` SQL SECURITY DEFINER */
/*!50001 VIEW `person_search` AS select `pu`.`p_uuid` AS `p_uuid`,`pu`.`full_name` AS `full_name`,`pu`.`given_name` AS `given_name`,`pu`.`family_name` AS `family_name`,`pu`.`alternate_names` AS `alternate_names`,`pu`.`expiry_date` AS `expiry_date`,`ps`.`last_updated` AS `updated`,`ps`.`last_updated_db` AS `updated_db`,(case when (`ps`.`opt_status` not in ('ali','mis','inj','dec','unk','fnd')) then 'unk' else `ps`.`opt_status` end) AS `opt_status`,(case when ((`pd`.`opt_gender` not in ('mal','fml','cpx')) or isnull(`pd`.`opt_gender`)) then 'unk' else `pd`.`opt_gender` end) AS `opt_gender`,(case when isnull(cast(`pd`.`years_old` as unsigned)) then -(1) else `pd`.`years_old` end) AS `years_old`,(case when isnull(cast(`pd`.`minAge` as unsigned)) then -(1) else `pd`.`minAge` end) AS `minAge`,(case when isnull(cast(`pd`.`maxAge` as unsigned)) then -(1) else `pd`.`maxAge` end) AS `maxAge`,(case when (cast(`pd`.`years_old` as unsigned) is not null) then (case when (`pd`.`years_old` < 18) then 'youth' when (`pd`.`years_old` >= 18) then 'adult' end) when ((cast(`pd`.`minAge` as unsigned) is not null) and (cast(`pd`.`maxAge` as unsigned) is not null) and (`pd`.`minAge` < 18) and (`pd`.`maxAge` >= 18)) then 'both' when ((cast(`pd`.`minAge` as unsigned) is not null) and (`pd`.`minAge` >= 18)) then 'adult' when ((cast(`pd`.`maxAge` as unsigned) is not null) and (`pd`.`maxAge` < 18)) then 'youth' else 'unknown' end) AS `ageGroup`,`i`.`image_height` AS `image_height`,`i`.`image_width` AS `image_width`,`i`.`url_thumb` AS `url_thumb`,`i`.`url` AS `url`,`i`.`color_channels` AS `color_channels`,`i`.`original_filename` AS `original_filename`,(case when (`h`.`hospital_uuid` = -(1)) then NULL else `h`.`icon_url` end) AS `icon_url`,`inc`.`shortname` AS `shortname`,`inc`.`name` AS `name`,(case when ((`pu`.`hospital_uuid` not in (1,2,3)) or isnull(`pu`.`hospital_uuid`)) then 'public' else lcase(`h`.`short_name`) end) AS `hospital`,`pd`.`other_comments` AS `comments`,`pd`.`last_seen` AS `last_seen`,`ecl`.`person_id` AS `mass_casualty_id`,`ecl`.`triage_category` AS `triage_category` from ((((((`person_uuid` `pu` join `person_status` `ps` on((`pu`.`p_uuid` = `ps`.`p_uuid`))) left join `image` `i` on(((`pu`.`p_uuid` = `i`.`p_uuid`) and (`i`.`principal` = 1)))) join `person_details` `pd` on((`pu`.`p_uuid` = `pd`.`p_uuid`))) join `incident` `inc` on((`inc`.`incident_id` = `pu`.`incident_id`))) left join `hospital` `h` on((`h`.`hospital_uuid` = `pu`.`hospital_uuid`))) left join `edxl_co_lpf` `ecl` on((`ecl`.`p_uuid` = `pu`.`p_uuid`))) */;
/*!50001 SET character_set_client      = @saved_cs_client */;
/*!50001 SET character_set_results     = @saved_cs_results */;
/*!50001 SET collation_connection      = @saved_col_connection */;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2013-06-11 16:10:25
