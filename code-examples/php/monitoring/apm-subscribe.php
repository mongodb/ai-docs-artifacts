<?php

require_once './vendor/autoload.php';

// Replace the following with your MongoDB deployment's connection string
$uri = "mongodb+srv://<clusterUrl>/?replicaSet=rs&writeConcern=majority";

$client = new MongoDB\Client($uri);

// Replace <event name> with the name of the event you are subscribing to
$eventName = "<event name>";

$client->getManager()->addSubscriber(new MongoDB\Driver\Monitoring\CommandSubscriber(
    function(MongoDB\Driver\Monitoring\CommandStartedEvent $event) use ($eventName) {
        if ($eventName === $event->getCommandName()) {
            echo "received " . $eventName . ": " . json_encode($event->getCommand()) . "\n";
        }
    }
));

try {
    // Establish and verify connection to the "admin" database
    $command = new MongoDB\Driver\Command(array( "ping" => 1 ));
    $client->getManager()->executeCommand("admin", $command);
    echo "Connected successfully\n";
} finally {
    // Close the database connection on completion or error
    $client = null;
}

?>
