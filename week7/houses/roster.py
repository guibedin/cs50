from sys import argv, exit
from cs50 import SQL

def main():
    if len(argv) != 2:
        print("Usage: python roster.py <house name>")
        exit(1)

    db = SQL("sqlite:///students.db")

    house = argv[1]
    rows = db.execute("SELECT first, middle, last, birth FROM students WHERE house = ? ORDER BY last, first", house)

    for row in rows:
        first = row["first"]
        middle = row["middle"]
        last = row["last"]
        birth = row["birth"]

        if middle != None:
            print(f"{first} {middle} {last}, born {birth}")
        else:
            print(f"{first} {last}, born {birth}")


main()