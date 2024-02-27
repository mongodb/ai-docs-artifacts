<?php

require 'vendor/autoload.php'; 

// Replace the uri string with your MongoDB deployment's connection string.
$uri = "mongodb://localhost:27017";

$collection = (new MongoDB\Client($uri))->sample_mflix->movies;

try {
    $result = $collection->bulkWrite([
        [ 'insertOne' => [[
                'location' => [
                    'address' => [
                        'street1' => '3 Main St.',
                        'city' => 'Anchorage',
                        'state' => 'AK',
                        'zipcode' => '99501',
                    ]
                ]
            ]]
        ],
        [ 'insertOne' => [[
                'location' => [
                    'address' => [
                        'street1' => '75 Penn Plaza',
                        'city' => 'New York',
                        'state' => 'NY',
                        'zipcode' => '10001',
                    ]
                ]
            ]]
        ],
        [ 'deleteOne' => [[ 'location.address.street1' => '221b Baker St' ]]]
      ],
    );

    print_r("Inserted: " . $result->getInsertedCount() . "\n");
    print_r("Removed: " .  $result->getDeletedCount() . "\n");
    //var_dump($result);

} catch (MongoDB\Driver\Exception\Exception $e) {
    echo $e->getMessage(), "\n";
    echo $e->getFile(), "\n";
    echo $e->getLine(), "\n";
}

?>
