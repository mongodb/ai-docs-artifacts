<?php

require 'vendor/autoload.php';

// Replace the placeholders with your credentials
$uri =
  "mongodb+srv://<user>:<password>@<cluster-url>?writeConcern=majority";

$client = new MongoDB\Client($uri);

// Access a collection from a database
$collection = $client->selectCollection('<databaseName>', '<collectionName>');

try {
    // List the indexes on the collection and output them as an array
    $indexes = $collection->listIndexes();
    
    // Print the list of indexes
    echo "Existing indexes:\n";
    foreach ($indexes as $index) {
        print_r($index->getKey());
    }
} catch (Exception $e) {
    // Print any errors
    echo $e->getMessage(), "\n";
} finally {
    // Close the connection after the operation completes
    $client = null;
}

?>
