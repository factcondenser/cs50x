/**
 * helpers.c
 *
 * Helper functions for Problem Set 3.
 */

#include <cs50.h>
#include <math.h>
#include <string.h>
#include <stdio.h>

#include "helpers.h"

// prototypes
int * merge(int left[], int right[], int n);

/**
 * Returns true if value is in array of n values, else false.
 * Assumes array is sorted from least to greatest.
 */
bool search(int value, int values[], int n)
{
    // base cases (the first is a sanity check)
    if (n <= 0) return 0;
    if (n == 1 && value != values[0]) return 0;
    // recursive case; split sorted array into two halves...
    int mid = n/2;
    int *left = values;
    int *right = values + n/2;
    // ... and check if value in middle is desired value
    if (value == values[mid]) return 1;
    // search left half
    if (value < values[mid]) return search(value, left, mid);
    // search right half
    return search(value, right, n - mid);
}

/**
 * Sorts array of n values.
 */
void sort(int values[], int n)
{
    // base case
    if (n <= 1) return;
    // recursive case; split array into two halves
    int mid = n/2;
    int left[mid], right[n - mid];
    memcpy(left, values, mid * sizeof(int));
    memcpy(right, &values[mid], (n - mid) * sizeof(int));
    // sort left half
    sort(left, mid);
    // sort right half
    sort(right, n - mid);
    // merge the two sorted halves
    memcpy(values, merge(left, right, n), n * sizeof(int));
    return;
}

/**
 * Merge two least-to-greatest int arrays into
 * one l-to-g int array of size n
 */
int * merge(int left[], int right[], int n)
{
    int *sorted = malloc(n * sizeof(int));
    // track current position in left and right arrays, respectively
    int l = 0, r = 0;
    // iterate through all positions in sorted
    for (int i = 0; i < n; i++) {
        // compare ints in left and right; put smaller int (or
            // right int if equal) in sorted
        if (l < n/2 && r < n - (n/2)) {
            if (left[l] < right[r]) {
                sorted[i] = left[l];
                l++;
            } else {
                sorted[i] = right[r];
                r++;
            }
        // if iterated through all positions in left, fill remaining
            // positions in sorted with values from right...
        } else if (l >= n/2) {
            sorted[i] = right[r];
            r++;
        // ... and vice versa
        } else if (r >= n - (n/2)) {
            sorted[i] = left[l];
            l++;
        }
    }
    return sorted;
}