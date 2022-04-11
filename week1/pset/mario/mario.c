#include <cs50.h>
#include <stdio.h>

// prints the character c, n times
void print_n_char(char c, int n)
{
    for (int i = 0; i < n; i++)
    {
        printf("%c", c);
    }
}

int main(void)
{
    // get height
    int n = 0;
    while ((n <= 0) || (n > 8))
    {
        n = get_int("Height: ");
    }


    // loop through each layer in the pyramid
    for (int i = 1; i <= n; i++)
    {
        int n_spaces = n - i;
        int n_hashes = i;

        //print first half of pyramid layer
        print_n_char(' ', n_spaces);
        print_n_char('#', n_hashes);

        //print gap
        print_n_char(' ', 2);

        //print second half
        print_n_char('#', n_hashes);
        printf("\n");
    }
}

