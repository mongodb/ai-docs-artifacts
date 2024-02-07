#include <bsoncxx/builder/stream/document.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/options/client.hpp>
#include <mongocxx/uri.hpp>
#include <iostream>

using bsoncxx::builder::stream::document;
using mongocxx::options::client;
using mongocxx::options::ssl;

int main() {
    mongocxx::instance inst{};
    // Replace the following with values for your environment.
    std::string accessKeyId = "<AWS_ACCESS_KEY_ID>";
    std::string secretAccessKey = "<AWS_SECRET_ACCESS_KEY>";
    std::string clusterUrl = "<MongoDB cluster url>";
    std::string authMechanism = "MONGODB-AWS";
    
    std::string uri =
        "mongodb+srv://" + accessKeyId + ":" + secretAccessKey + "@" + clusterUrl + "/?authSource=%24external&authMechanism=" + authMechanism;

    // Uncomment the following lines if your AWS authentication setup requires a session token.
    // std::string sessionToken = "<AWS_SESSION_TOKEN>";
    // uri = uri + "&authMechanismProperties=AWS_SESSION_TOKEN:" + sessionToken;

    mongocxx::uri uri_to_connect(uri);
    client options_client;
    options_client.uri(uri_to_connect);
    mongocxx::client link{uri_to_connect};

    try {
        // Establish and verify connection.
        auto admin = link["admin"];
        document ping{};
        ping << "ping" << 1;
        admin.run_command(ping.view());
        std::cout << "Connected successfully to server." << std::endl;
    }
    catch(const std::exception& e) {
        std::cerr << e.what() << '\n';
    }
    return 0;
}
