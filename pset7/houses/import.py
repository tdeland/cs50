import csv
import cs50
from sys import argv

# get csv file from command line argument
if len(argv) != 2:
    print("Usage: python import.py characters.csv")
    exit()

# get handle to destination database
db = cs50.SQL("sqlite:///students.db")

with open(argv[1], 'r') as csvfile:

    # load csv contents
    characters = csv.DictReader(csvfile)

    for row in characters:

        # parse name into list first, (middle), last
        name = row['name'].split()

        # if middle name is missing, insert None
        if len(name) == 2:
            name.insert(1, None)

        # insert each student into students table of students.db
        db.execute("INSERT INTO students (first, middle, last, house, birth) VALUES(?, ?, ?, ?, ?)", name[0], name[1], name[2], row['house'], row['birth'])