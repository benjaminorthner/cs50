#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max number of candidates
#define MAX 9

// preferences[i][j] is number of voters who prefer i over j
int preferences[MAX][MAX];

// locked[i][j] means i is locked in over j
bool locked[MAX][MAX];

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
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void lock_pairs(void);
void print_winner(void);
int strength(pair p);

bool cycle(bool paths[][]);
void push(char element, int stack[], int *top, int stackSize);
void pop(int stack[], int *top, int stackSize)

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

        printf("\n");
    }

    add_pairs();
    sort_pairs();
    lock_pairs();
    print_winner();
    return 0;
}

// Update ranks given a new vote
bool vote(int rank, string name, int ranks[])
{
    // loop through all candidates
    for (int i = 0; i < candidate_count; i++)
    {
        if (strcmp(name, candidates[i]) == 0)
        {
            ranks[i] = rank;
            return true;
        }
    }

    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    // loop over all candidate pairs and find who wins in 1-1 match ups
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            // if i wins
            if (ranks[i] < ranks[j])
            {
                preferences[i][j]++;
            }
        }
    }
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    // loop over all candidate pairs
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            // if more people prefer i over j than people who prefer j over i
            if (preferences[i][j] > preferences[j][i])
            {
                pairs[pair_count].winner = i;
                pairs[pair_count].loser = j;
                pair_count++;
            }
        }
    }
}

// Sort pairs in decreasing order by strength of victory
// Using selection sort alg
void sort_pairs(void)
{
    // start point of comparison moves to the right
    for (int start = 0; start < pair_count; start++)
    {
        int maxStrength = 0;
        int maxStrengthIdx = start;
        // loop over all pairs from start to end
        for (int i = start; i < pair_count; i++)
        {
            // if strength of current pair higher than max, make it new max
            if (strength(pairs[i]) > maxStrength)
            {
                maxStrength = strength(pairs[i]);
                maxStrengthIdx = i;
            }
        }
        // swap element in start pos with element in maxStrengthIdx pos
        pair temp = pairs[start];
        pairs[start] = pairs[maxStrengthIdx];
        pairs[maxStrengthIdx] = temp;
    }
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    // loop through all pairs
    for (int i = 0; i < pair_count; i++)
    {
        // lock pair
        locked[pair[i].winner][pairs[i].loser] = true;

        // if cycle exists then unlock pair (hence skipping it)
        if (cycle(locked))
        {
            locked[pair[i].winner][pairs[i].loser] = false;
        }
    }
}

// Print the winner of the election
void print_winner(void)
{
    // TODO
    return;
}

// returns the strength of victory of the winning candidate in the pair
int strength(pair p)
{
    return preferences[p.winner][p.loser] - preferences[p.loser][p.winner];
}

// detect cycles in directed graphs (given by adjacency list) via Depth First Search
// https://www.baeldung.com/cs/detecting-cycles-in-directed-graph
bool cycle(int start_candidate, int stack[], int stack_size, bool pairs[MAX][MAX])
{
    // get neighbours (max = no of candidates - 1)
    int neighbours[candidate_count - 1];
    int neighbour_count = 0;
    
    // push current cadidate to stack
    
    // find neighbours by looking through pairs where v_start is the winner
    for (int i = 0; i < pair_count; i++)
    {
        if (pairs.winner == v_start)
        {
            // check if neighbour has been visited already
            if (pairs.loser in stack)
            {
                return true // true = cycle exists
            }
            neightbours[pair_count] = pairs.loser;
            neighbour_count++;
        }
        
        // if no neighbours found then return false (no cycle found in this branch)
        if (neighbour_count == 0)
        {
            // pop current node from stack
            return false;
        }
    }
    
    // loop through neighbours
    for (int i = 0; i < neighbour_count; i++)
    {
        return cycle(neighbours[i], stack, stack_size, pairs)
    }
}



// ------------ IMPLEMENTATION OF A STACK USED FOR DFS CYCLE FUNCTION ------------

void push(char element, int stack[], int *top, int stackSize)
{
    if(*top == -1)
    {
        stack[stackSize - 1] = element;
        *top = stackSize - 1;
    }
    else if(*top == 0)
    {
        printf("The stack is already full. \n");
    }
    else
    {
        stack[(*top) - 1] = element;
        (*top)--;
    }
}

void pop(int stack[], int *top, int stackSize)
{
    if(*top == -1)
    {
        printf("The stack is empty. \n");
    }
    else
    {
        printf("Element popped: %c \n", stack[(*top)]);
          // If the element popped was the last element in the stack
          // then set top to -1 to show that the stack is empty
        if((*top) == stackSize - 1)
        {
            (*top) = -1;
        }
        else
        {
            (*top)++;
        }
    }
}