#include <cs50.h>
#include <stdio.h>

int main(void)
{
    // TODO: Prompt for start size
    int pop_start = 1;
    while (pop_start < 9)
    {
        pop_start = get_int("Population start size (>= 9): ");
    }
    // TODO: Prompt for end size
    int pop_end = 1;
    while (pop_end < pop_start)
    {
        pop_end = get_int("Population end size: ");
    }

    // TODO: Calculate number of years until we reach threshold
    int years = 0;
    int n = pop_start;
    while (n < pop_end)
    {
        n += n / 3 - n / 4;
        years++;
    }

    // TODO: Print number of years
    printf("Years: %i\n", years);
}
