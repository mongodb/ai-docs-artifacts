<?php
require_once __DIR__ . "/vendor/autoload.php";

$uri = "<connection string uri>";

$client = new MongoDB\Client($uri);

class IMDB {
    public $rating;
    public $votes;
    public $id;

    function __construct($rating, $votes, $id) {
        $this->rating = $rating;
        $this->votes = $votes;
        $this->id = $id;
    }
}

class Movie {
    public $title;
    public $imdb;
    public $runtime;

    function __construct($title, $imdb, $runtime) {
        $this->title = $title;
        $this->imdb = $imdb;
        $this->runtime = $runtime;
    }
}

try {
    $db = $client->sample_mflix;
    $collection = $db->movies;
    
    $options = ['sort' => ['title' => 1]];

    $movies = $collection->find(['runtime' => ['$lt' => 15]], $options);

    if (!iterator_count($movies)) {
       trigger_error("No documents found!", E_USER_WARNING);
    }

    foreach ($movies as $movie) {
       var_dump($movie);
    }

} catch(Exception $e) {
    echo $e->getMessage();
} finally {
    $client = null;
}
?>
