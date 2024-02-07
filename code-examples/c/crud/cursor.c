#include <mongoc.h>
#include <bson.h>
#include <stdio.h>

// Replace the following string with your MongoDB deployment's connection string
const char *uri =
  "mongodb+srv://<user>:<password>@<cluster-url>?writeConcern=majority";

mongoc_client_t *client;
mongoc_collection_t *orders;
mongoc_cursor_t *cursor;

void perform_operations() {
  const bson_t *doc;
  client = mongoc_client_new(uri);
  orders = mongoc_client_get_collection(client, "test", "orders");

  // Asynchronous iteration by using cursor methods
  cursor = mongoc_collection_find_with_opts(orders, NULL, NULL, NULL);
  printf("async\n");
  while (mongoc_cursor_next(cursor, &doc)) {
    printf("%s\n", bson_as_json(doc, NULL));
  }
  mongoc_cursor_destroy(cursor);

  // Manual iteration using cursor methods
  cursor = mongoc_collection_find_with_opts(orders, NULL, NULL, NULL);
  while (mongoc_cursor_next(cursor, &doc)) {
    printf("%s\n", bson_as_json(doc, NULL));
  }
  mongoc_cursor_destroy(cursor);

  // Event-based iteration using cursor methods is not directly supported in C, alternative is to manually iterate like above
  
  // Fetch all documents using a linked list approach
  cursor = mongoc_collection_find_with_opts(orders, NULL, NULL, NULL);
  int count = 0;
  while (mongoc_cursor_next(cursor, &doc)) {
    count++;
  }
  printf("%d\n", count);
  mongoc_cursor_destroy(cursor);

  // Rewind the cursor
  cursor = mongoc_collection_find_with_opts(orders, NULL, NULL, NULL);
  count = 0;
  while (mongoc_cursor_next(cursor, &doc)) {
    count++;
  }
  printf("First count: %d\n", count);
  mongoc_cursor_rewind(cursor);
  count = 0;
  while (mongoc_cursor_next(cursor, &doc)) {
    count++;
  }
  printf("Second count: %d\n", count);
  
  // Close a cursor
  mongoc_cursor_destroy(cursor);

  // Close the connection
  mongoc_collection_destroy(orders);
  mongoc_client_destroy(client);
}

int main() {
  mongoc_init();
  
  perform_operations();
  
  mongoc_cleanup();

  return 0;
}
