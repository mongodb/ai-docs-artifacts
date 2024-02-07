MongoCollection<Document> myColl = mongoDatabase.getCollection("exampleCollection");

// Find any document where the first_name field is less than "Gunter"
Bson filter = Filters.lt("first_name", "Gunter");

// Update the "verified" field to "true" 
Bson update = Updates.set("verified", true);

// Set the collation using the German phonebook collation order
Collation collation = Collation.builder().locale("de@collation=phonebook").build();

// Find one and update with the collation
myColl.findOneAndUpdate(filter, update, new FindOneAndUpdateOptions().collation(collation)).subscribe();
