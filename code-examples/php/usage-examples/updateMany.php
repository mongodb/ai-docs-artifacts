<?php
require 'vendor/autoload.php';



abstract class Rating {
    const G = "G";
    const PG = "PG";
    const PG_13 = "PG-13";
    const R = "R";
    const NR = "NOT RATED";
}

// Replace the uri string with your MongoDB deployment's connection string.
$uri = "mongodb://localhost:27017";

$client = new MongoDB\Client($uri);

// Get the "movies" collection in the "sample_mflix" database
$collection = $client->sample_mflix->movies;

try {
    // Update all documents that match the specified filter
    $result = $collection->updateMany(
        ['rated' => Rating::G],
        ['$set' => ['random_review' => "After viewing I am ".(rand(0,100))."% more satisfied with life."]]
    );

    print_r("Updated ".$result->getModifiedCount()." documents\n");

} catch (Exception $e) {
    echo $e->getMessage(), "\n";
}

?>
