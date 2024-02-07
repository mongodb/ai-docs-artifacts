import org.mongodb.scala._
import scala.util.{Failure, Success}
import scala.concurrent.ExecutionContext.Implicits.global

val clusterUrl = "<MongoDB deployment url>"
val authMechanism = "MONGODB-AWS"

val uri = s"mongodb+srv://$clusterUrl/?authSource=%24external&authMechanism=$authMechanism"

val settings: MongoClientSettings = MongoClientSettings.builder()
                                    .applyConnectionString(new ConnectionString(uri))
                                    .build()

val client: MongoClient = MongoClient(settings)

val db = client.getDatabase("admin")
val command = Document("ping" -> 1)

db.runCommand(command).toFuture().onComplete {
  case Success(_) => println("Connected successfully to server.")
  case Failure(e) => e.printStackTrace()
}
client.close()
