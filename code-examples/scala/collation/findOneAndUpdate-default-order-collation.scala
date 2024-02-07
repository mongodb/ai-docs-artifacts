/*
   Update the "verified" field to "true" for the first document
   that precedes "Gunter" when ordered by using the
   default binary collation order.
*/
// start findOneAndUpdate default order collation
myColl.findAndUpdate(
  BSONDocument("first_name" -> BSONDocument("$lt" -> "Gunter")),
  BSONDocument("$set" -> BSONDocument("verified" -> true))
)
// end findOneAndUpdate default order collation
