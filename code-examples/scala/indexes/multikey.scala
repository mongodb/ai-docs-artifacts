import org.mongodb.scala._
import scala.concurrent.ExecutionContext.Implicits.global
import scala.util.{Failure, Success}

// Replace the placeholders with your credentials
val mongoClient: MongoClient = MongoClient("mongodb://<user>:<password>@<cluster-url>?writeConcern=majority")

val database: MongoDatabase = mongoClient.getDatabase("sample_mflix")
val collection: MongoCollection[Document] = database.getCollection("movies")

// Create a multikey index on the "cast" field in the "movies" collection
val indexResult = collection.createIndex(Document("cast" -> 1)).head()

indexResult.onComplete {
  case Success(indexName) => println(s"Index created: $indexName")
  case Failure(failure) => println(s"Failed: $failure")
}

// Prepare a query to find movies where "Viola Davis" is in the cast
val query: Document = Document("cast" -> "Viola Davis")

// Specify the fields we want to return
val projection: Document = Document("_id" -> 0, "cast" -> 1 , "title" -> 1)

val cursor: FindObservable[Document] = collection.find(query).projection(projection)

cursor.subscribe((doc: Document) => println(doc.toJson()))

// Close the MongoClient, this is scala so the client will not close automatically
mongoClient.close()
