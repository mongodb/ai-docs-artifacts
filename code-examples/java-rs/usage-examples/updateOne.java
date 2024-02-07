// Imports required for the MongoDB Java Reactive Streams Driver
import com.mongodb.MongoClientSettings;
import com.mongodb.reactivestreams.client.MongoClients;
import com.mongodb.reactivestreams.client.MongoClient;
import com.mongodb.reactivestreams.client.MongoDatabase;
import com.mongodb.reactivestreams.client.MongoCollection;
import com.mongodb.reactivestreams.client.Success;
import org.bson.Document;
import static com.mongodb.client.model.Filters.eq;
import static com.mongodb.client.model.Updates.set;
import com.mongodb.client.model.UpdateOptions;

import java.util.Random;

public class UpdateDocument {

    // Replace the uri string with your MongoDB deployment's connection string
    private static final String uri = "<connection string uri>";

    // Connect to the MongoDB instance
    private static final MongoClient mongoClient = MongoClients.create(uri);

    public static void main(String[] args) {

        MongoDatabase database = mongoClient.getDatabase("sample_mflix");
        MongoCollection<Document> collection = database.getCollection("movies");

        Document updateDoc = new Document();
        updateDoc.append("$set", new Document("plot", generateRandomPlot()));

        UpdateOptions options = new UpdateOptions().upsert(true);

        // Update a document - add a new plot
        Mono<UpdateResult> updateResult = Mono.from(collection.updateOne(eq("title", "Random Harvest"), updateDoc, options));

        updateResult.block();

        // Close the connection to MongoDB
        mongoClient.close();
    }

    // Generate a random plot for the movie
    private static String generateRandomPlot() {
        return "A harvest of random numbers, such as: " + new Random().nextDouble();
    }
}
