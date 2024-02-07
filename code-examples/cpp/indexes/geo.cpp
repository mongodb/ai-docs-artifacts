#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/json.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/stdx.hpp>
#include <mongocxx/uri.hpp>
#include <mongocxx/exception/exception.hpp>
#include <iostream>

// Replace the placeholders with your MongoDB deployment's credentials
const auto uri = mongocxx::uri{"mongodb+srv://<user>:<password>@<cluster-url>?writeConcern=majority"};

auto client = mongocxx::client{uri};

try {
  auto database = client.database("sample_mflix");
  auto movies = database.collection("movies");

  // Create a 2dsphere index on the "location.geo" field in the "movies" collection 
  auto index_specification = bsoncxx::builder::stream::document{}
                             << "location.geo" << "2dsphere" << bsoncxx::builder::stream::finalize;

  auto result = movies.create_index(std::move(index_specification));

  // Print the result of the index creation
  std::cout << "Index created: " << bsoncxx::to_json(result) << std::endl;

} catch (const mongocxx::exception& e) {
  std::cout << "An exception occurred: " << e.what() << std::endl;
} finally {
  // Close the connection after the operation completes
  client.close();
}
