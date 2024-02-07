MongoCollection<Document> collection;
//...

FindPublisher<Document> publisher = collection.find(Filters.where("sleep(100) || true"));
publisher.maxTime(50, TimeUnit.MILLISECONDS);

Mono.from(publisher).block();
