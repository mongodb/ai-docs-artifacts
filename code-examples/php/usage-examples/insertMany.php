<?php

require_once __DIR__ . "/vendor/autoload.php";

class Food {
    public $name;
    public $healthy;

    public function __construct($name, $healthy) {
        $this->name = $name;
        $this->healthy = $healthy;
    }
}

// Replace the uri string with your MongoDB deployment's connection string.
$uri = "mongodb://localhost:27017";

$client = new MongoDB\Client($uri);

try {
    $collection = $client->shopping->foods;

    $result = $collection->insertMany(
        [
            new Food("cake", false),
            new Food("lettuce", true),
            new Food("donut", false)
        ]
    );

    print($result->getInsertedCount() . " documents were inserted" . PHP_EOL);
} catch (Exception $e) {
    echo $e->getMessage();
} finally {
    $client = null;
}

?>
