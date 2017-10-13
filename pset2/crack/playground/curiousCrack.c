#include <stdio.h>
#include <cs50.h>
#include <unistd.h>
#include <crypt.h>
#include <string.h>

#define HASH "50.jPgLzVirkc"
#define SALT "50"

int main(void)
{
    char s[7] = "fiveC\0C";
    s[1] = 65;
    int i = 0;
    while (i < strlen(s))
    {
        printf("'%c'", s[i]);
        i++;
    }
}