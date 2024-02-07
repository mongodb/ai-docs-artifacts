import com.mongodb.ConnectionString;
import com.mongodb.reactivestreams.client.MongoClient;
import com.mongodb.reactivestreams.client.MongoClients;
import com.mongodb.reactivestreams.client.MongoCollection;
import com.mongodb.reactivestreams.client.MongoDatabase;
import org.bson.Document;

import java.util.Arrays;

public class Main {
    public static void main(String[] args) {
        ConnectionString connectionString = new ConnectionString("<connection string uri>");
        MongoClient client = MongoClients.create(connectionString);
        MongoDatabase database = client.getDatabase("sample_mflix");
        MongoCollection<Document> theaters = database.getCollection("theaters");

        theaters.bulkWrite(
                Arrays.asList(
                        new InsertOneModel<>(
                                new Document("location",
                                        new Document("address",
                                                new Document("street1", "3 Main St.")
                                                        .append("city", "Anchorage")
                                                        .append("state", "AK")
                                                        .append("zipcode", "99501"))
                                )
                        ),
                        new InsertOneModel<>(
                                new Document("location",
                                        new Document("address",
                                                new Document("street1", "75 Penn Plaza")
                                                        .append("city", "New York")
                                                        .append("state", "NY")
                                                        .append("zipcode", "10001"))
                                )
                        ),
                        new UpdateManyModel<>(
                                Filters.eq("location.address.zipcode", "44011"),
                                Updates.set("is_in_ohio", true),
                                new UpdateOptions().upsert(true)
                        ),
                        new DeleteOneModel<>(
                                Filters.eq("location.address.street1", "221b Baker St")
                        )
                )
        ).subscribe(new Subscriber<BulkWriteResult>() {
            @Override
            public void onSubscribe(Subscription s) {
                s.request(1);
            }

            @Override
            public void onNext(BulkWriteResult result) {
                System.out.println(result);
            }

            @Override
            public void onError(Throwable t) {
                System.err.println(t.getMessage());
            }

            @Override
            public void onComplete() {
                client.close();
            }
        });
    }
}
