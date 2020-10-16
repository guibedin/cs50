from sys import exit, argv
import csv


def main():

    if len(argv) != 3:
        print("Usage: python dna.py <database.csv> <dna.txt>")
        exit(1)

    db = []
    # Open file database, get STRs from header
    # Returns pointer to first byte of the file
    # Creates a dict with the values using csv
    with open(argv[1], "r") as database:
        STRs = database.readline().rstrip("\n").split(",")[1:]
        database.seek(0)
        reader = csv.DictReader(database)
        for row in reader:
            db.append(row)

    # Read dna sequence
    with open(argv[2], "r") as dna:
        sequence = dna.readline().rstrip("\n")

    STR_count = dict()

    for STR in STRs:
        STR_len = len(STR)
        count = 0
        STR_max = 0

        i = 0

        while i < len(sequence):
            eq = False

            if sequence[i:(i + STR_len)] == STR:
                eq = True

            if eq:
                count += 1
                i += STR_len
            else:
                if count >= STR_max:
                    STR_max = count
                count = 0
                i += 1

        STR_count[STR] = STR_max


    for person in db:
        found = True
        for STR in STRs:
            if STR_count[STR] != int(person[STR]):
                found = False

        if found:
            print(person['name'])
            exit(0)

    print("No match")

main()