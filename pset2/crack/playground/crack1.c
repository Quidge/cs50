#include <stdio.h>
#include <cs50.h>
#include <unistd.h>
#include <crypt.h>
#include <string.h>

#define HASH "50CPlMDLT06yY"

int main(void)
{
    int charSet[52] = {
        65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75,
        76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87,
        88, 89, 90, 97, 98, 99, 100, 101, 102, 103, 104,
        105, 106, 107, 108, 109, 110, 111, 112, 113, 114,
        115, 116, 117, 118, 119, 120, 121, 122
    };
    char attempt[5];
    attempt[1] = '\0';
    attempt[2] = '\0';
    attempt[3] = '\0';
    attempt[4] = '\0';

    string result = "";

    int counter0 = 0;
    int counter1 = 0;
    int counter2 = 0;
    int counter3 = 0;

    while (strcmp(HASH, result) != 0)
    {
        if (counter0 < 52)
        {
            attempt[0] = charSet[counter0];
            counter0++;
        }
        else if (counter1 < 52)
        {
            counter0 = 0;
            attempt[1] = charSet[counter1];
            counter1++;
        }
        else if (counter2 < 52)
        {
            counter0 = 0;
            counter1 = 0;
            attempt[2] = charSet[counter2];
            counter2++;
        }
        else if (counter3 < 52)
        {
            counter0 = 0;
            counter1 = 0;
            counter2 = 0;
            attempt[3] = charSet[counter3];
            counter3++;
        }
        result = crypt(attempt, HASH);
        //printf("%s\n", result);
        //printf("%i%i%i%i\n", counter0, counter1, counter2, counter3);
    };
    printf("pass: '%s', gives hash: %s\n", attempt, result);
}