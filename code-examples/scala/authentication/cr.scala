import org.mongodb.scala.{MongoClient, MongoDatabase, MongoCredential, ServerAddress}
import scala.concurrent.Await
import scala.concurrent.duration._
import scala.language.postfixOps

// Replace the following with values for your environment.
val username: String = "<username>"
val password: String = "<password>"
val clusterUrl: String = "<MongoDB cluster url>"

// Create credentials for authentication
val credential: MongoCredential = MongoCredential.createScramSha1Credential(username, "admin", password.toCharArray)

// Replace the following with your MongoDB deployment's connection string.
val uri: String = s"mongodb+srv://$username:$password@$clusterUrl/?authMechanism=SCRAM-SHA-1&ssl=true"

// Create a new MongoClient
val mongoClient: MongoClient = MongoClient(uri)

// Get a handle to the "admin" database
val database: MongoDatabase = mongoClient.getDatabase("admin")

// Function to connect to the server
def run(): Unit = {
  try {
    // Establish and verify connection
    val pingCommand: String = """{ ping: 1 }"""
    val commandFuture = database.runCommand(Document(pingCommand))

    // Block for the results of the command to prevent the driver outpacing the console
    Await.result(commandFuture.toFuture(), 2 seconds)

    println("Connected successfully to server")
  } finally {
    // Ensures that the client will close when you finish/error
    mongoClient.close()
  }
}

run()
