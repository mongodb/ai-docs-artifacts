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
    $replacement = new Haiku(
        "Even in Kyoto",
        "Even in Kyoto, Hearing the cuckoo’s cry, I long for Kyoto"
    );

    $filter = ['title' => new MongoDB\BSON\Regex('^Record', 'i')];

    $result = $collection->replaceOne($filter, $replacement);

    print_r($result);

} catch (Exception $e) {
    echo $e->getMessage();
}

?>
