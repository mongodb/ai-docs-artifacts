// Create a client 
MongoClient mongoClient = MongoClients.create();

// Get the database and collection
MongoDatabase database = mongoClient.getDatabase("myDb");
MongoCollection<Document> collection = database.getCollection("myColl");

// Build the query filter
Bson filter = Filters.eq("city", "New York");

// Build the collation
Collation collation = Collation.builder().locale("de").build();

// Build the sort criteria
Bson sortCriteria = Sorts.ascending("name");

// Execute the query
FindPublisher<Document> results = collection.find(filter)
                                            .collation(collation)
                                            .sort(sortCriteria);
