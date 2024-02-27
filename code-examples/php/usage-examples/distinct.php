<?php
require 'vendor/autoload.php'; // Include the MongoDB SDK

use MongoDB\Client as MongoClient; // Import the MongoDB classes required

$uri = 'mongodb://localhost:27017'; // replace the uri string with your MongoDB deployment's connection string.

$collection = (new MongoClient($uri))->sample_mflix->movies;

$filter = ['directors' => 'Barbra Streisand'];

try {
    $result = $collection->distinct('year', $filter);
    print_r($result);

} catch (Exception $e) {
    echo $e->getMessage(), "\n";
}
?>
