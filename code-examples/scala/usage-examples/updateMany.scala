import org.mongodb.scala._

object UpdateQueries {
  val mongoClient: MongoClient = MongoClient("<connection string uri>")
  val database: MongoDatabase = mongoClient.getDatabase("sample_mflix")
  val collection: MongoCollection[Document] = database.getCollection("movies")

  sealed trait Rating
  case object G extends Rating
  case object PG extends Rating
  case object PG_13 extends Rating
  case object R extends Rating
  case object NR extends Rating

  case class Movie(rated: Rating, random_review: Option[String] = None)

  def giveRandomReview(): String = 
    s"After viewing I am ${100 * scala.util.Random.nextDouble()}% more satisfied with life."

  def updateMovies() {
    val filter = Document("rated" -> G.toString())
    val update = Document("$set" -> Document("random_review" -> giveRandomReview()))
    
    collection.updateMany(filter, update).subscribe(
      (result: UpdateResult) => println(s"Updated ${result.getModifiedCount} documents"),
      (t: Throwable) => t.printStackTrace()
    )
  }

  def closeConnection() {
      mongoClient.close()
  }
}

object Main extends App {
  UpdateQueries.updateMovies()
  UpdateQueries.closeConnection()
}
