#include <bsoncxx/builder/stream/document.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/uri.hpp>

using bsoncxx::builder::basic::kvp;
using bsoncxx::builder::basic::make_document;

int main() {
  mongocxx::instance inst{};
  mongocxx::client conn{mongocxx::uri{"<connection string uri>"}};

  auto coll = conn["sample_mflix"]["movies"];

  auto cursor = coll.find(
      make_document(kvp("runtime", make_document(kvp("$lt", 15)))),
      bsoncxx::options::find{}.sort(make_document(kvp("title", 1))));

  size_t count = std::distance(cursor.begin(), cursor.end());
  if (count == 0) {
    std::cout << "No documents found!";
    return 1;
  }

  for(auto doc : cursor) {
    std::cout << bsoncxx::to_json(doc) << "\n";
  }
  
  return 0;
}
