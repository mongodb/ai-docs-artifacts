import com.mongodb.reactivestreams.client.MongoClients;
import com.mongodb.reactivestreams.client.MongoClient;
import com.mongodb.reactivestreams.client.MongoCollection;
import com.mongodb.reactivestreams.client.MongoDatabase;
import com.mongodb.client.result.DeleteResult;
import org.bson.Document;
import org.reactivestreams.Publisher;

import java.util.regex.Pattern;

public class DeleteDocuments {
    // replace "<connection string uri>" with your MongoDB deployment's connection string
    private static final String CONNECTION_STRING = "<connection string uri>";
    
    public static void main(String[] args) {
        MongoClient client = MongoClients.create(CONNECTION_STRING);
        
        MongoDatabase database = client.getDatabase("sample_mflix");
        MongoCollection<Document> collection = database.getCollection("movies");
        
        // Create regex pattern to match "Santa" in the title field
        Pattern regex = Pattern.compile("Santa");
        Document query = new Document("title", regex);
        
        Publisher<DeleteResult> deletePublisher = collection.deleteMany(query);
        
        // Asynchronously print the number of deleted documents
        deletePublisher.subscribe(deleteResult -> System.out.println("Deleted " + deleteResult.getDeletedCount() + " documents"),
            error -> System.out.println("Error: " + error.getMessage()),
            () -> client.close()
        );
    }
}
