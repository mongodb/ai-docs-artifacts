<?php
// Stable API - with options

// begin serverApiVersion
require 'vendor/autoload.php';

// Replace the placeholders in the connection string uri with your credentials
$uri = "mongodb+srv://<user>:<password>@<cluster-url>/?retryWrites=true&w=majority";

// Create options
$options = ['version' => '1', 'strict' => true, 'deprecationErrors' => true];

// Create a client with options to specify Stable API Version 1, return
// errors for commands outside of the API version, and raise exceptions
// for deprecated commands
$client = new MongoDB\Client($uri, [], ['serverApi' => $options]);

// end serverApiVersion

?>
