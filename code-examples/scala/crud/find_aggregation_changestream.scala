import org.mongodb.scala._
import org.mongodb.scala.model._
import org.mongodb.scala.model.Aggregates._
import org.mongodb.scala.model.filters._

import scala.concurrent.duration.Duration
import scala.concurrent.{Await, Future}


val mongoClient: MongoClient = MongoClient("mongodb://[username:password@ds000000.mlab.com:00000/db-name]")
val database: MongoDatabase = mongoClient.getDatabase("test")
val orders: MongoCollection[Document] = database.getCollection("orders")

def findOrderByNameAndDate(): Future[Seq[Document]] = {
  orders.find(
    Filters.and(
      Filters.eq("name", "Lemony Snicket"),
      Filters.gte("duration", 0), 
      Filters.lt("duration", 24*60)
    )
  )
  .into[Document](Seq())
  .toFuture()
}

def aggregateOrdersByStatus(): Future[Seq[Document]] = {
  orders.aggregate(Seq(
    `match`(Filters.and(Filters.gte("duration", 0), Filters.lt("duration", 24*60))),
    group("$status", Accumulators.sum("count", 1))
  ))
  .into[Document](Seq())
  .toFuture()
}

val changeStream: ChangeStreamObservable[Document] = 
  orders.watch(Seq(
    Aggregates.`match`(Filters.eq("operationType", "insert")),
    project(Document("fullDocument.name" -> 1, "fullDocument.address" -> 1))
  ))
changeStream.subscribe((change: ChangeEvent[Document]) => println(change))

def insertOrder() {
  orders.insertOne(Document(
    "date" -> java.util.Date, 
    "address" -> "667 Dark Avenue, San Francisco, CA, 94110",
    "name" -> "Lemony Snicket",
    "items" -> Seq(
      Document("name" -> "pizza", "toppings" -> Seq("pepperoni", "mushrooms"), "notes" -> "extra shrooms", "price" -> 11),
      Document("name" -> "wings", "sauce" -> "buffalo", "notes" -> "extra crispy", "price" -> 10)
    ),
    "status" -> "created"
  ))
}

def updateOrderAddress() {
  orders.updateOne(
    Filters.and(
      Filters.eq("address", "667 Dark Avenue, San Francisco, CA, 94110"),
      Filters.gte("duration", 0), 
      Filters.lt("duration", 24*60)
    ),
    Updates.set("address", "667 Dark Avenue, San Francisco, CA, 94103")
  )
}

def deleteOrder() {
  orders.deleteOne(
    Filters.and(
      Filters.eq("address", "13 Lousy Lane"),
      Filters.eq("name", "Violet Baudelaire"),
      Filters.gte("duration", 0), 
      Filters.lt("duration", 24*60)
    )
  )
}

def run() {
  Await.result(findOrderByNameAndDate(), Duration.Inf).foreach(println)
  Await.result(aggregateOrdersByStatus(), Duration.Inf).foreach(println)
  
  Thread.sleep(1000) // sleep 1 second to allow change stream to instantiate

  insertOrder()
  updateOrderAddress()
  deleteOrder()
  
  mongoClient.close()
}

run()
