// The C++ equivalent for querying a MongoDB collection using the MongoDB C++ Driver
#include <bsoncxx/builder/stream/document.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/stdx.hpp>
#include <mongocxx/uri.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/options/find.hpp>

int main(int, char**) {
    mongocxx::instance inst{};
    mongocxx::client conn{mongocxx::uri{}};

    bsoncxx::builder::stream::document document{};

    auto collection = conn["testdb"]["myColl"];

    // Set query parameters
    document << "city" << "New York";

    // Set collation option
    mongocxx::options::find opts{};
    opts.collation(bsoncxx::builder::stream::document{} << "locale" << "de" << bsoncxx::builder::stream::finalize);
    opts.sort(bsoncxx::builder::stream::document{} << "name" << 1 << bsoncxx::builder::stream::finalize);

    // Execute find query
    auto cursor = collection.find(document.view(), opts);

    // No actual result processing is done in this sample.
}
