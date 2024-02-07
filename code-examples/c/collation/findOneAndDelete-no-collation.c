/*
   Delete the first document that contains a value greater
   than 100 in the "a" field when ordered by using the default
   binary collation order.
*/
// start findOneAndDelete no collation
bson_t* query = BCON_NEW("a", "{", "$gt", BCON_INT32(100), "}");

mongoc_find_one_and_delete_opts_t* opts = NULL;
mongoc_bulk_write_flags_t flags;
flags = MONGOC_BULK_NONE;

bson_error_t error;
bson_t reply;
bool success = mongoc_collection_find_one_and_delete(myColl, query, opts, &reply, &error);
// end findOneAndDelete no collation
