MongoCollection<Document> myColl;

FindPublisher<Document> findPublisher = myColl.find(Filters.eq("type", "photograph"))
    .collation(Collation.builder()
        .locale("is")
        .caseFirst(CollationCaseFirst.UPPER)
        .build()
    );
Mono.from(findPublisher).subscribe();
