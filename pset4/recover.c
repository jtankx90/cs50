#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

int main(int argc, char *argv[])
{
    // check command-line arguments
    if (argc!=2)
    {
        printf("UsageL ./recover card.raw\n");
        return 1;
    }
    // open files; check if able to open
    FILE *file = fopen(argv[1], "r");
    if (file == NULL)
    {
        printf("Could not open file. \n");
        return 1;
    }

    char filename[8];
    const int SIZE = 512;
    uint8_t buffer[SIZE];
    int count = 0;
    sprintf(filename,"%03i.jpg", count);
    FILE *img = fopen(filename,"w");

    fseek(file, 0L,SEEK_END);
    size_t t = ftell(file);
    t = t / 512;
    rewind(file);
    for (int i = 0; i < t; i++)
    {
        fread(buffer, SIZE, 1, file);
        if (buffer[0]== 0xff &&  buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
        {
            if (count == 0)
            {
                //commentsss
                fseek(file,ftell(file),SEEK_SET);
              //  printf("HERE %ld \n",ftell(file));
                fwrite(buffer, SIZE, 1,img);
              //  printf("%s\n", filename);
                count +=1;
            }
            else
            {
                fclose(img);
                sprintf(filename,"%03i.jpg", count);
             //   printf("%s\n", filename);
                img = fopen(filename, "w");
                fwrite(buffer,SIZE,1,img);
                count += 1;
            }
        }
        else
        {
            if (count !=0)
            {
                fwrite(buffer, SIZE, 1,img);
            }
        }
    }
    fclose(file);
}