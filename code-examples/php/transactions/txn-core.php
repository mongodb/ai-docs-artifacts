<?php
require 'vendor/autoload.php'; // Include the MongoDB library

use MongoDB\Client;
use MongoDB\Driver\Exception\RuntimeException;

function cleanUp($client)
{
  $collectionsToClean = array("customers", "inventory", "orders");
  foreach($collectionsToClean as $collectionName) {
    try {
      $collection = $client->testdb->$collectionName;
      $collection->drop();
    } catch (RuntimeException $e) {}
  }
}

function setup($client){
  try {
    $customerColl = $client->testdb->customers;
    $inventoryColl = $client->testdb->inventory;

    $customerColl->insertOne(['_id' => 98765, 'orders' => []]);

    $inventoryColl->insertMany([
      ['item' => 'sunblock', 'item_id' => 5432, 'qty' => 85],
      ['item' => 'beach towel', 'item_id' => 7865, 'qty' => 41]
    ]);
  } catch (RuntimeException $e) {
    error_log('Unable to insert test data: ' . $e->getMessage());
  }
}

function queryData($client){
  $collectionsToQuery = array("customers", "inventory", "orders");
  foreach($collectionsToQuery as $collectionName) {
    $collection = $client->testdb->$collectionName;
    print_r(json_encode(iterator_to_array($collection->find())));
  }
}

function placeOrder($client, $cart, $payment){
  $session = $client->startSession();
  $transactionOptions = ['session' => $session];

  try {
    $session->startTransaction();

    $ordersCollection = $client->testdb->orders;

    $orderResult = $ordersCollection->insertOne([
      'customer' => $payment['customer'],
      'items' => $cart,
      'total' => $payment['total']
    ], $transactionOptions);

    $inventoryCollection = $client->testdb->inventory;
    
    foreach ($cart as $item) {  
      $inStock = $inventoryCollection->findOneAndUpdate(
        ['item_id' => $item['item_id'], 'qty' => ['$gte' => $item['qty']]],
        ['$inc' => ['qty' => -$item['qty']]],
        $transactionOptions
      );

      if ($inStock === null) {
        throw new Exception('Insufficient quantity or item ID not found.');
      }
    }

    $customersCollection = $client->testdb->customers;

    $customersCollection->updateOne(
      ['_id' => $payment['customer']],
      ['$push' => ['orders' => $orderResult->getInsertedId()]],
      $transactionOptions
    );

    $session->commitTransaction();

    error_log('Transaction successfully committed.');
  } catch (Exception $error) {
    error_log('An error occured in the transaction, performing a data rollback: ' . $error->getMessage());
    $session->abortTransaction();
  } finally {
    $session->endSession();
  }
}

function run(){
  $uri = getenv('MONGODB_URI');
  $client = new Client($uri);

  cleanUp($client);
  setup($client);
  
  $cart = [
    ['item' => 'sunblock', 'item_id' => 5432, 'qty' => 1, 'price' => 5.19],
    ['item' => 'beach towel', 'item_id' => 7865, 'qty' => 2, 'price' => 15.99]
  ];

  $payment = ['customer' => 98765, 'total' => 37.17];

  placeOrder($client, $cart, $payment);

  cleanUp($client);
  
  queryData($client);

  $client = null;
}

run();

?>
