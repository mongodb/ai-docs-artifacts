/*
   Retrieve documents that match the "year" value "1980"
   in descending order of the value of the "title" field,
   specifying a collation that uses the index.
*/
// start query index collation
myColl.find(Filters.eq("year", 1980)).sort(Sorts.descending("title"))
    .collation(Collation.builder().locale("en_US").build()).subscribe();
// end query index collation
