import org.mongodb.scala._

// Replace the uri string with your MongoDB deployment's connection string.
val uri: String = "<connection string uri>"

val client: MongoClient = MongoClient(uri)

case class IMDB(rating: Double, votes: Int, id: Int)

case class Movie(title: String, imdb: IMDB, runtime: Int)

object RunQuery extends App {

  val doc = Document("runtime" -> Document("$lt" -> 15))

  val observable: Observable[Document] = client.getDatabase("sample_mflix")
    .getCollection("movies")
    .find(doc)
    .sort(Document("title" -> 1))
    .projection(Document("_id" -> 0, "title" -> 1, "imdb" -> 1))

  observable.count().subscribe((count: Long) => {
    if (count == 0)
      println("No documents found!")
  })

  observable.subscribe((result: Document) => println(s"Document: $result"),
    (e: Throwable) => println(s"Error: $e"),
    () => client.close()
  )

}
