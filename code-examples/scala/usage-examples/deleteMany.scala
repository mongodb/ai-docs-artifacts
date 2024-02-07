// Need to import necessary packages
import org.mongodb.scala._
import org.mongodb.scala.model.Filters.regex
import scala.concurrent.ExecutionContext.Implicits.global

// Replace with your MongoDB connection string
val mongoClient: MongoClient = MongoClient("<connection string uri>")

val database: MongoDatabase = mongoClient.getDatabase("sample_mflix")
val collection: MongoCollection[Document] = database.getCollection("movies")

// The filter to delete documents that have "Santa" in their title
val filter = regex("title", "Santa")

collection.deleteMany(filter).subscribe(new Observer[DeleteResult] {
  override def onNext(result: DeleteResult): Unit = println("Deleted " + result.getDeletedCount + " documents")
  override def onError(e: Throwable): Unit = e.printStackTrace()
  override def onComplete(): Unit = println("Operation Complete")
})

// Close the MongoDB connection
mongoClient.close()
