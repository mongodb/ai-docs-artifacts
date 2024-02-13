<?php
/* 
   Delete the first document that contains a value greater
   than 100 in the "a" field when ordered by using the default
   binary collation order.
*/

// Create a MongoDB client
$client = new MongoDB\Client(); 

// Select a database
$db = $client->test; 

// Select a collection
$collection = $db->myColl;

// Find the first document where "a" > 100 and delete it
$result = $collection->findOneAndDelete(['a' => ['$gt'=>100]]); 

?>
