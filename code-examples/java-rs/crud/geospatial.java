import com.mongodb.ConnectionString;
import com.mongodb.reactivestreams.client.MongoClients;
import com.mongodb.reactivestreams.client.MongoClient;
import com.mongodb.reactivestreams.client.MongoCollection;
import com.mongodb.reactivestreams.client.MongoDatabase;
import org.bson.Document;
import com.mongodb.client.model.geojson.Point;
import com.mongodb.client.model.geojson.Polygon;
import com.mongodb.client.model.geojson.Position;
import com.mongodb.client.model.Filters;
import com.mongodb.Block;

import java.util.Arrays;

public class NearAndWithinPolygon {
    private static final ConnectionString URI = new ConnectionString("mongodb+srv://<user>:<password>@<cluster-url>?writeConcern=majority");
    
    public static void main(final String[] args) {

        final MongoClient client = MongoClients.create(URI);
        final MongoDatabase database = client.getDatabase("sample_mflix");
        final MongoCollection<Document> theaters = database.getCollection("theaters");
        
        // Find theaters within a certain proximity
        Block<Document> printBlock = document -> System.out.println(document.toJson());
        Point refPoint = new Point(new Position(-73.9667, 40.78));
        theaters.find(Filters.near("location.geo", refPoint, 10000.0, null)).forEach(printBlock).subscribe();

        // Find theaters within a specific geographic range
        Position position1 = new Position(-72, 40);
        Position position2 = new Position(-74, 41);
        Position position3 = new Position(-72, 39);
        Position position4 = new Position(-72, 40);
        Polygon polygon = new Polygon(Arrays.asList(position1, position2, position3, position4));
        theaters.find(Filters.geoWithin("location.geo", polygon)).forEach(printBlock).subscribe();

        // Close the client after the query completes
        client.close();
    }
}
