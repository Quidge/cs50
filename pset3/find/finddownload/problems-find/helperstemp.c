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
bool search(int target, int values[], int n)
{
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
            middleVal = values[middlei - 1];
        }

        if (target == middleVal)
        {
            return true;
        }
        else if (target > middleVal)
        {
            int rightSide[];
            for (int i = middlei + 1, int j = 0; i < n; i++)
            {
                rightSide[j] = values[i];
                j++;
            }
            search(target, rightSide, j + 1);
        }
        else
        {
            int leftSide[];
            int leftLength = 0;
            for (int i = 0; i < middle; i++)
            {
                leftSide[i] = values[i];
            }
            search(target, leftSide, i + 1);
        }
    }
   /* pseudo code:
   * take middle (rounded down) of array
   * if target IS middle, return true
   * if target > middle, recurse with right side
   * if target < middle, recurse with left side


   */

    // SLOW: linear search O(n)

    /*for (int i = 0; i < n; i++)
    {
        if (value == values[i])
        {
            return true;
        }
    }*/

    // FASTER: binary search O(log n) (requires sorted array)

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