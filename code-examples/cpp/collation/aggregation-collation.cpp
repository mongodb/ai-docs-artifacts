#include <bsoncxx/builder/stream/document.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/stdx.hpp>
#include <mongocxx/uri.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/pipeline.hpp>

int main() {
    mongocxx::instance inst{};
    mongocxx::client conn{mongocxx::uri{}};

    auto collection = conn["myDB"]["myColl"];
    mongocxx::pipeline p{};

    // start aggregation collation
    p.group(bsoncxx::builder::stream::document{} << "_id" << "$first_name" << "nameCount"
             << bsoncxx::builder::stream::open_document << "$sum" << 1 << bsoncxx::builder::stream::close_document << bsoncxx::builder::stream::finalize);
    p.sort(bsoncxx::builder::stream::document{} << "_id" << 1 << bsoncxx::builder::stream::finalize);
    
    mongocxx::options::aggregate opts{};
    opts.collation(bsoncxx::builder::stream::document{} << "locale" << "de@collation=phonebook" << bsoncxx::builder::stream::finalize);

    auto cursor = collection.aggregate(p, opts);
    // end aggregation collation

    return 0;
}
