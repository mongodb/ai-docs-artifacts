// Subscribe to an event

#include <bsoncxx/json.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/stdx.hpp>
#include <mongocxx/uri.hpp>

using bsoncxx::builder::basic::make_document;
using bsoncxx::builder::basic::kvp;

// Replace the following with your MongoDB deployment's connection string
const std::string uri = "mongodb+srv://<clusterUrl>/?replicaSet=rs&writeConcern=majority";

auto client = mongocxx::client{mongocxx::uri{uri}};

// Replace <event name> with the name of the event you are subscribing to
const std::string eventName = "<event name>";

// Subscribe to a specified event and print a message when the event is received
client[db][col].find_one_and_update(
    make_document(kvp(eventName, 1)), 
    make_document(kvp("$set", make_document(kvp("subscribed", true)))),
    options::find_one_and_update{}.return_document(options::return_document::k_after)
);

void run() {
  try {
    // Establish and verify connection to the "admin" database
    auto admin = client["admin"];
    auto result = admin.run_command(make_document(kvp("ping", 1)));

    std::cout << "Connected successfully" << std::endl;
  } catch (const std::exception &e) {
    std::cerr << "An error occurred: " << e.what() << std::endl;

    // Close the database connection on completion or error
    client.~client();
  }
}
run();
