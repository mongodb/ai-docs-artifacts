import com.mongodb.reactivestreams.client.MongoClients;
import com.mongodb.reactivestreams.client.MongoClient;
import com.mongodb.reactivestreams.client.MongoDatabase;
import com.mongodb.reactivestreams.client.MongoCollection;
import com.mongodb.reactivestreams.client.Success;
import org.bson.Document;

import static com.mongodb.client.model.Indexes.ascending;
import static com.mongodb.client.model.Filters.and;
import static com.mongodb.client.model.Filters.eq;
import static com.mongodb.client.model.Projections.excludeId;
import static com.mongodb.client.model.Projections.fields;
import static com.mongodb.client.model.Projections.include;
import static com.mongodb.client.model.Sorts.orderBy;

public class MainClass {

    public static void main(String[] args) {
        String uri = "mongodb+srv://<user>:<password>@<cluster-url>?writeConcern=majority";

        MongoClient mongoClient = MongoClients.create(uri);
        MongoDatabase database = mongoClient.getDatabase("sample_mflix");
        MongoCollection<Document> collection = database.getCollection("movies");

        // Creating Index
        collection.createIndex(ascending("type", "genre")).subscribe(success ->
                System.out.println("Index Created: " + success),
                throwable -> System.out.println("Failed to create index: " + throwable.getMessage()),
                () ->
                {
                    // Querying
                    collection.
                            find(and(eq("type", "movie"), eq("genre", "Drama")))
                            .projection(fields(include("type", "genre"), excludeId()))
                            .sort(orderBy(ascending("type"), ascending("genre")))
                            .forEach(document -> System.out.println(document.toJson()),
                                    throwable -> System.out.println("Query failed: " + throwable.getMessage()),
                                    () -> {
                                        // Closing the connection
                                        mongoClient.close();
                                    }
                            );
                }
        );
    }
}
