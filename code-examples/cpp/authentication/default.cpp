#include <bsoncxx/builder/stream/document.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/uri.hpp>
#include <mongocxx/stdx.hpp>

int main(int, char**) {

    mongocxx::instance inst{};
    auto username = "<username>";
    auto password = "<password>";
    auto clusterUrl = "<MongoDB cluster url>";

    auto encoded_username = mongocxx::uri::encode_userinfo(username);
    auto encoded_password = mongocxx::uri::encode_userinfo(password);

    mongocxx::uri uri("mongodb+srv://" + encoded_username + ":" + encoded_password + "@" + clusterUrl + "/?authSource=admin");
    mongocxx::client client(uri);

    try {
        bsoncxx::builder::stream::document document{};
        document << "ping" << 1;

        auto adminDB = client["admin"];
        auto result = adminDB.run_command(document.view());

        std::cout << "Connected successfully to server" << std::endl;
    }
    catch(const std::exception& e) {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
