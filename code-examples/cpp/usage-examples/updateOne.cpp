#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/json.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/stdx.hpp>
#include <mongocxx/uri.hpp>
#include <iostream>
#include <random>

using bsoncxx::builder::stream::document;

int main() {
    std::string uri = "<connection string uri>";
    mongocxx::client client{mongocxx::uri{uri}};

    try {
        auto database = client["sample_mflix"];
        auto movies = database["movies"];

        /* Generate some random double to simulate random number plot */
        std::random_device dev;
        std::mt19937 rng(dev());
        std::uniform_real_distribution<> dist(0.0, 1.0);
        auto randomPlot = "A harvest of random numbers, such as: " + std::to_string(dist(rng));
        
        auto filter_doc = document{} << "title" << "Random Harvest" << bsoncxx::builder::stream::finalize;
        auto update_doc = document{} << "$set" << bsoncxx::builder::stream::open_document 
                                      << "plot" << randomPlot << bsoncxx::builder::stream::close_document 
                                      << bsoncxx::builder::stream::finalize;

        // options for update operation
        mongocxx::options::update options;
        options.upsert(true);

        // Perform update operation
        auto result = movies.update_one(filter_doc.view(), update_doc.view(), options);

        std::cout << result->matched_count() << " document(s) matched the filter, updated " << result->modified_count() << " document(s)\n";
    } catch (const std::exception &e) {
        std::cerr << e.what() << "\n";
    }

    // Close the connection
    client.~client();

    return 0;
}
