// Implements a dictionary's functionality

#include <stdbool.h>
#include <string.h>
#include <strings.h>
#include <stdlib.h>
#include <stdio.h>
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
const unsigned int N = 5000;

// Hash table
node *Table[N];

// Load status
bool Loaded = false;

// Dictionary size;
unsigned int Size = 0;

// Returns true if word is in dictionary else false
bool check(const char *word)
{

    // hash word to obtain hash value
    // access linked list at that index in the hash table
    node *cursor = Table[hash(word)];

    // traverse linked list, looking for the word (strcasecmp)
    while (cursor != NULL)
    {
        if (strcasecmp(word, cursor->word) == 0)
        {
            return true;
        }

        cursor = cursor->next;
    }

    return false;
}

// Hashes word to a number
// djb2 algorithm: http://www.cse.yorku.ca/~oz/hash.html
unsigned int hash(const char *word)
{
    long seed = 5381;

    for (int i = 0; word[i] != '\0'; i++)
    {
        // bit shift and add character value
        seed = ((seed << 3) + seed) + tolower(word[i]);
    }

    // mod result to stay within bounds of Table
    return (unsigned int) seed % N;
}

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    // open dictionary file
    FILE *file = fopen(dictionary, "r");

    // check for NULL
    if (!file)
    {
        return false;
    }

    // define string for reading from dictionary
    char dict[LENGTH + 1];

    // read strings from dictionary file
    while (fscanf(file, "%s", dict) != EOF)
    {
        // create new node
        node *n = malloc(sizeof(n));

        // check that node was successfully created
        if (n == NULL)
        {
            fclose(file);
            return false;
        }

        // // initiate first pointer??
        // n->word = malloc(strlen(dict) + 1);

        // copy word into node
        strcpy(n->word, dict);

        // hash word to obtain hash value
        unsigned int hashed = hash(dict);

        // if first entry at hash, prepend Table list
        if (Table[hashed] == NULL)
        {
            Table[hashed] = n;
            n->next = NULL;
        }

        // Table entry already populated, insert
        else
        {
            n->next = Table[hashed];
            Table[hashed] = n;
        }

        // increment dictionary size counter
        Size++;
    }

    // close file
    fclose(file);

    //set Loaded flag for size check
    Loaded = true;
    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    if (Loaded)
    {
        return Size;
    }

    return 0;
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    for (unsigned int i = 0; i < N; i++)
    {
        node *head = Table[i];
        while (head != NULL)
        {
            node *tmp = head->next;
            free(head);
            head = tmp;
        }
    }

    return true;
}
