<?php
require 'vendor/autoload.php';

// Replace the uri string with your MongoDB deployment's connection string
$uri = "mongodb://localhost:27017/";
$client = new MongoDB\Client($uri);

try {
    $collection = $client->sample_mflix->movies;

    // Print the estimate of the number of documents in the "movies" collection
    $estimate = $collection->count();
    echo "Estimated number of documents in the movies collection: " . $estimate . "\n";

    // Print the number of documents in the "movies" collection that match the specified query
    $query = ['countries' => 'Canada'];
    $countCanada = $collection->count($query);
    echo "Number of movies from Canada: " . $countCanada . "\n";

} catch (Exception $e) {
    // Print any thrown exceptions
    echo $e->getMessage();
}
?>
