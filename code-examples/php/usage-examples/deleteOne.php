<?php
require_once __DIR__ . "/vendor/autoload.php";

// Replace the uri string with your MongoDB deployment's connection string
$uri = "mongodb://localhost:27017";

$client = new MongoDB\Client($uri);
$collection = $client->sample_mflix->movies;

// Delete the first document in the "movies" collection that matches
// the specified query document 
$filter = ['title' => 'Annie Hall'];
$options = ['limit' => 1];

try {
    $deleteResult = $collection->deleteOne($filter, $options);

    if ($deleteResult->getDeletedCount() === 1) {
        print("Successfully deleted one document.\n");
    } else {
        print("No documents matched the query. Deleted 0 documents.\n");
    }
} catch(MongoDB\Driver\Exception\Exception $e) {
    printf("Error: %s\n", $e->getMessage());
}

?>
