#include <stdio.h>
#include <cs50.h>

int main(void)
{
    // gets users name
    string name = get_string("Enter name:\n");
    printf("Hello %s\n", name);
}
