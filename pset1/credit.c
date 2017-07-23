#include <stdio.h> 
#include <cs50.h>
#include <string.h>

/* Function declarations */
bool valid(char* card_str);
void print_card_type(int num_digits, int first_dig, int second_dig);

int main(void)
{
    printf("Number: ");
    long long card_num = get_long_long();
    char card_str[50];
    int num_digits = sprintf(card_str, "%lld", card_num);
    int first_dig = card_str[0];
    int second_dig = card_str[1];
    
    bool is_valid = valid(card_str);
    if (is_valid) {
        print_card_type(num_digits, first_dig, second_dig);
    } else {
        printf("INVALID\n");
    }
}

/* Return true if number satisfies Luhn's algorithm, false otherwise */
bool valid(char* card_str)
{
    int length = strlen(card_str);
    int sum_one = 0;
    int sum_two = 0;
    for (int i = length-2; i > -1; i=i-2) {
        int prod = (card_str[i]-'0')*2;
        sum_one += prod/10+prod%10;
    }
    for (int i = length-1; i > -1; i=i-2) {
        sum_two += card_str[i]-'0';
    }
    
    /* Print sums for debugging */
    //printf("sum_one: %d\n", sum_one);
    //printf("sum_two: %d\n", sum_two);
    
    if ((sum_one+sum_two)%10==0) {
        return true;
    }
    return false;
}

/* Print card type based on identifying characteristics */ 
void print_card_type(int num_digits, int first_dig, int second_dig)
{
    if (num_digits == 15 && first_dig == '3' && (second_dig == '4' || second_dig == '7')) {
        printf("AMEX\n");
    } else if (num_digits == 16 && first_dig == '5') {
        for (int i = '1'; i < '6'; i++) {
            if (second_dig == i) {
                printf("MASTERCARD\n");
            }
        }
    } else if ((num_digits == 16 || num_digits == 13) && first_dig == '4') {
        printf("VISA\n");
    } else {
        printf("INVALID\n");
    }
}