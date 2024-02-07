import org.mongodb.scala._

// Please replace with your MongoDB deployment's connection string.
val mongoClient: MongoClient = MongoClient("<connection string uri>")

case class Address(street1: String, city: String, state: String, zipcode: String)

case class Theater(location: Map[String, Address], is_in_ohio: Option[Boolean])

def run(): Unit = {
  try {
    val database = mongoClient.getDatabase("sample_mflix")
    val collection = database.getCollection("theaters")

    val theater1 = Document("location" -> Document("address" -> Document("street1" -> "3 Main St.", "city" -> "Anchorage", "state" -> "AK", "zipcode" -> "99501")))

    val theater2 = Document("location" -> Document("address" -> Document("street1" -> "75 Penn Plaza", "city" -> "New York", "state" -> "NY", "zipcode" -> "10001")))

    val insertOne1 = InsertOneModel(theater1)

    val insertOne2 = InsertOneModel(theater2)

    val updateMany = UpdateManyModel(Document("location.address.zipcode" -> "44011"), Document("$set" -> Document("is_in_ohio" -> true)))

    val deleteOne = DeleteOneModel(Document("location.address.street1" -> "221b Baker St"))

    val requests = List[WriteModel[Document]](insertOne1, insertOne2, updateMany, deleteOne)

    val result = collection.bulkWrite(requests).toFuture().result()

    println(result)

  } finally {
    mongoClient.close()
  }
}

run()
