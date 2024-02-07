#include <bsoncxx/json.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/stdx.hpp>
#include <mongocxx/uri.hpp>
#include <mongocxx/instance.hpp>
#include <iostream>

void textSearch(const mongocxx::collection& movies, const std::string& search, bool negation = false, bool relevance = false) {
    bsoncxx::builder::stream::document document{};
    
    if(negation) 
        document << "$text" << bsoncxx::builder::stream::open_document << "$search" << search << bsoncxx::builder::stream::close_document;
    else
        document << "$text" << bsoncxx::builder::stream::open_document << "$search" << search << bsoncxx::builder::stream::close_document;
    
    auto cursor = movies.find(document.view());

    if(negation) 
        std::cout << "No documents found!" << std::endl;
 
    for(auto&& doc : cursor) {
        std::cout << bsoncxx::to_json(doc) << std::endl;
    }
}

int main(int, char**) {
    mongocxx::instance inst{};
    mongocxx::client conn{mongocxx::uri{"mongodb+srv://<user>:<password>@<cluster-url>?writeConcern=majority"}};

    auto db = conn["sample_mflix"];
    auto movies = db["movies"];
    
    textSearch(movies, "trek");
    textSearch(movies, "\"star trek\"");
    textSearch(movies, "\"star trek\" -\"into darkness\"", true);
    textSearch(movies, "\"star trek\" -\"into darkness\"", true, true);
  
   return EXIT_SUCCESS;
}
