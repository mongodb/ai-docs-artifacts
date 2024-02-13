<?php

require_once './vendor/autoload.php';

// Replace the following with values for your environment.
$username = urlencode("<username>");
$password = urlencode("<password>");
$clusterUrl = "<MongoDB cluster url>";

// Replace the following with your MongoDB deployment's connection string.
$authMechanism = "SCRAM-SHA-1";
$clientPEMFile = "path_to_pem_file";

$uri =
  "mongodb+srv://{$username}:{$password}@{$clusterUrl}/?authMechanism={$authMechanism}&ssl=true&sslPEMFile={$clientPEMFile}";

// Create a new MongoClient
$client = new MongoDB\Client($uri);

// Function to connect to the server
function run($client) {
  try {
      // Establish and verify connection
      $response = $client->admin->command(['ping' => 1]);
      echo "Connected successfully to server";
  
  } catch (Exception $e) {
      echo $e->getMessage(), "\n";
      exit;
  } finally {
      // Ensures that the client will close when you finish/error
      $client = null;
  }
}

run($client);
?>
