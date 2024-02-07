#include <iostream>
#include <bsoncxx/json.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/uri.hpp>

int main() {
    mongocxx::instance instance{};  // Output only the transformed code with no additional text.
    mongocxx::uri uri("<connection string uri>");
    mongocxx::client client(uri);

    auto db = client["sample_mflix"];

    // Run 'dbStats' command and print the result
    try {
        auto result = db.run_command(bsoncxx::from_json("{ 'dbStats' : 1 }"));
        std::cout << bsoncxx::to_json(result) << "\n";
    }
    catch(const std::exception& e) {
        std::cerr << "An exception occurred: " << e.what() << '\n';
    }

    return 0;
}
