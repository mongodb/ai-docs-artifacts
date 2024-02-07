/*
  Retrieve the total number of matching names, grouped by the first_name
  field and sorted by using the German phonebook collation.
*/

// start aggregation collation
bson_t *pipeline;
bson_t *group_stage;
bson_t *sort_stage;
bson_t *opts;

pipeline = bson_new();
group_stage = BCON_NEW(
  "$group", "{",
    "_id", BCON_UTF8("$first_name"),
    "nameCount", "{", "$sum", BCON_INT32(1), "}",
  "}"
);
sort_stage = BCON_NEW(
  "$sort", "{",
      "_id", BCON_INT32(1),
  "}"
);

bson_append_document(pipeline, "0", -1, group_stage);
bson_append_document(pipeline, "1", -1, sort_stage);

opts = BCON_NEW(
  "collation", "{",
    "locale", BCON_UTF8("de@collation=phonebook"),
  "}"
);

mongoc_collection_t *collection = mongoc_client_get_collection (client, "database_name", "collection_name");

mongoc_cursor_t *cursor = mongoc_collection_aggregate(collection, MONGOC_QUERY_NONE, pipeline, opts, NULL);

// end aggregation collation

bson_destroy(pipeline);
bson_destroy(group_stage);
bson_destroy(sort_stage);
bson_destroy(opts);
mongoc_collection_destroy(collection);
mongoc_cursor_destroy(cursor);
