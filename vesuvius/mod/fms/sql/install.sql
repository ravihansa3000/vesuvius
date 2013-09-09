--
-- @name        Facility Management
-- @version     1.0
-- @package     fms
-- @author      Clayton Kramer <clayton.kramer@mail.cuny.edu>
-- @about       Developed by the CUNY School of Professional Studies for the New York City Office of Emergency Management
-- @link        https://www.sps.cuny.edu/about
-- @link        http://sahanafoundation.org
-- @license     http://www.gnu.org/copyleft/lesser.html GNU Lesser General Public License (LGPL)
--

DROP TABLE IF EXISTS `fms_person_to_facility`;
DROP TABLE IF EXISTS `fms_person_to_transfer`;
DROP TABLE IF EXISTS `fms_facility_to_event`;
DROP TABLE IF EXISTS `fms_facility`;

CREATE TABLE `fms_facility` (
  `facility_uuid` int(11) NOT NULL AUTO_INCREMENT,
  `facility_name` varchar(64) DEFAULT NULL,
  `facility_code` varchar(10) DEFAULT NULL,
  `facility_resource_type_abbr` varchar(10) DEFAULT NULL,
  `facility_resource_status` varchar(40) DEFAULT NULL,
  `facility_capacity` int(11) DEFAULT NULL,
  `facility_activation_sequence` int(11) DEFAULT NULL,
  `facility_allocation_status` varchar(30) DEFAULT NULL,
  `facility_group` varchar(64) DEFAULT NULL,
  `facility_group_type` varchar(30) DEFAULT NULL,
  `facility_group_allocation_status` varchar(30) DEFAULT NULL,
  `work_email` varchar(255) DEFAULT NULL,
  `work_phone` varchar(32) DEFAULT NULL,
  `street_1` varchar(255) DEFAULT NULL,
  `street_2` varchar(255) DEFAULT NULL,
  `city` varchar(255) DEFAULT NULL,
  `state` varchar(255) DEFAULT NULL,
  `postal_code` varchar(30) DEFAULT NULL,
  `borough` varchar(30) DEFAULT NULL,
  `country` varchar(10) DEFAULT NULL,
  `longitude` decimal(18,12) DEFAULT NULL,
  `latitude` decimal(18,12) DEFAULT NULL,
  `created_at` timestamp NULL DEFAULT '0000-00-00 00:00:00',
  `updated_at` timestamp NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  PRIMARY KEY (`facility_uuid`),
  INDEX(`facility_uuid`),
  UNIQUE INDEX (`facility_name`, `facility_code`, `facility_resource_type_abbr`)
) ENGINE=InnoDB AUTO_INCREMENT=1 DEFAULT CHARSET=utf8;

INSERT INTO `fms_facility` 
VALUES (1,'Sample Evacuation Center','ec01','EC','Available',100,10,'Setup','EC Group','Evacuation Center','Active','','','165 Cadman Plaza East','','Brooklyn','NY','11201',NULL,NULL,'-73.989315000000','40.699381000000',CURRENT_TIMESTAMP,CURRENT_TIMESTAMP),
(2,'Sample Hurricane Shelter 1','hs01','HS','Available',100,10,'Setup','HS Group','Hurricane Shelter','Active','','','800 Poly Place','','Brooklyn','NY','11228',NULL,NULL,'-74.024484000000','40.609734000000',CURRENT_TIMESTAMP,CURRENT_TIMESTAMP),
(3,'Sample Special Medical Needs Shelter','smn01','SMN','Available',300,10,'Setup','SMN Group','Special Medical Needs Shelter','Active','','','110-00 Rockaway Blvd','','Queens','NY','11420',NULL,NULL,'-73.829799000000','40.677091000000',CURRENT_TIMESTAMP,CURRENT_TIMESTAMP),
(4,'Sample Hurricane Shelter 2','hs02','HS','Available',1500,15,'Standby','HS Group','Hurricane Shelter','Active','',NULL,'110-00 Rockaway Blvd',NULL,'Queens','NY','11420',NULL,NULL,'-73.829799000000','40.677091000000',CURRENT_TIMESTAMP,CURRENT_TIMESTAMP),
(5,'Sample POD','pod1','POD','Available',5000,15,'Standby','POD Group','Point of Dispensing','Active','',NULL,'110-00 Rockaway Blvd',NULL,'Queens','NY','11420',NULL,NULL,'-73.829799000000','40.677091000000',CURRENT_TIMESTAMP,CURRENT_TIMESTAMP);

CREATE TABLE `fms_facility_to_event` (
  `facility_uuid` int(11) NOT NULL,
  `incident_id` bigint(20) NOT NULL,
  `created_at` timestamp NULL DEFAULT '0000-00-00 00:00:00',
  `updated_at` timestamp NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  KEY `fk_fms_facility_to_event_1` (`facility_uuid`),
  KEY `fk_fms_facility_to_event_2` (`incident_id`),
  CONSTRAINT `fk_fms_facility_to_event_1` FOREIGN KEY (`facility_uuid`) REFERENCES `fms_facility` (`facility_uuid`) ON DELETE NO ACTION ON UPDATE CASCADE,
  CONSTRAINT `fk_fms_facility_to_event_2` FOREIGN KEY (`incident_id`) REFERENCES `incident` (`incident_id`) ON DELETE NO ACTION ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

INSERT INTO fms_facility_to_event (facility_uuid,incident_id,created_at) VALUES (1,1,CURRENT_TIMESTAMP);
INSERT INTO fms_facility_to_event (facility_uuid,incident_id,created_at) VALUES (2,1,CURRENT_TIMESTAMP);
INSERT INTO fms_facility_to_event (facility_uuid,incident_id,created_at) VALUES (3,1,CURRENT_TIMESTAMP);
INSERT INTO fms_facility_to_event (facility_uuid,incident_id,created_at) VALUES (4,1,CURRENT_TIMESTAMP);
INSERT INTO fms_facility_to_event (facility_uuid,incident_id,created_at) VALUES (5,1,CURRENT_TIMESTAMP);


CREATE TABLE `fms_person_to_transfer` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `user` varchar(60) NOT NULL,
  `p_uuid` varchar(128) NOT NULL,
  `src_facility` int(11) NOT NULL,
  `dest_facility` int(11) NOT NULL,
  `created_at` timestamp NULL DEFAULT '0000-00-00 00:00:00',
  `updated_at` timestamp NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`),
  KEY `fk_fms_person_to_transfer_1` (`p_uuid`),
  KEY `fk_fms_person_to_transfer_2` (`user`),
  KEY `fk_fms_person_to_transfer_3` (`src_facility`),
  KEY `fk_fms_person_to_transfer_4` (`dest_facility`),
  CONSTRAINT `fk_fms_person_to_transfer_1` FOREIGN KEY (`p_uuid`) REFERENCES `person_uuid` (`p_uuid`) ON DELETE NO ACTION ON UPDATE CASCADE,
  CONSTRAINT `fk_fms_person_to_transfer_2` FOREIGN KEY (`user`) REFERENCES `person_uuid` (`p_uuid`) ON DELETE NO ACTION ON UPDATE CASCADE,
  CONSTRAINT `fk_fms_person_to_transfer_3` FOREIGN KEY (`src_facility`) REFERENCES `fms_facility` (`facility_uuid`) ON DELETE NO ACTION ON UPDATE CASCADE,
  CONSTRAINT `fk_fms_person_to_transfer_4` FOREIGN KEY (`dest_facility`) REFERENCES `fms_facility` (`facility_uuid`) ON DELETE NO ACTION ON UPDATE CASCADE
) ENGINE=InnoDB AUTO_INCREMENT=1 DEFAULT CHARSET=utf8;


CREATE TABLE `fms_person_to_facility` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `p_uuid` varchar(128) NOT NULL,
  `facility_uuid` int(11) NOT NULL,
  `transfer_id` int(11) DEFAULT NULL,
  `in_date` datetime NOT NULL,
  `out_date` datetime DEFAULT NULL,
  `created_at` timestamp NULL DEFAULT '0000-00-00 00:00:00',
  `updated_at` timestamp NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`),
  KEY `facility_uuid` (`facility_uuid`),
  KEY `p_uuid` (`p_uuid`,`in_date`),
  KEY `fk_fms_person_to_facility_1` (`transfer_id`),
  CONSTRAINT `fk_fms_person_to_facility_1` FOREIGN KEY (`transfer_id`) REFERENCES `fms_person_to_transfer` (`id`) ON DELETE NO ACTION ON UPDATE CASCADE,
  CONSTRAINT `crs_signinout_fk_1` FOREIGN KEY (`p_uuid`) REFERENCES `person_uuid` (`p_uuid`) ON DELETE NO ACTION ON UPDATE CASCADE,
  CONSTRAINT `crs_signinout_fk_2` FOREIGN KEY (`facility_uuid`) REFERENCES `fms_facility` (`facility_uuid`) ON DELETE NO ACTION ON UPDATE CASCADE
) ENGINE=InnoDB AUTO_INCREMENT=1 DEFAULT CHARSET=utf8



