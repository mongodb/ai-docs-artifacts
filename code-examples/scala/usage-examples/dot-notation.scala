import org.mongodb.scala._

// start-no-error
case class TestType(field: Map[String, Int])

val mongoClient: MongoClient = MongoClient("<your connection string>")
val database: MongoDatabase = mongoClient.getDatabase("<your db>")
val collection: MongoCollection[Document] = database.getCollection[Document]("<your collection>")

collection.updateOne(
  new Document(),
  new Document("$set", new Document("field.nested", "A string"))
).results()

// end-no-error
// start-error
case class TestType2(field: Map[String, Int])

val mongoClient2: MongoClient = MongoClient("<your connection string>")
val database2: MongoDatabase = mongoClient2.getDatabase("<your db>")
val collection2: MongoCollection[Document] = database2.getCollection[Document]("<your collection>")

collection2.updateOne(
  new Document(),
  new Document(
    "$set",
    new Document("field", new Document("nested", "A string"))
  )
).results()
// end-error
// start-no-key
case class User(email: String)

val mongoClient3: MongoClient = MongoClient("<your connection string>")
val database3: MongoDatabase = mongoClient.getDatabase("<your database>")
val collection3: MongoCollection[Document] = database.getCollection[Document]("<your collection>")

val findObservable: FindObservable[Document] = collection.find(new Document("age", "Accepts any type!"))
// end-no-key
