import org.mongodb.scala._
import scala.concurrent.Await
import scala.concurrent.duration._
import scala.concurrent.ExecutionContext.Implicits.global

object Main {
  def main(args: Array[String]): Unit = {
    val uri = "mongodb+srv://<user>:<password>@<cluster-url>?retryWrites=true&w=majority"
    
    val mongoClient: MongoClient = MongoClient(uri)
    val database: MongoDatabase = mongoClient.getDatabase("sample_mflix")
    val collection: MongoCollection[Document] = database.getCollection("movies")

    val indexModel: IndexModel = IndexModel(Document("theaterId" -> 1), IndexOptions().unique(true))

    val observable = collection.createIndexes(Seq(indexModel))

    observable.subscribe(
      (createdIndexName: String) => println(s"Index Created: $createdIndexName"),
      (e: Throwable) => println(s"Error: $e"),
      () => mongoClient.close()
    )
    
    Await.result(observable.toFuture(), 10.seconds)
  }
}
