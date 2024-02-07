#include <bsoncxx/builder/stream/document.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/options/client.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/uri.hpp>
#include <mongocxx/stdx.hpp>
#include <iostream>
#include <exception>

int main() {
    mongocxx::instance inst{};
    mongocxx::options::client client_options{};

    // Replace the placeholder with your Atlas connection string
    std::string uri = "<connection string>";

    // serverApi options
    mongocxx::options::server_api sapi;
    sapi.version(mongocxx::options::server_api::version::k_version_1);
    sapi.strict(true);
    sapi.deprecation_errors(true);

    client_options.server_api_opts(std::move(sapi));
    mongocxx::client conn{mongocxx::uri{uri}, client_options};

    try {
        // Connect the client to the server
        // This is done automatically in C++ driver.
        // Send a ping to confirm a successful connection
        auto admin = conn["admin"];
        bsoncxx::builder::stream::document ping{};
        ping << "ping" << 1;
        auto reply = admin.run_command(ping.view());
        std::cout << "Pinged your deployment. You successfully connected to MongoDB!" << '\n';
    } catch(const std::exception& e) {
        std::cerr << e.what() << '\n';
        return 1;
    }

    // C++ driver automatically closes connection when client object is destroyed.
    return 0;
}
