import org.mongodb.scala._
import org.mongodb.scala.bson._
import org.mongodb.scala.model.Filters._
import org.mongodb.scala.model.geojson._

import scala.concurrent.Await
import scala.concurrent.duration._

object GeospatialQueries {

  val mongoClient: MongoClient = MongoClient("mongodb+srv://<user>:<password>@<cluster-url>?writeConcern=majority")
  val database: MongoDatabase = mongoClient.getDatabase("sample_mflix")
  val theaters: MongoCollection[Document] = database.getCollection("theaters")

  // Find theaters within a certain proximity
  def proximity(): Unit = {
    val point = new Point(new Position(-73.9667, 40.78))
    val query = near("location.geo", point, 10000.0, 0.0)

    val observable = theaters.find(query)

    observable.subscribe((doc: Document) => println(doc.toJson()))

    if (observable.toFuture().isCompleted && observable.toFuture().value.get.isSuccess && observable.toFuture().value.get.get.isEmpty) {
      println("No documents found!")
    }
  }

  // Find theaters within a specific geographic range
  def range(): Unit = {
    val polygon = new Polygon(
      new Position(-72, 40), // Polygon coordinates defining the range
      new Position(-74, 41),
      new Position(-72, 39),
      new Position(-72, 40),
    )
    val query = geoWithin("location.geo", polygon)

    val observable = theaters.find(query)

    observable.subscribe((doc: Document) => println(doc.toJson()))

    if (observable.toFuture().isCompleted && observable.toFuture().value.get.isSuccess && observable.toFuture().value.get.get.isEmpty) {
      println("No documents found!")
    }
  }

  def run(): Unit = {
    try {
      proximity()
      range()
    } finally {
      mongoClient.close()
    }
  }

  def main(args: Array[String]): Unit = {
    run()
  }
}
