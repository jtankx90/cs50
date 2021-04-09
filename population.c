#include <cs50.h>
#include <stdio.h>

int main(void)
{
    int startsize;
    do{
        startsize = get_int("Start size: ");


    }
    while (startsize < 9);


    int endsize;
    do{
        endsize = get_int("End Size: ");

    }
    while (endsize < startsize);





    int years = 0;

    int new;
    int remain =startsize;

    while(remain < endsize){
        new = startsize/3-startsize/4;
        remain = remain + new;
        years = years+1;
        startsize = remain;
    }




    printf("Years: %i\n",years);

}