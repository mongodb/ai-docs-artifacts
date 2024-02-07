/*
    Retrieve documents that match the "year" value "1980"
    in descending order of the value of the "title" field
    that does not use the collation index.
*/
// start index no collation
MongoCollection<Document> myColl = database.getCollection("myColl");
FindPublisher<Document> publisher = myColl.find(eq("year", 1980))
                                         .sort(descending("title"));
// end index no collation
