#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max voters and candidates
#define MAX_VOTERS 100
#define MAX_CANDIDATES 9

// preferences[i][j] is jth preference for voter i
int preferences[MAX_VOTERS][MAX_CANDIDATES];

// Candidates have name, vote count, eliminated status
typedef struct
{
    string name;
    int votes;
    bool eliminated;
}
candidate;

// Array of candidates
candidate candidates[MAX_CANDIDATES];

// Numbers of voters and candidates
int voter_count;
int candidate_count;

// Function prototypes
bool vote(int voter, int rank, string name);
void tabulate(void);
bool print_winner(void);
int find_min(void);
bool is_tie(int min);
void eliminate(int min);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: runoff [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX_CANDIDATES)
    {
        printf("Maximum number of candidates is %i\n", MAX_CANDIDATES);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i].name = argv[i + 1];
        candidates[i].votes = 0;
        candidates[i].eliminated = false;
    }

    voter_count = get_int("Number of voters: ");
    if (voter_count > MAX_VOTERS)
    {
        printf("Maximum number of voters is %i\n", MAX_VOTERS);
        return 3;
    }

    // Keep querying for votes
    for (int i = 0; i < voter_count; i++)
    {

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            // Record vote, unless it's invalid
            if (!vote(i, j, name))
            {
                printf("Invalid vote.\n");
                return 4;
            }
        }

        printf("\n");
    }

    // Keep holding runoffs until winner exists
    while (true)
    {
        // Calculate votes given remaining candidates
        tabulate();

        // Check if election has been won
        bool won = print_winner();
        if (won)
        {
            break;
        }

        // Eliminate last-place candidates
        int min = find_min();
        bool tie = is_tie(min);

        // If tie, everyone wins
        if (tie)
        {
            for (int i = 0; i < candidate_count; i++)
            {
                if (!candidates[i].eliminated)
                {
                    printf("%s\n", candidates[i].name);
                }
            }
            break;
        }

        // Eliminate anyone with minimum number of votes
        eliminate(min);

        // Reset vote counts back to zero
        for (int i = 0; i < candidate_count; i++)
        {
            candidates[i].votes = 0;
        }
    }
    return 0;
}

// Record preference if vote is valid
bool vote(int voter, int rank, string name)
{
    // TODO loop through candidatecount
    for (int i = 0; i < candidate_count; i++)
    {
        if (strcmp(name, candidates[i].name) == 0)
        {
            preferences[voter][rank] = i;
            return true;
        }
    }
    return false;
}

// Tabulate votes for non-eliminated candidates
void tabulate(void)
{
    int choice_num = 0;
    // TODO loop through voter count
    for (int i = 0; i < voter_count; i++)
    {
        while (true)
        {
            //instantiate variable for choice of candidate
            int candid_num = preferences[i][choice_num];
            if (!candidates[candid_num].eliminated)
            {
                // if not eliminated, vote +1
                candidates[candid_num].votes++;
                choice_num = 0;
                break;
            }
            else
            {
                // else if eliminated, move to next choice
                choice_num++;
            }
        }
    }
    return;
}

// Print the winner of the election, if there is one
bool print_winner(void)
{
    //number of votes to win
    float to_win = voter_count / 2;
    for (int j = 0; j < candidate_count; j++)
    {
        if (!candidates[j].eliminated)
        {
        //if not eliminated, if votes > require votes, print winner
            if (candidates[j].votes > to_win)
            {
                printf("%s\n", candidates[j].name);
                return true;
            }
        }
    // TODO
    }
    return false;
}

// Return the minimum number of votes any remaining candidate has
int find_min(void)
{
    //initailise new candidates struct to store not eliminated
    candidate x2[candidate_count];
    int candidate_left = 0;
    for (int j = 0; j < candidate_count; j++)
    {
        //if not eliminated, store the candidate in new struct
        if (!candidates[j].eliminated)
        {
            x2[j].name = candidates[j].name;
            x2[j].votes = candidates[j].votes;
            x2[j].eliminated = candidates[j].eliminated;
            //store variable for # candiate left
            candidate_left++;
        }
    }
    int min_vote;
    int to_checkvote;

    for (int j = 0; j < candidate_left - 1; j++)
    {
        //first loop, min vote = first candidate
        if (j == 0)
        {
            min_vote = x2[j].votes;
        }
        else
        {
            //next loop onwards, compare candidate j, j+1 votes
            if (x2[j].votes < x2[j+1].votes)
            {
                //store the min vote between those 2
                to_checkvote = x2[j].votes;
            }
            else
            {
                to_checkvote = x2[j+1].votes;
            }
            //check the min vote of compared against global min vote
            if (to_checkvote < min_vote)
            {
                min_vote = to_checkvote;
            }
        }
    }
    return min_vote;
}

// Return true if the election is tied between all candidates, false otherwise
bool is_tie(int min)
{
    //initailise new candidates struct to store not eliminated
    candidate x3[candidate_count];
    int candidate_left = 0;
    for (int j = 0; j < candidate_count; j++)
    {
        if (!candidates[j].eliminated)
        {
            //similar to find min vote function
            x3[j].name = candidates[j].name;
            x3[j].votes = candidates[j].votes;
            x3[j].eliminated = candidates[j].eliminated;
            candidate_left++;
        }
    }
    int check_counter = 0;
    for (int j = 0; j < candidate_left; j++)
    {
        if (x3[j].votes == min)
        {
            //count the number of candidates that are not eliminated and if it = to min vote
            check_counter++;
        }
    }
    // if equal, game is tied
    if (check_counter == candidate_left)
    {
        return true;
    }
    return false;
}

// Eliminate the candidate (or candidates) in last place
void eliminate(int min)
{
    // TODO
    for (int j =0; j < candidate_count; j++)
    {
        if (!candidates[j].eliminated)
        {
            //if not eliminated and if votes = min vote then eliminated
            if (candidates[j].votes == min)
            {
                candidates[j].eliminated = true;
            }
        }

    }
    return;
}