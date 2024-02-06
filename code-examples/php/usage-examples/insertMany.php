<?php

require_once __DIR__ . "/vendor/autoload.php";

// Replace the uri string with your MongoDB deployment's connection string.
$uri = "<connection string uri>";

$client = new MongoDB\Client($uri);

class Food {
    public $name;
    public $healthy;

    public function __construct($name, $healthy) {
        $this->name = $name;
        $this->healthy = $healthy;
    }
}

try {
    $database = $client->insertDB;
    $foods = $database->foods;

    $result = $foods->insertMany(
        [
            new Food("cake", false),
            new Food("lettuce", true),
            new Food("donut", false)
        ]
    );

    echo $result->getInsertedCount() . " documents were inserted" . PHP_EOL;
} catch (Exception $e) {
    echo $e->getMessage();
} finally {
    $client = null;
}

?>
