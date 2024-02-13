<?php
require 'vendor/autoload.php';

$uri = '<connection string uri>';
$client = new MongoDB\Client($uri);
$db = $client->testdb;
$inventory = $db->inventory;

// Insert the inventory
$inventory->insertMany([
  ['item' => 'sunblock', 'qty' => 85, 'price' => 6.0],
  ['item' => 'beach chair', 'qty' => 30, 'price' => 25.0]
]);

$orders = $db->orders;

$order1 = [
  ['item' => 'sunblock', 'qty' => 3],
  ['item' => 'beach chair', 'qty' => 1]
];

$order2 = [
  ['item' => 'volleyball', 'qty' => 1]
];

$order = $order1;

$session = $client->startSession();

try {
  $session->startTransaction();

  $total = 0;
  foreach ($order as $item) {
    $itemForUpdate = $inventory->findOne([
      'item' => $item['item'],
      'qty' => ['$gte' => $item['qty']]
    ]);

    if ($itemForUpdate === null) {
      $session->abortTransaction();
      $session->endSession();
      echo "Item not found or insufficient quantity.";
      return;
    }

    $inventory->updateOne(
      ['_id' => new MongoDB\BSON\ObjectId($itemForUpdate['_id'])],
      ['$inc' => ['qty' => -$item['qty']]],
      ['session' => $session]
    );

    $subTotal = $item['qty'] * $itemForUpdate['price'];
    $total += $subTotal;
  }

  $receipt = [
    'date' => new DateTime(),
    'items' => $order,
    'total' => $total,
  ];
  
  $orders->insertOne($receipt, ['session' => $session]);

  echo "Order successfully completed and recorded!\nReceipt:\n";
  print_r($receipt);

  $session->commitTransaction();
} catch (MongoDB\Driver\Exception\Exception $e) {
  $session->abortTransaction();
} finally {
  $session->endSession();
}
?>
