/**
 * Declares a dictionary's functionality.
 */

#ifndef DICTIONARY_H
#define DICTIONARY_H

#include <stdbool.h>
#include <stdlib.h>

// maximum length for a word
// (e.g., pneumonoultramicroscopicsilicovolcanoconiosis)
#define LENGTH 45

// maximum size of character set array (26 alpha chars + 1 for
// apostrophe)
#define CHARSETSIZE 27

// this dictionary will be built using trie structures. below
// is the prototype for that struct.
typedef struct node
{
    int end_string;
    struct node* next_char[CHARSETSIZE];
    struct node* prev_char;
}
node;

/**
 * Returns true if word is in dictionary else false.
 */
bool check(const char *word);

/**
 * Inserts individual words into root node.
 */
int insert(char word[]);

/**
 * Loads dictionary into memory. Returns true if successful else false.
 */
bool load(const char *dictionary);

/**
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */
unsigned int size(void);

/**
 * Unloads dictionary from memory.  Returns true if successful else false.
 */
bool unload(void);

#endif // DICTIONARY_H
