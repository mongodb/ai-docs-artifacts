/*
   Retrieve documents that match "New York" in the "city" field,
   sorted by the "name" field by using the German collation.
*/
// start find sort collation
val collation = Collation.builder().locale("de").build()
collection.find(Filters.eq("city", "New York")).sort(Sorts.ascending("name")).collation(collation).iterator()
// end find sort collation
