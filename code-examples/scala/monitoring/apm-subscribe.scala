import org.mongodb.scala._
import com.mongodb.event._

// Change the following with your MongoDB deployment's connection string
val uri: String = "mongodb+srv://<clusterUrl>/?replicaSet=rs&writeConcern=majority"

val mongoClientSettings: MongoClientSettings = MongoClientSettings.builder
  .applyConnectionString(new ConnectionString(uri))
  .addCommandListener(new CommandListener {
    override def commandSucceeded(event: CommandSucceededEvent): Unit = {
      println(s"received ${event.getName()}: ${event.getResponse.toJson()}")
    }
  })
  .build
val mongoClient: MongoClient = MongoClient(mongoClientSettings)

try {
  // Establish and verify connection to the "admin" database
  mongoClient.getDatabase("admin").runCommand(Document("ping" -> 1))
  println("Connected successfully")
} catch {
  case e: Throwable => println(e)
} finally {
  // Close the database connection on completion or error
  mongoClient.close()
}
