#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <ctype.h>

/* 1. Get full name string from user input
* 1.5 Initialize a position var.
* 2. Parse string for first instance of non-whitespace char. Save that as position i.
* 3. Print a capitalized character at string position i.
*/

int main(void)
{
    string s = get_string();
    int pos = 0;
    while (pos < strlen(s))
    {
        if (s[pos] > 64 && s[pos] < 123 && (s[pos - 1] == 32 || s[pos - 1] == 0))
        {
            printf("%c", toupper(s[pos]));
        }
        pos++;
    }
    printf("\n");
}