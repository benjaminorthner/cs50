// Implements a dictionary's functionality

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 2];
    struct node *next;
}
node;

// Number of buckets in hash table
const unsigned int N = 100000;

// Number of words in the dictionary
unsigned int dictWordCount = 0;

// Hash table
node *table[N];

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    // convert word to lowercase
    size_t len = strlen(word);
    char *wordLower = calloc(len + 1, sizeof(char));

    for (size_t i = 0; i < len; ++i)
    {
        wordLower[i] = tolower((unsigned char)word[i]);
    }

    // find hash of the word to be checked
    unsigned int checkHash =  hash(wordLower);

    // init pointer pointing at bin
    node *dictPointer = table[checkHash];

    // check if position in hash table is occupied at all
    if (dictPointer != NULL)
    {
        // keep stepping through linked list in bin until word is found or next is NULL
        while (dictPointer->next != NULL)
        {
            // check if the word matches
            if (strcmp(dictPointer->word, wordLower) == 0)
            {
                free(wordLower);
                return true;
            }
            // else step into next part of linked list
            dictPointer = dictPointer->next;
        }

        // check if last element in linked list is the correct word
        if (strcmp(dictPointer->word, wordLower) == 0)
        {
            free(wordLower);
            return true;
        }
    }

    free(wordLower);
    return false;
}

// Hashes word to a number
// adapted from djb2 from http://www.cse.yorku.ca/~oz/hash.html
// using int instead of long
unsigned int hash(const char *word)
{
    unsigned int hash_ = 5381;
    int c;

    while ((c = *word++))
    {
        hash_ = ((hash_ << 5) + hash_) + c; /* hash * 33 + c */
    }

    // Note we take modulo N because the hash function may produce more unique numbers than we have bins available
    return hash_ % N;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // open dict File for reading
    FILE *dictFile = fopen(dictionary, "r");
    if (dictFile == NULL)
    {
        printf("Could not open dictionary file!\n");
        return false;
    }

    // allocates just enough memory for each word
    char *word = malloc((LENGTH + 2) * sizeof(char));

    // reads the dictionary line by line -> word by word into variable (note \n is also imported)
    while (fgets(word, (LENGTH + 2) * sizeof(char), dictFile))
    {
        dictWordCount++;

        // replace the newline character with a \0 (to indicate the end of the word)
        // NOTE: strlen() returns the length of the word including '\n'
        word[strlen(word) - 1] = '\0';

        // create new node for word
        node *n = malloc(sizeof(node));
        if (n == NULL)
        {
            printf("Failed to allocate memory for new node in hash table!");
            return false;
        }

        // deepcopy word into node
        for (int i = 0; i < LENGTH + 1; i++)
        {
            n->word[i] = word[i];
        }

        // set pointer to next word to NULL
        n->next = NULL;

        // add note to the hash table
        // use hash function to find the bin
        unsigned int bin = hash(n->word);

        //then follow along the linked list in the bin until you reach the end and add the new node there
        // if bin has never been acessed before then set it to point to the new node
        if (table[bin] == NULL)
        {
            table[bin] = n;
        }
        else
        {
            // step along linked list until the last node is reached
            node *tmp = table[bin];
            while (tmp->next != NULL)
            {
                tmp = tmp->next;
            }
            // now tmp is the last node in the linked list so add the new node as the next node of that linked list
            tmp->next = n;
        }
    }

    free(word);
    fclose(dictFile);
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    return dictWordCount;
}

// Unloads dictionary from memory, returning true if successful
bool unload(void)
{
    // step through all N bins
    for (int n = 0; n < N; n++)
    {
        // if spot in hash table is not empty (i.e. points to a node)
        if (table[n] != NULL)
        {
            // save that pointer in tmp
            node *tmp = table[n];

            // Step through linked list, save pointer to next and free memory
            while (tmp->next != NULL)
            {
                node *freeMe = tmp;
                tmp = tmp->next;
                free(freeMe);
            }
            free(tmp);
        }
    }
    return true;
}
