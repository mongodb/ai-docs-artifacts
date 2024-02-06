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

$uri = "<connection string uri>";
$client = new MongoDB\Client($uri);
$database = $client->insertDB;
$haiku = $database->haiku;

try {
  $newHaiku = new Haiku("Record of a Shriveled Datum", "No bytes, no problem. Just insert a document, in MongoDB");
  $insertResult = $haiku->insertOne($newHaiku);
  echo "A document was inserted with the _id:" . $insertResult->getInsertedId();
} catch(Exception $e){
  echo $e->getMessage();
} finally {
  $client = null;
}
?>
