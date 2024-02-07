/*
   Retrieve documents that match the "year" value "1980"
   in descending order of the value of the "title" field
   that does not use the collation index.
*/
// start index no collation
mongocxx::options::find opts;
opts.sort(document{} << "title" << -1 << finalize);

auto cursor = myColl.find(document{} << "year" << 1980 << finalize, opts);
// end index no collation
