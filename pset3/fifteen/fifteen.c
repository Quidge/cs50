/**
 * fifteen.c
 *
 * Implements Game of Fifteen (generalized to d x d).
 *
 * Usage: fifteen d
 *
 * whereby the board's dimensions are to be d x d,
 * where d must be in [DIM_MIN,DIM_MAX]
 *
 * Note that usleep is obsolete, but it offers more granularity than
 * sleep and is simpler to use than nanosleep; `man usleep` for more.
 */

#define _XOPEN_SOURCE 500

#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// constants
#define DIM_MIN 3
#define DIM_MAX 9

// I really wish these weren't out in the open as global vars.
// board
int board[DIM_MAX][DIM_MAX];

// dimensions
int d;

// prototypes
void clear(void);
void greet(void);
void init(void);
void draw(void);
bool move(int tile);
bool won(void);

int main(int argc, string argv[])
{
    // ensure proper usage
    if (argc != 2)
    {
        printf("Usage: fifteen d\n");
        return 1;
    }

    // ensure valid dimensions
    d = atoi(argv[1]);
    if (d < DIM_MIN || d > DIM_MAX)
    {
        printf("Board must be between %i x %i and %i x %i, inclusive.\n",
            DIM_MIN, DIM_MIN, DIM_MAX, DIM_MAX);
        return 2;
    }

    // open log
    FILE *file = fopen("log.txt", "w");
    if (file == NULL)
    {
        return 3;
    }

    // greet user with instructions
    greet();

    // initialize the board
    init();

    // accept moves until game is won
    while (true)
    {
        // clear the screen
        clear();

        // draw the current state of the board
        draw();

        // log the current state of the board (for testing)
        for (int i = 0; i < d; i++)
        {
            for (int j = 0; j < d; j++)
            {
                fprintf(file, "%i", board[i][j]);
                if (j < d - 1)
                {
                    fprintf(file, "|");
                }
            }
            fprintf(file, "\n");
        }
        fflush(file);

        // check for win
        if (won())
        {
            printf("ftw\n");
            break;
        }

        // prompt for move
        printf("Tile to move: ");
        int tile = get_int();

        // quit if user inputs 0 (for testing)
        if (tile == 0)
        {
            break;
        }

        // log move (for testing)
        fprintf(file, "%i\n", tile);
        fflush(file);

        // move if possible, else report illegality
        if (!move(tile))
        {
            printf("\nIllegal move.\n");
            usleep(500000);
        }

        // sleep thread for animation's sake
        usleep(500000);
    }

    // close log
    fclose(file);

    // success
    return 0;
}

/**
 * Clears screen using ANSI escape sequences.
 */
void clear(void)
{
    printf("\033[2J");
    printf("\033[%d;%dH", 0, 0);
}

/**
 * Greets player.
 */
void greet(void)
{
    clear();
    printf("WELCOME TO GAME OF FIFTEEN\n");
    usleep(2000000);
}

/**
 * Initializes the game's board with tiles numbered 1 through d*d - 1
 * (i.e., fills 2D array with values but does not actually print them).
 */
void init()
{
    int counter = 1;
    for (int row = 0; row < d; row++)
    {
        for (int col = 0; col < d; col++)
        {
            board[row][col] = d*d - counter;
            counter++;
        }
    }
    // Exception to deal with odd number of tiles. Tile '1' and '2'
    // should be switched if there are an odd number of tiles. Remember
    // an even number of spaces will give an odd number of movable game
    // tiles.
    if (d%2 == 0)
    {
        board[d-1][d-2] = 2;
        board[d-1][d-3] = 1;
    }
}

/**
 * Prints the board in its current state.
 */
void draw(void)
{
    printf("\n");
    for (int row = 0; row < d; row++)
    {
        for (int col = 0; col < d; col++)
        {
            int val = board[row][col];
            if (val != 0)
            {
                // Numbers < 10 take up one character, > 9 take up two
                // characters. The additional space for single char
                // numbers is necessary to avoid screwing up the spacing.
                if (val < 10)
                {
                    printf("  %i ", val);
                }
                else
                {
                    printf(" %i ", val);
                }
            }
            else
            {
                printf("  - ");
            }
        }
        printf("\n");
    }
    printf("\n");
}

/**
 * If tile borders empty space, moves tile and returns true, else
 * returns false.
 */
bool move(int tile)
{
    // inputting -42 will reorder all tiles into a winning configuration
    if (tile == -42)
    {
        int counter = 1;
        for (int row = 0; row < d; row++)
        {
            for (int col = 0; col < d && counter < d*d; col++)
            {
                board[row][col] = counter;
                counter++;
            }
        }
        board[d-1][d-1] = 0;
        return true;
    }

    int rowPos = -1;
    int colPos = -1;

    // There is 110% a more elegent way to do this. One way would be having
    // a single direction var and storing 1,2,3,4 or 0 for up=true, right=true, ...,
    // none=true respectively.
    int oldVal;
    bool above;
    bool below;
    bool left;
    bool right;

    for (int row = 0; row < d; row++)
    {
        for (int col = 0; col < d; col++)
        {
            if (board[row][col] == tile)
            {
                if (tile == -1)
                {
                    eprintf("%ix%i\n", row, col);
                    break;
                }
                rowPos = row;
                colPos = col;

                above = (row - 1 < 0 || board[row-1][col] != 0) ? false : true;
                below = (row + 1 >= d || board[row+1][col] != 0) ? false : true;
                left = (col - 1 < 0 || board[row][col-1] != 0) ? false : true;
                right = (col + 1 >= d || board[row][col+1] != 0) ? false : true;
            }
        }
    }
    if (rowPos < 0 || colPos < 0)
    {
        return false;
    }
    else
    {
        oldVal = board[rowPos][colPos];
    }
    if (above)
    {
        board[rowPos][colPos] = 0;
        board[rowPos - 1][colPos] = oldVal;
    }
    else if (below)
    {
        board[rowPos][colPos] = 0;
        board[rowPos + 1][colPos] = oldVal;
    }
    else if (left)
    {
        board[rowPos][colPos] = 0;
        board[rowPos][colPos - 1] = oldVal;
    }
    else if (right)
    {
        board[rowPos][colPos] = 0;
        board[rowPos][colPos + 1] = oldVal;
    }
    else //no directions allow movement; the tile is landlocked and move
    // is illegal
    {
        return false;
    }

    return true;
}

/**
 * Returns true if game is won (i.e., board is in winning configuration),
 * else false.
 */
bool won(void)
{
    /*  'check' is compared against every cell incrementally from top-left
    *   to bottom-right and incremented after each comparison. If at any
    *   point the check fails, it must mean that one of the tiles is
    *   out of order and the game is not in a winning configuration.
    */
    int check = 1;
    for (int row = 0; row < d; row++)
    {
        for (int col = 0; col < d && check < d*d; col++)
        {
            if (board[row][col] != check)
            {
                return false;
            }
            check++;
        }
    }
    return true;
}
