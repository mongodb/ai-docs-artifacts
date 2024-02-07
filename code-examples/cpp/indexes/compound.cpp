#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/json.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/stdx.hpp>
#include <mongocxx/uri.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/database.hpp>
#include <mongocxx/cursor.hpp>

int main() {
    mongocxx::instance inst{};
    mongocxx::uri uri("mongodb+srv://<user>:<password>@<cluster-url>/test?retryWrites=true&w=majority");
    mongocxx::client conn{uri};

    auto coll = conn["sample_mflix"]["movies"];
    
    bsoncxx::builder::basic::document index_doc{};
    index_doc.append(kvp("type", 1), kvp("genre", 1));
    coll.create_index(std::move(index_doc.view()));
    
    bsoncxx::builder::stream::document filter_builder, sort_builder, projection_builder;
    filter_builder << "type" << "movie" << "genre" << "Drama";
    sort_builder << "type" << 1 << "genre" << 1;
    projection_builder << "_id" << 0 << "type" << 1 << "genre" << 1;

    mongocxx::cursor cursor = coll.find(filter_builder.view()).sort(sort_builder.view()).projection(projection_builder.view());

    for(auto doc : cursor) {
        std::cout << bsoncxx::to_json(doc) << std::endl;
    }

    return EXIT_SUCCESS;
}
