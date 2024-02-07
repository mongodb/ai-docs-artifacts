import org.mongodb.scala._

object MongoApp {

  val mongoClient: MongoClient = MongoClient("mongodb+srv://<user>:<password>@<cluster-url>?writeConcern=majority")
  val database: MongoDatabase = mongoClient.getDatabase("sample_mflix")
  val collection: MongoCollection[Document] = database.getCollection("movies")

  def runMongoApp(): Unit = {
    val indexResultObservable = collection.createIndex(
      Document("type" -> 1, "genre" -> 1)
    )

    indexResultObservable.subscribe((indexName: String) =>
      println(s"Index created: $indexName")
    )

    val query = Document("type" -> "movie", "genre" -> "Drama")
    val sort = Document("type" -> 1, "genre" -> 1)
    val projection = Document("_id" -> 0, "type" -> 1, "genre" -> 1)

    val findObservable = collection
      .find(query)
      .sort(sort)
      .projection(projection)

    findObservable.subscribe(
      (doc: Document) => println(doc.toJson()),
      (e: Throwable) => e.printStackTrace(),
      () => println("Operation Completed")
    )
  }

  def close(): Unit = {
    mongoClient.close()
  }

  def main(args: Array[String]): Unit = {
    try {
      runMongoApp()
    } catch {
      case e: Throwable => e.printStackTrace()
    } finally {
      close()
    }
  }

}
