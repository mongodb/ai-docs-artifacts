<?php

require_once __DIR__ . "/vendor/autoload.php";

// Replace the placeholders with your credentials
$uri = 'mongodb+srv://<user>:<password>@<cluster-url>?retryWrites=true&w=majority';

$client = new MongoDB\Client($uri);

try {
    $database = $client->selectDatabase('sample_mflix');
    $movies = $database->selectCollection('movies');

    // Create a multikey index on the "cast" field in the "movies" collection
    $result = $movies->createIndex(['cast' => 1]);

    // Print the result of creating the index
    echo "Index created: $result\n";

    // Query to find movies cast by "Viola Davis"
    $query = ['cast' => 'Viola Davis'];
    $projection = ['_id' => 0, 'cast' => 1, 'title' => 1];
    $options = ['projection' => $projection];

    $cursor = $movies->find($query, $options);

    foreach ($cursor as $document) {
        var_dump($document);
    }

} catch(MongoDB\Driver\Exception\Exception $e) {
    echo $e->getMessage(), "\n";
    echo $e->getFile(), "\n";
    echo $e->getLine(), "\n";
    exit;
} finally {
    $client = null;
}
?>
