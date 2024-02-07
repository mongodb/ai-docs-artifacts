import com.mongodb.ConnectionString;
import com.mongodb.reactivestreams.client.MongoClient;
import com.mongodb.reactivestreams.client.MongoClients;
import com.mongodb.reactivestreams.client.MongoDatabase;
import org.bson.Document;

import java.util.concurrent.CountDownLatch;
import java.util.concurrent.TimeUnit;

public class Main {
    public static void main(String[] args) {
        String username = "<username>";
        String password = "<password>";
        String clusterUrl = "<MongoDB cluster url>";
        String authMechanism = "SCRAM-SHA-256";

        String uri = "mongodb+srv://" + username + ":" + password + "@" + clusterUrl + "/?authMechanism=" + authMechanism;

        ConnectionString connectString = new ConnectionString(uri);

        MongoClient mongoClient = MongoClients.create(connectString);

        MongoDatabase database = mongoClient.getDatabase("admin");

        CountDownLatch latch = new CountDownLatch(1);

        database.runCommand(new Document("ping", 1)).subscribe(new OperationSubscriber<Document>(){
            @Override
            public void onNext(Document document) {
                System.out.println("Connected successfully to server");
            }

            @Override
            public void onError(Throwable throwable) {
                System.out.println("Error: " + throwable.getMessage());
                latch.countDown();
            }

            @Override
            public void onComplete() {
                System.out.println("Command completed successfully");
                latch.countDown();
            }
        });

        try {
            if (!latch.await(30, TimeUnit.SECONDS)) {
                System.out.println("Operation took too long");
            }
        } catch (InterruptedException e) {
            e.printStackTrace();
        } finally {
            mongoClient.close();
        }
    }
}

abstract class OperationSubscriber<T> implements Subscriber<T> {
    @Override
    public void onSubscribe(Subscription subscription) {
        subscription.request(1);
    }

    @Override
    public abstract void onNext(T t);

    @Override
    public abstract void onError(Throwable throwable);

    @Override
    public abstract void onComplete();
}
