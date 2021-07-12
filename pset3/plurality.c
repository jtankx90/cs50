#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max number of candidates
#define MAX 9

// Candidates have name and vote count
typedef struct
{
    string name;
    int votes;
}
candidate;

// Array of candidates
candidate candidates[MAX];

// Number of candidates
int candidate_count;

// Function prototypes
bool vote(string name);
void print_winner(void);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: plurality [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i].name = argv[i + 1];
        candidates[i].votes = 0;
    }

    int voter_count = get_int("Number of voters: ");

    // Loop over all voters
    for (int i = 0; i < voter_count; i++)
    {
        string name = get_string("Vote: ");

        // Check for invalid vote
        if (!vote(name))
        {
            printf("Invalid vote.\n");
        }


    }

    // Display winner of election
    print_winner();
}

// Update vote totals given a new vote
bool vote(string name)
{
    //loop with number of candidates as index
    for (int i = 0; i< candidate_count; i++)
    {
        //check if name are equal
        if(strcmp(name, candidates[i].name) ==0)
            {
            // if equal votes +1; exit function
            candidates[i].votes++;
            return true;
            }
    }
    return false;
}

// Print the winner (or winners) of the election
void print_winner(void)
{
    //initialise parameters
    string winner[candidate_count];
    int votes = 0;
    int index = 0;
    // TODO
    for (int i = 0; i < candidate_count; i++)
    {
        //check if candidates votes is >= 0 for first loop
        if (candidates[i].votes >= votes)
        {
            //first loop always
            winner[index] = candidates[i].name;
            votes = candidates[i].votes;
            index++;
        }
    }
    for (int i = 0; i < index; i++)
    {
        printf("%s\n", winner[i]);
    }

    return;

}