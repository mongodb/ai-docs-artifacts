mongoc_collection_t *collection;
mongoc_cursor_t *cursor;
bson_error_t error;
bson_t *query;

query = BCON_NEW ("type", BCON_UTF8 ("photograph"));
cursor = mongoc_collection_find_with_opts (collection, query, NULL, NULL);

if (mongoc_cursor_error (cursor, &error)) {
    fprintf (stderr, "%s\n", error.message);
} 

bson_destroy (query);
mongoc_cursor_destroy (cursor);
