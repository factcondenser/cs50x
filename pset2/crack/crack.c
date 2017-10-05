#define _XOPEN_SOURCE
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

int breakcrypt(char *k, char *h);

/* Cracks passwords of up to 4 characters that have been encrypted with the DES crypt() function */
int main(int argc, char *argv[])
{
    /* Include a clock_t for my own experimentation with different implementations of this program */
    //clock_t begin = clock();
    
    /* Instruct user on proper usage if not exactly two command-line arguments */
    if (argc != 2 ) {
        printf("Usage: ./crack hash\n");
        exit(1);
    }
    
    /* Set up the parameters that need to be passed to crypt() */
    // Store the address of the user-provided hash
    char *hash = argv[1];
    // Create array to store generated keys and initiate all values in the array to NULL
    char key[5] = { '\0', '\0', '\0', '\0', '\0' };
    // Store address of a char array containing all possible chars in key
    char *keychars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
    int keycharslen = strlen(keychars);
    
    /* Permute through every possible key of length up to 4 */
    for (int keylen = 1; keylen < 5; keylen++) {
        // Set the first character in key to each of the characters enumerated in keychars...
        for (int i = 0; i < keycharslen; i++) {
            key[0] = keychars[i];
            if (keylen > 1) {
                // ...and do the same for the second character in key if it exists...
                for (int j = 0; j < keycharslen; j++) {
                    key[1] = keychars[j];
                    if (keylen > 2) {
                        // ...and do the same for the thrid character in key if it exists...
                        for (int k = 0; k < keycharslen; k++) {
                            key[2] = keychars[k];
                            if (keylen > 3) {
                                // ...and do the same for the fourth character in key if it exists
                                for (int l = 0; l < keycharslen; l++) {
                                    key[3] = keychars[l];
                                    if (breakcrypt(key, hash)) goto END;
                                }
                            } else if (breakcrypt(key, hash)) goto END;
                        }
                    } else if (breakcrypt(key, hash)) goto END;
                }
            } else if (breakcrypt(key, hash)) goto END;
        }
    }
    
    /* If the hash of the generated key matches the user-provided hash, skip directly to here */
    END: ;
    /* Calculate and print out the time taken for this program to complete */
    // clock_t end = clock();
    // double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    // printf("%f\n", time_spent);
    /* Exit the program with no errors */
    exit(0);
}

/* Runs DES crypt() function on a provided string and returns true if resulting hash matches provided hash; false otherwise */ 
int breakcrypt(char *s, char *h)
{
    // Create array to store salt and initialize all values in the array to NULL    
    char salt[3] = { '\0', '\0', '\0' };
    // Set salt equal to first two characters of the provided hash in accordance with the crypt() man pages
    salt[0] = h[0];
    salt[1] = h[1];
    // Compare the hash resulting from running crypt() on the provided string with the provided hash
    if (strcmp(crypt(s, salt), h) == 0) {
        // Print the string if resulting hash matches provided hash
        printf("%s\n", s);
        return 1;
    }
    return 0;
}