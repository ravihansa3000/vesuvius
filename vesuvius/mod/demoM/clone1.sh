#!/bin/sh 
#clones the existing Vesuvius branch into new instances

sname=${1}
dbname=${2}
pswrd=${3}
host=${4}
dbuser=${5}
salt=${6}
storedPassword=${7}
#echo "enter db user"
#read dbUser
echo $sname
 mkdir /var/www/${sname}
 #mysql -u root -psneha227 -e "create user '$2'@'localhost' indentified by '$3';";
 #copy recursively into newInstancessname

 cp -R  /var/www/project/vesuvius /var/www/${sname}
 #creates new db and new user and grants the privileges
 #echo "enter name of database $dbname"

 mysql -u root -psneha227 -e "create database ${2};"; 
 #MYSQL = `which mysql`
 #EXPECTED_ARGS=3

 Q1="CREATE USER '$5'@'$4' identified by '$3';"
 Q2="use ${dbname};"
 Q3="GRANT ALL ON *.* TO '$5'@'$4' identified by '$3';"
 Q4="FLUSH PRIVILEGES;"
 sql="${Q1}${Q2}${Q3}${Q4}"
 mysql -u root -psneha227 -e "$sql";
 #dumps the required tables into the current database
 mysqldump -u${5} -p${3} ${dbname}  < /var/www/${sname}/vesuvius/backups/vesuviusStarterDb_v092.sql;
 mysql -u${5} -p${3} ${dbname} < /var/www/${sname}/vesuvius/backups/vesuviusStarterDb_v092.sql;
#copies the configuration file
#cp /var/www/project_vesu/vesuvius/vesuvius/conf/sahana.conf.example /var/www/${sname}/vesuvius/conf/sahana.conf
rep1='$conf['db_name'] = ""' 
rep2='$conf['db_host'] = ""'
rep3='$conf['db_user'] = ""'
rep4='$conf['db_pass'] = ""'
r1='$conf['db_name'] = "${dbname}"' 
r2='$conf['db_host'] = "${host}"'
r3='$conf['db_user'] = "${dbuser}"' 
r4='$conf['db_pass'] = "${pswrd}"'
sed -i 's/project/'${dbname}'/' /var/www/${sname}/vesuvius/conf/sahana.conf
sed -i 's/project/'${dbuser}'/' /var/www/${sname}/vesuvius/conf/sahana.conf
sed -i 's/123456/'${pswrd}'/' /var/www/${sname}/vesuvius/conf/sahana.conf
sed -i 's/Sahana Vesuvius/'${sname}'/' /var/www/${sname}/vesuvius/conf/sahana.conf
#copies the .htaccess file
#cp /var/www/${sname}/vesuvius/www/htaccess.example /var/www/${sname}/vesuvius/www/.htaccess

sed -i 's:project/vesuvius/:'${sname}'/vesuvius/:' /var/www/${sname}/vesuvius/www/.htaccess
#creates a symlink
ln -s /var/www/${sname}/vesuvius/www /var/www/${sname}/www

 #mkdir /var/www/${sname}/vesuvius/www/tmp ;
 #mkdir /var/www/${sname}/vesuvius/www/tmp/pfif_logs ;
 #mkdir /var/www/${sname}/vesuvius/www/tmp/pfif_cache ;
 #mkdir /var/www/${sname}/vesuvius/www/tmp/plus_cache ;
 #mkdir /var/www/${sname}/vesuvius/www/tmp/rap_cache ;
 #mkdir /var/www/${sname}/vesuvius/www/tmp/mpres_cache ;
 #deletes the root user from database
 p1="use ${2};"
 p2="DELETE FROM `users` WHERE `user_id` = 1 ; "
 del="${p1}${p2}";
 mysql -u${5} -p${3} -e "$del";

 p3="INSERT INTO `users` (user_id,p_uuid,user_name,password,salt) values (1, 1, ${5}, ${7}, ${6});"
 mysql -u${5} -p${3} -e "$p3";


#sudo chown -R sneha:www-data {$sname}
#sudo chmod -R g+s {$sname}