<?php

require 'vendor/autoload.php';

$username = urlencode("<username>");
$password = urlencode("<password>");
$clusterUrl = "<MongoDB cluster url>";

$uri = "mongodb+srv://{$username}:{$password}@{$clusterUrl}?authSource=admin"; 

// Create a new MongoClient
$client = new MongoDB\Client($uri);

// Function to connect to the server
function run() 
{
    global $client;

    try {
        // Establish and verify connection
        $command = new MongoDB\Driver\Command(['ping' => 1]);
        $client->getManager()->executeCommand('admin', $command);

        echo "Connected successfully to server.\n";
    } catch (Exception $e) {
        echo $e->getMessage(), "\n";
        exit;
    } finally {
        // Ensures that the client will close when you finish/error
        $client = null;
    }
}

run();
