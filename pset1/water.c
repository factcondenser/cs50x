#include <stdio.h>
#include <cs50.h>

int main(void)
{
    printf("Minutes: ");

    int minutes = get_int();

    int bottles = minutes * 12;

    printf("%s%d\n", "Bottles: ", bottles);
}