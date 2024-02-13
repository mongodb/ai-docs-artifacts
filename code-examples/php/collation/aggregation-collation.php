<?php

// Create a new Manager instance
$manager = new MongoDB\Driver\Manager("mongodb://localhost:27017");

// Specify the aggregation pipeline
$pipeline = [
    ['$group' => ['_id' => '$first_name', 'nameCount' => ['$sum' => 1]]],
    ['$sort' => ['_id' => 1]],
];

// Specify the options
$options = [
    'collation' => ['locale' => 'de@collation=phonebook'],
];

// Create a new command
$command = new MongoDB\Driver\Command([
    'aggregate' => 'myColl',
    'pipeline' => $pipeline,
    'cursor' => new stdClass,
]);

// Execute the command
$cursor = $manager->executeCommand('db', $command, $options);

// Retrieve the results
foreach ($cursor as $document) {
    var_dump($document);
}

?>
