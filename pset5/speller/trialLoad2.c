#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#define CHARSETSIZE 27
#define LENGTH 45

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

unsigned int size(node *root);
void insert(char word[], node *root);
int unload(node *root);
int load(FILE *fp, node *root);

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "Program supplied with more or less than 2 arguments\n");
        return 1;
    }

    FILE *fp = fopen(argv[1], "r");
    if (fp == NULL)
    {
        fclose(fp);
        printf("Could not locate %s\n", argv[1]);
        return 2;
    }

    // setup root node
    node *root = malloc(sizeof(node));
    for (int i = 0; i < CHARSETSIZE; i++)
    {
        root -> next_char[i] = NULL;
    }
    root -> prev_char = NULL;

    /*char word[] = "abra's";
    char word2[] = "elephant";
    insert(word, root);
    insert(word2, root);
    printf("all words loaded\n");

    printf("%i\n", size(root));
    unload(root);
    printf("%i\n", size(root));*/

    load(fp, root);
    printf("%i words loaded\n", size(root));
    unload(root);
    printf("%i\n", size(root));
    fclose(fp);
    free(root);
}

void insert(char word[], node *root)
{
    node *head = root;
    char *c = word;

    while (*c != '\0')
    {

        if (*c == 39)
        {
            // left bracket comes immediately after z, and - 'a' will
            // be == 26
            *c = 0x7B;
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

int load(FILE *fp, node *root)
{
    int curWordSize = 0;
    char word[LENGTH];

    char ch = fgetc(fp);
    while (ch != EOF)
    {
        printf("%i / %c\n", ch, ch);
        if (ch == '\n')
        {
            printf("newline!\n");
            curWordSize++;
            word[curWordSize] = '\0';
            insert(word, root);
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

        ch = fgetc(fp);
    }

    if (feof(fp) != 1)
    {
        return 1;
    } else
    {
        return 0;
    }
}