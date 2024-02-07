/*
   Delete the first document that contains a value greater
   than 100 in the "a" field when ordered by using the
   English numeric collation order.
*/

// Creating a connection
mongocxx::instance inst{};
mongocxx::client conn{mongocxx::uri{}};

// Accessing the database
mongocxx::database db = conn["myDB"];
mongocxx::collection coll = db["myColl"];

// Constructing the filter document
bsoncxx::builder::stream::document filter_document{};
filter_document << "a" << bsoncxx::builder::stream::open_document
                << "$gt" << 100
                << bsoncxx::builder::stream::close_document;

// Constructing the options document with collation
bsoncxx::builder::stream::document options_document{};
options_document << "collation" << bsoncxx::builder::stream::open_document
                << "locale" << "en"
                << "numericOrdering" << true
                << bsoncxx::builder::stream::close_document;

// Deleting the document
coll.find_one_and_delete(filter_document.view(), options_document.view());
