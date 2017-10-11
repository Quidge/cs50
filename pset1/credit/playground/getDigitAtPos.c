#include <stdio.h>
#include <cs50.h>

int pow3(int num, int power);
long long getDigit(long long num, int pos);

int main(void)
{
    printf("Number?: ");
    long long num = get_long_long();
    printf("Which digit position do you want? (1-indexed): ");
    int pos = get_int();

    printf("%lld\n", getDigit(num, pos));
}

/* 932 / 10 = 93 + r2
*  932 - 2 = 930
*  930 / 10 = 93
*  93 / 10 = 9 + r3
*/

long long getDigit(long long num, int pos)
{
    long long remainder = 0;
    long long workingNum = num;
    for (int i = 0; i < pos; i++)
    {
        remainder = workingNum % 10;
        workingNum = (workingNum - remainder) / 10;
    }
    return remainder;
}

int pow3(int num, int power)
{
    int result = 1;
    if (power == 0)
    {
        return result;
    }
    else
    {
        for (int i = 0; i < power; i++)
        {
            result = result * num;
        }
    }
    return result;
}