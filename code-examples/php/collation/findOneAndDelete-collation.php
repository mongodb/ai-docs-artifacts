<?php
// Create the connection
$manager = new MongoDB\Driver\Manager("mongodb://localhost:27017");

// Specify the filter criteria
$filter = ['a' => ['$gt' => 100]];

// Specify the options
$options = ['collation' => ['locale' => 'en', 'numericOrdering' => true]];

// Create and execute the command
$bulk = new MongoDB\Driver\BulkWrite();
$bulk->delete($filter, $options);

$manager->executeBulkWrite('dbname.myColl', $bulk);
?>
