/*
   Retrieve documents that match the "year" value "1980"
   in descending order of the value of the "title" field
   that does not use the collation index.
*/

// Using MongoDB Scala Driver
val collection: MongoCollection[Document] = database.getCollection("myColl") 

val queryFilter = Filters.eq("year", 1980)

val sortDefinition = Sorts.descending("title")

val findIterable: FindObservable[Document] = collection
  .find(queryFilter)
  .sort(sortDefinition)
