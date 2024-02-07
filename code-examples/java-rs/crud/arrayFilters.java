import com.mongodb.ConnectionString;
import com.mongodb.reactivestreams.client.*;
import org.bson.Document;

import java.util.Arrays;
import java.util.regex.Pattern;
import java.util.concurrent.TimeUnit;

public class Main {
    public static void main(String[] args) {
        String uri = "mongodb+srv://<user>:<password>@<cluster-url>?writeConcern=majority";
        ConnectionString connString = new ConnectionString(uri);
        MongoClientSettings settings = MongoClientSettings.builder().applyConnectionString(connString).retryReads(true).retryWrites(true).build();
        MongoClient mongoClient = MongoClients.create(settings);

        executeOperations(mongoClient);
        
        mongoClient.close();
    }
    
    private static void executeOperations(MongoClient mongoClient) {
        MongoDatabase database = mongoClient.getDatabase("test");
        MongoCollection<Document> coll = database.getCollection("testColl");

        printData(coll);
        runFirstArrayElement(coll);
        runAllArrayElements(coll);
        arrayFiltersIdentifier(coll);
    }

    private static void printData(MongoCollection<Document> coll) {
        coll.find().subscribe(doc -> System.out.println(doc.toJson()));
    }

    private static void runFirstArrayElement(MongoCollection<Document> coll) {
        coll.find().subscribe(doc -> System.out.println(doc.toJson()));
        
        Document query = new Document("entries.x", new Document("$type", "string"));
        Document updateDoc = new Document("$inc", new Document("entries.$.y", 33));

        coll.updateOne(query, updateDoc).subscribe(result -> {
            System.out.println("Modified count: " + result.getModifiedCount());
            printData(coll);
        });
    }

    private static void runAllArrayElements(MongoCollection<Document> coll) {
        coll.find().subscribe(doc -> System.out.println(doc.toJson()));
        
        Document query = new Document("date", "5/15/2023");        
        Document updateDoc =  new Document("$unset", new Document("calls.$[].duration", ""));

        coll.updateOne(query, updateDoc).subscribe(result -> {
            System.out.println("Modified count: " + result.getModifiedCount());
            printData(coll);
        });
    }

    private static void arrayFiltersIdentifier(MongoCollection<Document> coll) {
        coll.find().subscribe(doc -> System.out.println(doc.toJson()));
       
        Document query = new Document("date", "11/12/2023");        
        Document updateDoc = new Document("$mul", new Document("items.$[i].quantity", 2));
        UpdateOptions options = new UpdateOptions().arrayFilters(Arrays.asList(
            Filters.and(
                Filters.eq("i.recipe", "Fried rice"),
                Filters.ne("i.item", Pattern.compile("oil"))
            )
        ));
        coll.updateOne(query, updateDoc, options).subscribe(result -> {
            System.out.println("Modified count: " + result.getModifiedCount());
            printData(coll);
        });
    }
}
