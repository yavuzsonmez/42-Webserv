<?php 
function parse_raw_http_request(array &$a_data)
{
  // read incoming data
  $input = file_get_contents('php://input');
 
  // grab multipart boundary from content type header
  preg_match('/boundary=(.*)$/', $_SERVER['CONTENT_TYPE'], $matches);
   
  // content type is probably regular form-encoded
  if (!count($matches))
  {
	// we expect regular puts to containt a query string containing data
	parse_str(urldecode($input), $a_data);
	return $a_data;
  }
   
  $boundary = $matches[1];
 
  // split content by boundary and get rid of last -- element
  $a_blocks = preg_split("/-+$boundary/", $input);
  array_pop($a_blocks);
	   
  // loop data blocks
  foreach ($a_blocks as $id => $block)
  {
	if (empty($block))
	  continue;
	// parse uploaded files
	if (strpos($block, 'image/png') !== FALSE)
	{
	   // find image/png and jump to the magic byte. it is 13 bytes behind the end of the string "image/png", so we do a + 13.
	   $block = substr($block, strpos($block, 'image/png') + 13);
	   $a_data['files']['testfile'] = $block; // we set the block 
	}
  }
}

$array = array();
parse_raw_http_request($array);
print_r($array);

// now delete the path from the array
$filename = $array['file'];
// delete the file
unlink($filename);

// redirect to the cms.php
echo "<script> location.href='/cms/cms.php'; </script>";

?>