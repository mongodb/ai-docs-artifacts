<?php
require 'vendor/autoload.php';

// Replace placeholders with deployment credentials
$mongoUser = "<user>";
$mongoPwd = "<password>";
$clusterUrl = "<cluster-url>";

// Create MongoDB client
$uri = "mongodb+srv://$mongoUser:$mongoPwd@$clusterUrl";
$client = new MongoDB\Client($uri);

try {
    // Access database and collection
    $database = $client->sample_mflix;
    $movies = $database->movies;

    /* Create a 2dsphere index on the "location.geo" field in the
    "movies" collection */
    $result = $movies->createIndex(["location.geo" => "2dsphere"]);

    // Print the result of the index creation
    echo "Index created: " . $result;

} catch (Exception $e) {
    // Print any thrown errors
    echo 'Caught exception: ',  $e->getMessage(), "\n";

} finally {
    // Close the connection
    $manager = $client->getManager();
    $manager->executeCommand($database, new MongoDB\Driver\Command(['killCursors' => 'movies']));
}
