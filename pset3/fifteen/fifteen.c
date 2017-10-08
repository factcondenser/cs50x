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
#include <string.h>

/* constants */
// limits
#define DIM_MIN 3
#define DIM_MAX 9
#define MAX_LENGTH 4 // max length of input
// style
#define NUM_COLORS 7 // number of colors used in rainbow()
#define TILE_HILIGHT 33 // 30-37 blk,red,grn,yel,blu,mag,cya,whi; 39 none
#define TILE_BOLD 1 // 1 for bold; 22 for no bold
// movement keys
#define UP "i"
#define UP_ALT "w"
#define LEFT "j"
#define LEFT_ALT "a"
#define DOWN "k"
#define DOWN_ALT "s"
#define RIGHT "l"
#define RIGHT_ALT "d"
// movement values; translated to movements by move()
#define UP_INT 1000 // must be greater than DIM_MAX * DIM_MAX - 1
#define LEFT_INT UP_INT + 1
#define DOWN_INT UP_INT + 2
#define RIGHT_INT UP_INT + 3

// board
int board[DIM_MAX][DIM_MAX];
// dimensions
int d;
// coordinates of empty space
int emprow;
int empcol;

// prototypes
void clear(void);
void greet(void);
void init(void);
void draw(void);
bool move(int tile);
bool won(void);
void swaptiles(int row, int col, int row2, int col2);
bool moveable(int row, int col);
void rainbow(char *str, int n);

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
    if (file == NULL) return 3;

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
            // print a colorful congratulatory message
            if (d == DIM_MIN) printf("Not bad, but can you solve a more challenging one now?");
            else if (d > DIM_MIN && d < DIM_MAX) rainbow("Congratualations! You're pretty good at this!", 7);
            else if (d == DIM_MAX) rainbow("Wow! You're a true master of this game! Maybe take a break now?", 7);
            printf("\n");
            // prompt user to play again or quit
            printf("Enter new dimension (ranged %i-%i) to play again\n",
                DIM_MIN, DIM_MAX);
            printf("Enter 'q' to quit\n");
            // retry getting user input if necessary
            RETRY:;
            char newd = get_char();
            // quit
            if (newd - '0' == 'q' - '0') break;
            // prompt user to re-enter dimension
            if (newd - '0' < DIM_MIN || newd - '0' > DIM_MAX) {
                printf("Board must be between %i x %i and %i x %i, inclusive.\n",
                    DIM_MIN, DIM_MIN, DIM_MAX, DIM_MAX);
                goto RETRY;
            // begin again with new board
            } else {
                d = newd - '0';
                init();
                continue;
            }
        }

        // prompt for move
        printf("Tile to move: ");
        // wait for input
        WAIT:;
        char *input = malloc(MAX_LENGTH * sizeof(char));
        input = get_string();
        // log input (for testing)
        fprintf(file, "%s\n", input);
        fflush(file);
        // reset stdin to take input from terminal when
        // end of (optional) input file is reached
        if (input == NULL) {
            freopen("/dev/tty", "r", stdin);
            goto WAIT;
        }
        // quit if user inputs 0 (for testing)
        if (strcmp(input, "0") == 0) break;

        int tile;
        if (strcmp(input, UP) == 0 ||
            strcmp(input, UP_ALT) == 0) tile = UP_INT;
        else if (strcmp(input, LEFT) == 0 ||
            strcmp(input, LEFT_ALT) == 0) tile = LEFT_INT;
        else if (strcmp(input, DOWN) == 0 ||
            strcmp(input, DOWN_ALT) == 0) tile = DOWN_INT;
        else if (strcmp(input, RIGHT) == 0 ||
            strcmp(input, RIGHT_ALT) == 0) tile = RIGHT_INT;
        else tile = atoi(input);

        // if user enters non-integer, prompt for an integer
        if (tile == 0)
        {
            printf("Retry: ");
            goto WAIT;
        }

        // log move (for testing)
        fprintf(file, "%i\n", tile);
        fflush(file);

        // move if possible, else report illegality
        if (!move(tile)) {
            printf("\n%s\n", (tile < 0 || tile > d * d - 1) ?
            "Tile does not exist." : "Illegal move.");
            usleep(500000);
        }

        // sleep thread for animation's sake
        usleep(1000);
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
    usleep(1000000);
    printf("Press '%s' or '%s' to move tile UP\n", UP, UP_ALT);
    printf("Press '%s' or '%s' to move tile DOWN\n", DOWN, DOWN_ALT);
    printf("Press '%s' or '%s' to move tile LEFT\n", LEFT, LEFT_ALT);
    printf("Press '%s' or '%s' to move tile RIGHT\n", RIGHT, RIGHT_ALT);
    usleep(1000000);
    printf("\n");
    printf("Enter any key to begin\n");
    get_char();
}

/**
 * Initializes the game's board with tiles numbered 1 through d*d - 1
 * (i.e., fills 2D array with values but does not actually print them).
 */
void init(void)
{
    int tile = d * d - 1;
    for (int i = 0; i < d; i++) {
        for (int j = 0; j < d; j++) {
            board[i][j] = tile--;
        }
    }
    // for boards with odd number of tiles, swap 3rd-to-last and 2nd-to-last tiles
    if (d % 2 == 0) {
        swaptiles(d - 1, d - 2, d - 1, d - 3);
    }
    // initialize coordinates of empty space
    emprow = empcol = d - 1;
}
/**
 * Prints the board in its current state.
 */
void draw(void)
{
    // iterate through all tiles in the board
    for (int i = 0; i < d; i++) {
        for (int j = 0; j < d; j++) {
            // highlight moveable tiles by turning on color and bold font
            if (moveable(i, j) && !won()) printf("\033[%i;%im",
                TILE_HILIGHT, TILE_BOLD);
            int curtile = board[i][j];
            // if tile is single digit, print space for aesthetic reasons
            if (curtile < 10) printf(" ");
            // print a space to represent empty space
            curtile == 0 ? printf(" ") : printf("%i", curtile);
            // turn off color and bold font for next tile
            printf("\033[39;0m");
            // print extra space between columns for aesthetic reasons
            if (j < d - 1) printf("   ");
        }
        // print extra space between rows for aesthetic reasons
        printf("\n\n");
    }
}

/**
 * If tile borders empty space, moves tile and returns true, else
 * returns false.
 */
bool move(int tile)
{
    // enable movement using keys assigned to UP, LEFT, DOWN, and RIGHT
    if (tile == UP_INT && emprow < d - 1) {
        swaptiles(emprow + 1, empcol, emprow, empcol);
        emprow++;
        return true;
    }
    if (tile == LEFT_INT && empcol < d - 1) {
        swaptiles(emprow, empcol + 1, emprow, empcol);
        empcol++;
        return true;
    }
    if (tile == DOWN_INT && emprow > 0) {
        swaptiles(emprow - 1, empcol, emprow, empcol);
        emprow--;
        return true;
    }
    if (tile == RIGHT_INT && empcol > 0) {
        swaptiles(emprow, empcol - 1, emprow, empcol);
        empcol--;
        return true;
    }

    // enable movement using typed integers
    // find and record position of given tile
    for (int i = 0; i < d; i++) {
        for (int j = 0; j < d; j++) {
            if (board[i][j] == tile) {
                if (moveable(i, j)) {
                    // move the tile
                    swaptiles(i, j, emprow, empcol);
                    // record new coordinates of empty space
                    emprow = i;
                    empcol = j;
                    return true;
                }
            }
        }
    }
    return false;
}

/**
 * Returns true if game is won (i.e., board is in winning configuration),
 * else false.
 */
bool won(void)
{
    // check that empty space is in last row and last column
    if (emprow == d - 1 && empcol == d - 1) {
        int tile = 1;
        for (int i = 0; i < d; i++) {
            for (int j = 0; j < d; j++) {
                // if any tile is out of place, return false
                if (board[i][j] != tile++) return false;
                // if last tile is reached without returning false, return true
                if (tile == d * d) return true;
            }
        }
    }
    return false;
}

/**
 * Swaps two tiles.
 */
void swaptiles(int row, int col, int row2, int col2)
{
    int t = board[row][col];
    board[row][col] = board[row2][col2];
    board[row2][col2] = t;
}

/**
 * Returns true if tile borders empty space, else false.
 */
bool moveable(int row, int col)
{
    if ((row == emprow && abs(col - empcol) == 1) ||
    (col == empcol && abs(row - emprow) == 1)) return true;
    return false;
}

/**
 * Prints a given string n times, char by char, using different colors.
 */
void rainbow(char *str, int n)
{
     // do the following n times
     for (int i = 0; i < n; i++) {
        // iterate through available colors; start printing with different color each time
        for (int j = NUM_COLORS; j > 0; j--) {
            // save current position of cursor (ie beginning of string)
            printf("\033[s");
            // clear to end of line (erase entire string)
            printf("\033[K");
            // iterate through chars of string
            int k = 0, l = j;
            while (str[k] != 0) {
                // cycle back to first color if necessary
                if (l >= NUM_COLORS) l = 0;
                // print current char with current color
                printf("\033[%im", 31 + l++);
                // special case for dealing with Chinese characters (and possibly other symbols)
                if (str[k] - '0' < -16) {
                    for (int m = 0; m < 3; m++) {
                        printf("%c", str[k++]);
                    }
                } else printf("%c", str[k++]);
            }
            // move cursor down to next line for aesthetic reasons
            printf("\n");
            usleep(85000);
            // return to beginning of string
            printf("\033[u");
        }
     }
}