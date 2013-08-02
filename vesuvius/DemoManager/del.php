    <?php
   // $global['approot'] = getcwd()."/../../";
    #require("../../conf/sahana.conf");
    #require("./conf.inc");
    
    include('connection.php');
    
    //$id=$_GET['demoId'];
    if(isset($_POST['shortname']) && (isset($_POST['pswrd'])))
    {
        $sname=$_POST['shortname']; 
        //$id = $_POST['demoId'];
        echo "test";
    $q="select dbname from demoDb where shortname = '$sname'";
    $res= mysql_query($q);
    $val=mysql_result($res);
    

    
    mysql_query("DELETE from demoDb where shortname = '$sname'");
    $out =shell_exec("sh /home/sneha/unclone.sh $sname $val");
    
    header('location:adminpage.php');
   }
   
    //$q=mysql_query("select shortname, userid from demoDb where "
    
    ?>