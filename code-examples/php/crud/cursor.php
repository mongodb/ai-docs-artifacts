<?php
require_once __DIR__ . "/vendor/autoload.php";

// replace this with your MongoDB deployment's connection string
$uri = "mongodb+srv://<user>:<password>@<cluster-url>?retryWrites=true&w=majority";

$client = new MongoDB\Client($uri);

// Asynchronous iteration using cursor methods
function asyncIteration() {
    $collection = $client->test->orders;
    $cursor = $collection->find();
    foreach ($cursor as $document) {
        print_r($document);
    }
}
// Manual iteration using cursor methods
function manualIteration() {
    $collection = $client->test->orders;
    $cursor = $collection->find();
    foreach ($cursor as $document) {
        print_r($document);
    }
}
// Fetch all documents using iterator_to_array method
function fetchAll() {
    $collection = $client->test->orders;
    $cursor = $collection->find();
    $allValues = iterator_to_array($cursor);
    echo count($allValues);
}
// Rewind the cursor
function rewindCursor() {
    $collection = $client->test->orders;
    $cursor = $collection->find();
    $firstResult = iterator_to_array($cursor);
    echo "First count: " . count($firstResult);
    $cursor->rewind();
    $secondResult = iterator_to_array($cursor);
    echo "Second count: " . count($secondResult);
}
// Close a cursor
function closeCursor() {
    $collection = $client->test->orders;
    $cursor = $collection->find();
    $cursor->reset();
}

try {
    asyncIteration();
    manualIteration();
    fetchAll();
    rewindCursor();
    closeCursor();
} finally {
    $client = null;
}
?>
