#include <stdio.h>
#include <cs50.h>

int getValidInt(void);
void repeat(int number, string character);

int main(void)
{
    int height = getValidInt();


    for (int i = 1; i < height + 1; i++)
    {
        int spaces = height - i;
        int hashes = height - spaces;
        repeat(spaces, " ");
        repeat(hashes, "#");
        repeat(2, " ");
        repeat(hashes, "#");
        printf("\n");
    }
}

// Prompte user for valid integer
int getValidInt(void)
{
    int i;
    do
    {
        i = get_int("Height (integer between 1 and 8):\n");
    }
    while (i < 1 || i > 8);
    return i;
}

// Print a string a specified number of times
void repeat(int number, string character)
{
    for (int i = 0; i < number; i++)
    {
        printf("%s", character);
    }
}