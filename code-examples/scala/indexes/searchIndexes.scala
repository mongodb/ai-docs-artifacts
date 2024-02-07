import org.mongodb.scala._
import org.mongodb.scala.bson.BsonDocument
import scala.concurrent.{Await, Future}
import scala.concurrent.duration._
import scala.util.{Failure, Success}

// Search Index operations 
object SearchIndexManager {

  val mongoClient: MongoClient = MongoClient("mongodb+srv://<user>:<password>@<cluster-url>?writeConcern=majority")

  val database: MongoDatabase = mongoClient.getDatabase("<databaseName>")
  val collection: MongoCollection[Document] = database.getCollection("<collectionName>")

  // Create a search index
  val index1: BsonDocument = BsonDocument("name" -> "search1",
    "definition" -> BsonDocument("mappings" -> BsonDocument("dynamic" -> true)))
  createSearchIndex(index1)

  // List search indexes
  listSearchIndexes()

  // Update a search index
  val index2: BsonDocument = BsonDocument("mappings" -> BsonDocument("dynamic" -> true, "fields" ->
    BsonDocument("description" -> BsonDocument("type" -> "string"))))
  updateSearchIndex("search1", index2)

  // Dropping (deleting) a search index
  dropSearchIndex("search1")

  mongoClient.close()

  def createSearchIndex(index: BsonDocument): Unit = {
    collection.createIndex(index)
  }

  def listSearchIndexes(): Unit = {
    val listSearchIndexes: Future[Seq[String]] = collection.listIndexes().map( doc => doc.toJson()).toFuture()
    listSearchIndexes onComplete {
      case Success(searchIndexes) => println("Existing search indexes:\n" + searchIndexes.mkString("\n"))
      case Failure(e) => println("Failed with: " + e)
    }
  }

  def updateSearchIndex(name: String, index: BsonDocument): Unit = {
    collection.dropIndex(name)
    collection.createIndex(index)
  }

  def dropSearchIndex(name: String): Unit = {
    collection.dropIndex(name)
  }
}

// Run the SearchIndexManager object
SearchIndexManager
