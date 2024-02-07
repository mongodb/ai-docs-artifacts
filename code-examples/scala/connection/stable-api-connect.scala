import org.mongodb.scala._

import scala.concurrent.ExecutionContext.Implicits.global
import scala.util.{Failure, Success}

object MongoDBScalaClient { 
  def main(args: Array[String]): Unit = {
    
    val settings: MongoClientSettings = MongoClientSettings.builder()
      .applyConnectionString(new ConnectionString("<connection string>"))
      .serverApi(new ServerApi.Builder().version(ServerApiVersion.V1).strict(true).deprecationErrors(true).build())
      .build()

    val mongoClient: MongoClient = MongoClient(settings)
     
    val db = mongoClient.getDatabase("admin")
    
    val command = new org.bson.Document("ping", 1)
    db.runCommand(command).toFuture().onComplete {
      case Success(response) => println("Pinged your deployment. You successfully connected to MongoDB!")
      case Failure(e) => println(s"An error has occured: $e")
    }
    
    sys.addShutdownHook {
      mongoClient.close()
    }
  }
}
