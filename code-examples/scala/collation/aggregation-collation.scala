/*
  Retrieve the total number of matching names, grouped by the first_name
  field and sorted by using the German phonebook collation.
*/
import org.mongodb.scala._
import org.mongodb.scala.bson._
import org.mongodb.scala.model._

val collation: Collation = Collation.builder().locale("de@collation=phonebook").build()

val groupStage: Bson = Aggregates.group("$first_name", Accumulators.sum("nameCount", 1))
val sortStage: Bson = Aggregates.sort(Sorts.ascending("_id"))

myColl.aggregate(Seq(groupStage, sortStage)).collation(collation).toFuture()
