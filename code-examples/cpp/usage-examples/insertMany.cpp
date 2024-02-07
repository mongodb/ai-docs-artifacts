#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/json.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/stdx.hpp>
#include <mongocxx/uri.hpp>
#include <iostream>

using namespace bsoncxx::builder::stream;

int main(int, char**) {
  mongocxx::instance inst{};
  mongocxx::client conn{mongocxx::uri{"<connection string uri>"}};

  auto collection = conn["insertDB"]["foods"];

  auto builder = document{};
  auto bulk = collection.create_bulk_operation();
  bulk.append(builder << "name" << "cake" << "healthy" << false << finalize);
  bulk.append(builder << "name" << "lettuce" << "healthy" << true << finalize);
  bulk.append(builder << "name" << "donut" << "healthy" << false << finalize);
  builder.clear();

  try {
    auto result = bulk.execute();
    std::cout << result->inserted_count() << " documents were inserted" << std::endl;
  } catch (const std::exception& xcp) {
    std::cout << "Connection failure: " << xcp.what() << std::endl;
  }

  return 0;
}
