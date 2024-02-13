<?php
// Connection parameters
$manager = new MongoDB\Driver\Manager("mongodb://localhost:27017");

// French Canadian collation
$collation = ['locale' => 'fr_CA'];

// Create the "souvenirs" collection with the French Canadian collation
$createCommand = new MongoDB\Driver\Command([
    'create' => 'souvenirs',
    'collation' => $collation
]);

// Execute the command
$manager->executeCommand('mydb', $createCommand);
?>
