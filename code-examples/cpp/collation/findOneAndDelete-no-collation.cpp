/* 
   Delete the first document that contains a value greater
   than 100 in the "a" field when ordered by using the default
   binary collation order.
*/
// start findOneAndDelete no collation

bsoncxx::stdx::optional<bsoncxx::document::value> maybe_result =
    myColl.find_one_and_delete(document{} << "a" << open_document <<
                                  "$gt" << "100" << close_document << finalize);

// end findOneAndDelete no collation
