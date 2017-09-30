#define _XOPEN_SOURCE
#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>

void breakcrypt(string k, string h);

int main(int argc, string argv[])
{
    if (argc != 2 ) {
        printf("Usage: ./crack hash\n");
        exit(1);
    }
    
    string hash = argv[1];
    char key[5];
    string keychars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
    int keycharslen = strlen(keychars);
    
    for (int keylen = 1; keylen < 5; keylen++) {
        for (int i = 0; i < keycharslen; i++) {
            key[0] = keychars[i];
            if (keylen > 1) {
                for (int j = 0; j < keycharslen; j++) {
                    key[1] = keychars[j];
                    if (keylen > 2) {
                        for (int k = 0; k < keycharslen; k++) {
                            key[2] = keychars[k];
                            if (keylen > 3) {
                                for (int l = 0; l < keycharslen; l++) {
                                    key[3] = keychars[l];
                                    breakcrypt(key, hash);
                                }
                            } else breakcrypt(key, hash);
                        }
                    } else breakcrypt(key, hash);
                }
            } else breakcrypt(key, hash);
        }
    } 
}

void breakcrypt(string s, string h)
{
    char salt[3];
    string saltchars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789./";
    int saltcharslen = strlen(saltchars);
    for (int i = 0; i < saltcharslen; i++) {
        for (int j = 0; j < saltcharslen; j++) {
            salt[0] = saltchars[i];
            salt[1] = saltchars[j];
            if (crypt(s, salt) == h) printf("%s\n", s);
        } 
    }
}