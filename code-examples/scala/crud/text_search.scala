import org.mongodb.scala._
import scala.concurrent.ExecutionContext.Implicits.global
import scala.util.{Failure, Success}

object MongoDBSearch {

  val mongoClient: MongoClient = MongoClient("mongodb+srv://<user>:<password>@<cluster-url>?writeConcern=majority")
  val database: MongoDatabase = mongoClient.getDatabase("sample_mflix")
  val movies: MongoCollection[Document] = database.getCollection("movies")

  def searchWord(): Unit = {
    val query: Document = Document("$text" -> Document("$search" -> "trek"))
    val projection: Document = Document("_id" -> 0, "title" -> 1)
    val cursor: FindObservable[Document] = movies.find(query).projection(projection)

    cursor.toFuture().onComplete {
      case Failure(e) => e.printStackTrace()
      case Success(results) => results.foreach(println)
    }
  }

  def searchPhrase(): Unit = {
    val query: Document = Document("$text" -> Document("$search" -> "\"star trek\""))
    val projection: Document = Document("_id" -> 0, "title" -> 1)
    val cursor: FindObservable[Document] = movies.find(query).projection(projection)

    cursor.toFuture().onComplete {
      case Failure(e) => e.printStackTrace()
      case Success(results) => results.foreach(println)
    }
  }

  def searchNegation(): Unit = {
    val query: Document = Document("$text" -> Document("$search" -> "\"star trek\" -\"into darkness\""))
    val projection: Document = Document("_id" -> 0, "title" -> 1)
    val cursor: FindObservable[Document] = movies.find(query).projection(projection)

    cursor.toFuture().onComplete {
      case Failure(e) => e.printStackTrace()
      case Success(results) => results.foreach(println)
    }
  }

  def searchRelevance(): Unit = {
    val query: Document = Document("$text" -> Document("$search" -> "\"star trek\" -\"into darkness\""))
    val sort: Document = Document("score" -> Document("$meta" -> "textScore"))
    val projection: Document = Document("_id" -> 0, "title" -> 1, "score" -> Document("$meta" -> "textScore"))
    val cursor: FindObservable[Document] = movies.find(query).sort(sort).projection(projection)

    cursor.toFuture().onComplete {
      case Failure(e) => e.printStackTrace()
      case Success(results) => results.foreach(println)
    }
  }

  def main(args: Array[String]): Unit = {
    searchWord()
    searchPhrase()
    searchNegation()
    searchRelevance()

    // Close the connection to the MongoDB instance
    mongoClient.close()
  }
}
