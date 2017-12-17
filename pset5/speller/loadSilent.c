#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#define CHARSETSIZE 27
#define LENGTH 45

#define DICTIONARY "dictionaries/large"

// Model heavily taken from CS50 shorts on tries and
// http://www.crazyforcode.com/trie-data-structure-implementation/

// Also, helpful understanding of when typedef is necessary from
// https://stackoverflow.com/questions/612328/difference-between-struct-and-typedef-struct-in-c
typedef struct node
{
    int end_string;
    struct node* next_char[CHARSETSIZE];
    struct node* prev_char;
}
node;

node *root = NULL;

unsigned int size(node *root);
void insert(char word[]);
int unload(node *root);
int load(const char *dictionary);

int main(int argc, char *argv[])
{
    if (argc != 2 && argc != 3)
    {
        printf("Usage: ./program [dictionary] text");
        return 1;
    }

    char *dictionary = (argc == 3) ? argv[1] : DICTIONARY;

    char *text = (argc == 3) ? argv[2] : argv[1];
    FILE *fp = fopen(text, "r");
    if (fp == NULL)
    {
        fclose(fp);
        printf("Could not locate %s\n", text);
        return 2;
    }

    // setup root node
    root = malloc(sizeof(node));
    if (root == NULL) {
        //crash out. everything depends on root being valid
        fprintf(stderr, "It appears root is NULL");
        return 2;
    }
    for (int i = 0; i < CHARSETSIZE; i++)
    {
        root -> next_char[i] = NULL;
    }
    root -> prev_char = NULL;

    load(dictionary);
    printf("%i words loaded\n", size(root));
    unload(root);
    printf("%i\n", size(root));
    fclose(fp);
    free(root);
}

void insert(char word[])
{
    if (root == NULL)
    {
        fprintf(stderr, "root node is NULL");
        return;
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
}

unsigned int size(node *root)
{
    unsigned int count = 0;
    node *head = root;

    if (head -> end_string == 1)
    {
        count++;
    }

    for (int i = 0; i < CHARSETSIZE; i++)
    {
        if (head -> next_char[i] != NULL)
        {
            count += size(head -> next_char[i]);
        }
    }
    return count;
}

int unload(node *root)
{
    node *head = root;

    for (int i = 0; i < CHARSETSIZE; i++)
    {
        if (head -> next_char[i] != NULL)
        {
            unload(head -> next_char[i]);
        }
        free(head -> next_char[i]);
        head -> next_char[i] = NULL;
    }
    return 1;
}

int load(const char *dictionary)
{
    FILE *dicptr = fopen(dictionary, "r");
    if (dicptr == NULL)
    {
        fprintf(stderr, "Could not load %s\n", dictionary);
        return 0;
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
            insert(word);
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
        return 1;
    } else
    {
        return 0;
    }
}