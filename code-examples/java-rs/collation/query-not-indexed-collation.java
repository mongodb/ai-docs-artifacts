MongoCollection<Document> myColl;

Collation collation = Collation.builder()
    .locale("en_US")
    .collationStrength(CollationStrength.SECONDARY)
    .build();

myColl.find(Filters.eq("year", 1980))
    .collation(collation)
    .sort(Sorts.descending("title"))
    .subscribe(new Observer<Document>() {
        @Override
        public void onNext(Document doc) {
            System.out.println(doc.toJson());
        }
        @Override
        public void onError(Throwable e) {
            System.out.println("Error: " + e.getMessage());
        }
        @Override
        public void onComplete() {
            System.out.println("All done.");
        }
    });
