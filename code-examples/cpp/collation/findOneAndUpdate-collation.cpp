/*
   Update the "verified" field to "true" for the first document
   that precedes "Gunter" when ordered by using the
   German phonebook collation order.
*/
// start findOneAndUpdate collation
mongocxx::options::find_one_and_update options{};
options.collation(bsoncxx::builder::basic::make_document(bsoncxx::builder::basic::kvp("locale", "de@collation=phonebook")));

auto filter = bsoncxx::builder::basic::make_document(bsoncxx::builder::basic::kvp("first_name", bsoncxx::builder::basic::make_document(bsoncxx::builder::basic::kvp("$lt", "Gunter"))));
auto update = bsoncxx::builder::basic::make_document(bsoncxx::builder::basic::kvp("$set", bsoncxx::builder::basic::make_document(bsoncxx::builder::basic::kvp("verified", true))));

myColl.findOneAndUpdate(filter.view(), update.view(), options);
// end findOneAndUpdate collation
