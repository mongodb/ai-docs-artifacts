<?php
require 'vendor/autoload.php'; // Include the MongoDB PHP driver

// Replace the uri string with your MongoDB deployment's connection string
$uri = "<connection string uri>";

// Initialize the MongoDB client
$client = new MongoDB\Client($uri);

// Get the 'insertDB' database and 'haikus' collection
$database = $client->selectDatabase('insertDB');
$haikus = $database->selectCollection('haikus');

// Function to manage the change stream
function watchChanges($haikus){
    // Initialize the change stream
    $changeStream = $haikus->watch();

    try {
        // Iterate over all change events and print them out
        foreach ($changeStream as $change) {
            echo "Received change: \n";
            print_r($change);
        }
    } finally {
        // No matter what, release the resources by closing the change stream and client connection
        $changeStream->close();
        $client->close();
    }
}

// Start watching for changes
watchChanges($haikus);
?>
