/*
    Update the "verified" field to "true" for the first document
    that precedes "Gunter" when ordered by using the
    default binary collation order.
*/
mongoc_collection_t *collection;
mongoc_find_and_modify_opts_t *opts;
bson_t *query, *update, reply;
bson_error_t error;
bool success;

collection = mongoc_client_get_collection(client, "mydb", "myColl");

query = BCON_NEW("first_name", "{", "$lt", "Gunter", "}");
update = BCON_NEW("$set", "{", "verified", BCON_BOOL(true), "}");

opts = mongoc_find_and_modify_opts_new();
mongoc_find_and_modify_opts_set_update(opts, update);

success = mongoc_collection_find_and_modify_with_opts(collection, query, opts, &reply, &error);

if (!success) {
    fprintf(stderr, "Error: %s\n", error.message);
}

bson_destroy(query);
bson_destroy(update);
bson_destroy(&reply);
mongoc_find_and_modify_opts_destroy(opts);
mongoc_collection_destroy(collection);
