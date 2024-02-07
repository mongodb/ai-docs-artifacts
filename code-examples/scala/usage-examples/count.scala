import org.mongodb.scala._

object CountDocuments {

  private val mongoClient: MongoClient = MongoClient("<connection string uri>")
  private val database: MongoDatabase = mongoClient.getDatabase("sample_mflix")
  private val movies: MongoCollection[Document] = database.getCollection("movies")

  def run(): Unit = {
    val countFuture = movies.count().head()

    countFuture.foreach((count: Long) => println(s"Estimated number of documents in the movies collection: $count"))

    val filter = Document("countries" -> "Canada")

    val countCanadaFuture = movies.countDocuments(filter).head()

    countCanadaFuture.foreach((count: Long) => println(s"Number of movies from Canada: $count"))
  }
}

// Run the code with safety
try {
  CountDocuments.run()
} catch {
  case e: Throwable => e.printStackTrace()
} finally {
  CountDocuments.mongoClient.close()
}
