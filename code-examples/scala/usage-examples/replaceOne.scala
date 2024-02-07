import org.mongodb.scala._

// Replace the uri string with your MongoDB deployment's connection string.
val uri: String = "<connection string uri>"

val mongoClient: MongoClient = MongoClient(uri)
val database: MongoDatabase = mongoClient.getDatabase("sample_mflix");
val movies: MongoCollection[Document] = database.getCollection("movies");

val random = new scala.util.Random

val doc: Document = Document("title" -> ("The Cat from Sector " + (Math.floor(random.nextFloat() * 1000) + 1)))
val titleFilter: Document = Document("title" -> Document("$regex" -> "The Cat from"))
val updateFuture: SingleObservable[UpdateResult] = movies.replaceOne(titleFilter, doc)

updateFuture.subscribe(new Observer[UpdateResult] {
  override def onNext(result: UpdateResult): Unit = println("Modified " + result.getModifiedCount + " document(s)")
  override def onError(e: Throwable): Unit = println("Error: " + e)
  override def onComplete(): Unit = {
    mongoClient.close()
    println("Completed")
  }
})
