import org.mongodb.scala._

// Replace the uri string with your MongoDB deployment's connection string.
val uri = "<connection string uri>"

val mongoClient: MongoClient = MongoClient(uri)

val database: MongoDatabase = mongoClient.getDatabase("insertDB")

val haiku: MongoCollection[Document] = database.getCollection("haiku")

val doc: Document = Document("title" -> "Record of a Shriveled Datum",
      "content" -> "No bytes, no problem. Just insert a document, in MongoDB")

haiku.insertOne(doc).printHeadResult("Insert Result: ")
    , (results: InsertOneResult) => println("A document was inserted with the _id: " + results.getInsertedId))

mongoClient.close().printHeadResult()
