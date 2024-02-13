<?php

require 'vendor/autoload.php';  // include composer's autoloader

$collection = (new MongoDB\Client)->test->myColl;

$updateResult = $collection->findOneAndUpdate(
    ['first_name' => ['$lt' => "Gunter"]],
    ['$set' => ['verified' => true]],
    ['collation' => ['locale' => 'de', 'strength' => 2]]
);

?>
