from cs50 import get_int

#print("Height: ", end="")
total_lines = get_int("Height: ")

while (total_lines > 23 or total_lines < 0):
    total_lines = get_int("Height: ")

for cur_line in range(1, total_lines):
    # print spaces on left side of i'th level
    for j in range(total_lines - cur_line):
        print(" ", end="")
    # print left sidde # signs
    for j in range(cur_line):
        print("#", end="")
    # print middle empty space column
    print(" ", end="")
    # print right side # signs
    for j in range(cur_line):
        print("#", end="")
    print("")