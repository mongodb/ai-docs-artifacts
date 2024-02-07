import org.mongodb.scala.{MongoClient, Document}
import scala.concurrent.Await
import scala.concurrent.duration._
import scala.util.{Failure, Success}

// Enter your MongoDB cluster URL, username, and password
val username = "<username>"
val password = "<password>"
val clusterUrl = "<MongoDB cluster url>"

val authMechanism = "SCRAM-SHA-1"

val mongoClient: MongoClient = MongoClient(s"mongodb+srv://$username:$password@$clusterUrl/?authMechanism=$authMechanism")

try {
  val database = mongoClient.getDatabase("admin")
  val command = Document("ping" -> 1)
  val result = database.runCommand(command).head()

  Await.result(result, 10.seconds) // Waiting for the results from MongoDB server
  println("Connected successfully to server")
} catch {
  case t: Throwable => println("An error has occurred: " + t.getMessage)
} finally {
  mongoClient.close()
}
