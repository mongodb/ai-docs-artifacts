<?php
require_once __DIR__ . "/vendor/autoload.php";

$uri = "mongodb://localhost:27017";

$client = new MongoDB\Client($uri);

try {
    $collection = $client->sample_mflix->movies;

    $filter = ['runtime' => ['$lt' => 15]];
    $options = ['sort' => ['title' => 1]];

    $movies = $collection->find($filter, $options);

    foreach ($movies as $movie) {
       print_r($movie);
    }

} catch(Exception $e) {
    echo $e->getMessage();
}
?>
