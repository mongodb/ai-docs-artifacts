<?php
require_once __DIR__ . "/vendor/autoload.php";

// Replace the following with values for your environment.
$clusterUrl = "<MongoDB cluster url>";
$clientPEMFile = "<path to the client pem certificate file>";

$authMechanism = "MONGODB-X509";

// Replace the following with your MongoDB deployment's connection string.
$uri =
 "mongodb+srv://$clusterUrl/?authMechanism=$authMechanism&ssl=true&sslPEMKeyFile=$clientPEMFile";

// Create a new MongoDB\Client
$client = new MongoDB\Client($uri);

// Function to connect to the server
function run($client) {
  try {
    // Establish and verify connection
    $client->admin->command(['ping' => 1]);
    echo "Connected successfully to server";
  } catch (Exception $e) {
    echo $e;
  } finally {
    // Ensures that the client will close when you finish/error
    $client = null;
  }
}
run($client);
?>
