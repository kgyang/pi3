<?php
echo "<h1> Welcome to YK&FLL's Home</h1>\n";

echo "<h2>";
echo date('Y-m-d H:i:s ');
echo "</h2>\n";

echo "<p>";
system('/home/pi/pi3/script/temperature.sh');
system('/home/pi/pi3/script/ch2o.py');
echo "</p>\n";

echo "<h2>History</h2>\n";

$filename = '/home/pi/env_sample_data.txt';
$f = fopen($filename, 'r');
$ln = 0;
if ($f) {
    while( fgets($f) != false) {
        $ln++;
    }
    fseek($f, 0);
    $i = 0;
    while( ($line = fgets($f)) != false) {
        $i++;
        if (($ln - $i) <= 48) {
          echo "<p>";
          echo $line;
          echo "</p>\n";
        }
    }
    fclose($f);
}

?>
