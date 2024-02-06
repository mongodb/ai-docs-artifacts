<?php
require_once(__DIR__ . '/vendor/autoload.php'); //Path to autoload.php file

// Replace this with your MongoDB deployment's connection string.
$uri = "mongodb+srv://<user>:<password>@<cluster-url>?retryWrites=true&w=majority";
$conn = new MongoDB\Client($uri);
try {
    $database = $conn->test;
    $orders = $database->orders;

    // Search for orders by name and within a specific date range
    $findResult = $orders->find([
        'name' => "Lemony Snicket",
        'date' => [
            '$gte' => new MongoDB\BSON\UTCDateTime(strtotime("00:00:00")),
            '$lt' => new MongoDB\BSON\UTCDateTime(strtotime("23:59:59"))
        ]
    ]);
    var_dump(iterator_to_array($findResult));

    // Group orders by status within the last week
    $aggregateResult = $orders->aggregate([
        ['$match' =>
            ['date' =>
                [
                    '$gte' => new MongoDB\BSON\UTCDateTime(strtotime("-7 days")),
                    '$lt' => new MongoDB\BSON\UTCDateTime(strtotime("now"))
                ]
            ]
        ],
        ['$group' =>
            [
                '_id' => '$status',
                'count' => ['$sum' => 1]
            ]
        ]
    ]);
    var_dump($aggregateResult);

    /* NOTE: PHP MongoDB driver does not yet support watch() function. */

    // Insert a new order document
    $insertResult = $orders->insertOne([
        'date' => new MongoDB\BSON\UTCDateTime(strtotime("now")),
        'address' => "667 Dark Avenue, San Francisco, CA, 94110",
        'name' => "Lemony Snicket",
        'items' => [
            [
                'name' => "pizza",
                'toppings' => ["pepperoni", "mushrooms"],
                'notes' => "extra shrooms",
                'price' => 11
            ],
            [
                'name' => "wings",
                'sauce' => "buffalo",
                'notes' => "extra crispy",
                'price' => 10
            ]
        ],
        'status' => "created"
    ]);
    var_dump($insertResult->getInsertedCount()); // should be 1

    // Update an existing order's address
    $updateResult = $orders->updateOne(
        [
            'address' => "667 Dark Avenue, San Francisco, CA, 94110",
            'date' => [
                '$gte' => new MongoDB\BSON\UTCDateTime(strtotime("00:00:00")),
                '$lt' => new MongoDB\BSON\UTCDateTime(strtotime("23:59:59"))
            ]
        ],
        ['$set' => ['address' => "667 Dark Avenue, San Francisco, CA, 94103"]]
    );
    var_dump($updateResult->getModifiedCount()); // should be 1

    // Delete an order document based on specified conditions
    $deleteResult = $orders->deleteOne([
        'address' => "13 Lousy Lane",
        'name' => "Violet Baudelaire",
        'date' => [
            '$gte' => new MongoDB\BSON\UTCDateTime(strtotime("00:00:00")),
            '$lt' => new MongoDB\BSON\UTCDateTime(strtotime("23:59:59"))
        ]
    ]);
    var_dump($deleteResult->getDeletedCount()); // should be 0

} catch (Exception $e) {
    echo $e->getMessage();
}

$conn = null;
?>
