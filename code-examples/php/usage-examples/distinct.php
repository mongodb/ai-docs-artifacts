<?php
require 'vendor/autoload.php'; // Include the MongoDB SDK

use MongoDB\Client as MongoClient; // Import the MongoDB classes required

$uri = '<connection string uri>'; // replace the uri string with your MongoDB deployment's connection string.

$client = new MongoClient($uri);

$collection = $client->sample_mflix->movies;

$options = ['typeMap' => ['root' => 'array', 'document' => 'array', 'array' => 'array']];

try {
    $filter = ['directors' => 'Barbra Streisand'];
    $cursor = $collection->find($filter, $options);

    $distinctYears = [];

    foreach ($cursor as $document) {
        if (!in_array($document['year'], $distinctYears)) {
            $distinctYears[] = $document['year'];
        }
    }

    print_r($distinctYears);

} catch (Exception $e) {
    echo $e->getMessage(), "\n";
} finally {
    $client = null;
}
?>
