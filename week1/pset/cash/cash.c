#import <stdio.h>
#import <cs50.h>
#import <math.h>

int count_coins(float total);

int main(void)
{
    float change_dollars = 0;

    // ask user for change amount until they give positive float value
    do
    {
        change_dollars = get_float("Changed owed: ");
    }
    while (change_dollars < 0);

    // convert from dollars to cents
    int change_cents = round(change_dollars * 100);

    printf("%i\n", count_coins(change_cents));
    return 0;
}


// counts the total number of coins needed to make up the total
int count_coins(float total)
{
    // end condition that breaks out of recursion
    if (total == 0)
    {
        return 0;
    }

    int available_coin_count = 4;
    int available_coins[4] = {25, 10, 5, 1};

    // init the counter for how many coins we need to make up total
    int coins_needed = 0;
    for (int i = 0; i < available_coin_count; i++)
    {
        // if something is left after removing the i'th coin from the total
        if (total - available_coins[i] >= 0)
        {
            // add the coin to our coins needed counter
            coins_needed++;

            // add the coins needed for the remaining amount to the counter
            coins_needed += count_coins(total - available_coins[i]);

            // return the total coin count
            return coins_needed;
        }
    }
    // incase of error
    return -1;
}