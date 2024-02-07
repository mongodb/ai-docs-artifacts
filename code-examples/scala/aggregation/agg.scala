import org.mongodb.scala._
import scala.concurrent.Await
import scala.concurrent.duration._
import scala.util.{Failure, Success}

val mongoClient: MongoClient = MongoClient(System.getenv("MONGODB_URI"))
val database = mongoClient.getDatabase("aggregation")
val collection = database.getCollection("restaurants")

// Begin data insertion
// Create sample documents
val docs = Seq(
  Document("_id" -> 0, "stars" -> 3, "categories" -> Seq("Bakery", "Sandwiches"), "name" -> "Rising Sun Bakery"),
  Document("_id" -> 1, "stars" -> 4, "categories" -> Seq("Bakery", "Cafe", "Bar"), "name" -> "Cafe au Late"),
  Document("_id" -> 2, "stars" -> 5, "categories" -> Seq("Coffee", "Bakery"), "name" -> "Liz's Coffee Bar"),
  Document("_id" -> 3, "stars" -> 3, "categories" -> Seq("Steak", "Seafood"), "name" -> "Oak Steakhouse"),
  Document("_id" -> 4, "stars" -> 4, "categories" -> Seq("Bakery", "Dessert"), "name" -> "Petit Coookie")
)

// Insert documents into the restaurants collection
val futureResult = collection.insertMany(docs)

futureResult.onComplete {
    case Success(result) => println(s"Successfully inserted ${result.getInsertedCount} documents")
    case Failure(e) => println(s"Failed to insert documents: $e")
}

// End data insertion

// Begin aggregation
// Define an aggregation pipeline with a match stage and a group stage
val pipeline = Seq(
  Document("$match" -> Document("categories" -> "Bakery")),
  Document("$group" -> Document("_id" -> "$stars", "count" -> Document("$sum" -> 1)))
)

// Execute the aggregation
val futureAggResult = collection.aggregate(pipeline).toFuture()

// Print the aggregated results
futureAggResult.map { docs =>
  docs.foreach(doc => println(doc.toJson()))
}.recover { case t => println(s"Exception: $t") }

mongoClient.close()  // Whether aggregation was successful or not, close the client connection
