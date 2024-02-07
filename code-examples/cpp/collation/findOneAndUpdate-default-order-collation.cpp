/*
   Update the "verified" field to "true" for the first document
   that precedes "Gunter" when ordered by using the
   default binary collation order.
*/

// start findOneAndUpdate default order collation
mongocxx::collection coll;
bsoncxx::builder::stream::document filter_builder, update_builder;

filter_builder << "first_name" << bsoncxx::builder::stream::open_document
               << "$lt" << "Gunter" << bsoncxx::builder::stream::close_document;

update_builder << "$set" << bsoncxx::builder::stream::open_document
               << "verified" << true << bsoncxx::builder::stream::close_document;

coll.find_one_and_update(filter_builder.view(), update_builder.view());
// end findOneAndUpdate default order collation
