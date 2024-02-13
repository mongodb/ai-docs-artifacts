<?php
require 'vendor/autoload.php';

// Replace this with your MongoDB deployment's connection string.
$uri = "mongodb+srv://<user>:<password>@<cluster-url>?w=majority";

$collection = (new MongoDB\Client($uri))->sample_mflix->movies;

try {
    // Create a unique index on the "theaterId" field in the "movies" collection.
    $result = $collection->createIndex(['theaterId' => 1], ['unique' => true]);
    echo "Index created: " . $result;
} catch (MongoDB\Driver\Exception\Exception $e) {
    echo $e->getMessage(), "\n";
}
?>
