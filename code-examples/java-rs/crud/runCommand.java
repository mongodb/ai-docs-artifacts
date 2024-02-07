import com.mongodb.reactivestreams.client.MongoClients;
import com.mongodb.reactivestreams.client.MongoClient;
import com.mongodb.reactivestreams.client.MongoDatabase;
import com.mongodb.reactivestreams.client.MongoCollection;
import org.bson.Document;

public class Main {

    public static void main(String[] args) {
        // Replace the uri string with your MongoDB deployment's connection string
        String uri = "<connection string uri>";

        // Create a new client and connect to MongoDB
        MongoClient mongoClient = MongoClients.create(uri);

        try {
            // Connect to the "testDB" database
            MongoDatabase database = mongoClient.getDatabase("testDB");

            Document command = new Document().append("checkMetadataConsistency", 1);

            // Run a cursor command to check metadata consistency within the database
            database.runCommand(command).subscribe(doc -> {
                System.out.println(doc.toJson());
            }, error -> {
                System.out.println("There was an error: " + error);
            }, () -> {
                // Once the operation finishes we can safely close the MongoClient
                mongoClient.close();
            });
        } catch (Exception e) {
            System.out.println(e.getMessage());
            mongoClient.close();
        }

    }
}
