#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

int calculate_index(int letters, int words, int sentences);
int count_letters(string text);
int count_words(string text);
int count_sentences(string text);
int ispunctuation(char c);

int main(void) {

    string text = get_string("Text: ");

    int letters = count_letters(text);
    int words = count_words(text);
    int sentences = count_sentences(text);
    /*
    printf("%i letter(s)\n", letters);
    printf("%i word(s)\n", words);
    printf("%i sentece(s)\n", sentences);
    */
    int grade = calculate_index(letters, words, sentences);
    if(grade >= 16) {
        printf("Grade 16+\n");
    } else if(grade < 1) {
        printf("Before Grade 1\n");
    } else {
        printf("Grade %i\n", grade);
    }

}

int calculate_index(int letters, int words, int sentences) {

    float L, S;
    L = (letters * 100) / (float) words;
    S = (sentences * 100) / (float) words;

    return round(0.0588 * L - 0.296 * S - 15.8);
}

int count_letters(string text) {

    int letters = 0;

    for(int i = 0, n = strlen(text); i < n; i++) {
        if(isalpha(text[i])) {
            letters++;
        }
    }

    return letters;
}

int count_words(string text) {

    int words = 0;
    for(int i = 0, n = strlen(text); i < n; i++) {
        if(isspace(text[i])) {
            words++;
        }
    }
    // Last word doesn't have space, so it wasn't counted in the 'for' loop.
    words++;
    return words;
}

int count_sentences(string text) {

    int sentences = 0;
    for(int i = 0, n = strlen(text); i < n; i++) {
        if(ispunctuation(text[i])) {
            sentences++;
        }
    }

    return sentences;
}

int ispunctuation(char c) {

    if(c == '?' || c == '.' || c == '!') {
        return 1;
    }
    return 0;
}