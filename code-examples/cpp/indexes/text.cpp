#include <bsoncxx/builder/stream/document.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/uri.hpp>
#include <mongocxx/stdx.hpp>

int main() {
    mongocxx::instance inst{};
    mongocxx::client conn{mongocxx::uri{"mongodb+srv://<user>:<password>@<cluster-url>?writeConcern=majority"}};

    auto coll = conn["testDB"]["blogPosts"];

    // Create a text index on the "title" and "body" fields
    bsoncxx::builder::stream::document indexBuilder;
    indexBuilder << "title"
                 << "text"
                 << "body"
                 << "text";

    coll.create_index(indexBuilder.view(), {}, [&]() {
        class index weights;
        weights.body = 10;
        weights.title = 3;
        return weights;
    }());

    std::cout << "Index created\n";

    mongocxx::options::find opts{};
    opts.projection(make_document(kvp("_id", 0), kvp("title", 1)));

    // Query for documents where body or title contain "life ahead"
    bsoncxx::stdx::optional<bsoncxx::document::value> maybe_result =
        coll.find_one(document{} << "$text"
                                 << open_document << "$search"
                                 << "life ahead" << close_document << finalize, opts);

    while (maybe_result) {
        std::cout << bsoncxx::to_json(*maybe_result) << "\n";
        maybe_result = cursor->next();
    }

    return 0;
}
