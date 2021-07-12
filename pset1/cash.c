#include <stdio.h>
#include <cs50.h>
#include <math.h>

int main(void)
{
    float float_no;
    do
    {
        float_no = get_float("Change owed: "); //get user input, check if input is >=0
    }
    while(float_no < 0);


    int no_coins=0;
    do
    {
        if (float_no - 0.25 >=0)
        {
            no_coins = no_coins+1;
            float_no = float_no - 0.25;
            //printf("0.25 %f\n",float_no);
            //printf("%i",no_coins);
        }

        else if(round(float_no*100-10)>=0){
            no_coins++;
            float_no = float_no-0.1;
          //  printf("0.1 %f\n",float_no);
        }

        else if(round(float_no*100-5)>=0){
            no_coins++;
            float_no=float_no-0.05;
          //  printf("0.05 %f\n",float_no);
        }

        else if(round(float_no*100 - 1)>=0){
            no_coins++;
            float_no=float_no-0.01;
           // printf("0.01 %f\n",float_no);
        }
        else{
            break;
        }


    }

    while(float_no>0);

     printf("%i\n", no_coins);

}