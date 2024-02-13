<?php

require 'vendor/autoload.php';

$client = new MongoDB\Client;

$collection = $client->myDb->myColl;

$options = ['collation' => ['locale' => 'de']];

$cursor = $collection->find(['city' => 'New York'], $options);

$cursor = $cursor->sort(['name' => 1]);

foreach ($cursor as $document) {
    var_dump($document);
}
