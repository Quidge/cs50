from cs50 import get_string
from sys import argv
from crypt import crypt

def main():
    if (len(argv) != 2):
        print("Wrong # of args")
        return 1

    chSet = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"
    chSet2 = " abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"
    chSet3 = "ROFL"

    for ch0 in chSet3:
        for ch1 in chSet3:
            for ch2 in chSet3:
                for ch3 in chSet3:
                    for ch4 in chSet3:
                        string = f"{ch0}{ch1}{ch2}{ch3}{ch4}"
                        if crypt(string, argv[1]) == argv[1]:
                            print("what")
                            return 5

if __name__ == "__main__":
    main()