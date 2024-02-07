/*
   Retrieve documents that match "photograph" in the "type" field,
   sorted by the Iceland collation and uppercase precedence.
*/
// start specified collation

bson_t *filter = bson_new();
BSON_APPEND_UTF8(filter, "type", "photograph");

bson_t *opts = bson_new();
bson_t collation;
bson_append_document_begin(opts, "collation", -1, &collation);
BSON_APPEND_UTF8(&collation, "locale", "is");
BSON_APPEND_UTF8(&collation, "caseFirst", "upper");
bson_append_document_end(opts, &collation);

mongoc_cursor_t *cursor = mongoc_collection_find_with_opts(myColl, filter, opts, NULL);

// end specified collation

bson_destroy(filter);
bson_destroy(opts);
