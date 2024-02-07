#include <bsoncxx/builder/stream/document.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/stdx.hpp>
#include <mongocxx/uri.hpp>

mongocxx::instance instance{}; 
mongocxx::client client{mongocxx::uri{}};

auto database = client["<your db>"];
auto myColl = database["<your collection>"];
bsoncxx::builder::stream::document document{};
document << "field.nested" << "A string";
myColl.update_one({}, document.view());

auto database2 = client["<your db>"];
auto myColl2 = database2["<your collection>"];
bsoncxx::builder::stream::document document2{};
document2 << "field" << bsoncxx::builder::stream::open_document
            << "nested" << "A string" 
            << bsoncxx::builder::stream::close_document;
myColl2.update_one({}, document2.view());

auto database3 = client["<your db>"];
auto myColl3 = database3["<your collection>"];
bsoncxx::builder::stream::document document3{};
document3 << "age" << "Accepts any type!";
auto cursor = myColl3.find(document3.view());
