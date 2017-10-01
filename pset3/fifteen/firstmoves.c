#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// constants
#define DIM_MIN 3
#define DIM_MAX 9

#define UP_INT 1000

#define BEGIN "b"

// dimensions
int d;

int main(int argc, string argv[])
{
    // ensure proper usage
    if (argc != 2)
    {
        printf("Usage: firstmoves d\n");
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

    printf("%s\n", BEGIN);

    // iterate through all rings of tiles
    for (int i = d; i > 1; i = i - 2) {
        // for each ring, iterate through all positions
        for (int j = 0; j < i * 2 - 3; j++) {
            // iterate through all movement directions
            for (int k = 3; k >= 0; k--) {
                // for each direction, iterate through all positions
                for (int l = 0; l < i - 1; l++) {
                    printf("%d\n", UP_INT + k);
                }
            }
        }
        if (i > 2) {
            printf("%d\n", UP_INT + 2);
            printf("%d\n", UP_INT + 3);
        }
    }

    /**
     8
        13x
            3
                7x
            2
                7x
            1
                7x
            0
                7x
    6
        9x
            3
                5x
            2
                5x
            1
                5x
            0
                5x
    4
        5x
            3
                3x
            2
                3x
            1
                3x
            0
                3x
    2
        1x
            3
                1x
            2
                1x
            1
                1x
            0
                1x
    */
}