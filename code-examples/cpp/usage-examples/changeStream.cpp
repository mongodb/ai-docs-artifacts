```cpp
// Libraries to include
#include <bsoncxx/json.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/pipeline.hpp>
#include <mongocxx/change_stream.hpp>

int main() {
    // Initialize MongoDB instance
    mongocxx::instance inst{};
    
    // Replace the uri string with your MongoDB deployment's connection string.
    mongocxx::client client{mongocxx::uri{"<connection string uri>"}};

    // Connect to the "insertDB" database
    auto database = client.database("insertDB");
    
    // Connect to the "haikus" collection
    auto haikus = database.collection("haikus");

    // Create pipeline and set options
    mongocxx::options::change_stream opts{};
    mongocxx::pipeline pipeline{};

    // Open a Change Stream on the "haikus" collection and print changes
    auto change_stream = haikus.watch(pipeline.view(), opts);
    for (auto&& change : change_stream) {
        std::cout << "Received change:\n" << bsoncxx::to_json(change) << "\n";
    }
    
    // No explicit closing of change streams or connections needed as it is handled by the MongoDB C++ Driver

    return 0;
}
```
