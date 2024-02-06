<?php
// Delete multiple documents
require 'vendor/autoload.php'; // Include Composer's autoloader

// Replace the uri string with your MongoDB deployment's connection string
$uri = "<connection string uri>";

$client = new MongoDB\Client($uri);

try {
    $database = $client->sample_mflix;
    $movies = $database->movies;
    
    // Delete all documents that match the specified regular expression in the title field from the "movies" collection
    $result = $movies->deleteMany(['title' => new MongoDB\BSON\Regex('Santa', 'i')]); 
    
    // Print the number of deleted documents
    echo "Deleted " . $result->getDeletedCount() . " documents\n";
} catch (Exception $e) {
    // Print any thrown exceptions
    echo $e->getMessage(), "\n";
} finally {
    // Close the connection after the operation completes
    $client->close();
}
?>
