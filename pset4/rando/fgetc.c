#include <stdio.h>

int main(void)
{
    FILE* ptr = fopen("./file.txt", "r");
    if (ptr == NULL)
        return 1;

    char ch;
    while ((ch = fgetc(ptr)) != EOF)
    {
        printf("%c", ch);
    }
    printf("\n");
    fclose(ptr);
}