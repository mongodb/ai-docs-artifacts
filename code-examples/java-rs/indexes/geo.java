import com.mongodb.reactivestreams.client.MongoClients;
import com.mongodb.reactivestreams.client.MongoClient;
import com.mongodb.reactivestreams.client.MongoCollection;
import com.mongodb.reactivestreams.client.MongoDatabase;
import org.bson.Document;
import org.bson.conversions.Bson;
import org.reactivestreams.Publisher;

import java.util.concurrent.CompletableFuture;
import java.util.concurrent.ExecutionException;

public class CreateGeospatialIndex {

    public static void main(String[] args) throws ExecutionException, InterruptedException {
        // Connect to MongoDB
        MongoClient mongoClient = MongoClients.create("mongodb+srv://<user>:<password>@<cluster-url>?writeConcern=majority");
        
        {
            // Get database and collection
            MongoDatabase database = mongoClient.getDatabase("sample_mflix");
            MongoCollection<Document> collection = database.getCollection("movies");
            
            // Create index
            Bson index = new Document("location.geo", "2dsphere");
            Publisher<String> publisher = collection.createIndex(index);
            
            // Print index creation result
            CompletableFuture<String> future = new CompletableFuture<>();
            publisher.subscribe(new PrintDocumentSubscriber<>(future));
            System.out.println("The index was created with the following name: " + future.get());
        }
        
        // Close the connection
        mongoClient.close();
    }
}
