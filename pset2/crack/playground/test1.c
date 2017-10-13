#include <stdio.h>
#include <cs50.h>
#include <unistd.h>
#include <crypt.h>
#include <string.h>

#define HASH "50.jPgLzVirkc"
#define SALT "50"

int main(void)
{
    printf("hash: %s\ncrypt: %s\npass: '%s'", HASH, crypt("hi", SALT), "hi");
}