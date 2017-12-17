/**
 * Implements a dictionary's functionality.
 */

#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#include "dictionary.h"

node *root = NULL;

/**
 * Returns true if word is in dictionary else false.
 */
bool check(const char *word)
{
    if (root == NULL)
    {
        fprintf(stderr, "check encountered NULL root");
        return false;
    }

    /*// build out new word that is lowercase and has { instead of '
    char wordArray[LENGTH];

    int index = 0;
    char c = word;
    while (*c != '\0')
    {
        if (*c == 39)
        {
            *c = 123;
        }
        wordArray[index] = *c;
        index++;
        c++;
    }
    wordArray[index + 1] = '\0';*/

    node *trav = root;

    // because param is a const, i can't change it to do lowercase or
    // deal with apostrophes. word_i keeps track of the REAL chracter/word position
    char word_i = *word;
    char c = *word_i;
    while (c != '\0')
    {
        // convert it to lower case
        c = tolower(c);

        // change any apostrophes to left bracket {
        if (c == 39)
        {
            c = 123;
        }
        if (trav -> next_char[c - 'a'] == NULL)
        {
            return false;
        }
        trav = trav -> next_char[c - 'a'];
        word_i++;
        c = *word_i;
    }

    if (trav -> end_string == 1)
    {
        // we ran out of letters, so we're at the end of the word.
        // if:
        //  1) we're at the end of the word
        //  2) trav is actually at the correct node (...it should be)
        //  3) end_string == 1, signifying this was originally loaded into the dict as an actual word

        return true;
    }

    return false;
}

/**
 * Loads dictionary into memory. Returns true if successful else false.
 */
bool load(const char *dictionary)
{
    FILE *dicptr = fopen(dictionary, "r");
    if (dicptr == NULL)
    {
        fprintf(stderr, "Could not load %s\n", dictionary);
        return false;
    }

    // setup root node
    root = malloc(sizeof(node));
    for (int i = 0; i < CHARSETSIZE; i++)
    {
        root -> next_char[i] = NULL;
    }
    root -> prev_char = NULL;

    if (root == NULL)
    {
        printf("fuck you\n");
    }

    int curWordSize = 0;
    char word[LENGTH];

    char ch = fgetc(dicptr);
    while (ch != EOF)
    {
        //printf("%i / %c\n", ch, ch);
        if (ch == '\n')
        {
            //printf("newline!\n");
            curWordSize++;
            word[curWordSize] = '\0';

            int attempt = insert(word);

            if (attempt != 0)
            {
                fprintf(stderr, "insert failed; insert returning error code %d\n", attempt);
                return false;
            }

            printf("%s\n", word);
            for (int i = 0; i < curWordSize + 1; i++)
            {
                word[i] = 0;
            }

            curWordSize = 0;
        } else
        {
            word[curWordSize] = ch;
            curWordSize++;
        }

        ch = fgetc(dicptr);
    }

    if (feof(dicptr) != 0)
    {
        return true;
    } else
    {
        return false;
    }
}

/**
 * Inserts individual words into root node.
 */
int insert(char word[])
{
    if (root == NULL)
    {
        fprintf(stderr, "root node is NULL\n");
        return 1;
    }
    node *head = root;
    char *c = word;

    while (*c != '\0')
    {

        if (*c == 39)
        {
            // left bracket comes immediately after z, and - 'a' will
            // be == 26
            *c = '{';
        }
        if (head -> next_char[*c - 'a'] == NULL)
        {
            // child doesn't exist, so setup new child
            node *child = malloc(sizeof(node));
            child -> prev_char = head;
            child -> end_string = 0;
            for (int i = 0; i < CHARSETSIZE; i++)
            {
                child -> next_char[i] = NULL;
            }

            // child becomes new head
            head -> next_char[*c - 'a'] = child;
            head = child;
        }
        else
        {
            head = head -> next_char[*c - 'a'];
        }
        c++;
    }
    head -> end_string = 1;

    return 0;
}

/**
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */
unsigned int size(void)
{
    // TODO
    return 0;
}

/**
 * Unloads dictionary from memory. Returns true if successful else false.
 */
bool unload(void)
{
    // TODO
    return false;
}
