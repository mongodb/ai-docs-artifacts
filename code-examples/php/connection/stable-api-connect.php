<?php

require 'vendor/autoload.php';

use MongoDB\Client;
use MongoDB\Driver\Command;
use MongoDB\Driver\Exception\ConnectionException;

// Replace the placeholder with your Atlas connection string
$uri = '<connection string>';

// Create a MongoClient
$client = new MongoDB\Client($uri);

function run($client) {
    try {
        $command = new MongoDB\Driver\Command(['ping' => 1]);

        // Connect the client to the server
        $manager = $client->getManager();

        // Send a ping to confirm a successful connection
        $output = $manager->executeCommand('admin', $command);
        
        echo "Pinged your deployment. You successfully connected to MongoDB!\n";
        
    } catch(ConnectionException $e) {
        echo $e->getMessage();
    } finally {
        // Ensures that the client will close when you finish/error
        unset($client);
    }
}

try {
    run($client);
} catch(Exception $e) {
    echo $e->getMessage(); 
}

?>
