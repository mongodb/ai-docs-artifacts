import com.mongodb.ConnectionString;
import com.mongodb.async.client.MongoClients;
import com.mongodb.async.client.MongoClient;
import com.mongodb.async.client.MongoDatabase;
import com.mongodb.async.client.MongoCollection;
import com.mongodb.connection.netty.SocksProxy;
import com.mongodb.client.model.Filters;

import org.bson.Document;

public class Main {
    private static final String connectionString = "<connection string uri>";
    private static final SocksProxy proxy = new SocksProxy("<host>", 1080, "<username>", "<password>");

    public static void main(final String[] args) {
        final MongoClientSettings settings = MongoClientSettings.builder()
            .applyConnectionString(new ConnectionString(connectionString))
            .applyToSslSettings(builder -> builder.proxy(proxy))
            .build();
        final MongoClient mongoClient = MongoClients.create(settings);
        final MongoDatabase database = mongoClient.getDatabase("myDB"); 
        final MongoCollection<Document> collection = database.getCollection("myColl");

        SingleResultCallback<Document> printCallback = new SingleResultCallback<Document>() {
            @Override
            public void onResult(final Document result, final Throwable t) {
                System.out.println(result.toJson());
            }
        };

        collection.find().first(printCallback);

        try {
            printCallback.wait();
        } catch (InterruptedException e) {
            e.printStackTrace();
        } finally {
            mongoClient.close();
        }
    }
}
