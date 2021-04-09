#include <stdio.h>
#include <cs50.h>
#include <math.h>


int main(void)
{
    long cc_no;
    cc_no = get_long("Number: ");


    int length = 0;
    long n = cc_no;

    //while loop to get length of number
    do
    {
        n = n / 10;
        length++;
    }
    while (n > 0);


    // initialise parameters
    int cc_array[length];

    n = cc_no;
    int sum_numbers= 0;
    int index=1;


    for( int j = length; j > 0; j--){
        //storing digit into array, starting from further right
        cc_array[j] = n % 10;
        n = n / 10;
           if (j!=0)
           {
            if (index%2==0)
            {
                int j_multipler = cc_array[j] * 2;

                if (j_multipler >= 10)
                {
                int temp_num=j_multipler;
                int num_sum=0;

                    //for loop for x2 >10; loop 2 times to get sum
                for (int i=0;i<2;i++){
                num_sum = temp_num % 10;
                temp_num= temp_num / 10;
                sum_numbers=sum_numbers + num_sum;}}
                else
                {
                sum_numbers=sum_numbers+j_multipler;}
            }
            else
            {
            sum_numbers=sum_numbers+cc_array[j];}
           }
        index++;}



//check to see if checksum if multiple of 10
 if(sum_numbers % 10 == 0)
 {
      if(length == 15 && cc_array[1] == 3 && (cc_array[2] == 4|| cc_array[2] == 7))
      {
          printf("AMEX\n");
      }

     else if ((length == 13) || (length == 16))
      {


          if (cc_array[1] == 5 &&cc_array[2] < 6){
              printf("MASTERCARD\n");
          }
          else if (cc_array[1] == 4)
          {
            printf("VISA\n");
          }
          else
          {
            printf("INVALID\n");
          }
        }
    else{printf("INVALID\n");}

  }
  else{
    printf("INVALID\n");}
}