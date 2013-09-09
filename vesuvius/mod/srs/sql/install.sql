--
-- @name        Staff Registry Service
-- @version     1.0
-- @package     srs
-- @author      Clayton Kramer <clayton.kramer@mail.cuny.edu>
-- @license	    http://www.gnu.org/copyleft/lesser.html GNU Lesser General Public License (LGPL)
-- @about       Developed by the CUNY School of Professional Studies for the New York City Office of Emergency Management
-- @link        https://www.sps.cuny.edu/about
-- @link        http://sahanafoundation.org
--

DROP TABLE IF EXISTS `srs_person_to_vol_status`;
DROP TABLE IF EXISTS `srs_person_to_org`;
DROP TABLE IF EXISTS `srs_person_to_staff_types`;
DROP TABLE IF EXISTS `srs_organizations`;
DROP TABLE IF EXISTS `srs_staff_types`;
DROP TABLE IF EXISTS `srs_vol_status`;

CREATE TABLE `srs_organizations` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `name` varchar(255) NOT NULL,
  `incident_id` bigint(20) NOT NULL,
  `created_at` timestamp NULL DEFAULT '0000-00-00 00:00:00',
  `updated_at` timestamp NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`),
  KEY `index2` (`name`),
  KEY `fk_srs_organizations_1` (`incident_id`),
  CONSTRAINT `fk_srs_organizations_1` FOREIGN KEY (`incident_id`) REFERENCES `incident` (`incident_id`) ON DELETE NO ACTION ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

INSERT INTO `srs_organizations` VALUES (1,'Default Organization',1, CURRENT_TIMESTAMP, CURRENT_TIMESTAMP);


CREATE TABLE `srs_vol_status` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `description` varchar(255) NOT NULL,
  `incident_id` bigint(20) NOT NULL,
  `created_at` timestamp NULL DEFAULT '0000-00-00 00:00:00',
  `updated_at` timestamp NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`),
  KEY `index2` (`description`),
  KEY `fk_srs_vol_status_1` (`incident_id`),
  CONSTRAINT `fk_srs_vol_status_1` FOREIGN KEY (`incident_id`) REFERENCES `incident` (`incident_id`) ON DELETE NO ACTION ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

INSERT INTO `srs_vol_status` VALUES (1,'Agency Assigned',1, CURRENT_TIMESTAMP, CURRENT_TIMESTAMP), (2,'Volunteer',1, CURRENT_TIMESTAMP, CURRENT_TIMESTAMP);

CREATE TABLE `srs_person_to_vol_status` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `p_uuid` varchar(128) NOT NULL,
  `volstatus_id` int(11) NOT NULL,
  `incident_id` bigint(20) NOT NULL,
  `created_at` timestamp NULL DEFAULT '0000-00-00 00:00:00',
  `updated_at` timestamp NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`),
  KEY `index2` (`p_uuid`),
  KEY `fk_srs_person_to_vol_status_1` (`p_uuid`),
  KEY `fk_srs_person_to_vol_status_2` (`incident_id`),
  KEY `fk_srs_person_to_vol_status_3` (`volstatus_id`),
  CONSTRAINT `fk_srs_person_to_vol_status_1` FOREIGN KEY (`p_uuid`) REFERENCES `person_uuid` (`p_uuid`) ON DELETE NO ACTION ON UPDATE CASCADE,
  CONSTRAINT `fk_srs_person_to_vol_status_2` FOREIGN KEY (`incident_id`) REFERENCES `incident` (`incident_id`) ON DELETE NO ACTION ON UPDATE CASCADE,
  CONSTRAINT `fk_srs_person_to_vol_status_3` FOREIGN KEY (`volstatus_id`) REFERENCES `srs_vol_status` (`id`) ON DELETE NO ACTION ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

CREATE TABLE `srs_person_to_org` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `p_uuid` varchar(128) NOT NULL,
  `org_id` int(11) NOT NULL,
  `incident_id` bigint(20) NOT NULL,
  `created_at` timestamp NULL DEFAULT '0000-00-00 00:00:00',
  `updated_at` timestamp NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`),
  KEY `index2` (`p_uuid`),
  KEY `fk_srs_person_to_org_1` (`p_uuid`),
  KEY `fk_srs_person_to_org_2` (`incident_id`),
  KEY `fk_srs_person_to_org_3` (`org_id`),
  CONSTRAINT `fk_srs_person_to_org_1` FOREIGN KEY (`p_uuid`) REFERENCES `person_uuid` (`p_uuid`) ON DELETE NO ACTION ON UPDATE CASCADE,
  CONSTRAINT `fk_srs_person_to_org_2` FOREIGN KEY (`incident_id`) REFERENCES `incident` (`incident_id`) ON DELETE NO ACTION ON UPDATE CASCADE,
  CONSTRAINT `fk_srs_person_to_org_3` FOREIGN KEY (`org_id`) REFERENCES `srs_organizations` (`id`) ON DELETE NO ACTION ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;


CREATE TABLE `srs_staff_types` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `description` varchar(255) NOT NULL,
  `incident_id` bigint(20) NOT NULL,
  `created_at` timestamp NULL DEFAULT '0000-00-00 00:00:00',
  `updated_at` timestamp NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`),
  KEY `index2` (`description`),
  KEY `fk_srs_staff_types_1` (`incident_id`),
  CONSTRAINT `fk_srs_staff_types_1` FOREIGN KEY (`incident_id`) REFERENCES `incident` (`incident_id`) ON DELETE NO ACTION ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;


INSERT INTO `srs_staff_types` VALUES 
(1,'Staff',1, CURRENT_TIMESTAMP, CURRENT_TIMESTAMP), 
(2,'Operator',1, CURRENT_TIMESTAMP, CURRENT_TIMESTAMP), 
(3,'Medical Nurse',1, CURRENT_TIMESTAMP, CURRENT_TIMESTAMP),
(4,'Specialist',1, CURRENT_TIMESTAMP, CURRENT_TIMESTAMP);



CREATE TABLE `srs_person_to_staff_types` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `p_uuid` varchar(128) NOT NULL,
  `stafftype_id` int(11) NOT NULL,
  `incident_id` bigint(20) NOT NULL,
  `created_at` timestamp NULL DEFAULT '0000-00-00 00:00:00',
  `updated_at` timestamp NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`),
  KEY `index2` (`p_uuid`),
  KEY `fk_srs_person_to_staff_types_1` (`p_uuid`),
  KEY `fk_srs_person_to_staff_types_2` (`incident_id`),
  KEY `fk_srs_person_to_staff_types_3` (`stafftype_id`),
  CONSTRAINT `fk_srs_person_to_staff_types_1` FOREIGN KEY (`p_uuid`) REFERENCES `person_uuid` (`p_uuid`) ON DELETE NO ACTION ON UPDATE CASCADE,
  CONSTRAINT `fk_srs_person_to_staff_types_2` FOREIGN KEY (`incident_id`) REFERENCES `incident` (`incident_id`) ON DELETE NO ACTION ON UPDATE CASCADE,
  CONSTRAINT `fk_srs_person_to_staff_types_3` FOREIGN KEY (`stafftype_id`) REFERENCES `srs_staff_types` (`id`) ON DELETE NO ACTION ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;


DROP VIEW IF EXISTS srs_staff_search;
CREATE VIEW srs_staff_search
AS
SELECT
  p.p_uuid,
  p.given_name,
  p.family_name,
  p.full_name,
  p.incident_id,
  pd.birth_date,
  pd.years_old,
  pd.opt_gender,
  ps.opt_status,
  ps.last_updated,
  ps.creation_time,
  c1.contact_value AS home_phone,
  c4.contact_value AS email,
  c5.contact_value AS street_1,
  c6.contact_value AS street_2,
  c7.contact_value AS city,
  c8.contact_value AS state,
  c9.contact_value AS postal,
  o.id AS org_id,
  o.name AS org_name,
  vs.id AS vol_id,
  vs.description AS vol_type,
  st.id as stafftype_id,
  st.description AS staff_type,
  wc1.contact_value AS work_phone,
  wc4.contact_value AS work_email,
  wc5.contact_value AS work_street_1,
  wc6.contact_value AS work_street_2,
  wc7.contact_value AS work_city,
  wc8.contact_value AS work_state,
  wc9.contact_value AS work_postal,
  ptf.in_date,
  CASE WHEN ps.opt_status = 'in' THEN NULL ELSE ptf.out_date END AS out_date,
  CASE WHEN ps.opt_status = 'trn' THEN ptf.dest_facility ELSE NULL END AS dest_facility,
  f.facility_uuid,
  f.facility_name,
  f.facility_code,
  f.facility_group,
  sl1.language AS altlang1,
  sl2.language AS altlang2,
  sl3.language AS altlang3
FROM person_uuid p
  INNER JOIN srs_person_to_org pto ON p.p_uuid = pto.p_uuid
  LEFT JOIN srs_organizations o ON pto.org_id = o.id
  INNER JOIN srs_person_to_vol_status ptvs ON p.p_uuid = ptvs.p_uuid
  LEFT JOIN srs_vol_status vs ON ptvs.volstatus_id = vs.id
  INNER JOIN srs_person_to_staff_types ptst ON p.p_uuid = ptst.p_uuid
  LEFT JOIN srs_staff_types st ON ptst.stafftype_id = st.id
  LEFT JOIN person_details pd ON p.p_uuid = pd.p_uuid
  LEFT JOIN person_status ps ON p.p_uuid = ps.p_uuid
  LEFT JOIN contact c1 ON (c1.p_uuid = p.p_uuid AND c1.opt_contact_type = 'curr')
  LEFT JOIN contact c4 ON (c4.p_uuid = p.p_uuid AND c4.opt_contact_type = 'email')
  LEFT JOIN contact c5 ON (c5.p_uuid = p.p_uuid AND c5.opt_contact_type = 'street_1')
  LEFT JOIN contact c6 ON (c6.p_uuid = p.p_uuid AND c6.opt_contact_type = 'street_2')
  LEFT JOIN contact c7 ON (c7.p_uuid = p.p_uuid AND c7.opt_contact_type = 'city')
  LEFT JOIN contact c8 ON (c8.p_uuid = p.p_uuid AND c8.opt_contact_type = 'state')
  LEFT JOIN contact c9 ON (c9.p_uuid = p.p_uuid AND c9.opt_contact_type = 'postal')
  LEFT JOIN contact wc1 ON (wc1.p_uuid = p.p_uuid AND wc1.opt_contact_type = 'w_curr')
  LEFT JOIN contact wc4 ON (wc4.p_uuid = p.p_uuid AND wc4.opt_contact_type = 'w_email')
  LEFT JOIN contact wc5 ON (wc5.p_uuid = p.p_uuid AND wc5.opt_contact_type = 'w_street_1')
  LEFT JOIN contact wc6 ON (wc6.p_uuid = p.p_uuid AND wc6.opt_contact_type = 'w_street_2')
  LEFT JOIN contact wc7 ON (wc7.p_uuid = p.p_uuid AND wc7.opt_contact_type = 'w_city')
  LEFT JOIN contact wc8 ON (wc8.p_uuid = p.p_uuid AND wc8.opt_contact_type = 'w_state')
  LEFT JOIN contact wc9 ON (wc9.p_uuid = p.p_uuid AND wc9.opt_contact_type = 'w_postal')
  LEFT JOIN crs_latest_ptf ptf ON p.p_uuid = ptf.p_uuid
  LEFT JOIN fms_facility f ON f.facility_uuid = ptf.facility_uuid
  LEFT JOIN crs_person_to_language pl1 ON p.p_uuid = pl1.p_uuid AND pl1.ordinal = 1
  LEFT JOIN crs_language sl1 ON pl1.language = sl1.id
  LEFT JOIN crs_person_to_language pl2 ON p.p_uuid = pl2.p_uuid AND pl2.ordinal = 2
  LEFT JOIN crs_language sl2 ON pl2.language = sl2.id
  LEFT JOIN crs_person_to_language pl3 ON p.p_uuid = pl3.p_uuid AND pl3.ordinal = 3
  LEFT JOIN crs_language sl3 ON pl3.language = sl3.id