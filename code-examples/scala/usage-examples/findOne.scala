import org.mongodb.scala._

case class IMDB(rating: Int, votes: Int, id: Int)

trait Movie {
  val title: String
  val year: Int
  val released: java.util.Date
  val plot: String
  val `type`: String
  val imdb: IMDB
}

type MovieSummary = (String, IMDB)

val uri = "<connection string uri>"
val mongoClient: MongoClient = MongoClient(uri)
val database = mongoClient.getDatabase("sample_mflix")
val collection = database.getCollection("movies")

def findMovie(): Unit = {
  val movie = collection.find(equal("title", "The Room"))
    .sort(descending("rating"))
    .projection(fields(include("title", "imdb"), excludeId()))
    .first()
    
  movie.subscribe(new Observer[Document] {
    override def onNext(result: Document): Unit = println(result)
    override def onError(e: Throwable): Unit = println(s"Failed: $e")
    override def onComplete(): Unit = println("Completed")
  })
}

findMovie()

mongoClient.close()
