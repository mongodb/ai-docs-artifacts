<?php

require 'vendor/autoload.php';

// Search Index operations

// Replace the following with your MongoDB deployment's connection string
$uri = "mongodb+srv://<user>:<password>@<cluster-url>?writeConcern=majority";

$client = new MongoDB\Client($uri);
$database = $client->selectDatabase("<databaseName>");
$collection = $database->selectCollection("<collectionName>");

// start createSearchIndex example
// Create a search index
$index1 = [
    'name' => 'search1',
    'definition' => [
        'mappings' => [
            'dynamic' => true,
        ],
    ],
];

$collection->createIndex($index1);
// end createSearchIndex example

// start listSearchIndexes example
// List search indexes
$indexes = $collection->listIndexes();
echo "Existing search indexes:\n";
foreach ($indexes as $idx) {
    var_dump($idx);
}
// end listSearchIndexes example

// start updateSearchIndex example
// Update a search index
$index2 = [
    'mappings' => [
        'dynamic' => true,
        'fields' => [
            'description' => [
                'type' => 'string'
            ],
        ],
    ]
];

$collection->dropIndex('search1');
$collection->createIndex($index2, ['name' => 'search1']);
// end updateSearchIndex example

// start dropSearchIndex example
// Dropping (deleting) a search index
$collection->dropIndex('search1');
// end dropSearchIndex example
