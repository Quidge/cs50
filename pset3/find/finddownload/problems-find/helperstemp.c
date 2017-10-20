/**
 * helpers.c
 *
 * Helper functions for Problem Set 3.
 */

#include <cs50.h>

#include "helpers.h"

/**
 * Returns true if value is in array of n values, else false.
 */
bool search(int value, int values[], int n)
{
    if (n < 1)
    {
        return false;
    }

    // TODO: implement a FASTER searching algorithm

    for (int i = 0; i < n; i++)
    {
        if (value == values[i])
        {
            return true;
        }
    }
    return false;
}

/**
 * Sorts array of n values.
 */
void sort(int values[], int n)
{
    int finalArray[n];
    int countingArray[65536];

    for (int i = 0; i < n; i++)
    {
        countingArray[values[i]] = countingArray[values[i]] + 1;
    }
    int currentIndex = 0;
    for (int j = 0; j < 65536; j++)
    {
        for (int k = 0; countingArray[j] > 0 && k < countingArray[j]; k++)
        {
            finalArray[curren˜tIndex] = countingArray[j];
            currentIndex++;
        }
    }
    values = finalArray;
    return;
}

