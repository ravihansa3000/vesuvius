#!usr/bin/bash

dir=/var/www/${1}
rm -r $dir


mysql -uroot -psneha227 -e "drop database ${2};";
#use demoDb;
#delete from demoDb where demoId=${3}