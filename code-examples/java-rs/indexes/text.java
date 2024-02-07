import com.mongodb.reactivestreams.client.*;
import org.bson.Document;

public class Main {
    public static void main(String[] args) {
        MongoClient mongoClient = MongoClients.create("mongodb+srv://<user>:<password>@<cluster-url>?writeConcern=majority");
        MongoDatabase database = mongoClient.getDatabase("testDB");
        MongoCollection<Document> collection = database.getCollection("blogPosts");

        Document indexFields = new Document("title", "text").append("body", "text");
        Document weights = new Document("title", 3).append("body", 10);
        Document indexOptions = new Document("default_language", "english").append("weights", weights);

        collection.createIndex(indexFields, indexOptions).subscribe(new OperationSubscriber<>());

        Bson filter = Filters.text("life ahead");
        Bson projection = Projections.fields(Projections.include("title"), Projections.excludeId());

        collection.find(filter).projection(projection).forEach(doc -> System.out.println(doc.getString("title")),
                Throwable::printStackTrace, () -> mongoClient.close());
    }
    
    static class OperationSubscriber<T> implements Subscriber<T> {
        @Override
        public void onSubscribe(Subscription s) {
            s.request(1);
        }

        @Override
        public void onNext(T t) {
            System.out.println("Operation successful");
        }

        @Override
        public void onError(Throwable t) {
            System.out.println("Operation failed");
            t.printStackTrace();
        }

        @Override
        public void onComplete() {
            System.out.println("Operation completed");
        }
    }
}
