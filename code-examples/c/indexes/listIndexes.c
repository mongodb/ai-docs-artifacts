#include <mongoc/mongoc.h>
#include <bson/bson.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
   mongoc_client_t *client;
   mongoc_collection_t *collection;
   bson_error_t error;
   mongoc_cursor_t *cursor;
   const bson_t *doc;

   // Initialize the MongoDB C Driver.
   mongoc_init();

   // Replace the placeholders with your credentials.
   client = mongoc_client_new("mongodb+srv://<user>:<password>@<cluster-url>?writeConcern=majority");

   // Access a collection from a database.
   collection = mongoc_client_get_collection(client, "<databaseName>", "<collectionName>");

   // List the indexes on the collection.
   cursor = mongoc_collection_find_indexes_with_opts(collection, bson_new(), NULL, &error);

   if(cursor){
        printf("Existing indexes:\n");
        
        // Output the indexes as an array.
        while (mongoc_cursor_next (cursor, &doc)) {
            char *str = bson_as_canonical_extended_json (doc, NULL);
            printf ("%s\n", str);
            bson_free (str);
        }
   } else {
       printf("Error getting indexes: %s\n", error.message);
   }

   // Clean up.
   mongoc_cursor_destroy (cursor);
   mongoc_collection_destroy (collection);
   mongoc_client_destroy (client);

   mongoc_cleanup ();

   return 0;
}
