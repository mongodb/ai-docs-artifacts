<?php
/* 
   Retrieve documents that match the "year" value "1980"
   in descending order of the value of the title field 
   specifying a collation that uses the index.
*/

require 'vendor/autoload.php'; // Include MongoDB library

// Establish a connection to MongoDB
$mongoClient = new MongoDB\Client("mongodb://localhost:27017");

// Select the target collection
$myColl = $mongoClient->selectCollection('db_name', 'myColl');

// Define the search filter
$filter = ['year' => 1980];

// Define the collation
$options = ['collation' => ['locale' => 'en_US']];

// Define the sort order
$options['sort'] = ['title' => -1];

// Execute the query
$cursor = $myColl->find($filter, $options);
?>
