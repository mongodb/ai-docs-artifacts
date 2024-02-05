'''
Delete the first document that contains a value greater
than 100 in the "a" field when ordered by using the default
binary collation order.
'''
# start find_one_and_delete no collation
myColl.find_one_and_delete({ "a": { "$gt": 100 } })
# end find_one_and_delete no collation