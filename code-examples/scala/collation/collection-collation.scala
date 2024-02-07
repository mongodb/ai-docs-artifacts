// Import necessary MongoDB classes
import org.mongodb.scala._
import org.mongodb.scala.model._
import org.mongodb.scala.model.CreateCollectionOptions

// Instantiate the MongoClient and get the database
val mongoClient: MongoClient = MongoClient()
val database: MongoDatabase = mongoClient.getDatabase("mydb")

// Create the "souvenirs" collection with a French Canadian collation
database.createCollection(
  "souvenirs",
  CreateCollectionOptions().collation(Collation.builder().locale("fr_CA").build())
)
