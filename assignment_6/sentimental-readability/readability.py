from cs50 import get_string
import re


def ColemanLindau(string):
    sentences = len(re.findall("[.!?]", string))
    wrds = string.split(" ")
    words = len(wrds)
    letters = len(re.findall("[a-zA-Z]", string))

    L = (letters/words)*100

    S = (sentences/words)*100

    return 0.0588*L - 0.296*S - 15.8


def grading(string):
    grade = round(ColemanLindau(string))
    text = ""
    if grade < 1:
        text = "Before Grade 1"
    elif grade >= 16:
        text = "Grade 16+"
    else:
        text = f"Grade {grade}"
    return text


input = get_string("Text: ")

print(grading(input))
