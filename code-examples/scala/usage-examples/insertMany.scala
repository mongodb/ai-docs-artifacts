import org.mongodb.scala._

case class Food(name: String, healthy: Boolean)

object MongoDBScala extends App {

  // Replace the uri string with your MongoDB deployment's connection string.
  val mongoClient: MongoClient = MongoClient("<connection string uri>")
  val database: MongoDatabase = mongoClient.getDatabase("insertDB")
  val collection: MongoCollection[Document] = database.getCollection("foods")

  val foodList: Seq[Food] = Seq(
    Food("cake", false),
    Food("lettuce", true),
    Food("donut", false)
  )

  val documents: Seq[Document] = foodList.map(food => Document("name" -> food.name, "healthy" -> food.healthy))

  val insertObservable = collection.insertMany(documents)

  insertObservable.subscribe(new Observer[Completed] {
    override def onNext(result: Completed): Unit = println(s"${result.getInsertedId} documents were inserted")
    override def onError(e: Throwable): Unit = println(s"Failed: $e")
    override def onComplete(): Unit = println("Completed")
  })

  // It's good practice to close the MongoClient instance when no longer needed
  // so that all the resources it is holding are properly cleaned up
  mongoClient.close()
}
