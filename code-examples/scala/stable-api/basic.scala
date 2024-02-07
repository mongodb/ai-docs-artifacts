import org.mongodb.scala._

// Replace the placeholders in the connection string uri with your credentials
val uri: String = "mongodb+srv://<user>:<password>@<cluster-url>?retryWrites=true&w=majority"

// Create Settings with API version
val settings: MongoClientSettings = MongoClientSettings.builder()
    .applyConnectionString(new ConnectionString(uri))
    .serverApi(ServerApi.builder().version(ServerApiVersion.V1).build())
    .build()

// Create a client with specified settings
val mongoClient: MongoClient = MongoClient(settings)
