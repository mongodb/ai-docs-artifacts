<?php

require 'vendor/autoload.php';

// Replace the placeholders with your connection string and SOCKS5 proxy server details
$uri = '<connection string uri>';
$options = [
    'driver' => 'mongodb',
    'host' => '<host>',
    'port' => 1080,
    'username' => '<username>',
    'password' => '<password>',
];

$client = new MongoDB\Client($uri, $options);
$collection = $client->selectCollection('myDB', 'myColl');

try {
    // Retrieve the first document from the MongoDB collection and print it
    $document = $collection->findOne();
    print_r($document);
} catch (Exception $e) {
    echo 'Exception: ', $e->getMessage(), "\n";
} finally {
    // Close the connection after the operation completes
    $client = null;
}
