<?
include('connection.php');
if(isset($_POST['username']) && (isset($_POST['pswrd'])))
{
	$uname=$_POST['username'];
	$pass=$_POST['pswrd'];
	$q=mysql_query("select * from admin where user='$uname' and password='$pass'");
	$num=mysql_num_rows($q);
	if($num==1)
	{
		header('location:adminpage.php');
	}
}
?>