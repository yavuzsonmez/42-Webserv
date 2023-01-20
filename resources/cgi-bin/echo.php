<?php echo("TEST")?>

<!DOCTYPE html>
<html lang="en">
<link href="https://cdn.jsdelivr.net/npm/bootstrap@5.0.2/dist/css/bootstrap.min.css" rel="stylesheet" integrity="sha384-EVSTQN3/azprG1Anm3QDgpJLIm9Nao0Yz1ztcQTwFspd3yD65VohhpuuCOmLASjC" crossorigin="anonymous">

<div class="card" style="width: 100%;">
  <div class="card-body">
    <h5 class="card-title">Welcome to the PetRoulette Webserver!</h5>
    <h6 class="card-subtitle mb-2 text-muted">Following links may be usefule to you:</h6>
    <div class="card">
		<div class="card-body">
			<a href="/testargs.php?Firstname=Handsome&Lastname=Evaluator&Age=Young&Home=42">Argument test</a>
		</div>
	</div>
	<div class="card">
		<div class="card-body">
			<a href="/info.php">Info PHP</a>
		</div>
	</div>
	<div class="card">
		<div class="card-body">
			<a href="/cms/cms.php">CMS PHP (create upload folder before uploading files please!)</a>
		</div>
	</div>
	<br>
  </div>
</div>

</html>
