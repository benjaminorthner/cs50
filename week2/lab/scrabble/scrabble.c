#include <ctype.h>
#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Points assigned to each letter of the alphabet
int POINTS[] = {1, 3, 3, 2, 1, 4, 2, 4, 1, 8, 5, 1, 3, 1, 1, 3, 10, 1, 1, 1, 1, 4, 4, 8, 4, 10};

int compute_score(string word);
bool isAlphabetic(char c);
int alphabetPos(char c);

int main(void)
{
    // Get input words from both players
    string word1 = get_string("Player 1: ");
    string word2 = get_string("Player 2: ");

    // Score both words
    int score1 = compute_score(word1);
    int score2 = compute_score(word2);

    // TODO: Print the winner

    if (score1 > score2)
    {
        printf("Player 1 wins!\n");
    }
    else if (score1 < score2)
    {
        printf("Player 2 wins!\n");
    }
    else
    {
        printf("Tie!\n");
    }
}

int compute_score(string word)
{
    int len = strlen(word);
    int score = 0;

    for (int i = 0; i < len; i++)
    {
        // only increase score if alphabetic
        if (isAlphabetic(word[i]))
        {
            score += POINTS[alphabetPos(word[i])];
        }
    }
    return score;
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