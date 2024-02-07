// Delete a document
import com.mongodb.reactivestreams.client.MongoClients;
import com.mongodb.reactivestreams.client.MongoClient;
import com.mongodb.reactivestreams.client.MongoDatabase;
import com.mongodb.reactivestreams.client.MongoCollection;
import org.bson.Document;
import com.mongodb.client.result.DeleteResult;
import org.bson.BsonString;
import java.util.concurrent.CompletionStage;
import java.util.concurrent.CompletableFuture;

// Replace the uri string with your MongoDB deployment's connection string
String uri = "<connection string uri>";

MongoClient client = MongoClients.create(uri);

void run() {
  try {
    MongoDatabase database = client.getDatabase("sample_mflix");
    MongoCollection<Document> movies = database.getCollection("movies");

    /* Delete the first document in the "movies" collection that matches
    the specified query document */
    Document query = new Document("title", new BsonString("Annie Hall"));
    CompletionStage<DeleteResult> result = movies.deleteOne(query).toFuture();
    
    /* Print a message that indicates whether the operation deleted a
    document */
    result.whenComplete((deleteResult, throwable) -> {
      if (deleteResult.getDeletedCount() == 1) {
        System.out.println("Successfully deleted one document.");
      } else {
        System.out.println("No documents matched the query. Deleted 0 documents.");
      }
    });
  } finally {
    // Close the connection after the operation completes
    client.close();
  }
}

// Run the program and print any thrown exceptions
public static void main(String[] args) {
  try {
    run();
  } catch (Exception e) {
    e.printStackTrace();
  }
}
