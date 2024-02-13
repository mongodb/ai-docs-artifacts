<?php

// Instantiate MongoDB driver manager
$manager = new MongoDB\Driver\Manager();

/*
    Retrieve documents that match "photograph" in the "type" field,
    sorted by the Iceland collation and uppercase precedence.
*/
// start specified collation
$filter = ['type' => 'photograph'];
$options = ['collation' => ['locale' => 'is', 'caseFirst' => 'upper']];

$query = new MongoDB\Driver\Query($filter, $options);
$cursor = $manager->executeQuery('myColl', $query);
// end specified collation
