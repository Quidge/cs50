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

    // setup root manode
    node *root = malloc(sizeof(node));
    for (int i = 0; i < 26; i++)
    {
        root -> next_char[i] = NULL;
    }
    root -> prev_char = NULL;

    char *word = "ab";
    insert(word, root);
    insert("abra", root);
    insert("aba", root);
    insert("data", root);
    insert("abraca", root);
    //unsigned int count = size(root);

    printf("%i\n", size(root));
    //node *second = root -> next_char[0];
    //printf("%s\n", (root -> next_char[0] != NULL) ? "true" : "false");
    //printf("%i\n", word[0]-65);

    free(root);
}

void insert(char *word, node *root)
{
    node *head = root;
    int c = 0;

    while (word[c] != '\0')
    {
        if (head -> next_char[word[c] - 'a'] == NULL)
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
}

unsigned int size(node *root)
{
    unsigned int count = 0;
    node *head = root;

    if (head -> end_string == 1)
    {
        count++;
    }

    for (int i = 0; i < 26; i++)
    {
        if (head -> next_char[i] != NULL)
        {
            count = count + size(head -> next_char[i]);
        }
    }
    return count;
}