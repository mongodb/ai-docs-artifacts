import org.mongodb.scala._
import scala.concurrent.Await
import scala.concurrent.duration._
import org.mongodb.scala.bson._

val collection: MongoCollection[Document] = ...
val script: BsonJavaScript = BsonJavaScript("sleep(100) || true")

val findObservable: FindObservable[Document] = collection.find(Filters.where(script)).maxTime(50, MILLISECONDS)

Await.result(findObservable.toFuture(), Duration(50, "millis"))
