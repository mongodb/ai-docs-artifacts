#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/json.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/stdx.hpp>
#include <mongocxx/uri.hpp>
#include <mongocxx/instance.hpp>

class MongoDBTransaction {

public:
    MongoDBTransaction() {
        // Replace the following string with your MongoDB deployment's connection string.
        std::string uri = "mongodb+srv://<user>:<password>@<cluster-url>?writeConcern=majority";
        client = mongocxx::client{mongocxx::uri{uri}};
    }

    void printData() {
        auto myDB = client["test"];
        auto myColl = myDB["testColl"];
        auto cursor = myColl.find({});
        for(auto document : cursor){
            std::cout << bsoncxx::to_json(document) << std::endl;
        }
    }

    void runFirstArrayElement() {
        auto myDB = client["test"];
        auto myColl = myDB["testColl"];

        printData();

        bsoncxx::builder::stream::document query{};
        query << "entries.x" << bsoncxx::builder::stream::open_document
              << "$type" << "string"
              << bsoncxx::builder::stream::close_document;

        bsoncxx::builder::stream::document updateDocument{};
        updateDocument << "$inc"
                       << bsoncxx::builder::stream::open_document
                       << "entries.$.y" << 33
                       << bsoncxx::builder::stream::close_document;

        auto result = myColl.update_one(query.view(), updateDocument.view());
        printData();
    }

    void runAllArrayElements() {
        auto myDB = client["test"];
        auto myColl = myDB["testColl"];

        printData();

        bsoncxx::builder::stream::document query{};
        query << "date" << "5/15/2023";

        bsoncxx::builder::stream::document updateDocument{};
        updateDocument << "$unset"
                       << bsoncxx::builder::stream::open_document
                       << "calls.$[].duration" << ""
                       << bsoncxx::builder::stream::close_document;

        auto result = myColl.update_one(query.view(), updateDocument.view());
        std::cout << "Modified Count: " << result->modified_count() << std::endl;

        printData();
    }

    void arrayFiltersIdentifier() {

        auto myDB = client["test"];
        auto myColl = myDB["testColl"];

        printData();

        bsoncxx::builder::stream::document query{};
        query << "date" << "11/12/2023";

        bsoncxx::builder::stream::document updateDocument{};
        updateDocument << "$mul"
                       << bsoncxx::builder::stream::open_document
                       << "items.$[i].quantity" << 2
                       << bsoncxx::builder::stream::close_document;

        mongocxx::options::update options{};
        options.array_filters(document{} << "i.recipe" << "Fried rice"
                                         << "i.item"
                                         << open_document << "$not"
                                         << open_document << "$regex" << "oil"
                                         << close_document
                                         << close_document << finalize);

        auto result = myColl.update_one(query.view(), updateDocument.view(), options);
        std::cout << "Modified Count: " << result->modified_count() << std::endl;

        printData();
    }

private:
    mongocxx::instance instance{};
    mongocxx::client client;

};
