import org.mongodb.scala._

import scala.util.{Failure, Success}
import scala.concurrent.ExecutionContext.Implicits.global

// Replace the following with values for your environment.
val accessKeyId = "<AWS_ACCESS_KEY_ID>"
val secretAccessKey = "<AWS_SECRET_ACCESS_KEY>"
val clusterUrl = "<MongoDB cluster url>"

val authMechanism = "MONGODB-AWS"

var uri = 
  s"mongodb+srv://$accessKeyId:$secretAccessKey@$clusterUrl/?authSource=%24external&authMechanism=$authMechanism"

// Uncomment the following lines if your AWS authentication setup requires a session token.
// val sessionToken = "<AWS_SESSION_TOKEN>"
// uri = uri + s"&authMechanismProperties=AWS_SESSION_TOKEN:$sessionToken"

// Create a new Mongo client.
val mongoClient: MongoClient = MongoClient(uri)

def run() = {
  val pingCommand = new BsonDocument("ping", new BsonInt32(1))

  mongoClient.getDatabase("admin").runCommand(pingCommand).toFuture().onComplete {
    case Success(_) => println("Connected successfully to server.")
    case Failure(e) => println("Error occurred: " + e.getMessage)
  }

  mongoClient.close()
}

run()
