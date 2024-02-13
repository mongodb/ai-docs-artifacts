/*
   Update the "verified" field to "true" for the first document
   that precedes "Gunter" when ordered by using the
   default binary collation order.
*/
// start findOneAndUpdate default order collation
$filter = ['first_name' => ['$lt' => 'Gunter']];
$update = ['$set' => ['verified' => true]];

$options = ['returnDocument' => MongoDB\Operation\FindOneAndUpdate::RETURN_DOCUMENT_AFTER];

$document = $manager->findOneAndUpdate('databaseName.collectionName', $filter, $update, $options);
// end findOneAndUpdate default order collation
