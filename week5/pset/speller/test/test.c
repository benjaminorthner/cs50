# include <stdio.h>
# include <string.h>

int main(void)
{
    char *word = "He\n";

    word[strlen(word) - 1] = 'a';
    char c;

    printf("%s\n", (c = *word++) ? "true" : "false");
    printf("%c\n", c);

    printf("%s\n", (c = *word++) ? "true" : "false");
    printf("%c\n", c);

    printf("%s\n", (c = *word++) ? "true" : "false");
    printf("%c\n", c);

    printf("%s\n", (c = *word++) ? "true" : "false");
    printf("%c\n", c);

    return 0;
}

