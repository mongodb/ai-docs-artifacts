<?php
require 'vendor/autoload.php';

$client = new MongoDB\Client;
$myDB = $client->myDB;
$myColl = $myDB->myColl;

$options = ['sort' => ['title' => -1], 'collation' => ['locale' => 'en_US', 'strength' => 2]];

$documents = $myColl->find(
    ['year' => 1980],
    $options);
?>
