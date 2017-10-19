#include <stdio.h>
#include <cs50.h>
#include <unistd.h>
#include <crypt.h>
#include <string.h>

int main(int argc, string argv[])
{
    if (argc != 2)
    {
        printf("Usage: ./crack hash\n");
        return 1;
    }

    string HASH = argv[1];

    int charSet[52] = {
        65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75,
        76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87,
        88, 89, 90, 97, 98, 99, 100, 101, 102, 103, 104,
        105, 106, 107, 108, 109, 110, 111, 112, 113, 114,
        115, 116, 117, 118, 119, 120, 121, 122
    };
    char attempt[5] = {'\0', '\0', '\0', '\0', '\0'};

    string result = "";

    int counters[] = {0,0,0,0};

    while (strcmp(HASH, result) != 0)
    {
        if (counters[0] < 52)
        {
            attempt[0] = charSet[counters[0]];
            counters[0]++;
        }
        else if (counters[1] < 52)
        {
            counters[0] = 0;
            attempt[1] = charSet[counters[1]], ;
            counters[1]++;
        }
        else if (counters[2] < 52)
        {
            counters[0] = 0;
            counters[1] = 0;
            attempt[2] = charSet[counters[2]];
            counters[2]++;
        }
        else if (counters[3] < 52)
        {
            counters[0] = 0;
            counters[1] = 0;
            counters[2] = 0;
            attempt[3] = charSet[counters[3]];
            counters[3]++;
        }
        result = crypt(attempt, HASH);
    };
    printf("%s\n", attempt);
}