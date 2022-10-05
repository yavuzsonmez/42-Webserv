<?php echo("TEST")?>

<!DOCTYPE html>
<html lang="en">
<head>
	<meta charset="UTF-8">
	<title>HELLO</title>
	<?php
		// print all headers with foreach
		echo "<br><b>HEADER LIST</b>" . "<br>";;
		foreach (getallheaders() as $name => $value) {
			echo "$name: $value" . "<br>";
		}
	?>
</head>
<body>
	<h1>Firstname: <?php echo("TEST");?></h1>
	<h1>Lastname: <?php echo $_GET['Lastname'];?></h1>
	<h1>Age: <?php echo $_GET['Age'];?></h1>
	<h1>Home: <?php echo $_GET['Home'];?></h1>
</body>
</html>