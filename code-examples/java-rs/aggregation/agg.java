import com.mongodb.ConnectionString;
import com.mongodb.reactivestreams.client.MongoClients;
import com.mongodb.reactivestreams.client.MongoClient;
import com.mongodb.reactivestreams.client.MongoCollection;
import com.mongodb.reactivestreams.client.MongoDatabase;
import org.bson.Document;
import java.util.Arrays;
import java.util.concurrent.CompletableFuture;
import static com.mongodb.client.model.Accumulators.sum;
import static com.mongodb.client.model.Aggregates.group;
import static com.mongodb.client.model.Aggregates.match;

public class Main {
    public static void main(String[] args) {
        final ConnectionString connStr = new ConnectionString(
                System.getenv("MONGODB_URI")
        );
        final MongoClient mongoClient = MongoClients.create(connStr);
        final MongoDatabase database = mongoClient.getDatabase("aggregation");
        final MongoCollection<Document> collection = database.getCollection("restaurants");

        // Insert Documents
        CompletableFuture
                .allOf(
                        Arrays.asList(
                                new Document("stars", 3).append("categories", Arrays.asList("Bakery", "Sandwiches")).append("name", "Rising Sun Bakery"),
                                new Document("stars", 4).append("categories", Arrays.asList("Bakery", "Cafe", "Bar")).append("name", "Cafe au Late"),
                                new Document("stars", 5).append("categories", Arrays.asList("Coffee", "Bakery")).append("name", "Liz's Coffee Bar"),
                                new Document("stars", 3).append("categories", Arrays.asList("Steak", "Seafood")).append("name", "Oak Steakhouse"),
                                new Document("stars", 4).append("categories", Arrays.asList("Bakery", "Dessert")).append("name", "Petit Cookie")
                        )
                       .stream()
                       .map(doc -> CompletableFuture.runAsync(() -> collection.insertOne(doc)))
                       .toArray(CompletableFuture[]::new)
                )
                .thenCompose(v -> {
                    // Define an aggregation pipeline
                    return
                            collection.aggregate(Arrays.asList(
                               match(new Document("categories", "Bakery")),
                               group("$stars", sum("count", 1))
                            ))
                            .toFuture();
                })
                .exceptionally(e -> {
                    e.printStackTrace();
                    return null;
                })
                .join()
                .forEach(doc -> System.out.println(doc.toJson()));

        mongoClient.close();
    }
}
