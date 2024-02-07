#include <bson/bson.h>
#include <mongoc/mongoc.h>
#include <stdio.h>

int main(int argc, char *argv[]) 
{
   mongoc_client_t *client;
   mongoc_collection_t *collection;
   bson_error_t error;
   bson_oid_t oid;
   bson_t *doc;
   
   mongoc_init ();

   // Replace with your connection string.
   client = mongoc_client_new ("<mongodb connection string>"); 
   collection = mongoc_client_get_collection (client, "insertDB", "haiku");

   doc = bson_new ();
   bson_oid_init (&oid, NULL);
   BSON_APPEND_OID (doc, "_id", &oid);
   
   BSON_APPEND_UTF8 (doc, "title", "Record of a Shriveled Datum");
   BSON_APPEND_UTF8 (doc, "content", "No bytes, no problem. Just insert a document, in MongoDB");

   if (!mongoc_collection_insert_one (collection, doc, NULL, NULL, &error)) {
      fprintf (stderr, "%s\n", error.message);
   } else {
      char *str = bson_oid_to_string(&oid);
      printf ("A document was inserted with the _id: %s\n", str);
      bson_free(str);
   }

   bson_destroy (doc);
   mongoc_collection_destroy (collection);
   mongoc_client_destroy (client);
   
   mongoc_cleanup ();

   return 0;
}
