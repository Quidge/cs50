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
            finalArray[currentIndex] = countingArray[j];
            currentIndex++;
        }
    }
    values = finalArray;
    return;
}