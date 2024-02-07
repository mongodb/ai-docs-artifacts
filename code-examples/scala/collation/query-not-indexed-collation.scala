/*
   Retrieve documents that match the "year" value "1980"
   in descending order of the value of the "title" field,
   specifying a collation that does not use the collation
   index.
*/

// start not indexed collation
myColl.find(Filters.eq("year", 1980))
  .sort(Sorts.orderBy(Sorts.descending("title")))
  .collation(Collation.builder().locale("en_US").collationStrength(CollationStrength.SECONDARY).build())
// end not indexed collation
