// Stable API - with options
// begin serverApiVersion

import org.mongodb.scala._

// Replace the placeholders in the connection string uri with your credentials
val uri: String = "mongodb+srv://<user>:<password>@<cluster-url>?retryWrites=true&w=majority"

/* Create a client with options to specify Stable API Version 1, return
errors for commands outside of the API version, and raise exceptions
for deprecated commands */
val settings: MongoClientSettings = MongoClientSettings.builder()
      .applyConnectionString(new ConnectionString(uri))
      .serverApi(new ServerApi(ServerApiVersion.V1, true, true))
      .build()

val client: MongoClient = MongoClient(settings)
// end serverApiVersion
