<?php

require 'vendor/autoload.php';

// Replace the uri string with your MongoDB deployment's connection string
$uri = "mongodb://localhost:27017";

try {
    $client = new MongoDB\Client($uri);

    // Print the storage statistics for the "sample_mflix" database by using the 'dbStats' command
    $command = new MongoDB\Driver\Command(["dbStats" => 1]);
    $result = $client->getManager()->executeCommand("sample_mflix", $command);

    var_dump($result->toArray());
} catch(Exception $e) {
    // If an error occurs it will be captured by the catch block and logged to the console
    error_log($e->getMessage());
}

?>
