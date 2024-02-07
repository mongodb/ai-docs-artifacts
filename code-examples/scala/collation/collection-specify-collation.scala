/*
  Retrieve documents that match "photograph" in the "type" field,
  sorted by the Iceland collation and uppercase precedence.
*/
// start specified collation
myColl.find(Filters.eq("type", "photograph")).collation(Collation.builder().locale("is").caseLevel(true).collationCaseFirst(CollationCaseFirst.UPPER).build())
// end specified collation
