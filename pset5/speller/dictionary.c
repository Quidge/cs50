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

    node *trav = root;

    const char *c = word;

    /* This while loop attempts to get to the ending trie node for
    * the word.
    * If it cannot (next 'letter' node for the word would be NULL),
    * then the word does not exist in the trie.
    */

    while (*c != '\0')
    {
        //printf("constant char c: %c\n", *c);
        char curChar = tolower(*c);
        //printf("curchar: %c\n", curChar);
        if (curChar == '\'')
        {
            curChar = '{';
        }
        if (trav -> next_char[curChar - 'a'] == NULL)
        {
            //printf("it's happening; curChar is: %c : ", curChar);
            return false;
        }
        trav = trav -> next_char[curChar - 'a'];
        c++;
    }

    /* If we make it down here, it must mean that no node has returned
    * NULL and that we are at node representing the last character in
    * the word.
    * If this current node has end_string == 1, then the word is in the
    * trie. If end_string != 1, it means that the SUBSTRING is in the
    * dictionary, but the word isn't, ie:
    * if you had a trie with a single word, 'catch', and you searched
    * for 'cat', you would have a situation where you could be at the
    * last node for that word ('t'), yet the end_string would be != 1.
    */

    if (trav -> end_string == 1)
    {
        // we ran out of letters, so we're at the end of the word.
        // if:
        //  1) we're at the end of the word
        //  2) trav is actually at the correct node (...it should be)
        //  3) end_string == 1, signifying this was originally loaded into the dict as an actual word

        return true;
    } else
    {
        //printf("not in dict, must be substring\n");
        return false;
    }
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
        printf("crash\n");
    }

    int curWordSize = 0;
    // setup word array with all null terminator chars
    char word[LENGTH+1];
    for (int k = 0; k < LENGTH+1; k++)
    {
        word[k] = '\0';
    }

    char ch = fgetc(dicptr);
    while (ch != EOF)
    {
        if (ch == '\n')
        {
            //word[curWordSize] = '\0';

            //printf("%s\n", word);
            int attempt = insert(word);

            if (attempt != 0)
            {
                fprintf(stderr, "insert failed; insert returning error code %d\n", attempt);
                return false;
            }

            // the word has been inserted, so reset the buffer
            for (int i = 0; i < LENGTH; i++)
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
        fclose(dicptr);
        return true;
    } else
    {
        fclose(dicptr);
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
    //char *c = word;
    int c = 0;

    while (word[c] != '\0')
    {

        if (word[c] == 39)
        {
            // left bracket comes immediately after z, and - 'a' will
            // be == 26
            word[c] = '{';
        }
        if (head -> next_char[word[c] - 'a'] == NULL)
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
            head -> next_char[word[c] - 'a'] = child;
            head = child;
        }
        else
        {
            head = head -> next_char[word[c] - 'a'];
        }
        c++;
    }
    head -> end_string = 1;

    return 0;
}

void printWords(node *head, char *prefix, int wordLen)
{
    node *trav = head;

    if (trav -> end_string != 0)
    {
        printf("*%s\n", prefix);
    } else
    {
        printf("%s\n", prefix);
    }

    for (int i = 0; i < CHARSETSIZE; i++)
    {
        if (trav -> next_char[i] != NULL)
        {
            char nextPre[wordLen+1];
            for (int j = 0; j < wordLen; j++)
            {
                nextPre[j] = prefix[j];
            }
            nextPre[wordLen] = i + 97;
            nextPre[wordLen+1] = 0;
            printWords(trav -> next_char[i], nextPre, wordLen+1);
        }
    }
}

unsigned int sizeRecursive(node *head)
{
    unsigned int count = 0;
    //node *travel = head;

    if (head -> end_string == 1)
    {
        count++;
    }

    for (int i = 0; i < CHARSETSIZE; i++)
    {
        if (head -> next_char[i] != NULL)
        {
            count += sizeRecursive(head -> next_char[i]);
        }
    }
    return count;
}

/**
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */
unsigned int size(void)
{
    return sizeRecursive(root);
}

int unloadRecursive(node *head)
{
    node *traveler = head;

    for (int i = 0; i < CHARSETSIZE; i++)
    {
        if (traveler -> next_char[i] != NULL)
        {
            unloadRecursive(traveler -> next_char[i]);
        }
        free(traveler -> next_char[i]);
        traveler -> next_char[i] = NULL;
    }
    return 1;
}

/**
 * Unloads dictionary from memory. Returns true if successful else false.
 */
bool unload(void)
{
    unloadRecursive(root);
    free(root);
    return true;
}
