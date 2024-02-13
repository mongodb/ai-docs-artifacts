<?php

require 'vendor/autoload.php'; // include Composer's autoloader

// Create the MongoDB client
$client = new MongoDB\Client("mongodb+srv://<user>:<password>@<cluster-url>?retryWrites=true&w=majority");

// Select the database and collection
$collection = $client->testDB->blogPosts;

// Create a text index on the "title" and "body" fields
$weights = array("body" => 10, "title" => 3);
$options = array("default_language" => "english", "weights" => $weights);

$collection->createIndex(
    array(
        'title' => 'text',
        'body' => 'text'
    ),
    $options
);

// Query for documents where body or title contain "life ahead"
$filter = array('$text' => array( '$search' => "life ahead" ));

// Only show the title field
$projection = array('_id' => 0, 'title' => 1);

$options = array('projection' => $projection);

$cursor = $collection->find($filter, $options);

// Log each document title containing the phrase "life ahead"
foreach ($cursor as $document) {
    echo $document['title'], "\n";
}

?>
