import org.mongodb.scala._

import scala.concurrent.Future
import scala.concurrent.ExecutionContext.Implicits.global

// begin-core
def coreTest(client: MongoClient): Future[Unit] = {
  val session = client.startSession()
  val transactionOptions = TransactionOptions.builder().readPreference(ReadPreference.primary()).build()
  val savingsColl = client.getDatabase("bank").getCollection("savings_accounts")
  val checkingColl = client.getDatabase("bank").getCollection("checking_accounts")

  session.startTransaction(transactionOptions)
  for {
    _ <- savingsColl.findOneAndUpdate(Filters.eq("account_id", "9876"), Updates.inc("amount", -100)).toFuture()
    _ <- checkingColl.findOneAndUpdate(Filters.eq("account_id", "9876"), Updates.inc("amount", 100)).toFuture()
    _ <- session.commitTransaction().head() // Commit the transaction
    _ = println("Transaction committed.")
  } yield ()
  recover {
    case error: MongoException =>
      println(s"An error occurred during the transaction: $error")
      session.abortTransaction()
  }.andThen {
    case _ => session.close()
  }
}
// end-core

// begin-conv
def convTest(client: MongoClient): Unit = {
  client.withSession { session =>
    session.startTransaction()
    val savingsColl = client.getDatabase("bank").getCollection("savings_accounts")
    val checkingColl = client.getDatabase("bank").getCollection("checking_accounts")
    for {
      _ <- savingsColl.findOneAndUpdate(Filters.eq("account_id", "9876"), Updates.inc("amount", -100)).toFuture()
      _ <- checkingColl.findOneAndUpdate(Filters.eq("account_id", "9876"), Updates.inc("amount", 100)).toFuture()
      _ = println("Transaction committed.")
    } yield ()
    recover {
      case error: MongoException =>
        println(s"An error occurred during the transaction: $error")
        session.abortTransaction()
    }.andThen {
      case _ => session.close()
    }
  }
}
// end-conv

def run(): Unit = {
  val uri = "<connection string>"
  val client = MongoClient(uri)

  try {
    coreTest(client)
    convTest(client)
  } finally {
    client.close()
  }
}

run().recover {
  case error: Throwable => println(s"Caught exception : $error")
}
