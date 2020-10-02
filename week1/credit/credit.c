#include <stdio.h>
#include <cs50.h>

int get_length(long card_number);
int get_first_two_digits(long card_number);
void check_card(long card_number, int first_two_digits, int length);

int main(void) {
    // Get card number from user
    long card_number = get_long("Number: ");

    int length = get_length(card_number);
    // Minimum length is 13, card is invalid otherwise
    if(length < 13) {
        printf("INVALID\n");
    } else {
        int first_two_digits = get_first_two_digits(card_number);
        check_card(card_number, first_two_digits, length);
    }
}

int get_length(long card_number) {

    int length;
    for(length = 0; card_number > 0; length++) {
        card_number = card_number / 10;
    }

    return length;
}

int get_first_two_digits(long card_number) {

    while(card_number > 0) {
        if(card_number / 10 > 10 && card_number / 10 < 100) {
            return (int)(card_number / 10);
        } else {
            card_number = card_number / 10;
        }
    }
    return -1;
}

int is_amex(int first_two_digits, int length) {
    if(length == 15 && (first_two_digits == 34 || first_two_digits == 37)) {
        return 1;
    }
    return 0;
}

int is_visa(int first_two_digits, int length) {
    if(first_two_digits / 10 == 4 && (length == 13 || length == 16)) {
        return 1;
    }
    return 0;
}

int is_mastercard(int first_two_digits, int length) {
    if(length == 16 && first_two_digits >= 51 && first_two_digits <= 55) {
        return 1;
    }
    return 0;
}

void check_card(long card_number, int first_two_digits, int length) {

    // Initialize variables that will store the sums
    int sum_products_digits = 0;
    int sum_digits = 0;
    int checksum = 0;

    for(int i = 1; i <= length; i++) {
        // sum every other digit * 2, starting from the second to last, that's why i = 1 initially
        if(i % 2 == 0) {
            long product = (card_number % 10) * 2;
            // Sum digits of the product
            if(product / 10 > 0) {
                sum_products_digits += (product % 10) + (product / 10);
            } else {
                sum_products_digits += product;
            }
        } else { // sum digits
            sum_digits += (card_number % 10);
        }
        // change "last digit" of card number
        card_number = card_number / 10;
    }
    checksum = (sum_products_digits + sum_digits) % 10;

    if(checksum == 0) {
        if(is_amex(first_two_digits, length)) {
            printf("AMEX\n");
        } else if(is_visa(first_two_digits, length)) {
            printf("VISA\n");
        } else if(is_mastercard(first_two_digits, length)) {
            printf("MASTERCARD\n");
        } else {
            printf("INVALID\n");
        }
    } else {
        printf("INVALID\n");
    }
}