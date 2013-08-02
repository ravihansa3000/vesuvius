<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="en" lang="en">
  <head>
    <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
    <title>Demo Admin</title>
    
     <link rel="stylesheet" type="text/css" media="screen" href="main.css" />
     <link rel="stylesheet" type="text/css" media="screen" href="menu1.css" />
  </head>
  
  <div id= 'page'>
  <div id = 'myMenu'>
  <ul>
  <li><a href="demoAdmin.php">Delete Instance</a></li>
  <li><a href="testdemo.html">Test Instance</a></li>
  <li><a href="demomanager1.html">Create Instance</a></li>
  <li><a href="adminpage.php">Admin Home</a></li>
  </ul></div>
  <br/><br/>
  
  

    <form name="Admin" action="del.php"  method="post">
    	<table width="274" border="0" align="center" cellpadding="2" cellspacing="0">
    <tr>
    <td colspan="2">
    <div align="center">
        
    
    </tr>
    <tr>
    <td><div align="center">Shortname:</div></td>
    <td><input type="text" name="shortname" /></td>
    </tr>
     <tr>
    <td><div align="center">Password:</div></td>
    <td><input type="password" name="pswrd" /></td>
    </tr>
 <tr>
    <td><div align="center"></div></td>
    <td><input name="submit" type="submit" value="Submit" /></td>
    </tr>
</table>
</form>
</div>


  </html>