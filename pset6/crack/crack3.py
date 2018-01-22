from sys import argv
from crypt import crypt

def main():

    if len(argv) != 2:
        print("Usage: python crack.py [HASH]")

    hashval = argv[1]

    chset = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"

    response = crack(chset, hashval)

    if not response:
        print("crack failed")
    else:
        print(response)


def crack(chset, hashval):

    for current in range(5):
        letrs = [ch for ch in chset]
        for y in range(current):
            letrs = [substr+i for i in chset for substr in letrs]
        for attempt in letrs:
            if crypt(attempt, hashval) == hashval:
                return attempt

    return False

if __name__ == "__main__":
    main()