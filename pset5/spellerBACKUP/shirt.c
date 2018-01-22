#include <stdio.h>

const int *number;

int main()
{
    int temp = 5;
    int temp2 = 2;
    number = &temp;
    number = &temp2;
    printf("%d\n", *number);
    char c = '\0';
    printf("%i\n", c);
}