#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/json.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/stdx.hpp>
#include <mongocxx/uri.hpp>

int main() {
    mongocxx::instance inst{};
    std::string uri = "<connection string uri>";
    mongocxx::client conn{mongocxx::uri{uri}};

    auto db = conn["sample_mflix"];
    auto collection = db["movies"];

    bsoncxx::builder::stream::document document{};
    document << "title" << bsoncxx::types::b_regex{"Santa"};

    auto result = collection.delete_many(document.view());
    std::cout << "Deleted " << result->deleted_count() << " documents" << std::endl;

    return 0;
}
