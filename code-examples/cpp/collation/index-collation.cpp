/*
   Create an index on the "title" field with the "en_US"
   locale collation, specifying ascending ordering of the
   "title" field.
*/
// start create index collation
mongocxx::collection myColl;
bsoncxx::builder::stream::document doc_value{};
doc_value << "title" << 1;
mongocxx::options::index index_options{};
index_options.collation(bsoncxx::builder::stream::document{} << "locale" << "en_US" << bsoncxx::builder::stream::finalize);
myColl.create_index(doc_value.view(), index_options);
// end create index collation
