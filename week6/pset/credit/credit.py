from cs50 import get_string
import sys


def sumDigits(i):
    return sum([int(x) for x in str(i)])


def luhn(cc_number):
    return str(sum([sumDigits(int(i)*2) for i in cc_number[-2::-2]]) + sum([int(i) for i in cc_number[::-2]]))[-1] == '0'


# get cc number from user
cc_number = get_string("Number: ")

if luhn(cc_number):

    # VISA CHECK
    if (len(cc_number) in [13, 16]) and cc_number[0] == '4':
        print("VISA")
        sys.exit(0)

    # AMEX CHECK
    elif len(cc_number) == 15 and (cc_number[0:2] in ['34', '37']):
        print("AMEX")
        sys.exit(0)

    # MC CHECK
    elif len(cc_number) == 16 and (cc_number[0:2] in ['51', '52', '53', '54', '55']):
        print("MASTERCARD")
        sys.exit(0)

print("INVALID")
sys.exit(1)