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
    while (n > 0)
    {
        // Set middle value; sets to right of middle if even: ie 4 vals, middleVal is index 2
        int middlei = (n%2 != 0) ? (n-1)/2 : n/2;
        int middleVal = values[middlei];

        // shortcircuit if middle happens to be target
        if (target == middleVal)
        {
            return true;
        }
        // If middle index is 0, search section n must be of length 1. And if the
        // previous if conditional didn't cause a short circuit, that must mean the
        // target is outside the range of the search array and the search should fail.
        else if (middlei == 0)
        {
            return false;
        }
        // Break the array that was passed in into left and right sections to be
        // recursively searched again.
        else
        {
            int subsection[middlei];
            int newLen = 0;
            // Don't include the middleindex value because that was already checked
            // against the target.
            if (target > middleVal)
            {
                // Offset because target is to the right of middle.
                for (int i = middlei + 1; i < n; i++)
                {
                    subsection[i - middlei - 1] = values[i];
                    newLen++;
                }
            }
            else
            {
                for (int i = 0; i < middlei; i++)
                {
                    subsection[i] = values[i];
                    newLen++;
                }
            }
            // Recursion. Pass in increasingly smaller subsections.
            return search(target, subsection, newLen);
        }
    }
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