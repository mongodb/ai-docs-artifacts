<?php
require 'vendor/autoload.php';

use MongoDB\Client;

$uri = 'mongodb+srv://<user>:<password>@<cluster-url>?retryWrites=true&w=majority';
$client = new Client($uri);

function printData()
{
    global $client;

    try {
        $myDB = $client->test;
        $myColl = $myDB->testColl;
        $cursor = $myColl->find();

        echo json_encode(iterator_to_array($cursor));
    } finally {
        $client->__destruct(); 
    }
}

function runFirstArrayElement()
{
    global $client;

    try {
        $myDB = $client->test;
        $myColl = $myDB->testColl;

        $cursor = $myColl->find();
        echo json_encode(iterator_to_array($cursor));
        
        $query = ['entries.x' => ['$type' => 2]];
        $updateDocument = ['$inc' => ['entries.$.y' => 33]];

        $result = $myColl->updateOne($query, $updateDocument);

        $cursor = $myColl->find();
        echo $result->getModifiedCount();
        echo json_encode(iterator_to_array($cursor));
    } finally {
        $client->__destruct(); 
    }
}

function runAllArrayElements()
{
    global $client;

    try {
        $myDB = $client->test;
        $myColl = $myDB->testColl;

        $cursor = $myColl->find();
        echo json_encode(iterator_to_array($cursor));

        $query = ['date' => '5/15/2023'];
        $updateDocument = ['$unset' => ['calls.$[].duration' => ""]];

        $result = $myColl->updateOne($query, $updateDocument);
        
        echo $result->getModifiedCount();

        $cursor = $myColl->find();
        echo json_encode(iterator_to_array($cursor));
    } finally {
        $client->__destruct(); 
    }
}

function arrayFiltersIdentifier()
{
    global $client;

    try {
        $myDB = $client->test;
        $myColl = $myDB->testColl;

        $cursor = $myColl->find();
        echo json_encode(iterator_to_array($cursor));

        $query = ['date' => '11/12/2023'];
        $updateDocument = ['$mul' => ['items.$[i].quantity' => 2]];
        $options = ['arrayFilters' => [['i.recipe' => 'Fried rice', 'i.item' => ['$not' => new MongoDB\BSON\Regex('oil')]]]];

        $result = $myColl->updateOne($query, $updateDocument, $options);

        echo $result->getModifiedCount();

        $cursor = $myColl->find();
        echo json_encode(iterator_to_array($cursor));
    } finally {
        $client->__destruct(); 
    }
}
?>
