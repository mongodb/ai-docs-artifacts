import org.mongodb.scala.MongoClients
import org.mongodb.scala.MongoClient
import org.mongodb.scala.MongoDatabase
import org.mongodb.scala.MongoCollection
import org.mongodb.scala.model.Filters._
import org.mongodb.scala.model.Updates._
import org.bson.Document
import scala.concurrent.Await
import scala.concurrent.duration._
import scala.language.postfixOps

object Sample {
  // Replace the following string with your MongoDB deployment's connection string.
  val uri ="mongodb+srv://<user>:<password>@<cluster-url>?writeConcern=majority";
  val client: MongoClient = MongoClients.create(uri);
  val database: MongoDatabase = client.getDatabase("test");
  val collection: MongoCollection[Document] = database.getCollection("testColl");

  def printData =
    Await.result(collection.find().toFuture(), 10.seconds).foreach(println)

  def runFirstArrayElement = {
    val query = Document.parse("""{"entries.x": { "$type" : "string" }}""")
    val update = inc("entries.$.y", 33)

    val result = collection.findOneAndUpdate(query, update)
    Await.result(result.toFuture(), 10.seconds).foreach(println)
  }

  def runAllArrayElements = {
    val query = Document.parse("""{"date": "5/15/2023"}""")
    val update = unset("calls.$[].duration")

    val result = collection.findOneAndUpdate(query, update)
    Await.result(result.toFuture(), 10.seconds).foreach(println)
  }

  def arrayFiltersIdentifier = {
    val query = Document.parse("""{"date": "11/12/2023"}""")
    val update = mul("items.$[i].quantity", 2)
    val options = Document.parse("""{"arrayFilters": [{"i.recipe": "Fried rice", "i.item": { "$not": { "$regex": "oil" }}}]}""")

    val result = collection.findOneAndUpdate(query, update, options)
    Await.result(result.toFuture(), 10.seconds).foreach(println)
  }
}
