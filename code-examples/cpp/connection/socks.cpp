#include <mongocxx/client.hpp>
#include <mongocxx/stdx.hpp>
#include <mongocxx/uri.hpp>
#include <mongocxx/instance.hpp>
#include <bsoncxx/json.hpp>

int main() {
    mongocxx::instance inst{};
    mongocxx::uri uri("<connection string uri>");
    std::string host = "<host>";
    std::uint16_t port = 1080;
    std::string username = "<username>";
    std::string password = "<password>";
    
    uri.default_option("proxy=socks5://" + username + ":" + password + "@" + host + ":" + std::to_string(port));

    mongocxx::client conn{uri};

    bsoncxx::stdx::optional<bsoncxx::document::value> maybe_result =
        conn["myDB"]["myColl"].find_one({});

    if(maybe_result) {
        std::cout << bsoncxx::to_json(*maybe_result) << "\n";
    }

    return 0;
}
