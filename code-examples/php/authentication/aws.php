<?php

require 'vendor/autoload.php'; // Include Composer's autoloader

// Replace the following with values for your environment.
$accessKeyId = urlencode("<AWS_ACCESS_KEY_ID>");
$secretAccessKey = urlencode("<AWS_SECRET_ACCESS_KEY>");
$clusterUrl = "<MongoDB cluster url>";

$authMechanism = "MONGODB-AWS";

$uri =
  "mongodb+srv://{$accessKeyId}:{$secretAccessKey}@{$clusterUrl}/?authSource=%24external&authMechanism={$authMechanism}";

// Uncomment the following lines if your AWS authentication setup requires a session token.
// $sessionToken = urlencode("<AWS_SESSION_TOKEN>");
// $uri = $uri . "&authMechanismProperties=AWS_SESSION_TOKEN:{$sessionToken}";

// Create a new MongoClient.
$client = new MongoDB\Client($uri);

try {
    // Establish and verify connection.
    $command = new MongoDB\Driver\Command(['ping' => 1]);
    $client->manager->executeCommand('admin', $command);
    echo "Connected successfully to server.";
} catch (MongoDB\Driver\Exception\Exception $e) {
    echo $e->getMessage(), "\n";
    echo $e->getTraceAsString(), "\n";
} finally {
    // Ensure that the client closes when it finishes/errors.
    $client = null;
}
?>
