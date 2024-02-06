<?php

require 'vendor/autoload.php';

// Replace the uri string with your MongoDB deployment's connection string
$uri = "<connection string uri>";

$client = new MongoDB\Client($uri);

try {
    // Get the "sample_mflix" database
    $db = $client->selectDatabase("sample_mflix");
    
    // Find and print the storage statistics for the "sample_mflix" database using the 'dbStats' command
    $command = new MongoDB\Driver\Command(["dbStats" => 1]);
    $result = $db->getManager()->executeCommand("sample_mflix", $command);
    
    var_dump($result->toArray());
} catch(Exception $e) {
    // If an error occurs it will be captured by the catch block and logged to the console
    error_log($e->getMessage());
} finally {
    // Close the database connection on completion or error
    //$manager->executeCommand($dbname, new MongoDB\Driver\Command(["logout" => 1])); 
    // PHP driver has no MongoClient::close() equivalent method because connections automatically end when a MongoClient object falls out of scope
}

?>
