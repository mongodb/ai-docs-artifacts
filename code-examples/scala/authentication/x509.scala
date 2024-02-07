import org.mongodb.scala.{MongoClient, MongoCommandException, Observable}
import scala.concurrent.Await
import scala.concurrent.duration.Duration

// Replace the following with values for your environment.
val clusterUrl = "<MongoDB cluster url>"
val clientPEMFile = "<path to the client pem certificate file>"
val authMechanism = "MONGODB-X509"

// Replace the following with your MongoDB deployment's connection string.
val uri = s"mongodb+srv://$clusterUrl/?authMechanism=$authMechanism&tls=true&tlsCertificateKeyFile=$clientPEMFile"

// Create a new MongoClient
val mongoClient: MongoClient = MongoClient(uri)

// Observable helper
def getResults[T](obs: Observable[T]): Seq[T] = {
  Await.result(obs.toFuture(), Duration(10, "second"))
}

// Function to connect to the server
def run(): Unit = {
  try {
    // Establish and verify connection
    val pingCommand = getResults(mongoClient.getDatabase("admin").runCommand(Document("ping" -> 1)))
    println("Connected successfully to server")
  } catch {
    case e: MongoCommandException => println("Failed to connect to server: " + e)
  } finally {
    // Ensures that the client will close when you finish/error
    mongoClient.close()
  }
}

run()
