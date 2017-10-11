#include <stdio.h>
#include <cs50.h>

int sizeCheck(long long num);

int main(void)
{
    long long num = get_long_long();
    printf("%i digits long", sizeCheck(num));
}

int sizeCheck(long long num)
{
    long long test = 1;
    int counter = 0;
    while (test < num)
    {
        counter++;
        test = test * 10;
    }
    return counter;
}