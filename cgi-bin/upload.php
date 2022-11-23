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
      
     // you'll have to var_dump $block to understand this and maybe replace \n or \r with a visibile char
      
     // parse uploaded files
     if (strpos($block, 'image/png') !== FALSE)
     {
      echo ("FOUND IMAGE PNG!");
      // find image/png and jump behind it
      $block = substr($block, strpos($block, 'image/png') + 13);
      echo ("BLOCK: START");
      //echo ($block);
       preg_match("/name=\"([^\"]*)\".*stream[\n|\r]+([^\n\r].*)?$/s", $block, $matches);
       $a_data['files']['testfile'] = $block;
     }
     // parse all other fields
     else
     {
       // skip content type
      $block = substr($block, strpos($block, "\r\n\r") + 4);
      echo ("AFTER\n");
      echo ($block);
      // match "name" and optional value in between newline sequences
      preg_match('/name=\"([^\"]*)\"[\n|\r]+([^\n\r].*)?\r$/s', $block, $matches);
      $a_data['notafile'] = $matches[2];
     }
   }
 }

// here's the data
$a_data = array();
parse_raw_http_request($a_data);

// now we can access our data
echo $a_data['files']['testfile'];
// create file from data array
$file = fopen('test.png', 'w');
fwrite($file, $a_data['files']['testfile']);
fclose($file);
print_r($a_data);

// check size of a_data
if (count($a_data) > 0)
{
    echo "Data received";
}
?>