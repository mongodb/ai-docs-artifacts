#include <bsoncxx/json.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/uri.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/exception/exception.hpp>
#include <iostream>

int main(int, char**) {
    mongocxx::instance inst{};
    mongocxx::uri uri("mongodb+srv://<MongoDB deployment url>/?authSource=%24external&authMechanism=MONGODB-AWS");
    mongocxx::client conn{uri};

    try {
        auto admin = conn["admin"];
        auto command = bsoncxx::builder::basic::make_document(bsoncxx::builder::basic::kvp("ping", 1));
        auto res = admin.run_command(command.view());
        std::cout << "Connected successfully to server." << std::endl;

    } catch(const mongocxx::exception& e) {
        std::cerr << "An exception occurred: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    
    return EXIT_SUCCESS;
}
