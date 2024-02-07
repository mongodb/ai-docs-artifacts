import com.mongodb.reactivestreams.client.MongoClients;
import com.mongodb.reactivestreams.client.MongoClient;
import com.mongodb.reactivestreams.client.MongoDatabase;
import com.mongodb.reactivestreams.client.MongoCollection;
import com.mongodb.client.result.InsertManyResult;

import org.bson.Document;

import java.util.Arrays;

public class Main {
    public static void main(String[] args) {
        MongoClient mongoClient = MongoClients.create("<connection string uri>");

        MongoDatabase database = mongoClient.getDatabase("insertDB");
        MongoCollection<Document> collection = database.getCollection("foods");

        List<Document> documents = Arrays.asList(
                new Document("name", "cake").append("healthy", false),
                new Document("name", "lettuce").append("healthy", true),
                new Document("name", "donut").append("healthy", false)
        );

        collection.insertMany(documents).subscribe(new Observer<InsertManyResult>() {
            @Override
            public void onNext(InsertManyResult result) {
                System.out.println(result.getInsertedIds().size() + " documents were inserted");
            }

            @Override
            public void onError(Throwable e) {
                System.out.println(e.getMessage());
                mongoClient.close();
            }

            @Override
            public void onComplete() {
                mongoClient.close();
            }
        });
    }
}
