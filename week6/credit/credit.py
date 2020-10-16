from cs50 import get_string
from sys import exit

def main():
    card_number = get_string("Number: ")
    card_len = len(card_number)

    # can only be amex
    if card_len == 15:
        check_valid(card_number, "AMEX")
        exit(0)

    # can only be visa
    if card_len == 13:
        check_valid(card_number, "VISA")
        exit(0)

    # might be master or visa - check initial numbers
    if card_len == 16:
        master_initials = ["51", "52", "53", "54", "55"]
        if card_number[:2] in master_initials:
            check_valid(card_number, "MASTERCARD")
            exit(0)

        if card_number[0] == "4":
            check_valid(card_number, "VISA")
            exit(0)

    print("INVALID")
    exit(0)


def check_valid(card_number, card_type):

    other = False
    every_other_sum = 0
    other_sum = 0

    for i in card_number[::-1]:
        digit = int(i)

        if other:
            mult = digit * 2
            if mult >= 10:
                mult = str(mult)
                every_other_sum += int(mult[0]) + int(mult[1])
            else:
                every_other_sum += mult
        else:
            other_sum += int(digit)

        other = not other

    if (every_other_sum + other_sum) % 10 == 0:
        print(card_type)
    else:
        print("INVALID")


main()