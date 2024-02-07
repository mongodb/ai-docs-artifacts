#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mongoc.h>
#include <bson.h>
#include <bcon.h>

int main (int argc, char *argv[]) {
    mongoc_client_t *client;
    mongoc_collection_t *collection;
    mongoc_write_concern_t *write_concern;
    bson_error_t error;
    bson_t *filter, *update;
    bson_t reply;
    bson_val_t *value;
    char *str;
    bool retval;
    int num;

    mongoc_init ();

    client = mongoc_client_new ("<connection string uri>");
    collection = mongoc_client_get_collection (client, "sample_mflix", "movies");
    write_concern = mongoc_write_concern_new ();
    mongoc_write_concern_set_w (write_concern, MONGOC_WRITE_CONCERN_W_MAJORITY);

    char title_regex[100] = "The Cat from";
    filter = BCON_NEW ("title", "{", "$regex", BCON_UTF8 (title_regex), "}");

    srand(time(NULL));
    num = rand() % 1000 + 1;
    char new_title[100];
    sprintf(new_title, "The Cat from Sector %d", num);
    update = BCON_NEW ("$set", "{", "title", BCON_UTF8 (new_title), "}");

    retval = mongoc_collection_replace_one (
       collection,
       filter,
       update,
       write_concern,
       &reply,
       &error);

    if (!retval) {
       fprintf (stderr, "Error: %s\n", error.message);
    } else {
       value = bson_get_field_value (&reply, "nModified");
       printf ("Modified %d document(s)\n", bson_value_get_int32(value));
    }

    bson_free (str);
    bson_destroy (&reply);
    bson_destroy (update);
    bson_destroy (filter);
    mongoc_write_concern_destroy (write_concern);
    mongoc_collection_destroy (collection);
    mongoc_client_destroy (client);

    mongoc_cleanup ();

    return 0;
}
