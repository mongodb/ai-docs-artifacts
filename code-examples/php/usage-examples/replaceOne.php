<?php
/* Connect to MongoDB */
$manager = new MongoDB\Driver\Manager("<connection string uri>");

$filter = ['title' => new MongoDB\BSON\Regex('^The Cat from', 'i')];
$newTitle = ['title' => 'The Cat from Sector ' . rand(1, 1000)];
  
$bulk = new MongoDB\Driver\BulkWrite;
$bulk->update($filter, ['$set' => $newTitle], ['multi' => false, 'upsert' => false]);

/* Execute BulkWrite */
$result = $manager->executeBulkWrite('sample_mflix.movies', $bulk);

echo "Modified " . $result->getModifiedCount() . " document(s)\n";

?>
