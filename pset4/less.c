
#include "helpers.h"
#include <math.h>
#include <stdio.h>


void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i <  height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            //calculating average
            float average = (image[i][j].rgbtRed + image[i][j].rgbtBlue + image[i][j].rgbtGreen)/3.0;
            int avg = round(average);
            image[i][j].rgbtRed = avg;
            image[i][j].rgbtBlue = avg;
            image[i][j].rgbtGreen =avg;

        }

    }

    return;
}


// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int red;
            int green;
            int blue;
            //caclulating each red/green blue
            float sepiaRed = 0.393 * image[i][j].rgbtRed + 0.769 * image[i][j].rgbtGreen + 0.189 * image[i][j].rgbtBlue;
            if (sepiaRed > 255.0)
            {
                red = 255;
              //  image[i][j].rgbtRed = red;

            }
            else
            {
                red = round(sepiaRed);
                //image[i][j].rgbtRed = sepiaRed;
            }

            float sepiaGreen = 0.349 * image[i][j].rgbtRed + 0.686 * image[i][j].rgbtGreen + 0.168 * image[i][j].rgbtBlue;
            if (sepiaGreen > 255.0)
            {
                green = 255;
            }
            else
            {
                green = round(sepiaGreen);
            }


           float sepiaBlue = 0.272 * image[i][j].rgbtRed + 0.534 * image[i][j].rgbtGreen + 0.131 * image[i][j].rgbtBlue;
           if (sepiaBlue > 255.0)
           {
               blue = 255;
           }
           else
           {
               blue = round(sepiaBlue);
           }
            image[i][j].rgbtBlue = blue;
            image[i][j].rgbtRed = red;
            image[i][j].rgbtGreen = green;


        }

    }



    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        int midpoint = width/2;

        for (int j = 0; j < midpoint; j++)
        {
            int startred = image[i][j].rgbtRed;
            int startblue = image[i][j].rgbtBlue;
            int startgreen = image[i][j].rgbtGreen;
            image[i][j].rgbtRed = image[i][width - j - 1].rgbtRed;
            image[i][j].rgbtBlue = image[i][width - j - 1].rgbtBlue;
            image[i][j].rgbtGreen = image[i][width - j - 1].rgbtGreen;
            image[i][width - j - 1].rgbtRed = startred;
            image[i][width - j - 1].rgbtBlue = startblue;
            image[i][width - j - 1].rgbtGreen = startgreen;
        }
    }

 return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
 RGBTRIPLE image2[height][width];

    for (int i = 0; i < height; i++)
        {
            for (int j = 0; j < width; j++)
            {
                 image2[i][j].rgbtRed = image[i][j].rgbtRed;
                 image2[i][j].rgbtBlue = image[i][j].rgbtBlue;
                 image2[i][j].rgbtGreen = image[i][j].rgbtGreen;
            //     printf("%i, %i, %i \n", image2[height][width].rgbtRed, image2[height][width].rgbtBlue,image2[height][width].rgbtGreen);
            }

        }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            //initialise
            float count = 0;
            float sumRed = 0;
            float sumBlue = 0;
            float sumGreen = 0;
            //double loop
            for (int row_add = -1; row_add < 2; row_add++)
            {
                for (int col_add = -1; col_add < 2; col_add++)
                {
                    //check that loopindex is within defined height and width
                    if (i+row_add >= 0 && i+row_add < height)
                    {
                        if (j+col_add >=0 && j+col_add < width)
                        {
                            sumRed += image2[i+row_add][j+col_add].rgbtRed;
                            sumGreen += image2[i+row_add][j+col_add].rgbtGreen;
                            sumBlue += image2[i+row_add][j+col_add].rgbtBlue;
                            count += 1.0 ;
                      //      printf("sumRed: %i, sumGree: %i, sumBlue: %i, count \n", sumRed,sumGreen,sumBlue);
                        }
                    }

                }

            }
        //assign values
        int red = round(sumRed/count);
        int blue = round(sumBlue/count);
        int green = round(sumGreen/count);
        image[i][j].rgbtRed = red;
        image[i][j].rgbtBlue = blue;
        image[i][j].rgbtGreen = green;


        }


    }


    return;
}
