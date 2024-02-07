```cpp
#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/json.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/stdx.hpp>
#include <mongocxx/uri.hpp>
#include <iostream>
#include <random>

using bsoncxx::builder::stream::document;
using bsoncxx::builder::stream::finalize;

int main() {
    const auto uri = mongocxx::uri{"<connection string uri>"};
    auto client = mongocxx::client{uri};

    auto db = client["sample_mflix"];
    auto collection = db["movies"];

    std::random_device rd;
    std::default_random_engine generator(rd());
    std::uniform_int_distribution<int> distribution(1, 1000);

    auto filter = document{} << "title" << bsoncxx::builder::stream::open_document
                              << "$regex"
                              << "The Cat from"
                              << bsoncxx::builder::stream::close_document << finalize;

    auto update = document{} << "title"
                              << ("The Cat from Sector " + std::to_string(distribution(generator)))
                              << finalize;

    auto result = collection.replace_one(filter.view(), update.view());

    if(result) {
        std::cout << "Modified " << result->modified_count() << " document(s)\n";
    }

    return 0;
}
```
