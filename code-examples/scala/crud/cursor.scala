import org.mongodb.scala._

// Use MongoClient to create a connection
val mongoClient: MongoClient = MongoClient("mongodb+srv://<user>:<password>@<cluster-url>?writeConcern=majority")

// Connect to a database
val database: MongoDatabase = mongoClient.getDatabase("test")

// Retrieve a collection
val orders: MongoCollection[Document] = database.getCollection("orders")

// Asynchronous iteration by using cursor methods
def asyncIteration(): Unit = {
  val cursor: FindObservable[Document] = orders.find()
  cursor.subscribe((doc: Document) => println(doc.toJson()))
}

// Manual iteration using cursor methods
def manualIteration(): Unit = {
  val cursor: FindObservable[Document] = orders.find()
  cursor.foreach(doc => println(doc.toJson()))
}

// Fetch all documents as an array using the toArray method
def fetchAll(): Unit = {
  val cursor: FindObservable[Document] = orders.find()

  cursor.toFuture().foreach { docs =>
    println(s"The collection contains ${docs.length} documents.")
  }
}


// Rewind the cursor (not directly possible in the MongoDB Scala Driver)
def rewind(): Unit = {}

// Close a cursor (nearly not needed in MongoDB Scala driver)
def close(): Unit = {}

def run(): Unit = {
  asyncIteration()
  manualIteration()
  fetchAll()
  mongoClient.close()
}

run()
