import org.mongodb.scala._

import scala.concurrent.Await
import scala.concurrent.duration.Duration
import scala.util.{Failure, Success}

object TextSearchBlog extends App {

  val mongoClient: MongoClient = MongoClient("mongodb+srv://<user>:<password>@<cluster-url>?writeConcern=majority")
  val database: MongoDatabase = mongoClient.getDatabase("testDB")
  val collection: MongoCollection[Document] = database.getCollection("blogPosts")

  import collection.BsonDocumentObservable

  import scala.concurrent.ExecutionContext.Implicits.global

  val indexFuture = collection.createIndex(Document("title" -> "text", "body" -> "text")).toFuture()
  
  indexFuture.onComplete {
    case Success(indexName) => {
      println(s"Index created: $indexName")

      val query = Document("$text" -> Document("$search" -> "life ahead"))
      val projection = Document("_id" -> 0, "title" -> 1)

      val resultFuture = collection.find(query).projection(projection).toFuture()

      resultFuture.onComplete {
        case Success(results) => results.foreach(doc => println(doc))
        case Failure(e) => println("Error: " + e)
      }

      Await.result(resultFuture, Duration.Inf)
      
    }
    case Failure(e) => println("Error: " + e)
  }

  Await.result(indexFuture, Duration.Inf)
  
  mongoClient.close()
}
