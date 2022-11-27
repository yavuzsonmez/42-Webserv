<?php
// The code is inspired by the following discussions and post:
// http://stackoverflow.com/questions/5483851/manually-parse-raw-http-data-with-php/5488449#5488449
// http://www.chlab.ch/blog/archives/webdevelopment/manually-parse-raw-http-data-php

/**
 * Parse raw HTTP request data
 *
 * Pass in $a_data as an array. This is done by reference to avoid copying
 * the data around too much.
 *
 * Any files found in the request will be added by their field name to the
 * $data['files'] array.
 *
 * @param   array  Empty array to fill with data
 * @return  array  Associative array of request data
 */

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

// here's the data
$a_data = array();
parse_raw_http_request($a_data);

// now we can access our data
// create file from data array
if ($a_data['files']['testfile']) {
  $randomFileName = md5(uniqid(rand(), true));
  $file = fopen('uploads/' . $randomFileName . '.png', 'w');
  fwrite($file, $a_data['files']['testfile']);
  fclose($file);
  echo "File created as test.png in cgi-bin!";
} else {
  echo "Sorry, we only accept .png files at the moment!";
}

// check size of a_data
if (count($a_data) > 0)
{
    echo "Data received";
    // redirect user to cms.php
    echo "<script> location.href='/cms/cms.php'; </script>";
}
?>