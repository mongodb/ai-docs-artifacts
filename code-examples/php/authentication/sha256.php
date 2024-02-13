<?php

require_once __DIR__ . "/vendor/autoload.php";

// Replace the following with your own values.
$username = "<username>";
$password = "<password>";
$clusterUrl = "<MongoDB cluster url>";

$authMechanism = "SCRAM-SHA-256";

// Replace the following with your MongoDB deployment's connection string.
$uri = sprintf('mongodb+srv://%s:%s@%s/?authMechanism=%s', 
    urlencode($username), urlencode($password), $clusterUrl, $authMechanism);

// Create a new MongoDB client
$client = new MongoDB\Client($uri);

try {
    // Select the database and run the ping command
    $command = new MongoDB\Driver\Command(['ping' => 1]);
    $client->getManager()->executeCommand('admin', $command);
    
    // If no exception thrown, we connected successfully.
    echo "Connected successfully to server\n";
    
} catch(MongoDB\Driver\Exception\Exception $e) {
    echo $e->getMessage(), "\n";
} finally {
    // Close the connection
    $client = null;
}
?>
