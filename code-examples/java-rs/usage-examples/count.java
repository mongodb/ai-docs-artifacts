import com.mongodb.ConnectionString;
import com.mongodb.reactivestreams.client.MongoClients;
import com.mongodb.reactivestreams.client.MongoClient;
import com.mongodb.reactivestreams.client.MongoCollection;
import com.mongodb.reactivestreams.client.MongoDatabase;
import org.bson.Document;
import com.mongodb.client.model.Filters;
import org.reactivestreams.Subscriber;
import org.reactivestreams.Subscription;

public class DocumentCountApp {
    public static void main(String[] args) {
        ConnectionString connString = new ConnectionString("<connection string uri>");
        MongoClient mongoClient = MongoClients.create(connString);
        MongoDatabase database = mongoClient.getDatabase("sample_mflix");
        MongoCollection<Document> collection = database.getCollection("movies");

        collection.countDocuments()
            .subscribe(new PrintSubscriber<Long>("Total # of documents: %s"));

        collection.countDocuments(Filters.eq("countries", "Canada"))
            .subscribe(new PrintSubscriber<Long>("Total # of documents from Canada: %s"));
          
        mongoClient.close();
    }

    static class PrintSubscriber<T> implements Subscriber<T> {
        private final String message;
        
        PrintSubscriber(final String message) {
            this.message = message;
        }
        
        @Override
        public void onSubscribe(final Subscription s) {
            s.request(Long.MAX_VALUE);
        }

        @Override
        public void onNext(final T t) {
            System.out.println(String.format(message, t));
        }

        @Override
        public void onError(final Throwable t) {
            System.out.println("Error occurred: " + t.getMessage());
        }

        @Override
        public void onComplete() {
            System.out.println("Done");
        }
    }
}
