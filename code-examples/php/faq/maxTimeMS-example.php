<?php
require 'vendor/autoload.php';

// Connect to MongoDB
$client = new MongoDB\Client("mongodb://localhost:27017");

// Choose the database and collection
$collection = $client->demo->beers;

// Create query with sleep command
$filter = ['where' => 'sleep(100) || true'];
$options = ['maxTimeMS' => 50];

// Execute find command
try {
    $result = $collection->find($filter, $options);
} catch (Exception $e) {
    echo 'Caught exception: ',  $e->getMessage(), "\n";
}
?>
