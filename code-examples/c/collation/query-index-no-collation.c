const mongoc_collection_t *collection;
const bson_t *document;
mongoc_cursor_t *cursor;

bson_t *filter = bson_new();
BSON_APPEND_INT32 (filter, "year", 1980);

bson_t *opts = bson_new();
bson_t order;

bson_append_document_begin(opts, "sort", -1, &order);
BSON_APPEND_INT32(&order, "title", -1);
bson_append_document_end(opts, &order);

cursor = mongoc_collection_find_with_opts(collection, filter, opts, NULL);

while (mongoc_cursor_next (cursor, &document)) {
    char *str;
    str = bson_as_json (document, NULL);
    printf ("%s\n", str);
    bson_free (str);
}

bson_destroy (filter);
bson_destroy (opts);
mongoc_cursor_destroy (cursor);
