#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/json.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/stdx.hpp>
#include <mongocxx/uri.hpp>
#include <mongocxx/instance.hpp>

int main() {
    mongocxx::instance inst{};
    std::string uri = "mongodb+srv://<user>:<password>@<cluster-url>?retryWrites=true&w=majority";
    mongocxx::client conn{mongocxx::uri{uri}};

    auto collection = conn["sample_mflix"]["movies"];

    bsoncxx::builder::stream::document index_builder{};
    index_builder << "theaterId" << 1;
    collection.create_index(index_builder.view(), {}, mongocxx::options::index{}.unique(true));

    return 0;
}
