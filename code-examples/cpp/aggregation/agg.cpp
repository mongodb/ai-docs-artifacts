// Perform an aggregation

#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/pipeline.hpp>
#include <bsoncxx/json.hpp>

int main() {
    mongocxx::instance inst{};
    mongocxx::client conn{mongocxx::uri{std::getenv("MONGODB_URI")}};
    
    auto db = conn["aggregation"];
    auto coll = db["restaurants"];
    
    // Create sample documents
    bsoncxx::document::value bakery1 = bsoncxx::builder::basic::make_document(
        bsoncxx::builder::basic::kvp("stars", 3),
        bsoncxx::builder::basic::kvp("categories", bsoncxx::builder::basic::make_array("Bakery", "Sandwiches")),
        bsoncxx::builder::basic::kvp("name", "Rising Sun Bakery"));
    
    bsoncxx::document::value bakery2 = bsoncxx::builder::basic::make_document(
        bsoncxx::builder::basic::kvp("stars", 4),
        bsoncxx::builder::basic::kvp("categories", bsoncxx::builder::basic::make_array("Bakery", "Cafe", "Bar")),
        bsoncxx::builder::basic::kvp("name", "Cafe au Late"));
    
    bsoncxx::document::value bakery3 = bsoncxx::builder::basic::make_document(
        bsoncxx::builder::basic::kvp("stars", 5),
        bsoncxx::builder::basic::kvp("categories", bsoncxx::builder::basic::make_array("Coffee", "Bakery")),
        bsoncxx::builder::basic::kvp("name", "Liz's Coffee Bar"));
    
    bsoncxx::document::value bakery4 = bsoncxx::builder::basic::make_document(
        bsoncxx::builder::basic::kvp("stars", 3),
        bsoncxx::builder::basic::kvp("categories", bsoncxx::builder::basic::make_array("Steak", "Seafood")),
        bsoncxx::builder::basic::kvp("name", "Oak Steakhouse"));
    
    bsoncxx::document::value bakery5 = bsoncxx::builder::basic::make_document(
        bsoncxx::builder::basic::kvp("stars", 4),
        bsoncxx::builder::basic::kvp("categories", bsoncxx::builder::basic::make_array("Bakery", "Dessert")),
        bsoncxx::builder::basic::kvp("name", "Petit Cookie"));
    
    coll.insert_many({bakery1.view(), bakery2.view(), bakery3.view(), bakery4.view(), bakery5.view()});
    
    // Define an aggregation pipeline
    mongocxx::pipeline p{};
    p.match(bsoncxx::builder::basic::make_document(bsoncxx::builder::basic::kvp("categories", "Bakery")));
    p.group(bsoncxx::builder::basic::make_document(bsoncxx::builder::basic::kvp("_id", "$stars"), bsoncxx::builder::basic::kvp("count", bsoncxx::builder::basic::make_document(bsoncxx::builder::basic::kvp("$sum", 1)))));
    
    // Execute the aggregation
    auto cursor = coll.aggregate(p);
    
    // Print the aggregated results
    for(auto doc : cursor) {
        std::cout << bsoncxx::to_json(doc) << std::endl;
    }
    
    return 0;
}
