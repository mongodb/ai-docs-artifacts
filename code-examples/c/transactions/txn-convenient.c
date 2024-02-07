#include <bson/bson.h>
#include <mongoc/mongoc.h>

int main(int argc, char *argv[]) {
  mongoc_client_t      *client;
  mongoc_collection_t  *inventory_collection, *orders_collection;
  bson_t               *inventory_items = bson_new();
  bson_t               filter;
  bson_t               update;
  bson_error_t         error;

  mongoc_init();

  client = mongoc_client_new ("<connection string uri>");
  mongoc_client_set_appname (client, "inventory_management");
  inventory_collection = mongoc_client_get_collection (client, "testdb", "inventory");
  orders_collection = mongoc_client_get_collection (client, "testdb", "orders");

  BSON_APPEND_UTF8(bson_append_array_begin(inventory_items, "inventory", -1), "item", "sunblock");
  BSON_APPEND_INT32(bson_append_array_begin(inventory_items, "inventory", -1), "qty", 85);
  BSON_APPEND_DOUBLE(bson_append_array_begin(inventory_items, "inventory", -1), "price", 6.0);

  BSON_APPEND_UTF8(bson_append_array_begin(inventory_items, "inventory", -1), "item", "beach chair");
  BSON_APPEND_INT32(bson_append_array_begin(inventory_items, "inventory", -1), "qty", 30);
  BSON_APPEND_DOUBLE(bson_append_array_begin(inventory_items, "inventory", -1), "price", 25.0);

  if(!mongoc_collection_insert_many(inventory_collection, &inventory_items, 2, NULL, NULL, &error)) {
    fprintf(stderr, "Insert failed: %s\n", error.message);
  }

  BSON_APPEND_UTF8(&filter, "$and", bson_new());
  BSON_APPEND_UTF8(bson_append_array_begin(&filter, &filter, -1), "item", "sunblock");
  BSON_APPEND_INT32(bson_append_array_begin(&filter, &filter, -1), "qty", 3);
  BSON_APPEND_UTF8(bson_append_array_begin(&filter, &filter, -1), "item", "beach chair");
  BSON_APPEND_INT32(bson_append_array_begin(&filter, &filter, -1), "qty", 1);

  BSON_APPEND_UTF8(&update, "item", "$set");
  BSON_APPEND_INT32(&update, "qty", 3);

  BSON_APPEND_UTF8(&filter, "$and", bson_new());
  BSON_APPEND_UTF8(bson_append_array_begin(&filter, &filter, -1), "item", "volleyball");
  BSON_APPEND_INT32(bson_append_array_begin(&filter, &filter, -1), "qty", 1);

  BSON_APPEND_UTF8(&update, "item", "$set");
  BSON_APPEND_INT32(&update, "qty", 1);

  if (!mongoc_collection_find_one_and_update(inventory_collection, &filter, &update, NULL, NULL, &error)) {
    fprintf (stderr, "Find one and update failed: %s\n", error.message);
  }

  bson_destroy(inventory_items);
  mongoc_collection_destroy(inventory_collection);
  mongoc_client_destroy(client);

  return 0;
}
