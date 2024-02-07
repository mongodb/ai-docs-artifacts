#include <bsoncxx/builder/stream/document.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/uri.hpp>

int main(int, char**) {
    mongocxx::instance inst{};
    mongocxx::uri uri("mongodb://localhost:27017/test?maxPoolSize=5000");
    mongocxx::client client(uri);
    
    try {
        client.database("test");
    }
    catch (const std::exception &e) {
        std::cout << "An exception occurred: " << e.what() << std::endl;
    }
    
    return 0;
}
