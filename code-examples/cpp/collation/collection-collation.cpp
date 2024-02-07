// Include the necessary headers
#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/json.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/uri.hpp>

int main() {
    mongocxx::instance instance{}; // This should be done only once.
    mongocxx::client conn{mongocxx::uri{}};

    bsoncxx::builder::stream::document collation_doc;
    collation_doc << "locale" << "fr_CA";
    
    auto collation = collation_doc.view();
    
    auto db = conn["test"];
    db.create_collection("souvenirs", { {}, { collation } });
    
    return 0;
}
