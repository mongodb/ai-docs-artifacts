<?php

require 'vendor/autoload.php';

use MongoDB\Client;
use MongoDB\BSON\UTCDateTime;

// Replace the uri string with your MongoDB deployment's connection string.
$uri = "<connection string uri>";

$client = new Client($uri);

class IMDB {
    public $rating;
    public $votes;
    public $id;
}

class Movie {
    public $title;
    public $year;
    public $released;
    public $plot;
    public $type;
    public $imdb;
}

class MovieSummary {
    public $title;
    public $imdb;
}

function run() {
    global $client;
    try {
        $database = $client->sample_mflix;
        $movies = $database->movies;

        $movie = $movies->findOne(
            ['title' => 'The Room'],
            [
                'sort' => ['rating' => -1],
                'projection' => ['_id' => 0, 'title' => 1, 'imdb' => 1],
            ]
        );
        print_r($movie);
    } catch (Exception $e) {
        error_log($e->getMessage());
    } finally {
        $client->close();
    }
}

run();

?>
