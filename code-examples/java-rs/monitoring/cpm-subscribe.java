import com.mongodb.reactivestreams.client.MongoClients;
import com.mongodb.reactivestreams.client.MongoClient;
import com.mongodb.reactivestreams.client.MongoDatabase;
import org.bson.Document;
import org.reactivestreams.Subscriber;
import org.reactivestreams.Subscription;

public class MongoEventSubscriber {

    // replace with your MongoDB deployment's connection String
    private final static String connectionString = "mongodb+srv://<clusterUrl>/?replicaSet=rs&writeConcern=majority";

    // replace with event name you are subscribing to
    private final static String eventName = "<event name>";

    public static void main(String[] args) {
        MongoClient mongoClient = MongoClients.create(connectionString);

        subscribeEvent(mongoClient);
        run(mongoClient);
    }

    public static void subscribeEvent(MongoClient mongoClient) {
        mongoClient.watch().subscribe(new Subscriber<>() {
            @Override
            public void onSubscribe(Subscription s) {
                s.request(Long.MAX_VALUE); 
            }

            @Override
            public void onNext(Document t) {
                if (eventName.equals(t.getString("operationType"))) {
                    System.out.println("\nReceived event:\n" + t.toJson());
                }
            }

            @Override
            public void onError(Throwable t) {
                System.out.println("Error occurred in MongoDB stream: " + t);
            }

            @Override
            public void onComplete() {
                System.out.println("Stream finished.");
            }
        });
    }

    public static void run(MongoClient mongoClient) {
        try {
            MongoDatabase database = mongoClient.getDatabase("admin");
            database.runCommand(new Document("ping", 1))
                    .subscribe(new Subscriber<>() {
                        @Override
                        public void onSubscribe(Subscription s) {
                            s.request(1);
                        }

                        @Override
                        public void onNext(Document document) {
                            System.out.println("\nConnected successfully!\n");
                        }

                        @Override
                        public void onError(Throwable t) {
                            System.out.println("Error connecting to MongoDB: " + t);
                        }

                        @Override
                        public void onComplete() {
                            mongoClient.close();
                            System.out.println("Connection to MongoDB closed.");
                        }
                    });
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}
