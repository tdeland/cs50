#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <ctype.h>


int main(int argc, string argv[])
{
    // check for cipher key command line argument
    if (argc != 2)
    {
        printf("Usage: ./substitution key\n");
        return 1;
    }

    // validate cipher key
    string key = argv[1];

    // check length
    if (strlen(key) != 26)
    {
        printf("Key must contain 26 characters.\n");
        return 1;
    }

    // check key characters...
    for (int i = 0; i < 26; i++)
    {
        // entire key to uppercase
        key[i] = toupper(key[i]);
        // ... for non-letters
        if (!isalpha(key[i]))
        {
            printf("Key must only contain alphabetic characters.\n");
            return 1;
        }
        // ... for repeat characters
        for (int j = 0; j < i; j++)
        {
            if (key[j] == key[i])
            {
                printf("Key must not contain repeated characters.\n");
                return 1;
            }
        }
    }

    // prompt for plain text
    string text = get_string("plaintext:  ");

    // encipher
    for (long i = 0, n = strlen(text); i < n; i++)
    {
        if (isalpha(text[i]))
        {
            // shift character according to key while preserving case
            text[i] = text[i] - toupper(text[i]) + key[toupper(text[i]) - 'A'] ;
        }
    }

    // return enciphered text
    printf("ciphertext: %s", text);
    printf("\n");
    return 0;
}