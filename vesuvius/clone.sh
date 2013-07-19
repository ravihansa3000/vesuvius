#!clones the existing Vesuvius branch into new instances
echo "enter shortname"
read shortname
 mkdir /var/www/${shortname}
 #copy recursively into newInstances1

 cp -R  /var/www/project_vesu /var/www/${shortname}
 #creates new db and new user and grants the privileges
 echo "enter name of database"
 read dbname
 mysql -u root -psneha227 -e "create database ${dbname}"; 
 #MYSQL = `which mysql`
 #EXPECTED_ARGS=3
 #Q1="CREATE USER 'testusertest'@'localhost' IDENTIFIED BY  'password1234';"
 Q2="use ${dbname};"
 Q3="GRANT ALL ON *.* TO 'testusertest'@'localhost' IDENTIFIED BY 'password1234';"
 Q4="FLUSH PRIVILEGES;"
 sql="${Q2}${Q3}${Q4}"
 mysql -u root -psneha227 -e "$sql";
 #dumps the required tables into the current database
 mysqldump -u testusertest -ppassword1234 ${dbname} < /var/www/${shortname}/project_vesu/vesuvius/vesuvius/backups/vesuviusStarterDb_v092.sql;
 mysql -u testusertest -p ${dbname} < /var/www/project_vesu/vesuvius/vesuvius/backups/vesuviusStarterDb_v092.sql;
#copies the configuration file
#cp /var/www/project_vesu/vesuvius/vesuvius/conf/sahana.conf.example /var/www/newInstance1/project_vesu/vesuvius/vesuvius/conf/sahana1.conf
sed -i 's/snehatest/'${dbname}'/' /var/www/${shortname}/project_vesu/vesuvius/vesuvius/conf/sahana.conf
sed -i 's/root/testusertest/' /var/www/${shortname}/project_vesu/vesuvius/vesuvius/conf/sahana.conf
sed -i 's/sneha227/password1234/' /var/www/${shortname}/project_vesu/vesuvius/vesuvius/conf/sahana.conf
#copies the .htaccess file
#cp /var/www/project_vesu/vesuvius/vesuvius/www/htaccess.example /var/www/newInstance1/project_vesu/vesuvius/vesuvius/www/.htaccess

sed -i 's:project_vesu/vesuvius/vesuvius/:'${shortname}'/project_vesu/vesuvius/vesuvius/:' /var/www/${shortname}/project_vesu/vesuvius/vesuvius/www/.htaccess
#creates a symlink
#ln -s /var/www/newInstance1/project_vesu/vesuvius/vesuvius/www /var/www/newInstance1

 mkdir /var/www/${shortname}/project_vesu/vesuvius/vesuvius/www/tmp ;
 mkdir /var/www/${shortname}/project_vesu/vesuvius/vesuvius/www/tmp/pfif_logs ;
 mkdir /var/www/${shortname}/project_vesu/vesuvius/vesuvius/www/tmp/pfif_cache ;
 mkdir /var/www/${shortname}/project_vesu/vesuvius/vesuvius/www/tmp/plus_cache ;
 mkdir /var/www/${shortname}/project_vesu/vesuvius/vesuvius/www/tmp/rap_cache ;
 mkdir /var/www/${shortname}/project_vesu/vesuvius/vesuvius/www/tmp/mpres_cache ;
 #deletes the root user from database
 #p1="use testdb;"
 #p2="DELETE FROM `users` WHERE `users`.`user_id` = 1 ; "
 #del="${p1}${p2}";
 #mysql -u testusertest -ppassword1234 -e "$del";
 

