// Implements a dictionary's functionality
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Number of buckets in hash table
const unsigned int N = 100001;

// Hash table
node *table[N];

// Store number of words while loading dictionary
unsigned int dict_size;

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    int index = hash(word);

    node *tmp = table[index];
    while(tmp != NULL) {
        if(strcasecmp(word, tmp->word) == 0) {
            return true;
        }
        tmp = tmp->next;
    }
    return false;
}

// Hashes word to a number
// Used hash function from http://www.cse.yorku.ca/~oz/hash.html
// adapted to lowercase every char of word
unsigned int hash(const char *word)
{
    unsigned long hash = 5381;
    int c;

    while ((c = tolower(*word++)))
        hash = (hash * 33) + c; /* hash * 33 + c */

    return hash % N;
}

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    // opens dictionary file
    FILE *dict = fopen(dictionary, "r");
    if(dict == NULL) {
        return false;
    }

    char *word = malloc(LENGTH + 1);
    int index = 0;
    dict_size = 0;
    while(fscanf(dict, "%s", word) != EOF) {

        // create a node for each word
        node *n = malloc(sizeof(node));
        if(n == NULL) {
            free(word);
            fclose(dict);
            return false;
        }

        // copy word to node
        strcpy(n->word, word);

        // hash word to get index
        index = hash(word);

        // add node to start of the list
        n->next = table[index];
        table[index] = n;

        // increase dict_size
        dict_size++;
    }


    free(word);
    fclose(dict);

    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    return dict_size;
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    // go through all indexes of table[]
    for(int i = 0; i < N; i++) {
        node *n = table[i];

        while(n != NULL) {
            node *tmp = n;
            n = n->next;
            free(tmp);
        }
    }
    return true;
}
