#include <chrono>
#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/json.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/stdx.hpp>
#include <mongocxx/uri.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/change_stream.hpp>
#include <mongocxx/options/change_stream.hpp>

int main() {
    using bsoncxx::builder::stream::open_document;
    using bsoncxx::builder::stream::close_document;
    using bsoncxx::builder::stream::document;
    using namespace std::chrono;

    mongocxx::instance inst{};
    std::string uri = "mongodb+srv://<user>:<password>@<cluster-url>?retryWrites=true&w=majority";
    mongocxx::client client{mongocxx::uri{uri}};

    auto database = client["test"];
    auto collection = database["orders"];

    // Find
    auto today = system_clock::now();
    auto tomorrow = today + hours(24);
    auto find_filter = document{}
            << "name" << "Lemony Snicket"
            << "date" << open_document
            << "$gte" << bsoncxx::types::b_date{today}
            << "$lt" << bsoncxx::types::b_date{tomorrow}
            << close_document
            << finalize;
    auto find_cursor = collection.find(find_filter.view());
    for (auto&& doc : find_cursor) {
        std::cout << bsoncxx::to_json(doc) << std::endl;
    }

    // Aggregate
    auto one_week_ago = system_clock::now() - hours(24*7);
    auto aggregate_filter = document{}
            << "date" << open_document
            << "$gte" << bsoncxx::types::b_date{one_week_ago}
            << "$lt" << bsoncxx::types::b_date{today}
            << close_document
            << finalize;
    auto group = document{}
            << "_id" << "$status"
            << "total" << open_document << "$sum" << 1 << close_document
            << finalize;
    mongocxx::pipeline p{};
    p.match(aggregate_filter.view()).group(group.view());
    auto aggregate_cursor = collection.aggregate(p);
    for (auto&& doc : aggregate_cursor) {
        std::cout << bsoncxx::to_json(doc) << std::endl;
    }

    // Watch for changes
    mongocxx::options::change_stream opts{};
    opts.full_document(mongocxx::options::change_stream::full_document::k_update_lookup);
    auto stream = collection.watch(opts);
    for(auto&& event : stream) {
        // Details
        auto view = event.view();
        if (view["operationType"].get_string().value == "insert") {
            std::cout << "Change detected: \n";
            std::cout << "Name: " << view["fullDocument"]["name"].get_utf8().value.to_string();
            std::cout << "Address: " << view["fullDocument"]["address"].get_utf8().value.to_string() << std::endl;
        }
    }

    // Insert
    bsoncxx::document::value order_doc = document{}
            << "date" << bsoncxx::types::b_date{system_clock::now()}
            << "address" << "667 Dark Avenue, San Francisco, CA, 94110"
            << "name" << "Lemony Snicket"
            << "items" << open_document
            << "pizza" << open_document
            << "toppings" << "[\"pepperoni\", \"mushrooms\"]"
            << "notes" << "extra shrooms"
            << "price" << 11
            << close_document
            << "wings" << open_document
            << "sauce" << "buffalo"
            << "notes" << "extra crispy"
            << "price" << 10
            << close_document
            << close_document
            << "status" << "created"
            << finalize;
    collection.insert_one(std::move(order_doc));

    // Update
    auto update_filter = document{}
            << "address" << "667 Dark Avenue, San Francisco, CA, 94110"
            << "date" << open_document
            << "$gte" << bsoncxx::types::b_date{today}
            << "$lt" << bsoncxx::types::b_date{tomorrow}
            << close_document
            << finalize;
    auto update_value = document{} << "$set" << open_document << "address" << "667 Dark Avenue, San Francisco, CA, 94103" << close_document << finalize;
    collection.update_one(update_filter.view(), update_value.view());

    // Delete
    auto delete_filter = document{}
            << "address" << "13 Lousy Lane"
            << "name" << "Violet Baudelaire"
            << "date" << open_document
            << "$gte" << bsoncxx::types::b_date{today}
            << "$lt" << bsoncxx::types::b_date{tomorrow}
            << close_document
            << finalize;
    collection.delete_one(delete_filter.view());

    return 0;
}
