<!--If no index.php is available, call this file with an absolute path to the directory you want to see the files from.-->
<link rel="stylesheet" href="https://cdn.jsdelivr.net/npm/bulma@0.9.4/css/bulma.min.css">
<section class="hero is-primary">
  <div class="hero-body">
    <p class="title">
      Directory listing
    </p>
    <p class="subtitle">
	<?php
		echo $_SERVER['LOCATION_DL'] . '<br>';
		// replace all & with a /

		if ($_SERVER['LOCATION_DL'] == '') {
			$dirpath = '.';
		} else {
			$dirpath = $_SERVER['LOCATION_DL'];
		}
		function buildPath($file) {
			// take last part of path and add it to current location
			
		}
		$dir = opendir($dirpath);
		echo "<h1>Directory Listing active</h1>";
		while ($file = readdir($dir)) {
		if (is_dir($_SERVER['LOCATION_DL'] . $file)) { // is a directory
			echo "<a href=\"" . buildPath($file) ."\">$file/</a><br>";
		} else { // is a file
			echo "<a href=\"" . buildPath($file) ."\">$file</a><br>";
		}
		}
		closedir($dir);
?>

    </p>
  </div>
</section>