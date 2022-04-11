from cs50 import get_int

h = 0

# get height from user
while not (1 <= h <= 8):
    h = get_int("Height: ")

# print pyramid
for i in range(1, h+1):
    print(' ' * (h - i) + '#'*i + '  ' + '#'*i)