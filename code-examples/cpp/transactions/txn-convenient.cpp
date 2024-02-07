#include <bsoncxx/json.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/stdx.hpp>
#include <mongocxx/uri.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/pipeline.hpp>

using bsoncxx::builder::basic::kvp;
using bsoncxx::builder::basic::make_document;

int main() {
    mongocxx::instance inst{};
    mongocxx::client conn{mongocxx::uri{"<connection string uri>"}};

    auto collection = conn["testdb"]["inventory"];
    collection.insert_many({
        // start-inventory
        make_document(kvp("item", "sunblock"), kvp("qty", 85), kvp("price", 6.0)),
        make_document(kvp("item", "beach chair"), kvp("qty", 30), kvp("price", 25.0))
        // end-inventory
    });
}

std::array<bsoncxx::document::value, 2> order1 = {
    // start-order-successful
    make_document(kvp("item", "sunblock"), kvp("qty", 3)),
    make_document(kvp("item", "beach chair"), kvp("qty", 1))
    // end-order-successful
};

std::array<bsoncxx::document::value, 1> order2 = {
    // start-order-fail
    make_document(kvp("item", "volleyball"), kvp("qty", 1))
    // end-order-fail
};

std::array<bsoncxx::document::value, 2>* order = &order1;

// start-transaction
try {
    auto session = conn.start_session();
    session.start_transaction();
    float total = 0.0;
    auto invColl = conn["testdb"]["inventory"];
    auto recColl = conn["testdb"]["orders"];

    for (const auto& orderItem : *order) {
        auto itemFields = orderItem.view();
               
        // Update the inventory for the purchased items
        auto maybeResult = invColl.find_one_and_update(
            session, 
            make_document(
                kvp("item", itemFields["item"].get_utf8().value.to_string()),
                kvp("qty", make_document(kvp("$gte", itemFields["qty"].get_int32().value)))),
            make_document(kvp("$inc", make_document(kvp("qty", -itemFields["qty"].get_int32().value))))
        );
        
        if (!maybeResult) { // End the transaction if item not found
            std::cout << "Item not found or insufficient quantity.\n";
            session.abort_transaction();
            return 0;
        }
        
        float subTotal = maybeResult->view()["qty"].get_int32().value * 
                         maybeResult->view()["price"].get_double().value;
        total += subTotal;
    }
    
    // Create a record of the purchase
	auto receipt = make_document(kvp("date", bsoncxx::types::b_date{
                                 std::chrono::system_clock::now()}),
                                 kvp("items", *order),
                                 kvp("total", total));
    recColl.insert_one(session, receipt.view());
 
    session.commit_transaction();
    
    std::cout << "Order successfully completed and recorded!\nReceipt:\n";
    std::cout << bsoncxx::to_json(receipt.view()) << "\n";
}
catch(const mongocxx::exception& e) {
    std::cerr << "An error occurred: " << e.what() << "\n";
}
// end-transaction
