import org.mongodb.scala._
import org.mongodb.scala.bson.collection.immutable.Document
import org.mongodb.scala.model.Filters._
import org.mongodb.scala.model.Updates._
import scala.concurrent.Await
import scala.concurrent.duration._
import scala.concurrent.ExecutionContext.Implicits.global

val mongoClient: MongoClient = MongoClient("<connection string uri>")
val database: MongoDatabase = mongoClient.getDatabase("testdb")

val inventoryCollection: MongoCollection[Document] = database.getCollection("inventory")
val ordersCollection: MongoCollection[Document] = database.getCollection("orders")

val inventoryData: Seq[Document] = Seq(
  Document("item" -> "sunblock", "qty" -> 85, "price" -> 6.0),
  Document("item" -> "beach chair", "qty" -> 30, "price" -> 25.0)
)
inventoryCollection.insertMany(inventoryData)

val order: Seq[Document] = Seq(
  Document("item" -> "sunblock", "qty" -> 3),
  Document("item" -> "beach chair", "qty" -> 1)
)

val session: ClientSession = mongoClient.startSession()

try {
  session.startTransaction()

  var total: Double = 0

  order.foreach { item =>
    val updateResult = Await.result(inventoryCollection.findOneAndUpdate(
      session,
      and(equal("item", item.getString("item")), gte("qty", item.getInteger("qty"))),
      inc("qty", -item.getInteger("qty"))
    ).head(), 10.seconds)

    if (updateResult == null) {
      session.abortTransaction()
      println("Item not found or insufficient quantity.")
      return
    }

    val subTotal: Double = item.getInteger("qty") * updateResult.getDouble("price")
    total = total + subTotal
  }

  val receipt = Document(
    "date" -> java.time.LocalDateTime.now(),
    "items" -> order,
    "total" -> total
  )

  Await.result(ordersCollection.insertOne(session, receipt).head(), 10.seconds)

  println(
    "Order successfully completed and recorded!\nReceipt:\n" +
      receipt.toJson()
  )

  session.commitTransaction()
} catch {
  case e: Exception =>
    println("Error processing transaction", e)
    session.abortTransaction()
} finally {
  session.close()
  mongoClient.close()
}
