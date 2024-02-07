#include <bson.h>
#include <mongoc.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char* argv[]) {
    mongoc_client_t* client;
    mongoc_collection_t* collection;
    mongoc_update_flags_t flags;
    bson_oid_t oid;
    bson_error_t error;
    bson_t* update;
    bson_t* filter;
    char* str;
    long rand_percent;
    long int ti;
    bson_t query, set;
    bool success;
    
    mongoc_init();
    
    client = mongoc_client_new("<connection string uri>");

    srand((unsigned int)time(&ti));

    collection = mongoc_client_get_collection(client, "sample_mflix", "movies");

    filter = BCON_NEW("rated", "G");

    rand_percent = rand() % 101;

    char random_review[60];
    sprintf(random_review, "After viewing I am %li%% more satisfied with life.", rand_percent);

    BSON_APPEND_UTF8(&set, "random_review", random_review);

    update = BCON_NEW("$set", "{", "random_review", BCON_UTF8(random_review), "}");

    if (!(success = mongoc_collection_update_many(collection, filter, update, NULL, NULL, &error))) {
        printf("Updated %d documents\n", success);
        return EXIT_FAILURE;
    }
    
    printf("Update Result: %s\n", success ? "SUCCESS" : "FAILURE");
    
    bson_destroy(filter);
    bson_destroy(update);
    mongoc_collection_destroy(collection);
    mongoc_client_destroy(client);
    mongoc_cleanup();
    
    return EXIT_SUCCESS;
}
