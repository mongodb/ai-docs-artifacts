import org.mongodb.scala._

// Replace the uri string with your MongoDB deployment's connection string
val uri: String = "<connection string uri>"

val client: MongoClient = MongoClient(uri)

try {

  // Get the "sample_mflix" database
  val db: MongoDatabase = client.getDatabase("sample_mflix")

  // Find and print the storage statistics for the "sample_mflix" database using the 'dbStats' command
  val result = db.runCommand(Document("dbStats" -> 1)).headResult()

  println(result)

} catch {

  // Print the stack trace of any errors that occurred
  case e: Throwable => e.printStackTrace

} finally {

  // Close the database connection on completion or error
  client.close()

}
