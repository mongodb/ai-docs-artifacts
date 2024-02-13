<?php

require_once __DIR__ . "/vendor/autoload.php";

use MongoDB\Driver\Manager;
use MongoDB\Driver\Session;
use MongoDB\Driver\Exception\Exception;

// Begin-core
function coreTest($manager) {
    $session = $manager->startSession();

    try {
        $session->startTransaction();

        $coll = $manager->selectCollection('bank', 'savings_accounts');
        $coll->updateOne([ 'account_id' => "9876"], ['$inc' => ['amount' => -100]], ['session' => $session]);

        $coll = $manager->selectCollection('bank', 'checking_accounts');
        $coll->updateOne([ 'account_id' => "9876"], ['$inc' => ['amount' => 100]], ['session' => $session]);

        // ... perform other operations

        $session->commitTransaction();
        echo "Transaction committed.";

    } catch (Exception $e) {
        echo "An error occurred during the transaction:" . $e->getMessage();
        $session->abortTransaction();

    } finally {
        $session->endSession();
    }
}
// End-core

// Begin-conv
function convTest($manager) {
    $session = $manager->startSession();

    $transactionOptions = [
        'readConcern' => new MongoDB\Driver\ReadConcern(MongoDB\Driver\ReadConcern::MAJORITY),
        'writeConcern' => new MongoDB\Driver\WriteConcern(MongoDB\Driver\WriteConcern::MAJORITY),
    ];

    $callback = function(Session $session) use ($manager) {
        $coll = $manager->selectCollection('bank', 'savings_accounts');
        $coll->updateOne([ 'account_id' => "9876"], ['$inc' => ['amount' => -100]], ['session' => $session]);

        $coll = $manager->selectCollection('bank', 'checking_accounts');
        $coll->updateOne([ 'account_id' => "9876"], ['$inc' => ['amount' => 100]], ['session' => $session]);

        // ... perform other operations      

        return "Transaction committed.";
    };

    echo $manager->executeTransaction($session, $callback, $transactionOptions);

    $session->endSession();
}
// End-conv

function run() {
    $uri =
        "<connection string>";
    $manager = new MongoDB\Driver\Manager($uri);

    try {
        coreTest($manager);
        convTest($manager);

    } finally {
        $manager->close();
    }
}
run();

?>
