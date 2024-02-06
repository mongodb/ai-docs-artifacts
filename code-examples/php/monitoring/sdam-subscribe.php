<?php

require_once __DIR__ . '/vendor/autoload.php'; 

// Replace the following with your MongoDB deployment's connection string
$uri = "mongodb+srv://<clusterUrl>/?replicaSet=rs&writeConcern=majority";

// Replace <event name> with the name of the event you are subscribing to
$eventName = '<event name>';

// Create client
$client = new MongoDB\Client($uri);

// Subscribe to a specified event and print a message when the event is received
MongoDB\Driver\Monitoring\addSubscriber(new class($eventName) implements MongoDB\Driver\Monitoring\CommandSubscriber {

    private $eventName;
    
    public function __construct($_eventName) {
        $this->eventName = $_eventName;
    }

    public function commandFailed( MongoDB\Driver\Monitoring\CommandFailedEvent $event ) {
        if ($event->getCommandName() == $this->eventName) {
            error_log("received {$this->eventName}: ".json_encode($event->getError(), JSON_PRETTY_PRINT));
        }
    }

    public function commandSucceeded( MongoDB\Driver\Monitoring\CommandSucceededEvent $event ) {
        if ($event->getCommandName() == $this->eventName) {
            error_log("received {$this->eventName}: ".json_encode($event->getReply(), JSON_PRETTY_PRINT));
        }
    }

    public function commandStarted( MongoDB\Driver\Monitoring\CommandStartedEvent $event ) {
        if ($event->getCommandName() == $this->eventName) {
            error_log("received {$this->eventName}: ".json_encode($event->getCommand(), JSON_PRETTY_PRINT));
        }
    }
});

// Establish and verify connection to the database
if ($client->getManager()->executeCommand('admin', new MongoDB\Driver\Command(['ping' => 1]))) {
    error_log("Connected successfully");
    // Close the database connection on completion
    $client = null;
}
?>
