from sys import argv, exit
from cs50 import SQL
import csv

def main():
    if len(argv) != 2:
        print("Usage: python import.py <csv file>")
        exit(1)

    db = SQL("sqlite:///students.db")

    fname = argv[1]
    with open(fname, "r") as file:
        reader = csv.DictReader(file)

        for row in reader:
            names = row["name"].split()
            house = row["house"]
            birth = int(row["birth"])

            if len(names) == 2:
                first = names[0]
                last = names[1]
                db.execute("INSERT INTO students (first, last, house, birth) VALUES (?, ?, ?, ?)", first, last, house, birth)
            else:
                first = names[0]
                middle = names[1]
                last = names[2]
                db.execute("INSERT INTO students (first, middle, last, house, birth) VALUES (?, ?, ?, ?, ?)", first, middle, last, house, birth)


main()