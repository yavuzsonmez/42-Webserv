<link href="https://cdn.jsdelivr.net/npm/bootstrap@5.2.3/dist/css/bootstrap.min.css" rel="stylesheet" integrity="sha384-rbsA2VBKQhggwzxH7pPCaAqO46MgnOM80zW1RWuH61DGLwZJEdK2Kadq2F9CUG65" crossorigin="anonymous">
<script src="https://cdn.jsdelivr.net/npm/bootstrap@5.2.3/dist/js/bootstrap.bundle.min.js" integrity="sha384-kenU1KFdBIe4zVF0s0G1M5b4hcpxyD9F7jL+jjXkk+Q2h455rYXK/7HAuoJl+0I4" crossorigin="anonymous"></script>

<header class="d-flex justify-content-center py-3">
      <ul class="nav nav-pills">
        <li class="nav-item"><a href="/cms/cms.php" class="nav-link active" aria-current="page">Home</a></li>
        <li class="nav-item"><a href="/cms/about.php" class="nav-link">About</a></li>
      </ul>
</header>

<div class="bg-dark text-secondary px-4 py-5 text-center">
    <div class="py-5">
      <h1 class="display-5 fw-bold text-white">Your newsfeed</h1>
      <div class="col-lg-6 mx-auto">
        <p class="fs-5 mb-4">Test around as you want. Use the button below to upload new images!</p>
        <div class="d-grid gap-2 d-sm-flex justify-content-sm-center">
		<form class="d-grid gap-2 d-sm-flex justify-content-sm-center" action="upload_image.php" method="post"
			enctype="multipart/form-data">
			<label for="file">Filename (only png files!):</label>
			<input type="file" name="file" id="file" /> 
			<br />
			<input class="btn btn-outline-light btn-lg px-4" type="submit" name="submit" value="Submit" />
		</form>
        </div>
      </div>
    </div>
  </div>

<?php

$dirname = "uploads/";
$images = glob($dirname."*.{jpg,jpeg,png,gif}", GLOB_BRACE);
// remove everything after the last dot
foreach($images as $image) {
	// display the image

	echo '<div class="bg-dark text-secondary px-4 py-5 text-center">';
	echo '<div class="py-5">';
	echo '<h1 class="display-5 fw-bold text-white"></h1>';
	echo '<div class="col-lg-6 mx-auto">';
	echo '<p class="fs-5 mb-4">'.$image.'</p>';
	echo '<div class="d-grid gap-2 d-sm-flex justify-content-sm-center">';
	echo '<img width="50%" src="'.$image.'" /><br />';
	// here we built the delete button
	echo '<form method="post"><input style="display:none"type="text" name="file" id="file" value="'.$image.'"/> <button type="submit" formaction="/cms/delete_image.php" class="btn btn-outline-info btn-lg px-4 me-sm-3 fw-bold">Delete picture</button></form>';
	echo '</div>';
	echo '</div>';
	echo '</div>';
	echo '</div>';
}

?>