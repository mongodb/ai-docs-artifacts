<?php
require_once __DIR__ . "/vendor/autoload.php";

// Replace the uri string with your MongoDB deployment's connection string
$uri = "<connection string uri>";

$client = new MongoDB\Client($uri);
$movies = $client->sample_mflix->movies;

// Delete the first document in the "movies" collection that matches
// the specified query document 
$filter = ['title' => 'Annie Hall'];
$options = ['limit' => 1];

try {
    $deleteResult = $movies->deleteOne($filter, $options);
    // Print a message that indicates whether the operation deleted a
    // document 
    if ($deleteResult->getDeletedCount() === 1) {
        printf("Successfully deleted one document.\n");
    } else {
        printf("No documents matched the query. Deleted 0 documents.\n");
    }
} catch(MongoDB\Driver\Exception\Exception $e) {
    printf("Error: %s\n", $e->getMessage());
}

// Close the connection after the operation completes
$client = null;
?>
