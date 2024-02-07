#include <bsoncxx/json.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/uri.hpp>
#include <iostream>

int main(int, char**) {
    mongocxx::instance inst{};
    std::string clusterUrl = "<MongoDB cluster url>";
    std::string clientPEMFile = "<path to the client pem certificate file>";
    std::string uri =
      "mongodb+srv://" + clusterUrl + "/?authMechanism=MONGODB-X509&ssl=true&sslCertificateKeyFile=" + clientPEMFile;

    try {
        mongocxx::client conn{mongocxx::uri{uri}};
        auto admin = conn["admin"];
        bsoncxx::document::value command = bsoncxx::from_json("{ ping : 1 }");

        auto result = admin.run_command(command.view());
        std::cout << "Connected successfully to server" << std::endl;
    }
    catch(const std::exception& e) {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
