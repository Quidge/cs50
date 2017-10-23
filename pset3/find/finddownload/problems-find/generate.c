/**
 * generate.c
 *
 * Generates pseudorandom numbers in [0,MAX), one per line.
 *
 * Usage: generate n [s]
 *
 * where n is number of pseudorandom numbers to print
 * and s is an optional seed
 */

#define _XOPEN_SOURCE

#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// upper limit on range of integers that can be generated
#define LIMIT 65536

int main(int argc, string argv[])
{
    // Return failure code and help message if acceptable number of args not provided
    if (argc != 2 && argc != 3)
    {
        printf("Usage: ./generate n [s]\n");
        return 1;
    }

    // I don't understand this. argv[1] should always be a positive number,
    // is atoi() designed to verify that and close out the program if false?
    // Does atoi() do exception handling?
    // After checking, the program exits with a 0 status code if run with args
    // "./generate word 3". No usage message.

    // OH. argv[] is set to be an array of strings. each element is an
    // array of characters. atoi() takes an element element of characters and converts
    // it to an integer.
    int n = atoi(argv[1]);

    // Determines what to do if optional seed arg is/isn't present.
    // Because when seed arg isn't present a time(NULL) arg is provided, I think
    // that srand48 mixes system time with the seed arg in some way when it is
    // provided.

    // No. srand must be run before drand can be run. This is running the
    // initialization necessary for drand to be run. Running srand initializes the
    // first 48bit number.
    if (argc == 3)
    {
        srand48((long) atoi(argv[2]));
    }
    else
    {
        srand48((long) time(NULL));
    }

    // For argv[1] iterations, print out random numbers. Cast the number to an
    // int, but I'm not sure why. Why not keep it a long?
    // There's some cool stuff happeneing here with the state being 'shifted'
    // after every use of drand().
    for (int i = 0; i < n; i++)
    {
        printf("%i\n", (int) (drand48() * LIMIT));
    }

    // success
    // Why make this explicit? Does it default return 0?
    return 0;
}
