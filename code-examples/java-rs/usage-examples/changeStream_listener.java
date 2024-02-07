import com.mongodb.reactivestreams.client.*;

import java.util.concurrent.TimeUnit;

public class ChangeStreamListener {

    private static final String CONNECTION_STRING = "<connection string uri>";

    public static void main(String[] args) {
        final MongoClient mongoClient = MongoClients.create(CONNECTION_STRING);
        final MongoDatabase database = mongoClient.getDatabase("insertDB");
        final MongoCollection<Document> haikus = database.getCollection("haikus");

        // Open a Change Stream on the "haikus" collection
        final ChangeStreamPublisher<Document> changeStream = haikus.watch();

        // Set up a change stream listener when change events are emitted
        changeStream.subscribe(new Subscriber<ChangeStreamDocument<Document>>() {
            @Override
            public void onSubscribe(Subscription subscription) {
                subscription.request(1);
            }

            @Override
            public void onNext(ChangeStreamDocument<Document> changeStreamDocument) {
                try {
                    // Print any change event
                    System.out.println("received a change to the collection: \t" + changeStreamDocument);

                    // Pause before inserting a document
                    TimeUnit.SECONDS.sleep(1);
                
                    // Insert a new document into the collection
                    haikus.insertOne(new Document().append("title", "Record of a Shriveled Datum")
                                                   .append("content", "No bytes, no problem. Just insert a document, in MongoDB"))
                          .subscribe(new ObserverSubscriber<Document>());

                    // Pause before closing the change stream
                    TimeUnit.SECONDS.sleep(1);

                    // Close the change stream and print a message to the console when it is closed
                    changeStream.unsubscribe();
                    System.out.println("closed the change stream");
                } catch (InterruptedException e) {
                    System.err.println(e.getMessage());
                    Thread.currentThread().interrupt();
                } finally {
                    // Close the database connection on completion or error
                    mongoClient.close();
                }
            }

            @Override
            public void onError(Throwable throwable) {
                System.err.println(throwable.getMessage());
                mongoClient.close();
            }

            @Override
            public void onComplete() {
                System.out.println("All data received");
                mongoClient.close();
            }
        });
    }
    
    private static final class ObserverSubscriber<T> implements Observer<T> {
        @Override
        public void onSubscribe(Subscription subscription) { subscription.request(Integer.MAX_VALUE); }

        @Override
        public void onNext(T t) {}

        @Override
        public void onError(Throwable t) { System.err.println(t.getMessage()); }

        @Override
        public void onComplete() {}
    }
}
