#include "helpers.h"
#include <math.h>
#include <stdio.h>


// Convert image to grayscale
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
                    if (i +row_add >= 0 && i + row_add < height)
                    {
                        if (j + col_add >=0 && j + col_add < width)
                        {
                            sumRed += image2[i + row_add][j + col_add].rgbtRed;
                            sumGreen += image2[i + row_add][j + col_add].rgbtGreen;
                            sumBlue += image2[i + row_add][j + col_add].rgbtBlue;
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

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    //initialise gx gy matrix
    int Gx[3][3] = {{-1,0,1},{-2,0,2},{-1,0,1}};
    int Gy[3][3] = {{-1,-2,-1},{0,0,0},{1,2,1}};
    //initalise new image
    RGBTRIPLE image2[height][width];

    //loop for new image
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
                //initalise parameters for image
            int sum_Gx_red = 0;
            int sum_Gy_red = 0;
            int sum_Gx_blue = 0;
            int sum_Gy_blue = 0;
            int sum_Gx_green = 0;
            int sum_Gy_green = 0;
            int gx_row_add = 0;
            for (int row_add = -1; row_add < 2; row_add++)
            {
                int gx_col_add = 0;
                for (int col_add = -1; col_add < 2; col_add++)
                {
                    //check that loopindex is within defined height and width
                    if (i + row_add >= 0 && i + row_add < height)
                    {
                        if (j + col_add >= 0 && j + col_add < width)
                        {
                            //sum of convol matrix * image color
                            sum_Gx_red += Gx[gx_row_add][gx_col_add] * image2[i + row_add][j + col_add].rgbtRed;
                            sum_Gy_red += Gy[gx_row_add][gx_col_add] * image2[i + row_add][j + col_add].rgbtRed;
                            sum_Gx_blue += Gx[gx_row_add][gx_col_add] * image2[i + row_add][j + col_add].rgbtBlue;
                            sum_Gy_blue += Gy[gx_row_add][gx_col_add] * image2[i + row_add][j + col_add].rgbtBlue;
                            sum_Gx_green += Gx[gx_row_add][gx_col_add] * image2[i + row_add][j + col_add].rgbtGreen;
                            sum_Gy_green += Gy[gx_row_add][gx_col_add] * image2[i + row_add][j + col_add].rgbtGreen;
                        }
                    }
                    gx_col_add += 1;
                }
                gx_row_add += 1;
            }
            //initialise parameters, calculate sqrt of gx2 + gy2
            float value_red = 0.0;
            float value_green = 0.0;
            float value_blue = 0.0;
            value_red = sqrt((sum_Gx_red*sum_Gx_red) + (sum_Gy_red*sum_Gy_red));
            value_green = sqrt((sum_Gx_green*sum_Gx_green) + (sum_Gy_green*sum_Gy_green));
            value_blue =  sqrt((sum_Gx_blue*sum_Gx_blue) + (sum_Gy_blue*sum_Gy_blue));
            int red = round(value_red);
            int green  =round(value_green);
            int blue = round(value_blue);
                //assign values
            if (red > 255)
            {
                image[i][j].rgbtRed = 255;
            }
            else
            {
                image[i][j].rgbtRed = red;
            }
            if (green > 255)
            {
                image[i][j].rgbtGreen = 255;
            }
            else
            {
                image[i][j].rgbtGreen = green;
            }
            if (blue > 255)
            {
                image[i][j].rgbtBlue = 255;
            }
            else
            {
                image[i][j].rgbtBlue = blue;
            }

        }
    }
    return;
}
