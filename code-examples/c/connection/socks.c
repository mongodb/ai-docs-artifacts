#include <mongoc/mongoc.h>
#include <stdio.h>
#include <fcntl.h> 
#include <assert.h>

int main(void)
{
  mongoc_client_t *client;
  mongoc_collection_t *collection;
  bson_t query;
  bson_t *doc;
  mongoc_cursor_t *cursor;

  mongoc_uri_t *uri;
  mongoc_client_pool_t *pool;

  bson_init (&query);

  // start-socks
  // Replace the placeholder with your connection string
  uri = mongoc_uri_new ("<connection string uri>");
  
  // Replace the placeholders with your SOCKS5 proxy server details
  mongoc_uri_set_option_as_int32 (uri, "proxyPort", 1080);
  mongoc_uri_set_option_as_utf8 (uri, "proxyHost", "<host>");
  mongoc_uri_set_option_as_utf8 (uri, "proxyUsername", "<username>");
  mongoc_uri_set_option_as_utf8 (uri, "proxyPassword", "<password>");

  // Create a new client with the proxy server details and request context
  pool = mongoc_client_pool_new (uri);
  client = mongoc_client_pool_pop (pool);
  // end-socks

  // Retrieve the first document from the MongoDB collection and print it
  collection = mongoc_client_get_collection (client, "myDB", "myColl");

  cursor = mongoc_collection_find_with_opts(collection, &query, NULL, NULL);
  while (mongoc_cursor_next (cursor, &doc)) {
    char *str;

    str = bson_as_json (doc, NULL);
    printf ("%s\n", str);
    bson_free (str);

    // Only retrieve the first document
    break;
  }
  
  mongoc_cursor_destroy (cursor);
  mongoc_collection_destroy (collection);

  // Close the connection after the operation completes
  mongoc_client_pool_push (pool, client);

  mongoc_client_pool_destroy (pool);
  mongoc_uri_destroy (uri);
}
