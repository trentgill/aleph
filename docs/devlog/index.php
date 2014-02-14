<html>

<head>

<style type="text/css">

   .date {
 font-weight:bold;
 font-family:Consolas, Monaco, Courier, monospace;
 background:#aaa;
 width:666px;
 padding:10px;
 margin:10px;
 margin-bottom:0px;
 }
   
   .log {
     font-family:"Helvetica Neue", "HelveticaNeueLT Std", Helvetica, Arial, sans-serif;
     white-space: pre-wrap;
 width:666px;
 background:#ddd;
 padding:10px;
 margin:10px;
 margin-top:0px;
    }
   </style>

   </head>

   <body>

 <?php


   $logdir = opendir('logs');
while (false !== ($log = readdir($logdir))) {
  if (strlen($log) > 2) {
    $logs[] = $log;
  }
}

sort($logs);

foreach ($logs as $thislog) {
  echo "<div class=\"date\">";
  echo "<h>".$thislog."</h>";
  echo "</div>";
  echo "<div class=\"log\">";
  echo file_get_contents("logs/".$thislog);
  echo "</div>";

}
	

?>

</body>

</html>