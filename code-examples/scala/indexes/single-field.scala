import org.mongodb.scala._
import scala.concurrent.Await
import scala.concurrent.duration.Duration
import scala.language.postfixOps

object MongoDBApp extends App {

  val mongoClient: MongoClient = MongoClient("mongodb+srv://<username>:<password>@<cluster-url>?writeConcern=majority")
  val database: MongoDatabase = mongoClient.getDatabase("sample_mflix")
  val collection: MongoCollection[Document] = database.getCollection("movies")

  collection.createIndex(Document("title" -> 1)).subscribe(
    (result: String) => println(s"Index created: $result"),
    (error: Throwable) => println(s"Failed: $error"),
    () => println("Done")
  )

  val query: Document = Document("title" -> "Batman")
  val sort: Document = Document("title" -> 1)
  val projection: Document = Document("_id" -> 0, "title" -> 1)

  val task = collection.find(query).sort(sort).projection(projection).collect()

  Await.result(task.toFuture(), Duration(10, "second")) foreach println

  mongoClient.close()
}
