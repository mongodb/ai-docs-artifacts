import org.mongodb.scala.model.Filters._
import org.mongodb.scala.model.Updates._
import org.mongodb.scala.model.Collation._
import org.mongodb.scala.MongoCollection
import org.bson.Document

val myColl: MongoCollection[Document] = // your mongo collection here

myColl.findOneAndUpdate(
  lt("first_name", "Gunter"),
  set("verified", true),
  Collation.builder().locale("de@collation=phonebook").build()
)
