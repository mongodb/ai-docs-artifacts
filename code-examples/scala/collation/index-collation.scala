/*
   Create an index on the "title" field with the "en_US"
   locale collation, specifying ascending ordering of the
   "title" field.
*/
// start create index collation
myColl.createIndex(
  Ascending("title"),
  IndexOptions().collation(Collation.builder().locale("en_US").build())
)
// end create index collation
