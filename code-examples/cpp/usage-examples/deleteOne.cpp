#include <iostream>
#include <mongocxx/client.hpp>
#include <mongocxx/stdx.hpp>
#include <mongocxx/uri.hpp>
#include <mongocxx/instance.hpp>
#include <bsoncxx/json.hpp>
#include <bsoncxx/builder/stream/document.hpp>

int main() {
    mongocxx::instance instance{};
    mongocxx::client client{mongocxx::uri{"<connection string uri>"}};

    auto collection = client["sample_mflix"]["movies"];

    bsoncxx::builder::stream::document document{};
    document << "title" << "Annie Hall";

    auto result = collection.delete_one(document.view());

    if(result) {
        if(result->deleted_count() == 1) {
            std::cout << "Successfully deleted one document." << std::endl;
        } else {
            std::cout << "No documents matched the query. Deleted 0 documents." << std::endl;
        }
    } else {
        std::cout << "Delete operation was not successful." << std::endl;
    }

    return 0;
}
