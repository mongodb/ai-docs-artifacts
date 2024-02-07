// Retrieve documents that match "photograph" in the "type" field
// start auto collation
mongocxx::cursor cursor = myColl.find(bsoncxx::builder::stream::document{} << "type" << "photograph" << bsoncxx::builder::stream::finalize);
// end auto collation.
