/*
   Delete the first document that contains a value greater
   than 100 in the "a" field when ordered by using the default
   binary collation order.
*/

// start findOneAndDelete no collation
myColl.findOneAndDelete(Document("a" -> Document("$gt" -> "100"))).map(_ => ())
// end findOneAndDelete no collation
