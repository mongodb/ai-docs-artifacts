import org.mongodb.scala.MongoCollection
import org.mongodb.scala.bson.collection.mutable.Document
import org.mongodb.scala.model.Sorts._
import org.mongodb.scala.model.Collation

val myColl: MongoCollection[Document] = // get the collection

// Create the query and sort
val query = Document("year" -> 1980)
val sort = descending("title")

myColl.find(query)
  .collation(Collation.builder().locale("en_US").build())
  .sort(sort)
  .results()
