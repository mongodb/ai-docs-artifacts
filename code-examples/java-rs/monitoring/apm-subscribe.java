import com.mongodb.reactivestreams.client.MongoClient;
import com.mongodb.reactivestreams.client.MongoClients;
import com.mongodb.reactivestreams.client.MongoDatabase;
import org.bson.Document;

// Replace "mongodb://localhost" with your MongoDB deployment's connection string
String uri = "mongodb://localhost";

MongoClient mongoClient = MongoClients.create(uri);
MongoDatabase database = mongoClient.getDatabase("admin");

// Replace "<event name>" with the name of the event you are subscribing to
String eventName = "<event name>";
// Subscribe to a specified event and print a message when the event is received
subscriberHelpers.CallbackSubscriber<Document> subscriber = new subscriberHelpers.CallbackSubscriber<>(doc -> {
  System.out.println(String.format("Received %s: %s", eventName, doc.toJson()));
});

database.runCommand(new Document("ping", 1))
  .subscribe(subscriber);

subscriber.await();

// Establish and verify connection to the "admin" database
if (subscriber.getReceived().isEmpty() || !subscriber.getReceived().get(0).getBoolean("ok")) {
  System.out.println("Connection unsuccessful");
} else {
  System.out.println("Connected successfully");
}

mongoClient.close();
