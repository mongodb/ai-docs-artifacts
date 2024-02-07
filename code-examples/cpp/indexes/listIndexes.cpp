// List indexes

#include <mongocxx/client.hpp>
#include <mongocxx/stdx.hpp>
#include <mongocxx/uri.hpp>
#include <bsoncxx/json.hpp>
#include <iostream>

// Replace the placeholders with your credentials
const std::string uri =
  "mongodb+srv://<user>:<password>@<cluster-url>/test?retryWrites=true&w=majority";

auto client = mongocxx::client{mongocxx::uri{uri}};

// Access a collection from a database
auto database = client["<databaseName>"];
auto collection = database["<collectionName>"];

int main() {
  try {
    // start listIndexes example
    // List the indexes on the collection and output them as an array
    auto cursor = collection.list_indexes();
    
    // Print the list of indexes
    std::cout << "Existing indexes:" << std::endl;
    for(auto doc : cursor){
        std::cout << bsoncxx::to_json(doc) << std::endl;
    }
    // end listIndexes example
  } catch (const std::exception& e) {
    // Print any errors
    std::cerr << "Exception caught: " << e.what() << '\n';
  }

  // Close the connection after the operation completes
  client.~client();
  
  return 0;
}
