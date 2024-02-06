<?php

require 'vendor/autoload.php'; 

// Replace the uri string with your MongoDB deployment's connection string.
$uri = "<connection string uri>";

$client = new MongoDB\Client($uri);
$database = $client->sample_mflix;
$theaters = $database->theaters;

try {
    $result = $theaters->bulkWrite([
        [
            'insertOne' => [
                'document' => [
                    'location' => [
                        'address' => [
                            'street1' => "3 Main St.",
                            'city' => "Anchorage",
                            'state' => "AK",
                            'zipcode' => "99501",
                        ]
                    ],
                ]
            ]
        ],
        [
            'insertOne' => [
                'document' => [
                    'location' => [
                        'address' => [
                            'street1' => "75 Penn Plaza",
                            'city' => "New York",
                            'state' => "NY",
                            'zipcode' => "10001",
                        ]
                    ],
                ]
            ]
        ],
        [
            'updateMany' => [
                'filter' => [ 'location.address.zipcode' => "44011" ],
                'update' => [ '$set' => [ 'is_in_ohio' => true ] ],
                'upsert' => true,
            ]
        ],
        [
            'deleteOne' => [
                'filter' => ['location.address.street1' => "221b Baker St"],
            ],
        ],
    ]);

    var_dump($result);

} catch (MongoDB\Driver\Exception\Exception $e) {
    echo $e->getMessage(), "\n";
    echo $e->getFile(), "\n";
    echo $e->getLine(), "\n";
}

?>
