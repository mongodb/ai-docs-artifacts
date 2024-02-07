import org.mongodb.scala._
import scala.concurrent.ExecutionContext.Implicits.global
import scala.util.{Failure, Success}

// Replace the placeholders with your MongoDB deployment's credentials
val mongoClient: MongoClient = MongoClient("mongodb+srv://<user>:<password>@<cluster-url>?writeConcern=majority")
val database = mongoClient.getDatabase("sample_mflix")
val movies = database.getCollection("movies")

val observable = movies.createIndex(Document("location.geo" -> "2dsphere"))

// Running the program and printing the result of the index creation
observable.subscribe(new Observer[String] {
  override def onNext(result: String): Unit = println(s"Index created: $result")
  override def onError(e: Throwable): Unit = println(s"Failed: $e")
  override def onComplete(): Unit = {
    println("Operation Completed")
    mongoClient.close() // Close the connection after the operation completes
  }
})
