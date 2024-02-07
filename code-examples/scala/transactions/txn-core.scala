import org.mongodb.scala._

object FullMongoDBTransaction extends App {

  val mongoClient: MongoClient = MongoClient()
  val testDb: MongoDatabase = mongoClient.getDatabase("testdb")

  def cleanUp(): Unit = {
    List("customers", "inventory", "orders").foreach(testDb.getCollection(_).drop().results())
  }

  def setup(): Unit = {
    val customerColl = testDb.getCollection("customers")
    val inventoryColl = testDb.getCollection("inventory")

    customerColl.insertOne(Document("_id" -> 98765, "orders" -> List())).results()
    inventoryColl.insertMany(List(
      Document("item" -> "sunblock", "item_id" -> 5432, "qty" -> 85),
      Document("item" -> "beach towel", "item_id" -> 7865, "qty" -> 41)
    )).results()
  }

  def queryData(): Unit = {
    List("customers", "inventory", "orders").foreach(coll => 
      println(testDb.getCollection(coll).find().results()))
  }

  def placeOrder(purchaseOrder: List[Document], payment: Document): Unit = {
    val session = mongoClient.startSession()

    try {
      session.startTransaction()
      
      val ordersColl = testDb.getCollection("orders")
      val inventoryColl = testDb.getCollection("inventory")
      val customerColl = testDb.getCollection("customers")
  
      val orderResult = ordersColl.insertOne(Document("customer" -> payment("customer"), "items" -> purchaseOrder, "total" -> payment("total"))).results()
  
      purchaseOrder.foreach(item => {
        val inStock = inventoryColl.findOneAndUpdate(
          Filters.and(Filters.equal("item_id", item("item_id")), Filters.gte("qty", item("qty"))),
          Updates.inc("qty", -item("qty"))
        )
        if (inStock == null) throw new Exception("Insufficient quantity or item ID not found.")
      })
  
      customerColl.updateOne(
        Filters.equal("_id", payment("customer")), 
        Updates.push("orders", orderResult)
      ).results()
  
      session.commitTransaction()
      println("Transaction successfully committed.")
      
    } catch {
      case _: MongoException => session.abortTransaction()
      case e: Exception => println(s"An error occured in the transaction, performing a data rollback: ${e.getMessage}")
    } finally {
      session.close()
    }
  }

  def run(): Unit = {
    cleanUp()
    setup()

    val cart = List(
      Document("item" -> "sunblock", "item_id" -> 5432, "qty" -> 1, "price" -> 5.19),
      Document("item" -> "beach towel", "item_id" -> 7865, "qty" -> 2, "price" -> 15.99)
    )
    val payment = Document("customer" -> 98765, "total" -> 37.17)

    placeOrder(cart, payment)
    cleanUp()

    mongoClient.close()
  }

  run()
  queryData()
}
```
Note: This code supposes that an implicit conversion from the Individual Observable to a blocking Iterable is in the scope to handle blocking operations. For instance, you can define this conversion as:

```scala
import scala.concurrent.Await
import scala.concurrent.duration.Duration

implicit class DocumentObservable[C](val observable: Observable[Document]) extends ImplicitObservable[Document] {
  override val converter: (Observable[Document]) => Seq[Document] = (obs: Observable[Document]) => Await.result(obs.toFuture(), Duration(10, "second"))
}

abstract class ImplicitObservable[C] {
  val observable: org.mongodb.scala.Observable[C]
  implicit def converter: Observable[C] => Seq[C]
}
