```cpp
#include <bsoncxx/builder/stream/document.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/stdx.hpp>
#include <mongocxx/uri.hpp>
#include <mongocxx/instance.hpp>

using bsoncxx::builder::stream::document;
using bsoncxx::builder::stream::finalize;
using bsoncxx::builder::stream::open_document;
using bsoncxx::builder::stream::close_document;

int main() {
    mongocxx::instance inst{};
    mongocxx::client conn{mongocxx::uri{"<connection string uri>"}};

    auto collection = conn["insertDB"]["haiku"]; 

    document newHaiku;

    newHaiku << "title" << "Record of a Shriveled Datum"
             << "content" << "No bytes, no problem. Just insert a document, in MongoDB";

    collection.insert_one(newHaiku.view());

    bsoncxx::stdx::optional<mongocxx::result::insert_one> result = collection.insert_one(newHaiku.view());
  
    if(result) {
        std::cout << "A document was inserted with the _id: " << result->inserted_id().get_oid().value.to_string() << std::endl;
    }

    return 0;
}
```
