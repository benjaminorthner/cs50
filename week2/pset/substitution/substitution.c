#import <stdio.h>
#import <cs50.h>
#import <string.h>

bool isAlphabetic(char c);
char toLower(char c);
char toUpper(char c);
string substitute(string key, string plaintext);
int alphabetPos(char c);

int main(int argc, char **argv)
{

    // ---------------- CHECKING KEY VALIDITY ------------------------

    // check if key is given at command line
    if (argc == 2)
    {
        string key = argv[1];

        // check if key has 26 digits
        if (key[26] != '\0')
        {
            printf("Key must contain 26 characters.\n");
            return 1;
        }

        // check if key contains only alphabetic characters
        // check if key contains every letter only once
        // do this by adding up all the ascii codes for each lower case letter, the sum should be 2847
        int letterSum = 0;

        for (int i = 0; i < 26; i++)
        {
            if (!isAlphabetic(key[i]))
            {
                printf("Key can only contain Alphabetic characters\n");
                return 1;
            }

            letterSum += (int) toLower(key[i]);
        }

        // check if key contains every letter only once
        if (letterSum != 2847)
        {
            printf("Key can only contain each character once\n");
            return 1;
        }
    }

    // warning message if no key is given
    else
    {
        printf("Usage: ./substitution key\n");
        return 1;
    }


    // ---------------- SUBSTITUTION IMPLEMENTATION ------------------------

    // safe to get key here because check already done
    string key = argv[1];

    // ask user for plaintext to be encrypted
    string plaintext = get_string("plaintext:\t");

    // print ciphertext
    printf("ciphertext:\t%s\n", substitute(key, plaintext));

    return 0;
}


// checks if a character is alphabetic
bool isAlphabetic(char c)
{
    // check if ascii value of character is in the range [A(65) , Z(90)]
    // or is in the range [a(97), z(122)]
    if ((c - 65) * (c - 90) <= 0 || (c - 97) * (c - 122) <= 0)
    {
        return true;
    }

    return false;
}

// converts alphabetic characters to lowercase
char toLower(char c)
{
    // if uppercase alphabetic character
    if ((c - 65) * (c - 90) <= 0)
    {
        return (char)(c + 32);
    }

    // otherwise do nothing
    return c;
}

// converts alphabetic characters to lowercase
char toUpper(char c)
{
    // if lowercase alphabetic character
    if ((c - 97) * (c - 122) <= 0)
    {
        return (char)(c - 32);
    }

    // otherwise do nothing
    return c;
}

string substitute(string key, string plaintext)
{
    // initialise ciphertext
    string ciphertext = plaintext;

    // generate ciphertext
    int len = strlen(plaintext);

    for (int i = 0; i < len; i++)
    {
        // check if current character in plaintext is non-alphabetic
        if (!isAlphabetic(plaintext[i]))
        {
            ciphertext[i] = plaintext[i];
        }

        // if character is alphabetic and lowercase then encrypt
        else if (toLower(plaintext[i]) == plaintext[i])
        {
            ciphertext[i] = toLower(key[alphabetPos(plaintext[i])]);
        }

        // if character is alphabetic and uppercase then encrypt
        else if (toUpper(plaintext[i]) == plaintext[i])
        {
            ciphertext[i] = toUpper(key[alphabetPos(plaintext[i])]);
        }
    }

    return ciphertext;
}

// returns the position in the alphabet of the char c (A->0, Z->25)
int alphabetPos(char c)
{
    // if uppercase
    if ((c - 65) * (c - 90) <= 0)
    {
        return (int)(c - 65);
    }

    // if lowercase
    if ((c - 97) * (c - 122) <= 0)
    {
        return (int)(c - 97);
    }

    // return -1 indicates parameter c was not alphabetical
    return -1;
}