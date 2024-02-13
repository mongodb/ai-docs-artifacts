<?php

require 'vendor/autoload.php';

// Replace the following with values for your environment.
$username = "<username>";
$password = "<password>";
$clusterUrl = "<MongoDB cluster url>";

$authMechanism = "SCRAM-SHA-1";

// Replace the following with your MongoDB deployment's connection string.
$uri = sprintf('mongodb://%s:%s@%s/?authMechanism=%s', 
    $username, 
    $password, 
    $clusterUrl, 
    $authMechanism
);

// Create a new MongoDB driver manager instance.
$manager = new MongoDB\Driver\Manager($uri);

function run($manager) {
    try {
        // Establish and verify connection
        $command = new MongoDB\Driver\Command(['ping' => 1]);
        $manager->executeCommand("admin", $command);

        echo "Connected successfully to server";
    } catch (MongoDB\Driver\Exception\Exception $e) {
        echo $e->getMessage(), "\n";
        echo $e->getTraceAsString(), "\n";
    } finally {
        // There's no need to "close" the connection in MongoDB PHP driver
    }
}

run($manager);
?>
