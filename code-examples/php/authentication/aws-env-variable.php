<?php

require 'vendor/autoload.php'; // Include necessary MongoDB PHP Driver libraries

use MongoDB\Client;

// Remember to specify your AWS credentials in environment variables.
$clusterUrl = "<MongoDB deployment url>";
$authMechanism = "MONGODB-AWS";

$uri =
  "mongodb+srv://{$clusterUrl}/?authSource=%24external&authMechanism={$authMechanism}";

// Create a new Client instance.
$client = new Client($uri);

try {
    // Establish and verify connection.
    $client->admin->command(['ping' => 1]);
    echo "Connected successfully to server.";
} catch (Exception $e) {
    // Catch and output any connection errors.
    echo $e->getMessage();
} finally {
    // Ensure that the client is closed when it finishes/errors.
    $client = null;
}
?>
