import org.mongodb.scala._

object ListIndexes {
  def main(args: Array[String]): Unit = {
    // Replace the placeholders with your credentials
    val connectionString: String = "mongodb+srv://<user>:<password>@<cluster-url>?writeConcern=majority"

    val mongoClient: MongoClient = MongoClient(connectionString)

    // Access a collection from a database
    val database: MongoDatabase = mongoClient.getDatabase("<databaseName>")
    val collection: MongoCollection[Document] = database.getCollection("<collectionName>")

    // List the indexes on the collection and output them as an array
    val listIndexesObservable: Observable[Document] = collection.listIndexes()

    listIndexesObservable.subscribe(new Observer[Document] {
      override def onNext(result: Document): Unit = println(result.toJson())

      override def onError(e: Throwable): Unit = println(s"Failed: $e")

      override def onComplete(): Unit = println("Completed")

    })

    // Close the connection after the operation completes
    mongoClient.close()
  }
}
