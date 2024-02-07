#include <bsoncxx/builder/stream/document.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/uri.hpp>

int main() {
    mongocxx::instance inst{};
    mongocxx::client client{mongocxx::uri{"mongodb+srv://<user>:<password>@<cluster-url>?writeConcern=majority"}};

    auto collection = client["sample_mflix"]["movies"];

    // Create a multikey index on the "cast" field in the "movies" collection
    bsoncxx::builder::stream::document index_builder;
    index_builder << "cast" << 1;
    collection.create_index(index_builder.view());

    // Perform a find operation where `cast` includes "Viola Davis"
    mongocxx::options::find opts{};
    opts.projection(bsoncxx::builder::stream::document{} << "_id" << 0 << "cast" << 1 << "title" << 1 << bsoncxx::builder::stream::finalize);
    bsoncxx::builder::stream::document filter_builder;
    filter_builder << "cast" << "Viola Davis";
    auto cursor = collection.find(filter_builder.view(), opts);

    client.close();

    return 0;
}
