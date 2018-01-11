from cs50 import get_string
from sys import argv
from crypt import crypt

def main():
    if (len(argv) != 2):
        print("Wrong # of args")
        return 1

    chSet = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"
    chSet2 = " abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"
    chSet3 = "rofl "

    '''for ch0 in chSet:
        for ch1 in chSet2:
            for ch2 in chSet2:
                for ch3 in chSet2:
                        string = f"{ch0}{ch1}{ch2}{ch3}"
                        #print(string)
                        print(crypt(string, argv[1]))
                        if crypt(string, argv[1]) == argv[1]:
                            print("what")
                            return 5
    '''
    print(crack(argv[1], chSet))

def crack(hashval, chSet):
    #withspace = " " + chSet
    chSet = " abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"


    for ch0 in chSet:
        for ch1 in chSet:
            for ch2 in chSet:
                for ch3 in chSet:
                    for ch4 in chSet:
                        #attempt = f"{ch0}{ch1}{ch2}{ch3}{ch4}".rstrip()
                        attempt = f"{ch4}{ch3}{ch2}{ch1}"
                        if crypt(attempt, argv[1]) == argv[1]:
                            return attempt

if __name__ == "__main__":
    main()