#include <stdio.h>
#include <cs50.h>

int main(void)
{
    // Prompt user for their name
    string name = get_string("What is your name?\n");
    // Print formatted string
    printf("hello, %s\n", name);
}