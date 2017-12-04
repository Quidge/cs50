#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

// Model heavily taken from CS50 shorts on tries and
// http://www.crazyforcode.com/trie-data-structure-implementation/

// Also, helpful understanding of when typedef is necessary from
// https://stackoverflow.com/questions/612328/difference-between-struct-and-typedef-struct-in-c
typedef struct node
{
    int end_string;
    struct node* next_char[26];
    struct node* prev_char;
}
node;

unsigned int size(node *root);
void insert(char *word, node *root);

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
        printf("Could not locate %s\n", argv[1]);
        return 2;
    }
    fclose(fp);

    // setup root node
    node *root = malloc(sizeof(node));
    for (int i = 0; i < 26; i++)
    {
        root -> next_char[i] = NULL;
    }
    root -> prev_char = NULL;

    char *word = "he";
    insert(word, root);
    //unsigned int count = size(root);

    printf("%i\n", size(root));

    free(root);
}

void insert(char *word, node *root)
{
    node *head = root;
    int c = 0;

    while (word[c] != '\0')
    {
        if (head -> next_char[word[c] - 65] == NULL)
        {
            // child doesn't exist, so setup new child
            node *child = malloc(sizeof(node));
            child -> prev_char = head;
            child -> end_string = 0;
            for (int i = 0; i < 26; i++)
            {
                child -> next_char[i] = NULL;
            }

            // child becomes new head
            head -> next_char[word[c] - 65] = child;
            head = child;
        }
        else
        {
            head = head -> next_char[word[c] - 65];
        }
        c++;
    }
    head -> end_string = 1;
    printf("%i\n", c);
}

unsigned int size(node *root)
{
    unsigned int count = 0;
    node *head = root;

    if (head -> end_string == 1)
    {
        return 1;
    }
    else
    {
        for (int i = 0; i < 26; i++)
        {
            if (head -> next_char[i] != NULL)
            {
                count = count + size(head -> next_char[i]);
            }
        }
    }
    return count;
}