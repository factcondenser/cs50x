#include <stdio.h>
#include <cs50.h>
#include <ctype.h>
#include <string.h>

int main(void)
{
    // Get the name of the user and store it as a string
    string name = get_string();
    int length = strlen(name);
    // Create flag for checking whether first initial has been printed yet
    bool first_init_printed = false;
    
    // Iterate over each char in name
    for (int i = 0; i < length; i++) {
        // Store the current char
        char curchar = name[i];
        // Continue to next char if current char is whitespace
        if (isspace(curchar)) continue;
        // Print first non-whitespace char (assumes this is first initial)
        if (!first_init_printed) {
            printf("%c", toupper(curchar));
            first_init_printed = true;
        // Print the next char that is immediately preceded by whitespace (assumes this is second initial)
        } else if (isspace(name[i-1])) {
            printf("%c", toupper(curchar));
        }
    }
    printf("\n");
}