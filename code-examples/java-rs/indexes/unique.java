import com.mongodb.reactivestreams.client.MongoClients;
import com.mongodb.reactivestreams.client.MongoClient;
import com.mongodb.reactivestreams.client.MongoDatabase;
import com.mongodb.reactivestreams.client.MongoCollection;
import com.mongodb.client.model.IndexOptions;
import com.mongodb.client.model.Indexes;
import org.bson.Document;

import java.util.concurrent.ExecutionException;

public class CreateUniqueIndex {

    public static void main(String[] args) {
        MongoClient mongoClient = MongoClients.create("mongodb+srv://<user>:<password>@<cluster-url>?writeConcern=majority");

        MongoDatabase database = mongoClient.getDatabase("sample_mflix");
        MongoCollection<Document> collection = database.getCollection("movies");

        collection.createIndex(Indexes.ascending("theaterId"), new IndexOptions().unique(true)).subscribe(result -> {
            System.out.println("Index created: " + result);
            mongoClient.close();
        }, throwable -> {
            System.err.println("Error: " + throwable.getMessage());
            mongoClient.close();
        });
    }
}
