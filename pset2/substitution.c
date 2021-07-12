#include <ctype.h>
#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>


int main(int argc, string argv[])
{
    // check if inputs =2
    if (argc != 2)
    {
        printf("Usage %s KEY \n", argv[0]);
        return 1;
    }
    // length of input
    int n = strlen(argv[1]);
    // check if length = 26
    if (n != 26)
    {
        printf("Key must contain 26 characters");
        return 1;
    }

    //array copy to check alphabetic
    char duplicate_array[n];
    for (int j = 0; j < n; j++)
    {
        //alphabetc check
        if (isalpha(argv[1][j]) == 0)
        {
            printf("Key must only contain alphabetic characters");
            return 1;
        }
        // copy array store only lower case
        duplicate_array[j] = tolower(argv[1][j]);

    }

    //check for repeated characters including upper and lower case
    for (int j = 0; j < n; j++)
    {
        for (int i = j+1; i < n; i++)
        {
            if (duplicate_array[j] == tolower(argv[1][i]))
            {
                printf("Key must not contain repeated characters");
                return 1;
            }
        }
    }

    //get string
    string cip_text = get_string("plaintext: ");
    //and initialise variables
    int text_n = strlen(cip_text);
    char decip_text[text_n];
    int char_check;
    int to_minus;

    for (int j = 0; j < text_n; j++)
    {
        //check if its space, punctuation or numeric
        if (cip_text[j] == 32 || ispunct(cip_text[j]) != 0 || isalpha(cip_text[j]) == 0)
        {
            decip_text[j] = cip_text[j];
        }
        else
        {
            // to_minus variable is the variable that stores the index
            for (int i = 0; i < n; i++)
            {
                //if upper case and not punctuation
                if (isupper(cip_text[j]) != 0 && ispunct(cip_text[j]) ==0)
                {
                    to_minus = 65;
                }
                //if lower case and not punctuation
                else if (islower(cip_text[j]) != 0 && ispunct(cip_text[j]) == 0)
                {
                    to_minus = 97;
                }

                char_check = cip_text[j] - to_minus;
                if (char_check == i)
                {
                    // 65 means is upper
                    if (to_minus ==65)
                    {
                        decip_text[j] = toupper(argv[1][i]);
                    }
                    // else, lower case
                    else
                    {
                        decip_text[j] = tolower(argv[1][i]);
                    }
                }
            }
        }
    }
    decip_text[text_n] = '\0';
    printf("ciphertext: %s\n", decip_text);

}