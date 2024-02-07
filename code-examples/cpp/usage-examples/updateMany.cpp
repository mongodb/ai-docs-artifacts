```C++
#include <bsoncxx/builder/stream/document.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/uri.hpp>
#include <mongocxx/database.hpp>
#include <mongocxx/collection.hpp>
#include <mongocxx/stdx.hpp>

#include <cstdlib>
#include <ctime>

using namespace mongocxx;

enum class Rating {
  G,
  PG,
  PG_13,
  R,
  NR
};

int main(int, char**) {
  
  // Replace the uri string with your MongoDB deployment's connection string
  uri MongoClientUri("<connection string uri>");
  client MongoClient(MongoClientUri);
  
  // Get the "movies" collection in the "sample_mflix" database
  database db = MongoClient["sample_mflix"];
  collection movies = db["movies"];

  // Generate random life satisfaction percentage
  std::srand(std::time(0));
  float satisfaction = ((std::rand() % 100) + 1) / (float)100;
  
  bsoncxx::builder::stream::document updateDoc{};
  updateDoc << "$set" << bsoncxx::builder::stream::open_document << "random_review"
            << ("After viewing I am " + std::to_string(satisfaction) + "% more satisfied with life.")
            << bsoncxx::builder::stream::close_document;
           
  try {
    // Update all documents that match the specified filter
    bsoncxx::builder::stream::document filterDoc{};
    filterDoc << "rating" << bsoncxx::builder::stream::open_document << "$eq"
              << static_cast<int>(Rating::G) << bsoncxx::builder::stream::close_document;

    mongocxx::stdx::optional<mongocxx::result::update> maybe_result =
        movies.update_many(filterDoc.view(), updateDoc.view());
    
    if (maybe_result) {
      std::cout << "Updated " << maybe_result->modified_count() << " documents\n";
    }
  }
  catch (const std::exception& e) {
    std::cout << "An exception occurred: " << e.what() << "\n";
  }
}
```
