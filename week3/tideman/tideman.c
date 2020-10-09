#include <cs50.h>
#include <stdio.h>
#include <string.h>

/*
Studied the following sources for this:

Directed Graphs and DFS / Cycle: (used for lock_pairs)
https://medium.com/@trykv/algorithms-on-graphs-lets-talk-depth-first-search-dfs-and-breadth-first-search-bfs-5250c31d831a
https://medium.com/@trykv/algorithms-on-graphs-directed-graphs-and-cycle-detection-3982dfbd11f5

Finding the source: (used for print_winner)
https://cs.stackexchange.com/questions/9133/finding-a-source-of-a-directed-acyclic-graph-in-linear-time
*/

// Max number of candidates
#define MAX 9

// preferences[i][j] is number of voters who prefer i over j
int preferences[MAX][MAX];

// locked[i][j] means i is locked in over j
bool locked[MAX][MAX];

// Global variable for cycle / dfs
bool visited[MAX][MAX];
bool explore[MAX][MAX];

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

    add_pairs();
    sort_pairs();
    lock_pairs();
    print_winner();

    return 0;
}

// Update ranks given a new vote
bool vote(int rank, string name, int ranks[])
{
    for(int i = 0; i < candidate_count; i++) {
        if(strcmp(name, candidates[i]) == 0) {
            ranks[rank] = i;
            return true;
        }
    }
    return false;
}

// Returns true if c1 is before c2 on ranks;
bool isbefore(int c1, int c2, int ranks[]) {

    int indexc1 = 0;
    int indexc2 = 0;

    for(int i = 0; i < candidate_count; i++) {
        if(ranks[i] == c1) {
            indexc1 = i;
        }
        if(ranks[i] == c2) {
            indexc2 = i;
        }
    }

    //printf("[C1: %i, IC1: %i] [C2: %i, IC2: %i] [Return %i]\n", c1, indexc1, c2, indexc2, (indexc1 < indexc2));
    return indexc1 < indexc2;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    for(int i = 0; i < candidate_count; i++) {
        for(int j = 0; j < candidate_count; j++) {
            // comparing same candidate = no preference
            if(i == j) {
                preferences[i][j] = 0;
            } else if(isbefore(ranks[i], ranks[j], ranks)) {
                // if rank if candidate ranks[i] is better than rank of candidate ranks[j]
                // preference of ranks[i] over ranks[j]
                preferences[ranks[i]][ranks[j]]++;
            }
        }
    }
    /*
    printf("\n");
    for(int i = 0; i < candidate_count; i++) {
        for(int j = 0; j < candidate_count; j++) {
            printf("%i ", preferences[i][j]);
        }
        printf("\n");
    }*/
    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    for(int i = 0; i < candidate_count; i++) {
        for(int j = 0; j < candidate_count; j++) {
            // candidate i has more preference over j than j has preference over i, add pair with i as winner
            if(preferences[i][j] > preferences[j][i]) {
                pairs[pair_count].winner = i;
                pairs[pair_count].loser = j;
                pair_count++;
            }
        }
    }

    /*printf("ADD PAIRS:\n");
    for(int i = 0; i < pair_count; i++) {
        printf("[Pair %i: [Winner: %i, Loser: %i]\n", i, pairs[i].winner, pairs[i].loser);
    }*/
    return;
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    // Bubble sort implementation
    int sorted = 0;
    while(sorted == 0) {
        sorted = 1;
        for(int i = 0; i < pair_count - 1; i++) {
            if(preferences[pairs[i].winner][pairs[i].loser] < preferences[pairs[i + 1].winner][pairs[i + 1].loser]) {
                pair temp = pairs[i + 1];
                pairs[i + 1] = pairs[i];
                pairs[i] = temp;
                sorted = 0;
            }
        }
    }
/*
    printf("SORTED PAIRS\n");
    for(int i = 0; i < pair_count; i++) {
        printf("[Pair %i: [Winner: %i, Loser: %i]]\n", i, pairs[i].winner, pairs[i].loser);
    }*/
    return;
}


bool dfs_check_cycle(int i, int j) {

    // got to a node that has been visited already - no cycle
    if(visited[i][j]) {
        return false;
    }
    // got to a node that is currently being explored - CYCLE!
    if(explore[i][j]) {
        return true;
    }

    explore[i][j] = true;
    for(int a = 0; a < candidate_count; a++) {
        // Check all neighbours of [i][j]
        if(locked[j][a] && !visited[j][a] && a != i) {
            if(dfs_check_cycle(a, a)) {
                return true;
            }
        }
    }

    explore[i][j] = false;
    visited[i][j] = true;

    return false;
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{

    for(int i = 0; i < pair_count; i++) {

        // clear visited and explore matrices after locking a pair
        for (int j = 0; j < candidate_count; j++)
        {
            for (int k = 0; k < candidate_count; k++)
            {
                visited[j][k] = false;
                explore[j][k] = false;
            }
        }

        // set locked to true - then check for cycle
        // if this lock created a cycle, remove it
        locked[pairs[i].winner][pairs[i].loser] = true;

        if(dfs_check_cycle(pairs[i].winner, pairs[i].loser)) {
            locked[pairs[i].winner][pairs[i].loser] = false;
        }
    }

    return;
}

// Print the winner of the election
void print_winner(void)
{
    int w = -1;
    bool anylocks = false;
    int vertices[candidate_count];

    for(int i = 0; i < candidate_count; i++) {
        vertices[i] = 0;
    }

    // if there are any locks from j over i, then this vertex can't be a source
    for(int i = 0; i < candidate_count; i++) {
        for(int j = 0; j < candidate_count; j++) {
            if(locked[j][i]) {
                vertices[i]++;
            }
        }
    }

    // the vertex with 0 locks is the source, therefore the winner
    for(int i = 0; i < candidate_count; i++) {
        if(vertices[i] == 0) {
            w = i;
        }
    }

    printf("%s\n", candidates[w]);
    return;
}