// Delete a document

import org.mongodb.scala._

// Replace the uri string with your MongoDB deployment's connection string
val uri: String = "<connection string uri>"

val mongoClient: MongoClient = MongoClient(uri)
val database: MongoDatabase = mongoClient.getDatabase("sample_mflix")
val collection: MongoCollection[Document] = database.getCollection("movies")

val query: Document = Document("title" -> "Annie Hall")

collection.deleteOne(query).subscribe(new Observer[DeleteResult] {
  override def onNext(result: DeleteResult): Unit = {
    if (result.getDeletedCount == 1) {
      println("Successfully deleted one document.")
    } else {
      println("No documents matched the query. Deleted 0 documents.")
    }
  }

  override def onError(e: Throwable): Unit = println(s"Error: $e")
  
  override def onComplete(): Unit = {
    println("Operation completed")
    mongoClient.close()
  }
})
