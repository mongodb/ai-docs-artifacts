#include <bsoncxx/json.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/stdx.hpp>
#include <mongocxx/uri.hpp>
#include <mongocxx/instance.hpp>

void cleanUp(mongocxx::client& client) {
  for (auto collection_name : {"customers", "inventory", "orders"}) {
    try {
      client["testdb"][collection_name].drop();
    } catch (const std::exception&) {}
  }
}

void setup(mongocxx::client& client) {
  try {
    auto customer_coll = client["testdb"]["customers"];
    auto inventory_coll = client["testdb"]["inventory"];

    // Insert order data for customer "98765" in the customers collection
    customer_coll.insert_one(bsoncxx::builder::basic::make_document(bsoncxx::builder::basic::kvp("_id", 98765), bsoncxx::builder::basic::kvp("orders", bsoncxx::builder::basic::array{})));

    // Insert inventory data for "sunblock" and "beach towel"
    inventory_coll.insert_many({
      bsoncxx::builder::basic::make_document(bsoncxx::builder::basic::kvp("item", "sunblock"), bsoncxx::builder::basic::kvp("item_id", 5432), bsoncxx::builder::basic::kvp("qty", 85)),
      bsoncxx::builder::basic::make_document(bsoncxx::builder::basic::kvp("item", "beach towel"), bsoncxx::builder::basic::kvp("item_id", 7865), bsoncxx::builder::basic::kvp("qty", 41))
    });
  } catch (const std::exception& e) {
    std::cout << "Unable to insert test data: " << e.what() << std::endl;
  }
}

void queryData(mongocxx::client& client) {
  for (auto collection_name : {"customers", "inventory", "orders"}) {
    auto cursor = client["testdb"][collection_name].find({});
    for (auto doc : cursor) {
      std::cout << bsoncxx::to_json(doc) << std::endl;
    }
  }
}

void placeOrder(mongocxx::client& client, bsoncxx::array::view cart, bsoncxx::document::view payment) {
  mongocxx::options::txn transaction_options;
  transaction_options.read_concern(mongocxx::read_concern::level::snapshot);
  transaction_options.write_concern(mongocxx::write_concern::acknowledge_level::majority);
  transaction_options.read_preference(mongocxx::read_preference::read_mode::primary);

  mongocxx::client_session session = client.start_session();

  auto orders_coll = client["testdb"]["orders"];
  auto inventory_coll = client["testdb"]["inventory"];
  auto customer_coll = client["testdb"]["customers"];

  try {
    session.start_transaction(transaction_options);
    
    auto order_result = orders_coll.insert_one(
      bsoncxx::builder::basic::make_document(
        bsoncxx::builder::basic::kvp("customer", payment["customer"].get_int32()),
        bsoncxx::builder::basic::kvp("items", cart),
        bsoncxx::builder::basic::kvp("total", payment["total"].get_double())
      )
    );

    for (const auto& item : cart) {
      auto in_stock = inventory_coll.find_one_and_update(
        session,
        bsoncxx::builder::basic::make_document(
          bsoncxx::builder::basic::kvp("item_id", item["item_id"].get_int32()),
          bsoncxx::builder::basic::kvp("qty", bsoncxx::builder::basic::make_document(bsoncxx::builder::basic::kvp("$gte", item["qty"].get_int32())))
        ),
        bsoncxx::builder::basic::make_document(bsoncxx::builder::basic::kvp("$inc", bsoncxx::builder::basic::make_document(bsoncxx::builder::basic::kvp("qty", -item["qty"].get_int32()))))
      );

      if (!in_stock) {
        throw std::runtime_error("Insufficient quantity or item ID not found.");
      }
    }

    customer_coll.update_one(
      session,
      bsoncxx::builder::basic::make_document(bsoncxx::builder::basic::kvp("_id", payment["customer"].get_int32())), 
      bsoncxx::builder::basic::make_document(bsoncxx::builder::basic::kvp("$push", bsoncxx::builder::basic::make_document(bsoncxx::builder::basic::kvp("orders",bsoncxx::types::b_oid{order_result->inserted_id()}))))
    );

    session.commit_transaction();
    std::cout << "Transaction successfully committed." << std::endl;

  } catch (const std::exception& e) {
    std::cout << "An error occured in the transaction, performing a data rollback: " << e.what() << std::endl;
    session.abort_transaction();
  } finally {
    session.end_session();
  }
}

int main() {
  mongocxx::instance instance{};
  mongocxx::client client(mongocxx::uri(process.env("MONGODB_URI")));

  cleanUp(client);
  setup(client);

  bsoncxx::builder::basic::array cart;
  cart.append(bsoncxx::builder::basic::make_document(kvp("item", "sunblock"), kvp("item_id", 5432), kvp("qty", 1), kvp("price", 5.19)));
  cart.append(bsoncxx::builder::basic::make_document(kvp("item", "beach towel"), kvp("item_id", 7865), kvp("qty", 2), kvp("price", 15.99)));

  auto payment = bsoncxx::builder::basic::make_document(kvp("customer", 98765), kvp("total", 37.17));

  placeOrder(client, cart.view(), payment.view());
  
  cleanUp(client);

  queryData(client);

  return 0;
}
