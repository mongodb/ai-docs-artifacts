import org.mongodb.scala._

import scala.concurrent.Await
import scala.concurrent.duration._

// Replace the uri string with your MongoDB deployment's connection string.
val uri = "<connection string uri>"

val mongoClient: MongoClient = MongoClient(uri)

// Open a database and get the collection 'haikus'
val haikus = mongoClient.getDatabase("insertDB").getCollection("haikus")

// Create the change stream observable
val changeStreamObservable = haikus.watch()

// Define the observer
val observer = new Observer[ChangeStreamDocument[Document]] {
  override def onNext(result: ChangeStreamDocument[Document]): Unit = 
    println(s"Received change: $result")
    
  override def onError(e: Throwable): Unit = 
    System.err.println(s"Error: $e")
    
  override def onComplete(): Unit = 
    println("Change Stream completed")
}

// Subscribe the observer to the change stream observable
changeStreamObservable.subscribe(observer)

// Keep alive for a while to receive changes. Adjust according to use case.
Await.result(changeStreamObservable.toFuture(), 30.seconds)

// Close the MongoDB client connection
mongoClient.close()
