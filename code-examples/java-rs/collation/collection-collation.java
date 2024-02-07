MongoClient mongoClient = MongoClients.create();

MongoDatabase db = mongoClient.getDatabase("myDB");

Collation collation = Collation.builder().locale("fr_CA").build();

Mono.from(db.createCollection("souvenirs", new CreateCollectionOptions().collation(collation)))
        .subscribe();
