from sys import argv, exit
from crypt import crypt

if len(argv) != 2:
    print("Usage: python crack.py [HASH]")

hashval = argv[1]

chset = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"
setrange = 52

attempt = "     " # 5 whitespace chars

c1 = 0
c2 = 0
c3 = 0
c4 = 0
c5 = 0

'''
The dance here isn't intuitive. The idea is that you'll try
'a _ _ _ _' through 'z _ _ _ _', but then it continues to
'a a _ _ _ _' through 'z a _ _ _', then 'a b _ _ _' through
'z b _ _ _', etc.
'''

while crypt(attempt.rstrip(), hashval) != hashval:
    if c1 < setrange:
        attempt = chset[c1]+attempt[1]+attempt[2]+attempt[3]+attempt[4]
        #attempt = f"{chset[c1]}"
        #attempt[0] = chset[c1]
        c1 += 1
    elif c2 < setrange:
        c1 = 0
        attempt = attempt[0]+chset[c2]+attempt[2]+attempt[3]+attempt[4]
        #attempt = f"{chset[c1]}{chset[c2]}"
        #attempt[1] = chset[c2]
        c2 += 1
    elif c3 < setrange:
        c1 = 0
        c2 = 0
        attempt = attempt[0]+attempt[1]+chset[c3]+attempt[3]+attempt[4]
        #attempt = f"{chset[c1]}{chset[c2]}{chset[c3]}"
        #attempt[2] = chset[c3]
        c3 += 1
    elif c4 < setrange:
        c1 = 0
        c2 = 0
        c3 = 0
        attempt = attempt[0]+attempt[1]+attempt[2]+chset[c4]+attempt[4]
        #attempt = f"{chset[c1]}{chset[c2]}{chset[c3]}{chset[c4]}"
        #attempt[3] = chset[c4]
        c4 += 1
    elif c5 < setrange:
        c1 = 0
        c2 = 0
        c3 = 0
        c4 = 0
        attempt = attempt[0]+attempt[1]+attempt[2]+attempt[3]+chset[c5]
        c5 += 1
    #print(attempt)

print(attempt.rstrip())