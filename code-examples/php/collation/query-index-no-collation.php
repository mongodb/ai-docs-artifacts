<?php
/* 
   Retrieve documents that match the "year" value "1980"
   in descending order of the value of the "title" field
   that does not use the collation index.
*/

// Start index no collation

$filter = ['year' => 1980];
$options = ['sort' => ['title' => -1]];

$query = new MongoDB\Driver\Query($filter, $options);

$cursor = $manager->executeQuery('myDB.myColl', $query);

// End index no collation
?>
