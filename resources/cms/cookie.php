<?php
		// set a cookie
		setcookie("Firstname", "Handsome", time()*5, "/","", 0);
		setcookie("Lastname", "Evaluator", time()*5, "/", "",  0);
		setcookie("Age", "Young", time()+3600, "/", "",  0);
		setcookie("Home", "42", time()+3600, "/", "",  0);
		session_start();
?>
<title>Cookie Display</title>
<style>
	/* Add a background color */
	body {
		background-color: #f2f2f2;
	}
	/* Style the cookie tiles */
	.cookie-tile {
		background-color: #cccccc;
		color: white;
		font-weight: bold;
		padding: 10px;
		margin: 10px;
		display: inline-block;
	}
</style>
<body>
	<h1>HELLO EVALUATOR! COOKIE WAS SET ON YOUR BROWSER!</h1>
	<h2>Also, if you new refresh this page, you will see the cookie values below:</h2>

<script>
	// display all cookies
	var cookies = document.cookie.split(";");
	// remove php session cookie from array
	cookies.pop();
	for (var i = 0; i < cookies.length; i++) {
		var cookie = cookies[i];
		var eqPos = cookie.indexOf("=");
		var name = eqPos > -1 ? cookie.substr(0, eqPos) : cookie;
		var value = cookie.substr(eqPos + 1);
		// create a div for each cookie and add the name and value
		var div = document.createElement("div");
		div.classList.add("cookie-tile");
		div.innerHTML = name + " = " + value;
		// add the div to the body
		document.body.appendChild(div);
	}
</script>

<p>BTW: We also started a session. Your session is <?php echo session_id(); ?></p>
</body>