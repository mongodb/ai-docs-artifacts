import com.mongodb.reactivestreams.client.MongoClients;
import com.mongodb.reactivestreams.client.MongoClient;
import com.mongodb.reactivestreams.client.MongoCollection;
import com.mongodb.reactivestreams.client.MongoDatabase;
import org.bson.Document;
import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.CompletableFuture;
import java.util.concurrent.ExecutionException;
import java.util.function.Consumer;

public class MongoReactiveExample {

    private static String MONGO_URI = "mongodb+srv://<user>:<password>@<cluster-url>?writeConcern=majority";

    private static MongoClient client = MongoClients.create(MONGO_URI);

    private static MongoDatabase database = client.getDatabase("test");

    private static MongoCollection<Document> orders = database.getCollection("orders");

    public static void main(String[] args) throws ExecutionException, InterruptedException {

        asyncIteration();
        manualIteration();
        streamAPI();
        eventAPI();
        fetchAll();
        rewind();
        client.close();
    }

    private static void asyncIteration() throws InterruptedException, ExecutionException {
        CompletableFuture<Void> future =
                ReactiveStreamsHelper.toListPublisher(orders.find()).thenAccept(docs -> docs.forEach(System.out::println)).toCompletableFuture();
        future.get();
    }

    private static void manualIteration() throws InterruptedException, ExecutionException {
        CompletableFuture<Void> future = ReactiveStreamsHelper.forEachPublisher(orders.find(), System.out::println).toCompletableFuture();
        future.get();
    }

    private static void streamAPI() throws InterruptedException, ExecutionException {
        CompletableFuture<Void> future =
                ReactiveStreamsHelper.toListPublisher(orders.find()).thenAccept(docs -> docs.forEach(System.out::println)).toCompletableFuture();
        future.get();
    }

    private static void eventAPI() throws InterruptedException, ExecutionException {
        CompletableFuture<Void> future =
                ReactiveStreamsHelper.toListPublisher(orders.find()).thenAccept(docs -> docs.forEach(System.out::println)).toCompletableFuture();
        future.get();
    }

    private static void fetchAll() throws InterruptedException, ExecutionException {
        CompletableFuture<List<Document>> future =
                ReactiveStreamsHelper.toListPublisher(orders.find()).toCompletableFuture();
        List<Document> allValues = future.get();
        System.out.println(allValues.size());
    }

    private static void rewind() throws InterruptedException, ExecutionException {
        CompletableFuture<List<Document>> future =
                ReactiveStreamsHelper.toListPublisher(orders.find()).toCompletableFuture();
        List<Document> firstResult = future.get();
        System.out.println("First count: " + firstResult.size());
        future = ReactiveStreamsHelper.toListPublisher(orders.find()).toCompletableFuture();
        List<Document> secondResult = future.get();
        System.out.println("Second count: " + secondResult.size());
    }
}
class ReactiveStreamsHelper {

    public static <T> CompletableFuture<List<T>> toListPublisher(final Publisher<T> publisher) {
        List<T> results = new ArrayList<>();
        CompletableFuture<List<T>> future = new CompletableFuture<>();
        publisher.subscribe(new Subscriber<T>() {
            @Override
            public void onSubscribe(final Subscription s) {
                s.request(Long.MAX_VALUE);
            }

            @Override
            public void onNext(final T t) {
                results.add(t);
            }

            @Override
            public void onError(final Throwable t) {
                future.completeExceptionally(t);
            }

            @Override
            public void onComplete() {
                future.complete(results);
            }
        });
        return future;
    }

    public static <T> CompletableFuture<Void> forEachPublisher(final Publisher<T> publisher, final Consumer<T> consumer) {
        return toListPublisher(publisher).thenAccept(docs -> docs.forEach(consumer));
    }
}
