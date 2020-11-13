import cs50
from sys import argv

# get house name from command line argument
if len(argv) != 2:
    print("Usage: python import.py characters.csv")
    exit()

#initialize database connection
db = cs50.SQL("sqlite:///students.db")

# get all students form specified house
query = db.execute("SELECT * FROM students WHERE house = ? ORDER BY last, first", argv[1])

# return formatted house roster
for row in query:
    print(f"{row['first']} {'' if row['middle'] == None else row['middle'] + ' '}{row['last']}, born {row['birth']}")