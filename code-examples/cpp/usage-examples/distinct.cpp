#include <mongocxx/client.hpp>
#include <mongocxx/stdx.hpp>
#include <mongocxx/uri.hpp>
#include <bsoncxx/json.hpp>
#include <bsoncxx/builder/stream/document.hpp>
#include <iostream>

using bsoncxx::builder::stream::document;
using bsoncxx::builder::stream::finalize;

int main() {
    mongocxx::uri uri("<connection string uri>");
    mongocxx::client client(uri);
    
    auto collection = client["sample_mflix"]["movies"];

    auto filter = document{} 
                  << "directors" << "Barbra Streisand" 
                  << finalize;
    
    auto cursor = collection.find(filter.view());
    
    std::vector<int> distinctYears;
    
    for(auto doc : cursor) {
        int year = doc["year"].get_int32();
        if(std::find(distinctYears.begin(), distinctYears.end(), year) == distinctYears.end())
            distinctYears.push_back(year);
    }

    for(auto year : distinctYears) {
        std::cout << year << std::endl;
    }

    return 0;
}
