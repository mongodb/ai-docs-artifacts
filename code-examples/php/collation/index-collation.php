<?php
/*
   Create an index on the "title" field with the "en_US"
   locale collation, specifying ascending ordering of the
   "title" field.
*/
// start create index collation
$manager = new MongoDB\Driver\Manager("mongodb://localhost:27017");
$command = new MongoDB\Driver\Command([
    'createIndexes' => 'myColl',
    'indexes'       => [
        [
            'key' => ['title' => 1 ],
            'collation' => [ 'locale' => 'en_US' ],
            'name' => 'titleIndex'
        ]
    ]
]);
$manager->executeCommand('db', $command);
// end create index collation
?>
