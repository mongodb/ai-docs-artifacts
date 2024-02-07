import org.mongodb.scala.model.Filters._
import org.mongodb.scala.model.Collation
import org.mongodb.scala.MongoCollection
import org.bson.Document

val myColl: MongoCollection[Document] = ???

val collation = Collation.builder().locale("en").numericOrdering(true).build()

myColl.findOneAndDelete(gt("a", "100")).collation(collation).subscribe()
