// Import required libraries
import com.mongodb.reactivestreams.client.MongoClients;
import com.mongodb.reactivestreams.client.MongoClient;
import com.mongodb.reactivestreams.client.MongoCollection;
import com.mongodb.reactivestreams.client.MongoDatabase;
import org.bson.Document;
import com.mongodb.client.model.Indexes;
import com.mongodb.client.model.Projections;
import com.mongodb.client.model.Sorts;

import java.util.Optional;

public class Main {
    // Replace the following with your MongoDB deployment's connection string
    private static final String connectionString = "mongodb+srv://<user>:<password>@<cluster-url>?writeConcern=majority";
    
    public static void main(String[] args) {
        // creating single field indexes
        try (MongoClient mongoClient = MongoClients.create(connectionString)){
            MongoDatabase sample_mflix = mongoClient.getDatabase("sample_mflix");
            MongoCollection<Document> movies = sample_mflix.getCollection("movies");

            // Create an ascending index on the "title" field in the "movies" collection
            movies.createIndex(Indexes.ascending("title")).subscribe(indexName -> System.out.println("Index created: " + indexName));
            
            // Define the query parameters
            Document query = new Document("title", "Batman");
            movies.find(query)
                    .sort(Sorts.ascending("title"))
                    .projection(Projections.include("title"))
                    .subscribe(
                            result -> System.out.println(result.toJson()),
                            Throwable::printStackTrace,
                            () -> System.out.println("Completed")
                    );
            Thread.sleep(5000);
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}
