#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max number of candidates
#define MAX 9

// preferences[i][j] is number of voters who prefer i over j
int preferences[MAX][MAX];

// locked[i][j] means i is locked in over j
bool locked[MAX][MAX];
bool locked_array[MAX][MAX];

// Each pair has a winner, loser
typedef struct
{
    int winner;
    int loser;
}
pair;


// Array of candidates
string candidates[MAX];
pair pairs[MAX * (MAX - 1) / 2];

int pair_count;
int candidate_count;

// Function prototypes
bool vote(int rank, string name, int ranks[]);
bool check_recursively(int winner, int loser, int count);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void lock_pairs(void);
void print_winner(void);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: tideman [candidate ...]\n");
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
        candidates[i] = argv[i + 1];
    }

    // Clear graph of locked in pairs
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            locked[i][j] = false;
        }
    }

    pair_count = 0;
    int voter_count = get_int("Number of voters: ");

    // Query for votes
    for (int i = 0; i < voter_count; i++)
    {
        // ranks[i] is voter's ith preference
        int ranks[candidate_count];

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            if (!vote(j, name, ranks))
            {
                printf("Invalid vote.\n");
                return 3;
            }
        }

        record_preferences(ranks);
    }

    printf("\n");
    add_pairs();
    sort_pairs();
    lock_pairs();
    print_winner();
    return 0;
}

// Update ranks given a new vote
bool vote(int rank, string name, int ranks[])
{
    for (int j = 0; j < candidate_count; j++)
    {
        // compare name
        if (strcmp(name, candidates[j]) == 0)
        {
            ranks[rank] = j;
            return true;
        }
    }
    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    int value;
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            if (j == i)
            {
                value = preferences[i][j];
                preferences[i][j] = value;
            }
            else
            {
                if (i == ranks[0])
                {
                    preferences[i][j]++;
                }
                else if (i != ranks[candidate_count - 1])
                {
                    for (int n = 0; n < candidate_count - 1; n++)
                    {
                        int first_can = ranks[n];
                        if (first_can == i)
                        {
                            for (int k = n + 1; k < candidate_count; k++)
                            {
                                int x_can = ranks[k];
                                if (x_can == j)
                                {
                                    preferences[i][j] ++;
                                    break;
                                }
                                else if (j == candidate_count - 1)
                                {
                                    preferences[i][j]= 0;
                                }
                            }
                        }
                        else if (ranks[n] == j)
                        {
                            value  = preferences[i][j];
                            preferences[i][j] = value;
                            break;

                        }
                    }
                }
                else
                {
                    value = preferences[i][j];
                    preferences[i][j] = value;
                }
            }
        }
    }
    return;
}



// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    // to do
    int num_par = 0;
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = i+  1; j < candidate_count; j++)
        {
            // if preferred
            if (preferences[i][j] != 0 || preferences[j][i] != 0)
            {
                // since array is symmetric, if i wins j then j loses to i
                if (preferences[i][j] < preferences[j][i])
                {
                    pairs[num_par].winner = j;
                    pairs[num_par].loser = i;
                    num_par++;
                }
                else if (preferences[i][j] > preferences[j][i])
                {
                    pairs[num_par].winner = i;
                    pairs[num_par].loser = j;
                    num_par++;
                }
            }
        }
    }

    pair_count = num_par;
    return;
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    int new_array[pair_count];
    int new_array2[pair_count];
    for (int j = 0; j < pair_count; j++)
    {
        int winner = pairs[j].winner;
        int loser = pairs[j].loser;
//      printf("Winner: %i; Loser: %i\n",preferences[winner][loser],preferences[loser][winner]);
        int temp_strength = preferences[winner][loser] - preferences[loser][winner];
        new_array[j] = temp_strength;
        new_array2[j] = temp_strength;
    }
    int final_array[pair_count];
    for (int j = 0; j < pair_count-1; j++)
    {
        for (int i = j + 1; i < pair_count; i++)
        {
            if (new_array[j] < new_array[i])
            {
                int j_fig = new_array[j];
                int i_fig = new_array[i];
                new_array[j] = i_fig;
                new_array[i] = j_fig;

            }

        }
    }
    pair sorted_pairs[pair_count];
    for (int j = 0; j < pair_count; j++)
    {
        for (int i=0; i < pair_count; i++)
        {
            if (new_array[j] == new_array2[i])
            {
                new_array2[i] = MAX * MAX + 1;
                sorted_pairs[j] = pairs[i];
                break;
            }
        }
    }
   for (int j =0; j < pair_count; j++)
    {
        pairs[j] = sorted_pairs[j];
    }
    // TODO
    return;
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    //storing values
    for (int j = 0; j < candidate_count; j++)
    {
        for (int i = 0; i < candidate_count; i++)
        {
            locked[i][j] = false;
            locked_array[i][j] = false;
        }
    }

    for (int j = 0; j < pair_count; j++)
    {
        //variables
        bool boo;
        int lockedcount = 0;
        // winner loser
        int winner = pairs[j].winner;
        int loser = pairs[j].loser;
        //retrieve recursion results - if locking takes places
        boo = check_recursively(winner,loser,candidate_count);
        locked[winner][loser] = boo;
        //locked_array to store original array
        locked_array[winner][loser] = boo;
    }
    return;
}

//recursive function to check loser node
bool check_recursively(int winner, int loser, int count)
{
    //default as true
    bool result= true;;
    // escape criteria for recursion
    if (count == 1)
    {
        result = true;
    }
    else
    {
        bool arr[candidate_count];
        // array for loser ( i.e. records loser pair with each candidates)
        for (int n = 0; n < candidate_count; n++)
        {
            arr[n] = locked_array[loser][n];
        }

        for (int j = 0; j < candidate_count; j++)
        {
            // if loser has win candidate j before, check if candidate j is winner if candidate j is the winner of the pair (with loser) then stop locking
            if (arr[j])
            {
                if (j != winner)
                {
                    // recursion counter to check if candidate j has lose before - recursive logic
                    count--;

                    // stop locking returns false - default result = true. Multiplication to ensure if theres 1 false = result will be false regardless of # of recursions done
                    result = result * check_recursively(winner, j, count);
                }

                else
                {
                    result = false;
                }
            }
        }
    }
    return result;
}


// Print the winner of the election
void print_winner(void)
{
    int winner;
    for (int j = 0; j < candidate_count; j++)
    {
        // for each candidate, if lost a pair, can't be winner
        int win_counter = 0;
        for (int i = 0; i < candidate_count; i++)
        {
            if (j != i)
            {
                // check if lose > 0, via checking the other candidates - if the other candidates have won candidate j
                if (locked[i][j])
                {
                    //if other candidates have won, break loop
                    break;
                }
                else
                {
                    // count the number of times of win if no lost before
                   win_counter ++;
                }
            }
        }
        // the winner should have no arrow points towards the winner, hence total win = number of candidates - 1 (himself)
        if (win_counter == candidate_count - 1)
        {
            printf("%s\n", candidates[j]);
            return;
        }
    }

}

