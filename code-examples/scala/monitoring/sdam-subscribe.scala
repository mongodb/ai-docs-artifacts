import org.mongodb.scala._

// Replace the following with your MongoDB deployment's connection string
val uri: String = "mongodb+srv://<clusterUrl>/?replicaSet=rs&writeConcern=majority"

val mongoClient: MongoClient = MongoClient(uri)

// Replace <event name> with the name of the event you are subscribing to
val eventName: String = "<event name>"

// Subscribe to a specified event and print a message when the event is received
mongoClient.getClusterDescription().addPropertyChangeListener((event: PropertyChangeEvent) => {
  if (event.getPropertyName() == eventName) {
    println(s"received ${eventName}: ${event.getNewValue()}")
  }
})

// Define the operation in observable
val observable: Observable[Document] = mongoClient.getDatabase("admin").runCommand(Document("ping" -> 1))

// Run and establish a connection to the database
observable.subscribe(new Observer[Document] {
  override def onNext(result: Document): Unit = {
    println("Connected successfully")
  }

  override def onError(e: Throwable): Unit = {
    println(s"Failed: ${e.getMessage()}")
  }

  override def onComplete(): Unit = {
    println("Completed")
    mongoClient.close()
  }
})
