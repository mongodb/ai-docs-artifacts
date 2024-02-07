#include <bsoncxx/json.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/uri.hpp>
#include <mongocxx/database.hpp>
#include <mongocxx/collection.hpp>

int main()
{
    mongocxx::instance inst{};
    mongocxx::uri uri{"mongodb+srv://<user>:<password>@<cluster-url>?retryWrites=true&w=majority"};
    mongocxx::client conn{uri};

    mongocxx::database db = conn["<databaseName>"];
    mongocxx::collection coll = db["<collectionName>"];
    
    try {
        // start createSearchIndex example
        // Create a search index
        auto index1 = bsoncxx::builder::stream::document{} 
            << "name" << "search1" 
            << "definition" 
            << open_document 
                << "mappings" 
                << open_document
                    << "dynamic" << true 
                << close_document
            << close_document 
            << bsoncxx::builder::stream::finalize;
        coll.create_index(index1.view());
        // end createSearchIndex example

        // start listSearchIndexes example
        // List indices
        auto cursor = coll.list_indexes();
        for (auto&& doc : cursor) {
            std::cout << bsoncxx::to_json(doc) << std::endl;
        }
        // end listIndexes example

        // start updateSearchIndex example
        // Update a search index
        auto index2 = bsoncxx::builder::stream::document{} 
            << "name" << "search1" 
            << "mappings" 
            << open_document 
                << "dynamic" << true 
                << "fields" 
                << open_document
                    << "description" 
                    << open_document
                        << "type" << "string" 
                    << close_document
                << close_document
            << close_document 
        << bsoncxx::builder::stream::finalize;
        coll.create_index(index2.view());
        // end updateSearchIndex example

        // start dropSearchIndex example
        // Dropping (deleting) a search index
        coll.drop_index("search1");
        // end dropSearchIndex example

        return 0;
    }
    catch(const mongocxx::exception& e)
    {
        std::cerr << e.what() << std::endl;
        return 1;
    }
}
