<? 
session_start();
if(isset($_POST['shortname']))
{
	$_SESSION['shortname']   = $_POST['shortname'];
$sname=$_SESSION['shortname'];

// do stuff here
$url =  "localhost/'$ur'";// this can be set based on whatever


header('Location: ' . $sname );
}

?>
