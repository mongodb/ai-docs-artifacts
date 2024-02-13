<?php

/* 
 Use PHP MongoDB extension
 Note: Replace the placeholders in the connection string with your credentials 
*/
$uri = 'mongodb+srv://<user>:<password>@<cluster-url>?retryWrites=true&w=majority';

/* 
  Create a client with options to specify Stable API Version 1
  Note: MongoDB PHP Driver doesn't directly provide an option to use MongoDB server API version. Instead API version needs to be passed as an option in the connection string.
  In this case 'serverApi=1' is added to the uri to use stable API Version 1
*/
$uri = $uri . '&serverApi=1';

$client = new MongoDB\Client($uri);

?>
