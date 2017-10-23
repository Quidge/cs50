#include <cs50.h>
#include <stdio.h>

void sort(int values[], int n);

int main(void) {
    int size = 6;
    int array[] = { 5, 2, 5, 6, 2, 3 };

    for (int i = 0; i < size; i++)
    {
        printf("%i ", array[i]);
    }
    printf("\n");

    sort(array, size);
    for (int i = 0; i < size; i++)
    {
        printf("%i ", array[i]);
    }
    printf("\n");
}

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