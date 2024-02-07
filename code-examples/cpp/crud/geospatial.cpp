#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/json.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/stdx.hpp>
#include <mongocxx/uri.hpp>

class GeospatialQueries {
  public:
    static void runQueries() {
        mongocxx::uri uri("mongodb+srv://<user>:<password>@<cluster-url>?writeConcern=majority");
        mongocxx::client client(uri);

        auto database = client["sample_mflix"];
        auto theaters = database["theaters"];

        proximity(theaters);
        range(theaters);

        client.close();
    }

  private:
    // Find theaters within a certain proximity
    static void proximity(mongocxx::collection& theaters) {
        bsoncxx::builder::stream::document query{};
        query << "location.geo" << open_document
              << "$near" << open_document
              << "$geometry" << open_document << "type" << "Point"
              << "coordinates" << open_array << -73.9667 << 40.78 << close_array
              << close_document
              << "$maxDistance" << 10000 << close_document;

        auto cursor = theaters.find(query.view());
        if(cursor.begin() == cursor.end()) {
            std::cout << "No documents found!" << std::endl;
        } else {
            for(auto&& doc : cursor) {
                std::cout << bsoncxx::to_json(doc) << std::endl;
            }
        }
    }

    // Find theaters within a specific geographic range
    static void range(mongocxx::collection& theaters) {
        bsoncxx::builder::stream::document query{};
        query << "location.geo" << open_document
              << "$geoWithin" << open_document
              << "$geometry" << open_document << "type" << "Polygon"
              << "coordinates" << open_array 
              << open_array << -72 << 40 << close_array
              << open_array << -74 << 41 << close_array
              << open_array << -72 << 39 << close_array
              << open_array << -72 << 40 << close_array
              << close_array
              << close_document << close_document;

        auto cursor = theaters.find(query.view());
        if(cursor.begin() == cursor.end())  {
            std::cout << "No documents found!" << std::endl;
        } else {
            for(auto&& doc : cursor) {
                std::cout << bsoncxx::to_json(doc) << std::endl;
            }
        }
    }
};

int main() {
    GeospatialQueries::runQueries();
    return 0;
}
