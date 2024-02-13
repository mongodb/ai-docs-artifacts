<?php
require_once "vendor/autoload.php";
$uri = getenv('MONGDODB_URI');
$client = new MongoDB\Client($uri);

// Begin data insertion
$db = $client->aggregation;
$coll = $db->restaurants;

// Create sample documents
$docs = [
    ['stars' => 3, 'categories' => ["Bakery", "Sandwiches"], 'name' => "Rising Sun Bakery"],
    ['stars' => 4, 'categories' => ["Bakery", "Cafe", "Bar"], 'name' => "Cafe au Late"],
    ['stars' => 5, 'categories' => ["Coffee", "Bakery"], 'name' => "Liz's Coffee Bar"],
    ['stars' => 3, 'categories' => ["Steak", "Seafood"], 'name' => "Oak Steakhouse"],
    ['stars' => 4, 'categories' => ["Bakery", "Dessert"], 'name' => "Petit Cookie"],
];

// Insert documents into the restaurants collection
$result = $coll->insertMany($docs);
// End data insertion
         
// Begin aggregation
// Define an aggregation pipeline with a match stage and a group stage
$pipeline = [
    ['$match' => ['categories' => "Bakery"]],
    ['$group' => ['_id' => '$stars', 'count' => ['$sum' => 1]]],
];

// Execute the aggregation
$aggCursor = $coll->aggregate($pipeline);

// Print the aggregated results
foreach ($aggCursor as $doc) {
    var_dump($doc);
}
// End aggregation
?>
