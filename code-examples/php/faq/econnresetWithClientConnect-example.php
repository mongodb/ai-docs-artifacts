<?php

require 'vendor/autoload.php'; 

$uri = 'mongodb://localhost:27017/test?maxPoolSize=5000';

$client = new MongoDB\Client($uri);

try {
    $client->listDatabases();

} catch (MongoDB\Driver\Exception\Exception $e) {
    echo $e->getMessage(), "\n";
}

?>
