import com.mongodb.client.model.Aggregates;
import com.mongodb.client.model.Collation;
import com.mongodb.reactivestreams.client.MongoCollection;
import com.mongodb.reactivestreams.client.MongoDatabase;
import java.util.Arrays;
import org.bson.Document;

// Assuming the collection has been instantiated
MongoCollection<Document> myColl;

// Define the collation to use German phonebook collation
Collation collation = Collation.builder().locale("de@collation=phonebook").build();

// Define the aggregation pipeline
List<Bson> pipeline = Arrays.asList(
  Aggregates.group("$first_name", Accumulators.sum("nameCount", 1)),
  Aggregates.sort(ascending("_id"))
);

// Execute the aggregation pipeline with the specified collation
myColl.aggregate(pipeline).collation(collation).subscribe(new PrintSubscriber<>());
