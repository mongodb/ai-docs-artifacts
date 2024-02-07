import org.mongodb.scala._

object MongoDBConnection {
  // Replace with values for your environment.
  val username: String = "<username>"
  val password: String = "<password>"
  val clusterUrl: String = "<MongoDB cluster url>"

  // Connection related configurations
  val settings: MongoClientSettings = MongoClientSettings.builder()
    .applyConnectionString(
      ConnectionString(
        s"mongodb+srv://${java.net.URLEncoder.encode(username, "UTF-8")}:${java.net.URLEncoder.encode(password, "UTF-8")}@$clusterUrl/?authMechanism=DEFAULT"
      )
    )
    .build()

  // Create a new MongoClient
  val mongoClient: MongoClient = MongoClient(settings)

  // Function to connect to the server
  def connectToDB(): Unit = {
    try {
      // Establish and verify the connection
      mongoClient
        .getDatabase("admin")
        .runCommand(Document("ping" -> 1))
        .printHeadResult("Connected successfully to server")
    } finally {
      // Ensures that the client will close when you finish/error
      mongoClient.close()
    }
  }
  def main(args: Array[String]): Unit = {
    try {
      connectToDB()
    } catch {
      case e: Exception =>
        println(e.printStackTrace())
    }
  }
}
