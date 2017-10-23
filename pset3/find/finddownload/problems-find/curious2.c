#include <stdio.h>
#include <cs50.h>

const int MAX = 65536;

int main(void)
{
    int array[MAX];
    for (int i = 0; i < MAX; i++)
    {
        printf("index %i, val: %i\n", i, array[i]);
    }
}