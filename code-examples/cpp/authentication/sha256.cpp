#include <bsoncxx/builder/stream/document.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/uri.hpp>
#include <iostream>

int main() {
    mongocxx::instance inst{};

    // Replace the following with your MongoDB deployment's connection string.
    auto uri = mongocxx::uri{"mongodb+srv://<username>:<password>@<mongodbsrv>/?authMechanism=SCRAM-SHA-256"};

    mongocxx::client conn{uri};

    try {
        auto admin = conn["admin"];
        bsoncxx::builder::stream::document document{};
        document << "ping" << 1;
        auto response = admin.run_command(document.view());
        std::cout << "Connected successfully to server" << std::endl;
    } catch(const std::exception& e) {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
