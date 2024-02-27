<?php

require 'vendor/autoload.php';


// Replace the uri string with your MongoDB deployment's connection string.
$uri = "mongodb://localhost:27017";

$client = new MongoDB\Client($uri);

try {
    $collection = $client->sample_mflix->movies;
    $filter = ['title' => 'The Room'];
    $options = [
         'sort' => ['rating' => -1],
         'projection' => ['_id' => 0, 'title' => 1, 'imdb' => 1],
    ];


    $movie = $collection->findOne($filter, $options);

    print_r($movie);

} catch (Exception $e) {
    error_log($e->getMessage());
}

?>
