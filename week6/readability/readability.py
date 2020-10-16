from cs50 import get_string
from sys import exit


def main():

    text = get_string("Text: ")

    words = len(text.split(" "))
    letters = 0
    sentences = 0

    for c in text:
        if str.isalpha(c):
            letters += 1
        if c in [".", "?", "!"]:
            sentences += 1

    L = (letters * 100) / words
    S = (sentences * 100) / words

    grade = round((0.0588 * L) - (0.296 * S) - 15.8)

    if grade < 1:
        print("Before Grade 1")
    elif grade >= 16:
        print("Grade 16+")
    else:
        print(f"Grade {grade}")


main()