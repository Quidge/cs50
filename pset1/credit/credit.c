#include <stdio.h>
#include <cs50.h>

int sizeCheck(long long num);
bool checkSum(long long num);
long long getDigit(long long num, int pos);
int getCardType(long long cardNumber);

int main(void)
{
    printf("Number: ");
    long long cardNum = get_long_long();
    int cardType = getCardType(cardNum);
    string result = "null\n";
    if (!checkSum(cardNum))
    {
        result = "INVALID\n";
    }
    switch (cardType)
    {
        case 1:
            result = "AMEX\n";
            break;
        case 2:
            result = "MASTERCARD\n";
            break;
        case 3:
            result = "VISA\n";
            break;
    }
    printf("%s", result);
}

bool checkSum(long long cardNum)
{
    int doubledSum = 0;
    int restSum = 0;
    for (int i = 0; i < sizeCheck(cardNum); i++)
    {
        int workDigit = getDigit(cardNum, i + 1);
        if (i % 2 == 0)
        {
            restSum = restSum + workDigit;
            //printf("i is: %i, workDigit is: %i, restSum is: %i\n", i, workDigit, restSum);
        }
        else
        {
            int midCheck = workDigit * 2;
            if (midCheck > 9)
            {
                doubledSum = doubledSum + getDigit(midCheck, 1) + getDigit(midCheck, 2);
            }
            else
            {
                doubledSum = doubledSum + midCheck;
            }
            //printf("i is: %i, workDigit is: %i, doubledSum is %i\n", i, workDigit, doubledSum);
        }
    }
    if ((doubledSum + restSum) % 10 == 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}

int getCardType(long long cardNumber)
{
    int cardType = 0; // INVALID
    int firstDigit = getDigit(cardNumber, sizeCheck(cardNumber));
    int secondDigit = getDigit(cardNumber, sizeCheck(cardNumber) - 1);
    if (firstDigit == 3 && (secondDigit == 7 || secondDigit == 5))
    {
        cardType = 1; // AMEX
    }
    else if (firstDigit == 5 && (secondDigit > 0 && secondDigit < 6))
    {
        cardType = 2; // MASTERCARD
    }
    else if (firstDigit == 4)
    {
        cardType = 3; // VISA
    }
    return cardType;
}

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