<?php

require_once __DIR__ . "/vendor/autoload.php";

// Replace the following with your MongoDB deployment's connection string
$uri = 'mongodb+srv://<user>:<password>@<cluster-url>?w=majority';

// Create a new client and connect to MongoDB
$client = new MongoDB\Client($uri);

try {
    // Connect to the "sample_mflix" database
    $database = $client->sample_mflix;
    
    // Access the database's "movies" collection
    $collection = $database->movies;

    // Create an ascending index on the "type" and "genre" fields in the "movies" collection
    $collection->createIndex(['type' => 1, 'genre' => 1]);

    // Define a query to find movies in the "Drama" genre
    $query = ['type' => 'movie', 'genre' => 'Drama'];

    // Define sorting criteria for the query results
    $options = [
        'sort' => ['type' => 1, 'genre' => 1],
        'projection' => ['_id' => 0, 'type' => 1, 'genre' => 1],
    ];

    // Execute the query using the defined criteria and options
    $cursor = $collection->find($query, $options);

} catch(Exception $e) {
    echo $e->getMessage();
} finally {
    // As of MongoDB driver 1.5.0, a MongoClient object has no close() method. Connections are implicitly cleaned up when the object is destroyed.
}

?>
