<?php
require 'vendor/autoload.php';

use MongoDB\Client;
use MongoDB\BSON;

$uri = "mongodb+srv://<user>:<password>@<cluster-url>?writeConcern=majority";
$client = new Client($uri);
$collection = $client->sample_mflix->theaters;

function proximity($theaters) {
    $coord = [-73.9667, 40.78];
    $radius = 10000;
    $options = ['maxDistance' => $radius];
    $filter = ['location.geo' => ['$near' => ['$geometry' => ['type' => 'Point','coordinates' => $coord], '$maxDistance' => $radius]]];
    
    $cursor = $theaters->find($filter);  
                
    if ($cursor->count() === 0) {
        echo "No documents found!\n";
    }
    foreach ($cursor as $doc) {
        var_dump($doc);
    }
}

function range($theaters) {
    $polyCoordinates = [
        [-72, 40],
        [-74, 41],
        [-72, 39],
        [-72, 40]
    ];
    
    $filter = ['location.geo' => ['$geoWithin' => ['$geometry' => ['type' => 'Polygon', 'coordinates' => [$polyCoordinates]]]]];
    
    $cursor = $theaters->find($filter);

    if ($cursor->count() === 0) {
        echo "No documents found!\n";
    }
    foreach ($cursor as $doc) {
        var_dump($doc);
    }
}

try {
    proximity($collection);
    range($collection);
} catch (Exception $e) {
    echo $e->getMessage();
}

$client->close();
?>
