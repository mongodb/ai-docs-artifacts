<?php

require 'vendor/autoload.php'; // include Composer's autoloader

$uri = "<connection string uri>";

$client = new MongoDB\Client($uri);

$collection = $client->insertDB->haikus;

$changeStream = $collection->watch();

$changeStreamIterator = new IteratorIterator($changeStream);

// Create our simulateAsyncPause function
function simulateAsyncPause() {
    sleep(1);
}

foreach ($changeStreamIterator as $document) {
    // Print any change event
    echo 'received a change to the collection:', PHP_EOL;
    print_r($document);

    // Simulate pause before inserting a document
    simulateAsyncPause();


    // Insert a new document into the collection
    // Assuming the $myColl refers to this collection "haikus"
    $collection->insertOne([
        'title' => 'Record of a Shriveled Datum',
        'content' => 'No bytes, no problem. Just insert a document, in MongoDB',
    ]);

    // Simulate pause before closing the change stream
    simulateAsyncPause();

    // Since PHP will automatically clean up resources once they no longer have any references to them,
    // we don't need to manually close the change stream

    // But we can still print a message to the console
    echo 'closed the change stream', PHP_EOL;
    
    // Unable to close MongoDB database connection since MongoDB extension does not support connection closure
}

?>
