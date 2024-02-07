import org.mongodb.scala._
import scala.util.Random

// Replace the uri string with your MongoDB deployment's connection string
val mongoClient: MongoClient = MongoClient("<connection string uri>")

val database: MongoDatabase = mongoClient.getDatabase("sample_mflix")
val collection: MongoCollection[Document] = database.getCollection("movies")

case class Movie(plot: String, title: String)

val plotString = s"A harvest of random numbers, such as: ${Random.nextFloat()}"

val filter: Bson = Filters.eq("title", "Random Harvest")
val update: Bson = Updates.set("plot", plotString)

collection.updateOne(filter, update, new UpdateOptions().upsert(true)).subscribe(
  (updateResult: UpdateResult) => {
    println(s"${updateResult.getMatchedCount} document(s) matched the filter, updated ${updateResult.getModifiedCount} document(s)")
    mongoClient.close()
  },
  (t: Throwable) => t.printStackTrace()
)
