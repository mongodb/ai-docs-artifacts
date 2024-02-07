// Specify the Collation
mongocxx::options::find opts{};
opts.collation(bsoncxx::builder::stream::document{} << "locale"
              << "en_US" << bsoncxx::builder::stream::finalize);

// Create the query for the documents with "year" = 1980
bsoncxx::builder::stream::document filter_builder{};
filter_builder << "year" << 1980;

// Fetch the results and sort in descending order by "title"
auto cursor = myColl.find(filter_builder.view(), opts);
for (auto&& doc : cursor) {
    std::cout << bsoncxx::to_json(doc) << std::endl;
}
auto order = bsoncxx::builder::stream::document{} << "title" << -1 << bsoncxx::builder::stream::finalize;
myColl.find(filter_builder.view(), opts).sort(order.view());
