import org.mongodb.scala._

import scala.concurrent.ExecutionContext.Implicits.global
import scala.concurrent.Future
import scala.concurrent.duration._

/* Database connection details */
val mongoClient: MongoClient = MongoClient("<connection string uri>")
val database: MongoDatabase = mongoClient.getDatabase("insertDB")
val collection: MongoCollection[Document] = database.getCollection("haikus")

/* Simulate async pause */
def simulateAsyncPause(): Future[Unit] = {
    Future {
        Thread.sleep(1000)
    }
}

var changeStream: ChangeStreamObservable[Document] = _

/* Main logic */
try {
    
  /* Start listening to changes */
  changeStream = collection.watch()

  /* Set change stream listener */
  changeStream.subscribe((doc: ChangeEvent[Document]) => println(s"Received a change to the collection: $doc"))
  
  /* Simulate pause */
  simulateAsyncPause()

  /* Insert a new document  */
  val docToInsert: Document = new Document("title", "Record of a Shriveled Datum")
    .append("content", "No bytes, no problem. Just insert a document, in MongoDB")

  collection.insertOne(docToInsert).toFuture().onComplete(_ => ())

  /* Simulate another pause */
  simulateAsyncPause()
  
  /* Close the change stream */
  changeStream.unsubscribe()
  println("closed the change stream")

} catch {
  case e: Throwable => e.printStackTrace()
} finally {
  /* Close the database connection on completion or error */
  mongoClient.close()
}
