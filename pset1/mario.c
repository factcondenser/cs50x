#include <stdio.h>
#include <cs50.h>

int main(void)
{
    int height = -1;
    while(height < 0 || height > 23) {
         printf("Height: ");
         height = get_int();
    }
    for(int i = 0; i < height; i++) {
        printf("%.*s%.*s%s%.*s\n", height-1-i, "                      ", i+1, "#######################", "  ", i+1, "#######################");
    }
}