#include <stdio.h>
#include <cs50.h>

int main(void)
{
    // initialise height variable and get input with while loop
    int height;
    do
    {
        height = get_int("Height: ");
    }
    while (height <= 0 || height >= 9);

    //initialise loop counter
    int i = 0;
    // counter variable for decreasing whitespace for each row
    int counter = height;

    //do loop for loop till required height
    do
    {
        if (i>0)
        {
            printf("%*s", counter, "");

            //for loop for same row printing
            for (int j = 1; j <= i; j++)
            {
                printf("%s", "#");
            }
            printf("  ");
            for (int j = 1; j <= i; j++)
            {
                printf("%s", "#");
            }
            printf("\n");

        }
        i++;
        counter--;
    }
    while (i <= height);

}