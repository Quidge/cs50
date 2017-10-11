#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <ctype.h>

int main(void)
{
    string s = get_string();
    // because the pset instructions tell me I don't have to worry about
    // crazy characters, this version should pass all tests and uses
    // less code.
    for (int i = 0, j = strlen(s); i < j; i++)
    {
        if (s[i - 1] < 65 && s[i] != 32)
        {
            printf("%c", toupper(s[i]));
        }
    }
    printf("\n");
}