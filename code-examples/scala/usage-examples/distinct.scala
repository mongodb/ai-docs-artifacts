import org.mongodb.scala._

// Replace the uri string with your MongoDB deployment's connection string.
val uri = "<connection string uri>"

val mongoClient: MongoClient = MongoClient(uri)

val database: MongoDatabase = mongoClient.getDatabase("sample_mflix")

val movies: MongoCollection[Document] = database.getCollection("movies")

val observable = movies.distinct("year", classOf[Int], Filters.eq("directors", "Barbra Streisand"))

// Attach an observer
observable.subscribe(new Observer[Int] {
  override def onNext(year: Int): Unit = println(year)
  override def onError(e: Throwable): Unit = e.printStackTrace()
  override def onComplete(): Unit = println("Operation completed")
})

// Don't forget to close the connection
mongoClient.close()
