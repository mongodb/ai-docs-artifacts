/*
   Update the "verified" field to "true" for the first document
   that precedes "Gunter" when sorted lexicographically.
*/
// start findOneAndUpdate default order collation
myColl.findOneAndUpdate(
  Filters.lt("first_name", "Gunter"),
  Updates.set("verified", true)
).subscribe(new Observer<Document>() {

  @Override
  public void onNext(final Document document) {
    System.out.println(document);
  }

  @Override
  public void onError(final Throwable t) {
    System.out.println("Error: " + t.getMessage());
  }

  @Override
  public void onComplete() {
    System.out.println("Operation Completed");
  }
});
// end findOneAndUpdate default order collation
