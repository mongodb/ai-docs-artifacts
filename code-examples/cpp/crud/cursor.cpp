#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/cursor.hpp>
#include <bsoncxx/json.hpp>

using bsoncxx::builder::basic::kvp;
using bsoncxx::builder::basic::make_document;
using namespace std;

// Replace the following string with your MongoDB deployment's connection string
string uri = "mongodb+srv://<user>:<password>@<cluster-url>?writeConcern=majority";

mongocxx::instance inst{};
mongocxx::client conn{mongocxx::uri{uri}};

void fetchDocumentsByIteration(mongocxx::collection& coll) {
  auto cursor = coll.find({});
  for (auto doc : cursor) {
    cout << bsoncxx::to_json(doc) << endl;
  }
}

void fetchDocumentByCursor(mongocxx::collection& coll) {
  auto cursor = coll.find({});
  if (cursor.begin() != cursor.end()) {
    bsoncxx::document::view doc = *cursor.begin();
    cout << bsoncxx::to_json(doc) << endl;
  }
}

void fetchAllDocuments(mongocxx::collection& coll) {
  auto cursor = coll.find({});
  vector<bsoncxx::document::value> docs(cursor.begin(), cursor.end());
  for (auto doc : docs) {
    cout << bsoncxx::to_json(doc) << endl;
  }
  cout << "Total documents: " << docs.size() << endl;
}

void rewindAndFetch(mongocxx::collection& coll) {
  auto cursor = coll.find({});
  vector<bsoncxx::document::value> docs(cursor.begin(), cursor.end());
  cout << "First count: " << docs.size() << endl;
  cursor = coll.find({});
  docs = vector<bsoncxx::document::value>(cursor.begin(), cursor.end());
  cout << "Second count: " << docs.size() << endl;
}

void closeCursor(mongocxx::collection& coll) {
  auto cursor = coll.find({});
  // cursor is automatically closed when it falls out of scope
}

int main() {
  mongocxx::database db = conn["test"];
  mongocxx::collection coll = db["orders"];

  fetchDocumentsByIteration(coll);
  fetchDocumentByCursor(coll);
  fetchAllDocuments(coll);
  rewindAndFetch(coll);
  closeCursor(coll);
  
  return 0;
}
