// Subscribe to SDAM event

#include <bsoncxx/json.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/uri.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/pool.hpp>
#include <mongocxx/cursor.hpp>

using bsoncxx::builder::basic::kvp;
using bsoncxx::builder::basic::make_document;

mongocxx::instance inst{};

// Replace the following with your MongoDB deployment's connection string
std::string uri = "mongodb+srv://<clusterUrl>/?replicaSet=rs&writeConcern=majority";

auto pool = mongocxx::pool{mongocxx::uri{uri}};

// Replace <event name> with the name of the event you are subscribing to
std::string eventName = "<event name>";

auto client = pool.acquire();
auto db = (*client)["admin"];

// Subscribe to a specified event and print a message when the event is received
db[<event name>].set_read_callback(
  [](const mongocxx::events::change_event& event) {
    std::cout << "received " << eventName << ": " 
              << bsoncxx::to_json(document.get().view()) << std::endl;
});

try {
  // Establish and verify connection to the database
  auto ping_command = make_document(kvp("ping", 1));
  auto ping_result = db.run_command(ping_command.view());
  std::cout << "Connected successfully" << std::endl;
} catch(const std::exception& e) {
  std::cout << e.what() << std::endl;
} finally {
  // Close the database connection on completion or error
  client->close();
}
