#include <stdio.h>

int main()
{
    /*char *str = "string's";

    int c = 0;
    while (str[c] != '\0')
    {
        printf("dec: %i, hex: %x\n", str[c], str[c]);
        if (str[c] == 39)
        {
            printf("true!\n");
        }
        c++;
    }*/

    char str[] = "hello";
    str[0] = 'N';
    printf("%s\n", str);

    char *strB = "hello";
    while (*strB != '\0')
    {
        if (*strB == 'l')
        {
            *strB = 'k';
        }
        strB++;
        printf("%c", *strB);
    }
    printf("\ndone\n");
}