import org.mongodb.scala._

val mongoClient: MongoClient = MongoClient("mongodb://localhost:27017/test?maxPoolSize=5000")

mongoClient.listDatabaseNames().results()
