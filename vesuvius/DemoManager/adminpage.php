<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="en" lang="en">

  <head>
    <meta http-equiv="refresh" content="120" >
    <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
    <title>Demo Admin</title>
    
     <link rel="stylesheet" type="text/css" media="screen" href="main.css" />
     <link rel="stylesheet" type="text/css" media="screen" href="menu1.css" />
     <link rel="stylesheet" type="text/css" media="screen" href="tab.css" />
    
  </head>
<body>
  <div id= 'page'>
  <div id = 'myMenu'>
<ul>
  <li><a href="demoAdmin.php">Delete Instance</a></li>
  <li><a href="testdemo.html">Test Instance</a></li>
  <li><a href="demomanager1.html">Create Instance</a></li>
  <li><a href="adminpage.php">Admin Home</a></li>
</ul></div>
<div id = 'append'></div>
</div>
</body>
<script src = '//ajax.googleapis.com/ajax/libs/jquery/1.10.1/jquery.min.js'></script>
<script>
$(function(){


    var object = <?php echo json_encode($array) ?>;
    console.log(object);
    for (var i = 0 ; i < object.length ; i++){

        $('#append').append("<div class='display'>"+"</div>");

        $('.display').append("<p class = 'shortname'>" + object[i].shortname  + '</p>'+ '<br>');

        $('.display').append("<p class ='username'>"+object[i].username + '</p>' + '<br>');

        $('.display').append("<p class ='dbUser'>"+object[i].dbUser + '</p>'+ '<br>');
        $('.display').append("<form action='demoAdmin.php' method = 'post'>" + "<input type = 'submit' value = 'delete'/>" + '</form>');
       

    }
  })
</script>





   



<div class="clear"></div>

  </html>



<?

include('connection.php');
$query="SELECT * FROM demoDb";
$result=mysql_query($query);
$num=mysql_num_rows($result);
mysql_close();

$i=1;
echo "<div id ='page'>";
echo "<div class='CSSTableGenerator' >";
echo "<table><tr><td>Shortname</td><td>Username</td><td>Time Left</td></tr>";
   while($row = mysql_fetch_array($result, MYSQL_ASSOC))
{
    $temp=$row['expDate'];
         $date = new DateTime("$temp");
          $now = new DateTime();
    echo "<tr><td>" . $row['shortname'] . "</td><td>" . $row['username'] . "</td><td>" . $date->diff($now)->format("%d days, %h hours and %i minutes") . "</td></tr>";
        

}
echo "</table>";
echo "</div>";
echo "</div>";


?>

                
                
                