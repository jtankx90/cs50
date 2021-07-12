#include <ctype.h>
#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

int count_sentence(char word);
int col_liau(float L, float S);

int main(void)
{
    //get sentence
    string sentence = get_string("Text: ");

    //initialise parameters
    float char_count = 0;
    float word_count = 1;
    float sent_count = 0;

    //loop through sentence
    for (int j = 0, n = strlen(sentence); j < n; j++)
    {
        if (sentence[j] >= 65)
        {
            char_count++;

        }
        else if (sentence[j] == 32)
        {
            word_count++;
        }

        sent_count = sent_count + count_sentence(sentence[j]);
    }

    //calculating parameters for the formula
    float L = 100 / word_count * char_count;
    float S = 100 / word_count * sent_count;
    int final = col_liau(L,S);

    if (final < 1)
    {
        printf("Before Grade 1\n");
    }

    else if (final >= 16)
    {
        printf("Grade 16+\n");
    }

    else
    {
        printf("Grade %i\n", final);
    }

}



int count_sentence(char word)
{
    //count number of sentence using index
    int score = 0;
    if (word == 33 || word == 63 || word == 46)
    {
        score++;
    }
    return score;
}


int col_liau(float L, float S)
{
    //compute index score
    float score = 0;
    score = 0.0588 * L - 0.296 * S - 15.8 ;
    // + 0.5 to original score such that when cast as integer, truncated
    int score2 = score + 0.5;
    return score2;
}