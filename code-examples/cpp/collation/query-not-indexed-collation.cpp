#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/json.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/stdx.hpp>
#include <mongocxx/uri.hpp>
#include <mongocxx/instance.hpp>

int main() {
  mongocxx::instance inst{};
  mongocxx::client conn{mongocxx::uri{}};

  auto coll = conn["testdb"]["myColl"];
  
  bsoncxx::builder::stream::document document{};
  
  mongocxx::options::find opts;
  opts.sort(bsoncxx::builder::stream::document{} << "title" << -1 << bsoncxx::builder::stream::finalize);
  opts.collation(bsoncxx::builder::stream::document{} << "locale" << "en_US" << "strength" << 2 << bsoncxx::builder::stream::finalize);

  auto cursor = coll.find(document.view() << "year" << 1980 << bsoncxx::builder::stream::finalize, opts);

  for(auto doc : cursor) {
    std::cout << bsoncxx::to_json(doc) << "\n";
  }

  return 0;
}
