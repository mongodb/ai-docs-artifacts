<?php
require 'vendor/autoload.php';

// Replace the uri string with your MongoDB deployment's connection string
$uri = "<connection string uri>";
$client = new MongoDB\Client($uri);

try {
    $database = $client->sample_mflix;
    $movies = $database->movies;

    // Print the estimate of the number of documents in the "movies" collection
    $estimate = $movies->count();
    echo "Estimated number of documents in the movies collection: " . $estimate . "\n";

    // Print the number of documents in the "movies" collection that match the specified query
    $query = ['countries' => 'Canada'];
    $countCanada = $movies->count($query);
    echo "Number of movies from Canada: " . $countCanada . "\n";
        
} catch (Exception $e) {
    // Print any thrown exceptions
    echo $e->getMessage();
} finally {
    // Close the connection after the operations complete
    $client = null;
}
?>
