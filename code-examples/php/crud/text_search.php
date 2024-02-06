<?php

require_once __DIR__ . "/vendor/autoload.php";

// Replace the following string with your MongoDB deployment's connection string
$uri = "mongodb+srv://<user>:<password>@<cluster-url>?retryWrites=true&w=majority";
$client = new MongoDB\Client($uri);

$db = $client->sample_mflix;
$movies = $db->movies;

function searchWord($movies) {
    // Create a query that searches for the string "trek"
    $filter = ['\$text' => ['$search' => "trek"]];
    $options = ['projection' => ['_id' => 0, 'title' => 1]];
    $cursor = $movies->find($filter, $options);
    if (count($cursor) === 0) {
        echo "No documents found!";
    }
    foreach ($cursor as $document) {
        var_dump($document);
    }
}

function searchPhrase($movies) {
    // Create a query that searches for the phrase "star trek"
    $filter = ['\$text' => ['$search' => "\"star trek\""]];
    $options = ['projection' => ['_id' => 0, 'title' => 1]];
    $cursor = $movies->find($filter, $options);
    if (count($cursor) === 0) {
        echo "No documents found!";
    }
    foreach ($cursor as $document) {
        var_dump($document);
    }
}

function searchNegation($movies) {
    // Create a query that searches for the phrase "star trek" while omitting "into darkness"
    $filter = ['\$text' => ['$search' => "\"star trek\"  -\"into darkness\""]];
    $options = ['projection' => ['_id' => 0, 'title' => 1]];
    $cursor = $movies->find($filter, $options);
    if (count($cursor) === 0) {
        echo "No documents found!";
    }
    foreach ($cursor as $document) {
        var_dump($document);
    }
}

function relevance($movies) {
    $filter = ['\$text' => ['$search' => "\"star trek\"  -\"into darkness\""]];
    $options = ['projection' => ['_id' => 0, 'title' => 1, 'score' => ['$meta' => "textScore"]],
                'sort' => ['score' => ['$meta' => "textScore"]]];
    $cursor = $movies->find($filter, $options);
    if (count($cursor) === 0) {
        echo "No documents found!";
    }
    foreach ($cursor as $document) {
        var_dump($document);
    }
}

try {
    searchWord($movies);
    searchPhrase($movies);
    searchNegation($movies);
    relevance($movies);
} catch (Exception $e) {
    echo $e->getMessage();
}
?>
