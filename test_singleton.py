from database import Database

db1 = Database()
db2 = Database()
print(db1 is db2)
print(id(db1) == id(db2))