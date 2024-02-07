import com.mongodb.ConnectionString;
import com.mongodb.reactivestreams.client.*;
import org.bson.Document;
import org.bson.conversions.Bson;
import java.util.Arrays;
import java.util.Date;
import java.util.concurrent.TimeUnit;

import static com.mongodb.client.model.Aggregates.*;
import static com.mongodb.client.model.Filters.*;
import static com.mongodb.client.model.Updates.set;
import static java.util.Collections.singletonList;

public class MongoDbCrudOperations {

    public static void main(String[] args) {
        String uri = "mongodb+srv://<user>:<password>@<cluster-url>?retryWrites=true&w=majority";
        MongoClient mongoClient = MongoClients.create(new ConnectionString(uri));

        MongoDatabase database = mongoClient.getDatabase("test");
        MongoCollection<Document> orders = database.getCollection("orders");

        Date startOfDay = new Date();
        startOfDay.setHours(00);
        startOfDay.setMinutes(00);
        startOfDay.setSeconds(00);

        Date endOfDay = new Date();
        endOfDay.setHours(23);
        endOfDay.setMinutes(59);
        endOfDay.setSeconds(59);

        // Find operation
        Bson findFilters = and(eq("name", "Lemony Snicket"), gte("date", startOfDay), lte("date", endOfDay));
        orders.find(findFilters).subscribe(new PrintDocumentSubscriber());

        // Aggregate operation
        long oneWeekAgo = new Date().getTime() - (1000 * 3600 * 24 * 7);
        List<Bson> aggregationPipeline = Arrays.asList(
                match(and(gte("date", new Date(oneWeekAgo)), lte("date", new Date()))),
                group("$status", sum("count", 1))
        );

        orders.aggregate(aggregationPipeline).subscribe(new PrintDocumentSubscriber());

        // Watch operation for insertions
        ChangeStreamPublisher<Document> changeStream = orders.watch(singletonList(eq("operationType", "insert")));
        changeStream.subscribe(new PrintChangeStreamEventSubscriber());

        // Insert operation
        Document newOrder = new Document("date", new Date())
                .append("address", "667 Dark Avenue, San Francisco, CA, 94110")
                .append("name", "Lemony Snicket")
                .append("items", Arrays.asList(
                        new Document()
                                .append("name", "pizza")
                                .append("toppings", Arrays.asList("pepperoni", "mushrooms"))
                                .append("notes", "extra shrooms")
                                .append("price", 11),
                        new Document()
                                .append("name", "wings")
                                .append("sauce", "buffalo")
                                .append("notes", "extra crispy")
                                .append("price", 10)))
                .append("status", "created");
        orders.insertOne(newOrder).subscribe(new PrintSubscriber<String>("Insert operation completed. Inserted Id: %s"));

        // Update operation
        Bson updateFilter = and(eq("address", "667 Dark Avenue, San Francisco, CA, 94110"), gte("date", startOfDay), lte("date", endOfDay));
        orders.updateOne(updateFilter, set("address", "667 Dark Avenue, San Francisco, CA, 94103")).subscribe(new PrintSubscriber<Long>("Update operation completed. Modified count: %s"));

        // Delete operation
        Bson deleteFilter = and(eq("address", "13 Lousy Lane"), eq("name", "Violet Baudelaire"), gte("date", startOfDay), lte("date", endOfDay));
        orders.deleteOne(deleteFilter).subscribe(new PrintSubscriber<Long>("Delete operation completed. Deleted count: %s"));

        try {
            TimeUnit.SECONDS.sleep(1);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }

        mongoClient.close();
    }

}
