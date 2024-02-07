#include <bson.h>
#include <mongoc.h>
#include <stdio.h>
#include <stdlib.h>

int
main (void)
{
   mongoc_client_t *client;
   mongoc_collection_t *collection;
   bson_error_t error;
   bson_oid_t oid;
   bson_t *doc, *pipeline, *match, *group;

   mongoc_init ();

   client = mongoc_client_new (getenv("MONGDODB_URI"));

   collection = mongoc_client_get_collection (client, "aggregation", "restaurants");

   // Prepare and insert the documents
   const char * restaurantNames[] = { "Rising Sun Bakery", "Cafe au Late", "Liz's Coffee Bar", "Oak Steakhouse", "Petit Cookie" };
   const int restaurantStars[] = {3, 4, 5, 3, 4};
   const char * restaurantCats[5][3] = { {"Bakery", "Sandwiches"}, {"Bakery", "Cafe", "Bar"}, {"Coffee", "Bakery"}, {"Steak", "Seafood"}, {"Bakery", "Dessert"}};
   int j;

   for(j = 0; j < 5; j++){
      doc = bson_new ();
      bson_oid_init (&oid, NULL);
      BSON_APPEND_OID (doc, "_id", &oid);
      BSON_APPEND_UTF8 (doc, "name", restaurantNames[j]);
      BSON_APPEND_INT32 (doc, "stars", restaurantStars[j]);
      bson_t child;
      BSON_APPEND_ARRAY_BEGIN (doc, "categories", &child);
      BSON_APPEND_UTF8 (&child, "0", restaurantCats[j][0]);
      BSON_APPEND_UTF8 (&child, "1", restaurantCats[j][1]);
      bson_append_array_end (doc, &child);

      if (!mongoc_collection_insert_one (collection, doc, NULL, NULL, &error)) {
         fprintf (stderr, "%s\n", error.message);
      }

      bson_destroy (doc);
   }

   // Define the aggregation pipeline
   pipeline = bson_new ();
   match = BCON_NEW ("$match", "{", "categories", BCON_UTF8 ("Bakery"), "}");
   group = BCON_NEW ("$group", "{", "_id", BCON_UTF8 ("$stars"), "count", "{", "$sum", BCON_INT32 (1), "}", "}");
   
   bson_t stages[2];
   bson_append_document_begin (pipeline, "0", 1, &stages[0]);
   bson_concat (&stages[0], match);
   bson_append_document_end (pipeline, &stages[0]);
   bson_destroy(match);
   
   bson_append_document_begin (pipeline, "1", 1, &stages[1]);
   bson_concat (&stages[1], group);
   bson_append_document_end (pipeline, &stages[1]);
   bson_destroy(group);

   mongoc_cursor_t *cursor = mongoc_collection_aggregate (collection,
   MONGOC_QUERY_NONE, pipeline, NULL, NULL);

   // Print the aggregated results
   const bson_t *next;
   while (mongoc_cursor_next (cursor, &next)) {
      char *str = bson_as_canonical_extended_json (next, NULL);
      printf ("%s\n", str);
      bson_free (str);
   }

   bson_error_t agg_error;
   if (mongoc_cursor_error (cursor, &agg_error)) {
      fprintf (stderr, "Cursor Failure: %s\n", agg_error.message);
      return EXIT_FAILURE;
   }

   bson_destroy (pipeline);
   mongoc_cursor_destroy (cursor);
   mongoc_collection_destroy (collection);
   mongoc_client_destroy (client);

   mongoc_cleanup ();

   return EXIT_SUCCESS;
}
