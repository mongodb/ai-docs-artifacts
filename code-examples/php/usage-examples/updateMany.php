<?php
require 'vendor/autoload.php';

// Replace the uri string with your MongoDB deployment's connection string.
$uri = "<connection string uri>";

$client = new MongoDB\Client($uri);

abstract class Rating {
    const G = "G";
    const PG = "PG";
    const PG_13 = "PG-13";
    const R = "R";
    const NR = "NOT RATED";
}

try {
    // Get the "movies" collection in the "sample_mflix" database
    $db = $client->sample_mflix;

    // Update all documents that match the specified filter
    $result = $db->movies->updateMany(
        ['rated' => Rating::G],
        ['$set' => ['random_review' => "After viewing I am ".(rand(0,100))."% more satisfied with life."]]
    );

    echo "Updated ".$result->getModifiedCount()." documents\n";
} catch (Exception $e) {
    echo $e->getMessage(), "\n";
} finally {
    // Close the database connection on completion or error
    $client->close();
}
?>
