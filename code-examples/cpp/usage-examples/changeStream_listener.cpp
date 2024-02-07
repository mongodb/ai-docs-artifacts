#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/json.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/uri.hpp>
#include <mongocxx/change_stream.hpp>

void simulateAsyncPause() {
    std::chrono::seconds timespan(1);
    std::this_thread::sleep_for(timespan);
}

int main() {
    mongocxx::instance inst{};
    auto uri = mongocxx::uri{"<connection string uri>"};
    mongocxx::client conn{uri};

    mongocxx::database db = conn["insertDB"];
    auto haikus = db["haikus"]; 
    // watch the collection
    mongocxx::options::change_stream opts{};
    mongocxx::change_stream change_stream = haikus.watch(opts);

    simulateAsyncPause();

    bsoncxx::builder::stream::document document{};
    // insert a new document into the collection
    document << "title" << "Record of a Shriveled Datum"
             << "content" << "No bytes, no problem. Just insert a document, in MongoDB";

    haikus.insert_one(document.view());

    simulateAsyncPause();

    // Close the change stream
    change_stream.close();
    return 0;
}
