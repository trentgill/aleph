<html>

<head>

 <style type="text/css">
.log {
  white-space: pre
  
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
  echo "<div class=\"log\">";
  echo "<h>".$thislog."</h>";
  echo file_get_contents("logs/".$thislog);
  echo "</div>";
}
	

?>

</body>

</html>