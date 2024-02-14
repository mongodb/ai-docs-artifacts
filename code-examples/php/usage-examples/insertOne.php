<?php
require_once __DIR__ . "/vendor/autoload.php";

class Haiku {
  public $title;
  public $content;

  function __construct($title, $content){
    $this->title = $title;
    $this->content = $content;
  }
}

$uri = "mongodb://localhost:27017";
$client = new MongoDB\Client($uri);

$collection = $client->poetry->haiku;

try {
    $newHaiku = new Haiku(
        "Record of a Shriveled Datum", 
        "No bytes, no problem. Just insert a document, in MongoDB"
    );

    $result = $collection->insertOne($newHaiku);

    print("A document was inserted with the _id:" . $result->getInsertedId());
} catch(Exception $e){
    echo $e->getMessage();
}
?>
