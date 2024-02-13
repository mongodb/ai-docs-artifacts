<?php

require_once __DIR__ . "/vendor/autoload.php";

// Replace the uri string with your MongoDB deployment's connection string
$uri = "<connection string uri>";

// Create a new client and connect to MongoDB
$client = new MongoDB\Client($uri);

try {
    // Connect to the "testDB" database
    $db = $client->testDB;

    // Run a command to check metadata consistency within the database
    $cursor = $db->command(['dbStats' => 1]);

    // Iterate through the cursor's results and print the contents
    foreach ($cursor as $document) {
        print_r($document);
    }
} catch(Exception $e) {
    echo $e->getMessage(), "\n";
} finally {
    // Close the MongoDB client connection
    $client = null;
}
?>
