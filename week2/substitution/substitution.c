#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

string cipher(string plaintext, string key);
int isalphabetical(string key);
int has_duplicate_letter(string key);

int main(int argc, string argv[]) {

    if(argc != 2) {
        printf("Usage: ./substitution key\n");
        return 1;
    }
    // if it has a key, assign it to a string. Easier name
    string key = argv[1];

    if(strlen(key) != 26) {
        printf("Key must contain 26 characters.\n");
        return 1;
    }
    if(!isalphabetical(key)) {
        printf("Key must contain only alphabetical characters.\n");
        return 1;
    }
    if(has_duplicate_letter(key)) {
        printf("Key can't contain the same letter more than once.\n");
        return 1;
    }

    string plaintext = get_string("plaintext: ");
    printf("ciphertext: %s\n", cipher(plaintext, key));
    return 0;
}

string cipher(string plaintext, string key) {


    int keylen = strlen(key);
    int textlen = strlen(plaintext);
    int position;
    // allocates memory for the cipher text with same size as plaintext
    string ciphertext = malloc(textlen + 1);
    int isuppercase = 0;

    for(int i = 0; i < textlen; i++) {
        // These calculate the position in which the letter should be replaced in key
        // If the letter of the plaintext is 'A' -> plaintext[i] - 65 = 0 -> first position in key
        // If 'B' -> plaintext[i] - 65 = 1 -> second position in key
        // Same for lowercase
        // If it's not upper or lower, then it's punctuation and shouldn't change.
        if(isupper(plaintext[i])) {
            position = (int) plaintext[i] - 65;
            isuppercase = 1;
        } else if(islower(plaintext[i])) {
            position = (int) plaintext[i] - 97;
            isuppercase = 0;
        } else {
            position = -1;
        }

        if(position >= 0) {
            if(isuppercase) {
                ciphertext[i] = toupper(key[position]);
            } else {
                ciphertext[i] = tolower(key[position]);
            }
        } else {
            ciphertext[i] = plaintext[i];
        }
    }

    return ciphertext;
}

int isalphabetical(string key) {

    for(int i = 0, n = strlen(key); i < n; i++) {
        if(!isalpha(key[i])) {
            return 0;
        }
    }
    return 1;
}

int has_duplicate_letter(string key) {

    int len = strlen(key);
    for(int i = 0; i < len - 1; i++) {
        for(int j = i + 1; j < len; j++) {
            if(key[i] == key[j]) {
                return 1;
            }
        }
    }
    return 0;
}