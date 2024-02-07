import org.mongodb.scala._

// Replace the uri string with your MongoDB deployment's connection string
val uri: String = "<connection-string-uri>"

// Create a new client and connect to MongoDB
val mongoClient: MongoClient = MongoClient(uri)

// Connect to the "testDB" database
val database: MongoDatabase = mongoClient.getDatabase("testDB")

// Create a command for checking metadata consistency
val command: Document = Document("checkMetadataConsistency" -> 1)

// Run the command and get the cursor for the results
val cursor: FindObservable[Document] = database.runCommand(command)

// Use the cursor to fetch results and print them
try {
  cursor.subscribe((doc: Document) => println(doc.toJson))
} catch {
  case e: Throwable => 
    Console.err.println(e)
} finally {
  // Close the MongoDB client connection
  mongoClient.close()
}
