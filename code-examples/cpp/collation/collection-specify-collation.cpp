/*
  Retrieve documents that match "photograph" in the "type" field,
  sorted by the Iceland collation and uppercase precedence.
*/

// Create a query filter document
bsoncxx::builder::stream::document filter{};
filter << "type" << "photograph";

// Create a find options document
mongocxx::options::find opts{};
opts.sort(bsoncxx::builder::basic::make_document(bsoncxx::builder::basic::kvp("type", "photograph")));
opts.collation(bsoncxx::builder::basic::make_document(bsoncxx::builder::basic::kvp("locale", "is"), bsoncxx::builder::basic::kvp("caseFirst", "upper")));

// Run the query
auto cursor = myColl.find(filter.view(), opts);

// Iterate over the results
for(auto&& doc : cursor) {
    std::cout << bsoncxx::to_json(doc) << std::endl;
}
