#include <ctype.h>
#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

int main(int argc, string argv[])
{
    // check the number of arguments exit if number not meet
    if (argc !=2)
    {
        printf("Usage: %s key \n", argv[0]);
        return 1;
    }

    int n = strlen(argv[1]);

    // loop through input; if input contains alphabet, exit
    for (int j = 0; j < n; j++)
    {
        if (atoi(argv[1]) == 0 || isdigit(argv[1][j]) == 0)
        {
            printf("Usage: %s key \n", argv[0]);
            return 1;
        }
    }

    // get user input text
    string cip_text = get_string("plaintext: ");
    // convert string to integer
    int index = atoi(argv[1]);
    // len of inputtext
    int n_text = strlen(cip_text);
    //initialise array for decipher text
    char deciph_text[n_text];


    // function to compute
    int multipler = index / 26;
    int sub = index - (26 * multipler);

    for (int j = 0; j < n_text; j++)
    {
        int new_char = cip_text[j] + sub;
        // if upper case and not punctuaction
        if (isupper(cip_text[j]) != 0 && ispunct(cip_text[j]) == 0)
        {
            // if after moving char is more than upper bound
            if (new_char > 90)
            {
                // subtract instead
                new_char = cip_text[j] - 26 + sub;
            }
        }
        //if lower case and not puncutuation
        else if (islower(cip_text[j]) != 0 && ispunct(cip_text[j]) == 0)
        {
            //if after moving is more than bound
            if (new_char > 122)
            {
                new_char = cip_text[j] - 26 + sub;
            }
        }
        // if space and punctuation, remains the same
        else if (cip_text[j] == 32 || ispunct(cip_text[j])!= 0)
        {
            new_char = cip_text[j];
        }
        deciph_text[j] = new_char;
    }
    // assigned last
    deciph_text[n_text] = '\0';
    printf("ciphertext: %s\n", deciph_text);
}