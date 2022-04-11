#import <stdio.h>
#import <cs50.h>
#import <string.h>
#import <math.h>

bool isAlphabetic(char c);
int countLetters(string s);
int countWords(string s);
int countSentences(string s);

int main(void)
{
    //get input from user
    string text = get_string("Text: ");

    int letters = countLetters(text);
    int words = countWords(text);
    int sentences  = countSentences(text);

    // calculate grade index
    float index = 100 * (0.0588 * (letters / (float)words) - 0.296 * (sentences / (float)words)) - 15.8;

    // print grade
    if (index < 1)
    {
        printf("Before Grade 1\n");
    }
    else if (index > 16)
    {
        printf("Grade 16+\n");
    }
    else
    {
        printf("Grade %i\n", (int)round(index));
    }

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

// counts how many alphabetic characters appear in string
int countLetters(string s)
{
    int len = strlen(s);
    int count = 0;
    for (int i = 0; i < len; i++)
    {
        if (isAlphabetic(s[i]))
        {
            count++;
        }
    }
    return count;
}

// counts words by checking if space appears after a character
int countWords(string s)
{
    int count  = 0;
    int len = strlen(s);
    char prevChar = '\0';

    // loop through all characters
    for (int i = 0; i < len; i++)
    {
        // if previous character is alphabetic and current is one of the characters below, add 1 word
        bool iA = isAlphabetic(prevChar);

        if ((iA && s[i] == ' ') || (iA && s[i] == '.') || (iA && s[i] == ';')
            || (iA && s[i] == '!') || (iA && s[i] == '?') || (iA && s[i] == ')')
            || (iA && s[i] == ',') || (iA && s[i] == ':') || (iA && s[i] == '"'))
        {
            count ++;
        }
        // set current character to previous character
        prevChar = s[i];
    }
    return count;
}


// assumes No. of sentences = No. of periods, question marks and exclamation marks
int countSentences(string s)
{
    int len = strlen(s);
    int count = 0;
    for (int i = 0; i < len; i++)
    {
        if (s[i] == '.' || s[i] == '?' || s[i] == '!')
        {
            count++;
        }
    }
    return count;
}