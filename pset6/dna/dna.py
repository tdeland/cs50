import csv
from sys import argv
import re

# check input arguments
if len(argv) != 3:
    print("Usage: python dna.py data.csv sequence.txt")
    exit()

# read DNA sequence
with open(argv[2], 'r') as file:
    dna = file.read()

# read database of STR counts
with open(argv[1], 'r') as csvfile:
    db = csv.DictReader(csvfile)

    # create dictionary for actual STR count and initialize zero matches
    strs = dict(zip(db.fieldnames[1:], [0] * len(db.fieldnames[1:])))

    # calculate STR count for sample DNA sequence
    for seg in strs:
        # match pattern (non-capture group repeating at least once)
        pattern = "(?:" + seg + ")+"
        m = re.findall(pattern, dna, re.IGNORECASE)
        # check for sequence match
        if m:
            # sort matches by length (smallest first)
            m.sort(key=len)
            # get last match (longest) and assign to dict entry
            strs[seg] = int(len(m[-1]) / len(seg))

    # check if sequence counts for each person match count in sample
    for row in db:
        # set match flag
        match = True
        # loop though all sequences in database
        for strp in list(strs):
            # cast to int for comparison to sequence count
            if int(row[strp]) != int(strs[strp]):
                # no match, set flag and get out
                match = False
                break
        # all sequence counts matched, that's our guy!
        if match:
            print(row["name"])
            exit()

# default return
print("No match")