from enum import Enum


class Operation(Enum):
    """Operations"""

    DELETED = 1
    INSERTED = 2
    SUBSTITUTED = 3

    def __str__(self):
        return str(self.name.lower())

def distances(a, b):
    """Calculate edit distance from a to b"""

    # must be initialized so that access is y,x; not x,y as in most
    # graph stuff
    # syntax: "(do this thing) for every increment from 0 to whatever"

    # a string is left side; # b string is top
    # first layer is y; second layer is x
    matrix = [[('ERR', None) for col in range((len(b) + 1))] for row in range((len(a) + 1))]

    # initialize right side border with edge case values
    cost = 0
    for i in range(len(b)+1):
        matrix[0][i] = (cost, Operation.INSERTED)
        cost += 1
    cost = 0
    # initialize left side border with edge cases
    for i in range(len(a)+1):
        matrix[i][0] = (cost, Operation.DELETED)
        cost += 1
    # initialize top right corner
    matrix[0][0] = (0, None)

    # begin building edit distances
    for y in range(1, len(a)+1):
        for x in range(1, len(b)+1):

            # this is the default cost of switching characters
            swapcost = 1

            # run a conditional to check if chars are same, and change
            # cost to for this matrix element if so
            try:
                if b[x-1] == a[y-1]:
                    swapcost = 0
            except Exception:
                print(f"x: {x}, y: {y}")
                print(f"Thrown error: {Exception}")
                # I should NOT receive out of bounds errors here.
                # both y and x loops should only go the length
                # of their word.

            # calc options to substitute, insert, and delete
            # because this is built from bottom right to top left, it's
            # actually behaving recursively even though it doesn't look like it.
            # Each step is only checking three possible options instead of
            # all possible paths, each time.
            options = []

            try:
                options = [
                        (matrix[y-1][x-1][0] + swapcost, Operation.SUBSTITUTED),
                        (matrix[y-1][x][0] + 1, Operation.INSERTED),
                        (matrix[y][x-1][0] + 1, Operation.DELETED)
                    ]
            except Exception:
                options = [("ERR", Operation.SUBSTITUTED)]
                print(f"These indices (may) be causing errors: y: {y}, x: {x}")
                print(f"ERROR: {Exception}")

            # set matrix position to best possible option (determined
            # by score which will be option[0])
            # sorted returns a new, sorted copy. sort() returns void and sorts
            # in place
            matrix[y][x] = sorted(options, key=lambda option: option[0])[0]
            #cost = min([matrix[y+1][x+1][0] + swapcost, matrix[y+1][x][0] + 2, matrix[y][x+1][0] + 2])
    return matrix