<?php
require 'vendor/autoload.php';

// Replace the following with your MongoDB deployment's connection string
$uri = "mongodb+srv://<clusterUrl>/?replicaSet=rs&writeConcern=majority";

$client = new MongoDB\Client($uri);

// Replace <event name> with the name of the event you are subscribing to
$eventName = "<event name>";

// Establish and verify connection
try {
    $ping = $client->admin->command(['ping' => 1]);
    echo "\nConnected successfully!\n";

    // Subscribe to the event
    $client->$eventName->insertOne(['event' => 'received']);

    // Fetch all records
    $allEvents = $client->$eventName->find();

    foreach($allEvents as $event) {
        var_dump($event);
    }

} catch(MongoDB\Driver\Exception\Exception $e) {
    echo $e->getMessage(), "\n";
} finally {
    // PHP MongoDB driver manages connections automatically, no need to close
}
?>
