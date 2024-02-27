<?php
// Delete multiple documents
require 'vendor/autoload.php'; // Include Composer's autoloader

// Replace the uri string with your MongoDB deployment's connection string
$uri = "mongodb://localhost:27017";

$client = new MongoDB\Client($uri);

try {
    $collection = $client->sample_mflix->movies;

    // Delete all documents that match the specified regular expression in the title field from the "movies" collection
    $regexFilter = new MongoDB\BSON\Regex('Santa', 'i');
    $result = $collection->deleteMany([ 'title' => $regexFilter ]);

    // Print the number of deleted documents
    echo "Deleted " . $result->getDeletedCount() . " documents\n";
} catch (Exception $e) {
    // Print any thrown exceptions
    echo $e->getMessage(), "\n";
}

?>
