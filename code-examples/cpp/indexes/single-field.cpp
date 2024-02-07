//Include mongocxx Driver 
#include <bsoncxx/builder/stream/document.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/uri.hpp>
#include <mongocxx/stdx.hpp>

int main() {
    mongocxx::instance instance{}; // Initialize MongoDB Driver
    mongocxx::client client{mongocxx::uri{ "mongodb+srv://<user>:<password>@<cluster-url>?writeConcern=majority" }};
 
    auto collection = client["sample_mflix"]["movies"];

    // Create an ascending index on the "title" field in the
    // "movies" collection.
    bsoncxx::builder::stream::document index_builder{};
    index_builder << "title" << 1;

    // Create the index.
    collection.create_index(index_builder.view()).get();

    // Define the query parameters
    bsoncxx::builder::stream::document query_builder{}, sort_builder{}, projection_builder{};
    query_builder << "title" << "Batman";
    sort_builder << "title" << 1;
    projection_builder << "_id" << 0 << "title" << 1;

    // Execute the query using the defined parameters
    mongocxx::cursor cursor = collection.find(query_builder.view()).sort(sort_builder.view()).project(projection_builder.view());

    return 0;
}
