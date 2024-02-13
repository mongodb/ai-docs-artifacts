<?php
// Including vendor library
require 'vendor/autoload.php';

// Replace the following with your MongoDB deployment's connection string
$uri =
  "mongodb://<user>:<password>@<cluster-url>";

// Creating MongoDB client
$client = new MongoDB\Client($uri);

try {

    // Selecting database
    $database = $client->sample_mflix;
    // Selecting collection
    $movies = $database->movies;

    // Create an ascending index on the "title" field in the "movies" collection.
    $indexedResult = $movies->createIndex(
        ['title' => 1],
        ['writeConcern' => new MongoDB\Driver\WriteConcern(MongoDB\Driver\WriteConcern::MAJORITY)]
    );
    var_dump($indexedResult);

    // Define the query parameters
    $filter = ['title' => 'Batman'];
    $options = [
        'projection' => ['_id' => 0, 'title' => 1],
        'sort' => ['title' => 1],
    ];

    // Execute the query using the defined parameters
    $cursor = $movies->find($filter, $options);

    // Loop over the results and fetching them
    foreach ($cursor as $document) {
        var_dump($document);
    }
} catch(Exception $e) {
    echo $e->getMessage(), "\n";
} finally {
    // TODO: Unfortunately PHP driver does not support a method for closing the connection yet
}

?>
