#include <bsoncxx/builder/stream/document.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/stdx.hpp>
#include <mongocxx/uri.hpp>
#include <mongocxx/instance.hpp>

// Replace the following with values for your environment.
const std::string username = mongocxx::stdx::string_view("<username>");
const std::string password = mongocxx::stdx::string_view("<password>");
const std::string clusterUrl = "<MongoDB cluster url>";

// Replace the following with your MongoDB deployment's connection string.
const std::string uri =
  "mongodb+srv://" + username + ":" + password + "@" + clusterUrl + "/?authMechanism=" + authMechanism + "&tls=true&tlsCertificateKeyFile=" + clientPEMFile;

int main() {
    mongocxx::instance inst{};
    mongocxx::client conn{mongocxx::uri{uri}};

    try {
        // Establish and verify connection
        auto admin = conn["admin"];
        bsoncxx::builder::stream::document ping{};
        ping << "ping" << 1;
        admin.run_command(ping.view());
        std::cout << "Connected successfully to server" << std::endl;
    } catch (const std::exception &e) {
        std::cout << "An error occurred: " << e.what() << std::endl;
    }

    // Connection will be closed here when conn object goes out of scope
    return 0;
}
