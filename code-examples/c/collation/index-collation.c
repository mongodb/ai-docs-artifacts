/*
   Create an index on the "title" field with the "en_US"
   locale collation, specifying ascending ordering of the
   "title" field.
*/

// start create index collation
bson_t *keys = bson_new();
BSON_APPEND_INT32(keys, "title", 1);   

mongoc_index_opt_t *myIndexOpt = mongoc_index_opt_alloc();
mongoc_index_opt_t opt;

mongoc_collation_t collation = { 
    "en_US", 
    MONGOC_COLLATION_CASE_LEVEL_DEFAULT, 
    MONGOC_COLLATION_CASE_FIRST_OFF, 
    MONGOC_COLLATION_STRENGTH_DEFAULT, 
    MONGOC_COLLATION_ALTERNATE_DEFAULT, 
    MONGOC_COLLATION_MAX_VARIABLE_DEFAULT, 
    true 
};

opt.collation = &collation;
opt.name = "title_index";
    
mongoc_collection_create_index(myColl, keys, &opt, &error);
    
bson_destroy(keys);
mongoc_index_opt_destroy(myIndexOpt);
// end create index collation
