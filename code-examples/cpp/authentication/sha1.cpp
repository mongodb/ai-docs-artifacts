#include <bsoncxx/builder/stream/document.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/uri.hpp>
#include <iostream>

int main() {
    mongocxx::instance inst{};
    std::string username = "<username>";
    std::string password = "<password>";
    std::string clusterUrl = "<MongoDB cluster url>";
    
    std::string uri = "mongodb+srv://" + username + ":" + password + "@" + clusterUrl + "/?authSource=admin&authMechanism=SCRAM-SHA-1";
    
    try {
        mongocxx::client conn{mongocxx::uri{uri}};
        
        bsoncxx::builder::stream::document document{};
        
        auto ping = bsoncxx::builder::stream::document{} << "ping" << 1 << bsoncxx::builder::stream::finalize;

        auto res = conn["admin"].run_command(ping.view());
        
        if(res) {
            std::cout << "Connected successfully to server" << std::endl;
        }
    } catch (const std::exception& ex) {
        std::cerr << "An error occurred: " << ex.what() << std::endl;
    }
    return 0;
}
