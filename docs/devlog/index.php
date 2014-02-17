<html>

<head>
<link rel="stylesheet" type="text/css" href="logs.css" />
   </head>

   <body>

   <p>
   this is ezra's development log for monome-aleph.
aleph source code and issues list on <a href="http://github.com/tehn/aleph">github</a>.
</p>

   <?php 


   $logdir = opendir('logs');

while (false !== ($logname = readdir($logdir))) {
  if (strlen($logname) > 2) {
    $logs[] = $logname;
  }
}
sort($logs);
$logs = array_reverse($logs);

// column of links to log entries
echo "<div class=\"loglinkbox\">\n\t";
foreach ($logs as $loglink) {
  echo "<div class=\"loglink\">";
  echo "<a href=\"#".$loglink."\">";
  echo $loglink." ( "; 
   $lines = file("logs/".$loglink);
   //  echo "<div class=\"logpreview\">".$lines[0]."</div>";
   echo trim($lines[0]);
   echo " ) ";
   echo "</a>";
   echo "</div>\n\t";
}
echo "</div>\n\n\n";
echo "<br /><br />\n\n";

// column of log entries
echo "<div class=\"logbox\">";
foreach ($logs as $log) {
  echo "<a name = ".$log.">";
  echo "</a>";
  echo "<div class=\"date\">";
  //  echo "<h>".$log."</h>";
  echo $log;
  echo "</div>";
  echo "<div class=\"log\">";
  echo file_get_contents("logs/".$log);
  echo "</div>\n\n";
}

echo "</div>";

?>
</body>

</html>
