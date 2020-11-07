#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <math.h>


int main(void)
{
    string s = get_string("Text: ");

    long letter_count = 0, word_count = 1, sentence_count = 0;

    for (long i = 0, n = strlen(s); i < n; i++)
    {
        // a-z, A-Z, increment letter
        if ((s[i] >= 'a' && s[i] <= 'z') || (s[i] >= 'A' && s[i] <= 'Z'))
        {
             letter_count++;
        }
        // space (word)
        else if (s[i] == ' ')
        {
            word_count++;
        }
        // punctuation (sentence)
        else if (s[i] == '.' || s[i] == '?' || s[i] == '!')
        {
            sentence_count++;
        }
    }

    // calculate floating point grade index
    float L = (float) letter_count / (float) word_count  * 100;
    float S = (float) sentence_count / (float) word_count  * 100;
    float idx = 0.0588 * L - 0.296 * S - 15.8;

    // print out formatted result
    if (idx >= 16)
    {
        printf("Grade 16+\n");
    }
    else if (idx < 1)
    {
        printf("Before Grade 1\n");
    }
    else
    {
        printf("Grade %i\n", (int) round(idx));
    }
}