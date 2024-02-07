#include <mongocxx/client.hpp>
#include <mongocxx/stdx.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/uri.hpp>
#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/builder/stream/helpers.hpp>
#include <bsoncxx/json.hpp>

using bsoncxx::builder::stream::document;
using bsoncxx::builder::stream::finalize;
using bsoncxx::builder::stream::open_document;
using bsoncxx::builder::stream::close_document;

int main() {
    mongocxx::instance inst{};
    mongocxx::client conn{mongocxx::uri{"<connection string uri>"}};

    auto database = conn["sample_mflix"];
    auto theaters = database["theaters"];

    using namespace bsoncxx::builder::stream;

    try {
        auto result = theaters.bulk_write(std::vector<mongocxx::model::write>{
            mongocxx::model::insert_one{
                document{} << "location" 
                << open_document << "address" 
                << open_document << "street1"
                << "3 Main St." << "city" << "Anchorage" << "state"
                << "AK" << "zipcode" << "99501" << close_document
                << close_document << finalize
            },
            mongocxx::model::insert_one{
                document{} << "location" 
                << open_document << "address" 
                << open_document << "street1" 
                << "75 Penn Plaza" << "city" << "New York" << "state"
                << "NY" << "zipcode" << "10001" << close_document
                << close_document << finalize
            },
            mongocxx::model::update_many{
                document{} << "location.address.zipcode"
                << "44011" << finalize,  
                document{} << "$set" << open_document << "is_in_ohio" 
                << true << close_document << finalize,
                mongocxx::options::update()[mongocxx::options::update::upsert(true)]
            },
            mongocxx::model::delete_one{
                document{} << "location.address.street1"
                << "221b Baker St" << finalize
            }
        });

        std::cout << bsoncxx::to_json(result->result().value()) << std::endl;
    }
    catch (const std::exception& xcp) {
        std::cout << "connection failed: " << xcp.what() << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
