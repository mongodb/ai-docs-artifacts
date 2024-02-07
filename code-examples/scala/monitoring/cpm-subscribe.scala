import org.mongodb.scala._

// Replace the following with your MongoDB deployment's connection string
val uri: String = "mongodb+srv://<clusterUrl>/?replicaSet=rs&writeConcern=majority"

val client: MongoClient = MongoClient(uri)

// Replace <event name> with the name of the event you are subscribing to
val eventName: String  = "<event name>"

// Subscribe to the event
client.getDatabase("admin").watch().subscribe((event: ChangeEvent) => println("Received event: " + event), 
                                          (e: Throwable) => println("Error happened: " + e))

def run(): Unit = {
  try {
    // Establish and verify connection
    client.getDatabase("admin").runCommand(Document("ping" -> 1)).subscribe((result: Document) => println("Connected successfully!"),
                                                                        (e: Throwable) => println("Error happened during connection verification: " + e))
  } finally {
    // Ensures that the client will close when you finish/error
   client.close()
  }
}

try {
run()
} catch {
 case e: Exception => println("Error happened: " + e.getMessage)
}
