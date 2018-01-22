from cs50 import get_int
from math import isnan

def validate(ccnum):
    numstr = str(ccnum)
    # immediate dismissal based on length
    if 13 > len(numstr) > 16:
        #print("Too short/too long")
        return False

    secondssum = 0
    firstssum = 0

    # this iterates backwards through the cc number
    # range(start, up to but stop before, increment)
    # doing the values in this way will allow the string indexing
    # to work correctly

    #print(f"Validating this cc number: {numstr}")

    for digit in range(len(numstr), 0, -1):
        # this targets every other digit, starting with the last digit
        #print(f"Digit: {numstr[digit-1]}", end="")
        if ((len(numstr) - digit) % 2 == 0):
            firstssum = firstssum + int(numstr[digit-1])
            #print(f" (null) | firstssum: {firstssum}")
        else:
            temp = int(numstr[digit-1]) * 2
            #print(f" x2= {temp} | ", end="")
            for tempdigit in str(temp):
                secondssum = secondssum + int(tempdigit)
                #print(f"split: {tempdigit}", end="")
            #print(f" | secondssum: {secondssum}")

    #print(f"secondssum: {secondssum}, firstssum: {firstssum}")

    # add both sums together; if the last digit is 0, then card is legit
    if (secondssum + firstssum) % 10 != 0:
        #print("Failed checksum")
        return False

    return True



def main():
    cardnumber = get_int("Number: ")
    while isnan(cardnumber):
        cardnumber = get_int("Number: ")
    strnum = str(cardnumber)

    # put it through the validator; if it gets through, it's PROBABLY
    # a real card
    # then, check the first numbers to determine type
    if validate(cardnumber):
        if int(strnum[0]) == 3 and (int(strnum[1]) == 7 or int(strnum[1] == 5)):
            print("AMEX")
        elif int(strnum[0]) == 5 and 6 > int(strnum[1]) > 0:
            print("MASTERCARD")
        elif int(strnum[0]) == 4:
            print("VISA")
        else:
            print("INVALID")
    else:
        print("INVALID")

if __name__ == "__main__":
    main()