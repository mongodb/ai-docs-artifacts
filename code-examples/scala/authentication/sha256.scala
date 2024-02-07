import org.mongodb.scala._

object MongoConnect {
  def main(args: Array[String]): Unit = {
    // Replace the following with values for your environment.
    val username = "<username>"
    val password = "<password>"
    val clusterUrl = "<MongoDB cluster url>"

    val authMechanism = "SCRAM-SHA-256"

    // Replace the following with your MongoDB deployment's connection string.
    val uri =
      s"mongodb+srv://${username}:${password}@${clusterUrl}/?authMechanism=${authMechanism}"

    // Create a new MongoClient
    val client = MongoClient(uri)
    
    // Function to connect to the server
    def run(): Unit = {
      client.getDatabase("admin").runCommand(Document("ping" -> 1)).collect().subscribe((result: Seq[Document]) => {
        println("Connected successfully to server")
      }, (e: Throwable) => e.printStackTrace(), () => client.close())
    }
    run()
  }
}
