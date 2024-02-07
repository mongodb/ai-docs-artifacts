import org.mongodb.scala._

import scala.concurrent.ExecutionContext.Implicits.global
import scala.concurrent.Future

// Replace the placeholder with your connection string
val mongoConnectionString = "<connection string uri>"

// Substitute placeholders with your SOCKS5 proxy server details
val settings = MongoClientSettings.builder()
  .applyToClusterSettings(b => b.hosts(List(new ServerAddress("<host>", 1080)).asJava))
  .credential(MongoCredential.createCredential("<username>", "admin", "<password>".toCharArray))
  .build()

// Create a new client with the proxy server details
val mongoClient: MongoClient = MongoClient(mongoConnectionString, settings)

val database: MongoDatabase = mongoClient.getDatabase("myDB")
val collection: MongoCollection[Document] = database.getCollection("myColl")

def runQuery(): Future[Document] = {
  collection.find().first().head()
}

runQuery().onComplete(println)

mongoClient.close()
