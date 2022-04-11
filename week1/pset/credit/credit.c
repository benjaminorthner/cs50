// Implementation of Luhn's Algorithm for VISA, MASTERCARD and AMERICAN EXPRESS

#import <stdio.h>
#import <cs50.h>
#import <math.h>

bool luhn(long n);
int digitalsum(int n);
int count_digits(long n);
int char_to_int(char c);
int get_first_two_digits(long n);
int get_first_digit(long n);

int main(void)
{
    long cc_number = get_long("Number: ");
    int cc_length = count_digits(cc_number);

    // get first digit
    int f = get_first_digit(cc_number);

    // get first two digits
    int ft = get_first_two_digits(cc_number);

    //if valid
    if (luhn(cc_number))
    {
        // VISA CHECK
        if ((cc_length == 13 || cc_length == 16) && f == 4)
        {
            printf("VISA\n");
        }
        else if (cc_length == 15 && (ft == 34 || ft == 37))
        {
            printf("AMEX\n");
        }
        else if (cc_length == 16 && (ft == 51 || ft == 52 || ft == 53 || ft == 54 || ft == 55))
        {
            printf("MASTERCARD\n");
        }
        else
        {
            printf("INVALID\n");
        }
    }
    else
    {
        printf("INVALID\n");
    }
}


// performs luhn's Algorithm and returns True or False based one check
bool luhn(long n)
{
    // initialise string to store number with correct size
    int length = count_digits(n);
    char str[length * sizeof(char)];

    // print number into string
    sprintf(str, "%li", n);

    // multiply every second digit by two, take the digital sum and add to a total
    int total1 = 0;
    for (int i = length - 2; i >= 0; i -= 2)
    {
        total1 += digitalsum(2 * char_to_int(str[i]));
    }

    // sum of the digits not multiplied by two
    int total2 = 0;
    for (int i = length - 1; i >= 0; i -= 2)
    {
        total2 += char_to_int(str[i]);
    }

    // convert sum of totals to string
    int total_sum = total1 + total2;
    int total_sum_length = count_digits(total_sum);
    char total_sum_string[(total_sum_length) * sizeof(char)];
    sprintf(total_sum_string, "%i", total_sum);

    //
    if (char_to_int(total_sum_string[total_sum_length - 1]) == 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}

// calculates the digital sum of a one or two digit number
int digitalsum(int n)
{
    if (count_digits(n) == 1)
    {
        return n;
    }
    else
    {
        // initialise 2 digit string, print integer to it and return last digit
        char sn[2 * sizeof(char)];
        sprintf(sn, "%i", n);

        //calc and return the sum of the digits (convert from char to int)
        int digit1 = char_to_int(sn[0 * sizeof(char)]);
        int digit2 = char_to_int(sn[1 * sizeof(char)]);

        return digit1 + digit2;
    }

}

// calculates the number of digits in an integer
int count_digits(long n)
{
    if (n == 0)
    {
        return 1;

    }
    else
    {
        return (int)(floor(log10(n)) + 1);

    }
}

// converts a 1 digit char to its corresponding integer eg. (char) '4' -> (int) 4
int char_to_int(char c)
{
    if (c == '0')
    {
        return 0;
    }
    else
    {
        return (int)(c - '0');
    }
}

int get_first_two_digits(long n)
{
    // initialise string to store number with correct size
    int length = count_digits(n);
    char str[length * sizeof(char)];

    // print number into string
    sprintf(str, "%li", n);

    return char_to_int(str[0]) * 10 + char_to_int(str[1]);
}

int get_first_digit(long n)
{
    // initialise string to store number with correct size
    int length = count_digits(n);
    char str[length * sizeof(char)];

    // print number into string
    sprintf(str, "%li", n);

    return char_to_int(str[0]);
}
