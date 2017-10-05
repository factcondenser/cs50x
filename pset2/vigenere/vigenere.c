#include <stdio.h>
#include <cs50.h>
#include <ctype.h>
#include <string.h>

int validkey(string s);

int main(int argc, string argv[])
{
    // Check that user has entered exactly two arguments, and that the key entered is valid
    if (argc != 2 || !validkey(argv[1])) {
        printf("Usage: ./vigenere k\n");
        return 1;
    }

    // Store the user-provided key and its length
    string k = argv[1];
    int klen = strlen(k);
    // Convert key to all-uppercase
    for (int i = 0; i < klen; i++) {
        k[i] = toupper(k[i]);
    }
    printf("plaintext: ");
    // Store the user-provided plaintext and its length
    string p = get_string();
    int plen = strlen(p);
    // Declare a string to store the ciphertext
    string c = p;
    // Create a flag to check whether a given plaintext char is uppercase]
    int j = 0;
    for (int i = 0; i < plen; i++) {
        if (isalpha(p[i])) {
            bool isupper = true;
            if (!isupper(p[i])) {
                isupper = false;
                p[i] = toupper(p[i]);
            }
            c[i] = (p[i] - 65 + (k[j] - 65)) % 26 + 65;
            if (!isupper) c[i] = tolower(c[i]);
            if (j == klen - 1) j = 0;
            else { j++; }
        }
    }
    printf("ciphertext: %s\n", c);
}

// Return true if key contains only alphabetic characters; false otherwise
int validkey(string s)
{
    for (int i = 0; i < strlen(s); i++) {
        if (!isalpha(s[i])) return 0;
    }
    return 1;
}