<h1>Welcome to YK&FLL's Home</h1>

<h2><?php echo date('Y-m-d H:i:s '); ?></h2>
<p><?php system('/home/pi/pi3/script/temperature.sh'); ?></p>

<h2>History</h2>
<?php 
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
      if (($ln - $i) <= 96) {
        echo "<p>";
        echo $line;
        echo "</p>";
      }
    }
    fclose($f);
  }
 ?>
