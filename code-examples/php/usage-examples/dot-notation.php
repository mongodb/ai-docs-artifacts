<?php

class TestType {
    public $field;
    
    function __construct() {
        $this->field = new \stdClass;
    }
}

$manager = new MongoDB\Driver\Manager("<your connection string>");
$bulk = new MongoDB\Driver\BulkWrite(['ordered' => true]);

// No-error update
$testTypeInst = new TestType();
$bulk->update([], ['$set' => ['field.nested' => 'A string']]);
$manager->executeBulkWrite('<your db>.<your collection>', $bulk);
$bulk = new MongoDB\Driver\BulkWrite(['ordered' => true]);

// Error update
$testTypeInst->field->nested = "A string";
$bulk->update([], ['$set' => ['field' => $testTypeInst->field]]);
$manager->executeBulkWrite('<your db>.<your collection>', $bulk);
$bulk = new MongoDB\Driver\BulkWrite(['ordered' => true]);

class User {
    public $email;
}

// No-key find
$query = new MongoDB\Driver\Query(['age' => 'Accepts any type!']);
$cursor = $manager->executeQuery('<your db>.<your collection>', $query);

?>
