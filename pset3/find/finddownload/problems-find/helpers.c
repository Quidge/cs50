/**
 * helpers.c
 *
 * Helper functions for Problem Set 3.
 */

#include <cs50.h>
#include <stdio.h>

#include "helpers.h"

/**
 * Returns true if value is in array of n values, else false.
 */
bool search(int target, int values[], int n)
{
    printf("searching for: %i\n", target);
    printf("passed array contains: ");
    for (int q = 0; q < n; q++)
    {
        printf("%i ", values[q]);
    }
    printf("\n");
    while (n > 0)
    {
        int middlei;
        int middleVal;
        if (n%2 != 0)
        {
            middlei = (n - 1) / 2;
            middleVal = values[middlei];
        }
        else
        {
            middlei = n/2;
            middleVal = values[middlei];
        }
        printf("middle index: %i, middleVal: %i\n", middlei, middleVal);
        if (target == middleVal)
        {
            printf("found!\n\n");
            return true;
        }
        else if (middlei == 0)
        {
            return false;
        }
        else if (target > middleVal)
        {
            printf("target > middleVal\n");
            int rightSide[middlei];
            int j = 0;
            for (int i = middlei + 1; i < n; i++)
            {
                rightSide[j] = values[i];
                j++;
            }
            return search(target, rightSide, j);
        }
        else
        {
            printf("target < middleVal\n");
            int leftSide[middlei];
            int leftLength = 0;
            for (int i = 0; i < middlei; i++)
            {
                leftSide[i] = values[i];
                leftLength++;
            }
            return search(target, leftSide, leftLength);
        }
    }
   /* pseudo code:
   * take middle (rounded down) of array
   * if target IS middle, return true
   * if target > middle, recurse with right side
   * if target < middle, recurse with left side
   */
   return false;
}

/**
 * Sorts array of n values.
 */
void sort(int values[], int n)
{
    const int MAX = 65536;

    // setup countingArray and be sure that all elements are value 0
    int countingArray[MAX];
    for (int i = 0; i < MAX; i++)
    {
        countingArray[i] = 0;
    }

    // loop through elements in array arg and increment values at corresponding
    // index positions in countingArray
    for (int i = 0; i < n; i++)
    {
        countingArray[values[i]] = countingArray[values[i]] + 1;
    }

    // init a counter to be used as the index, and a temp resulting array
    int sortedArrayIndex = 0;
    int tempResult[n];
    // run through all the possible numbers ([0, MAX]), but stop if original number of
    // arg array elements have been sorted
    for (int countArrayIndex = 0; countArrayIndex < MAX && sortedArrayIndex < n; countArrayIndex++)
    {
        // most of the time this will be 0, when it isn't
        int currentCheck = countingArray[countArrayIndex];
        if (currentCheck > 0)
        {
            for (int k = 0; k < currentCheck; k++)
            {
                tempResult[sortedArrayIndex] = countArrayIndex;
                sortedArrayIndex++;
            }
        }
    }
    // From how I understand find, I believe it wants sort to alter the the array. NOT create
    // a copy ...which seems stupid.
    for (int g = 0; g < n; g++)
    {
        values[g] = tempResult[g];
    }
    return;
}