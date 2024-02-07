// import necessary packages
import org.mongodb.scala.MongoCollection
import org.mongodb.scala.bson.collection.immutable.Document

// Retrieve documents that match "photograph" in the "type" field
// start auto collation
myColl.find(Document("type" -> "photograph"))
// end auto collation
