#include <stdio.h>
#include <cs50.h>

int main()
{
    long cc_number = get_long("Number: ");
    int digit1 = 0, digit2 = 0, num_digits = 0, sum = 0;

    while (cc_number > 0)
    {
        //Set to previous right-most digit (will be second to left upon exiting while loop)
        digit2 = digit1;
        // Get right-most digit
        digit1 = cc_number % 10;

        // Even digit, add as-is
        if (num_digits % 2 == 0)
        {
            sum += digit1;
        }
        // Odd digit
        else
        {
            // Multiply by two
            int multiple = 2 * digit1;
            // Add the sum of each resulting digit
            sum += (multiple / 10) + (multiple % 10);
        }

        // Decimate number
        cc_number /= 10;
        // Increment odd/even counter
        num_digits++;
    }

    // Valid CheckSum?
    bool is_valid = sum % 10 == 0;
    int first_two_digits = (digit1 * 10) + digit2;

    if (digit1 == 4 && (num_digits == 13 || num_digits == 16) && is_valid)
    {
        printf("VISA\n");
    }
    else if (first_two_digits >= 51 && first_two_digits <= 55 && num_digits == 16 && is_valid)
    {
        printf("MASTERCARD\n");
    }
    else if ((first_two_digits == 34 || first_two_digits == 37) && num_digits == 15 && is_valid)
    {
        printf("AMEX\n");
    }
    else
    {
        printf("INVALID\n");
    }
}