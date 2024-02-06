<?php

require 'vendor/autoload.php';

// Replace the uri string with your MongoDB deployment's connection string
$uri = "<connection string uri>";

$client = new MongoDB\Client($uri);

function run() {
    global $client;

    try {
        $database = $client->sample_mflix;
        $movies = $database->movies;

        /* Update a document that has the title "Random Harvest" to have a
        plot field with the specified value */
        $result = $movies->updateOne(
          ['title' => "Random Harvest"],
          [
            '$set' => [
              'plot' => "A harvest of random numbers, such as: " . rand(),
            ],
          ],
          // Set the upsert option to insert a document if no documents
          // match the filter
          ['upsert' => true]
        );

        // Print the number of matched and modified documents
        printf(
          "%d document(s) matched the filter, updated %d document(s)\n",
          $result->getMatchedCount(), $result->getModifiedCount()
        );
        
    } catch (Exception $e) {
        echo $e->getMessage();
    } finally {
        // Close the connection after the operation completes
        $client = null;
    }
}

// Run the program and print any thrown errors
run();

?>
