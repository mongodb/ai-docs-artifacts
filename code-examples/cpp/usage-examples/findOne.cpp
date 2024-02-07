#include <iostream>
#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/json.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/stdx.hpp>
#include <mongocxx/uri.hpp>
#include <bsoncxx/document/view.hpp>
#include <mongocxx/cursor.hpp>

// Replace the uri string with your MongoDB deployment's connection string.
const mongocxx::uri uri("my.mongodb.uri");

auto client = mongocxx::client(uri);
auto db = client["sample_mflix"];
auto movies = db.collection("movies");

class IMDB {
public:
int rating;
int votes;
int id;
};

class Movie {
public:
std::string title;
int year;
std::string released;
std::string plot;
std::string type;
IMDB imdb;
};

class MovieSummary {
public:
std::string title;
IMDB imdb;
};

int main() {
mongocxx::options::find opts{};
opts.sort(bsoncxx::builder::stream::document{} << "rating" << -1
                                       << bsoncxx::builder::stream::finalize)
    .projection(bsoncxx::builder::stream::document{} << "_id" << 0 << "title" << 1 << "imdb" << 1
                                             << bsoncxx::builder::stream::finalize);

auto movie = movies.find_one(bsoncxx::builder::stream::document{} << "title" << "The Room"
                                                           << bsoncxx::builder::stream::finalize,
                                         opts);
if (movie) {
    std::cout << bsoncxx::to_json(*movie) << "\n";
} else {
    std::cerr << "No document matches the provided query." << "\n";
}

client.close();
return 0;
}
