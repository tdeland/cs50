import re
from cs50 import get_string

# get text from user
s = get_string("Text: ")

# count number of letters, words, and sentences in text
letters = len(re.findall("\w", s))
words = len(re.findall("\s", s)) + 1
sentences = len(re.findall("[\.\?\!]", s))

# calculate Coleman-Liau index
l = letters / words * 100
s = sentences / words * 100
idx = 0.0588  * l - 0.296 * s -15.8

# return reading level based on index
if idx < 1:
    print("Before Grade 1")
elif idx >= 16:
    print("Grade 16+")
else:
    print(f"Grade {round(idx)}")