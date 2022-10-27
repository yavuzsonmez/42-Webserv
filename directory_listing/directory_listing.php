<!--If no index.php is available, call this file with an absolute path to the directory you want to see the files from.-->
<link rel="stylesheet" href="https://cdn.jsdelivr.net/npm/bulma@0.9.4/css/bulma.min.css">
<section class="hero is-primary">
  <div class="hero-body">
    <p class="title">
      Directory listing
    </p>
    <p class="subtitle">
	<?php
		$line = trim(fgets(fopen("php://stdin","r")));
		$array = explode("&", $line);
		$dirpath = $array[0];
		$location = $array[1];
		function buildPath($file, $location) {
			return $location . '/' . $file;
		}
		$dir = opendir($dirpath);
		while ($file = readdir($dir)) {
		if (is_dir($file)) { // is a directory
			echo "<a href=\"" . buildPath($file, $location) ."\">$file/</a><br>";
		} else { // is a file
			echo "<a href=\"" . buildPath($file, $location) ."\">$file</a><br>";
		}
		}
		closedir($dir);
?>

    </p>
  </div>
</section>