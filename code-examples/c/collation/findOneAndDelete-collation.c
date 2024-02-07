/*
   Delete the first document that contains a value greater
   than 100 in the "a" field when ordered by using the
   English numeric collation order.
*/

mongoc_collection_t *collection; 
mongoc_find_and_modify_opts_t *opts;
bson_error_t error;
bson_t *doc = NULL;

bool r; 

doc = BCON_NEW ("a", "{", "$gt", BCON_INT32 (100), "}");
opts = mongoc_find_and_modify_opts_new ();
mongoc_find_and_modify_opts_set_sort (opts, doc);

if (!mongoc_collection_find_and_modify_with_opts (collection, &query, opts, &reply, &error)) {
   //Handle error
}

bson_destroy (doc);
mongoc_find_and_modify_opts_destroy (opts);
mongoc_collection_destroy (collection);
