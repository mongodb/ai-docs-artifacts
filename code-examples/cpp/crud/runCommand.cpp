#include <bsoncxx/builder/stream/document.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/stdx.hpp>
#include <mongocxx/uri.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/cursor.hpp>

using bsoncxx::builder::stream::document;
using bsoncxx::builder::stream::finalize;
using mongocxx::uri;

// Replace the uri string with your MongoDB deployment's connection string
std::string uri_string = "<connection string uri>";

// Create a new instance and connect to MongoDB
mongocxx::instance inst{};
mongocxx::uri uri(uri_string);
mongocxx::client client(uri);

int main() {
    try {
        // start-runcommand
        // Connect to the "testDB" database
        mongocxx::database db = client["testDB"];

        // Run a command to check metadata consistency within the database
        bsoncxx::document::value command = 
            document{} << "checkMetadataConsistency" << 1 << finalize;

        // Execute the command
        mongocxx::cursor cursor = db.run_command(command.view());

        // Iterate through the cursor's results and print the contents
        for(auto doc : cursor) {
            std::cout << bsoncxx::to_json(doc) << std::endl;
        }
        // end-runcommand
    } catch(const std::exception& e) {
        // Handle error in a simple way: print it to standard error output
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    // End of the program: automatically close the MongoDB client connection
    return EXIT_SUCCESS;
}
