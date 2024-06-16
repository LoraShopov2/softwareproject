#include <stdio.h>
#include <stdlib.h>


int main(){
    int src, dst, digit, dst_num;
    char num;
    int decimal_num = 0;

    printf("enter the source base:");
    scanf("%d", &src);
    if (src < 2 || src > 16){
        printf("Invalid target base!");
        return -1;
    };

    printf("enter the target base:");
    scanf("%d", &dst);
    if (src < 2 || src > 16){
        printf("Invalid source base!");
        return -1;
    };
    
    printf("enter a number in base %d:", src);
    num = getchar();
    while ((num = getchar()) != '\n'){
        decimal_num *= src;
        if (num >='0' && num <= '9'){
            digit = num - '0';
        }
        else if (num >= 'A' && num <= 'F') {
            digit = num - 'A' + 10;
        }
        else{
            printf("Invalid input number!");
            return -1;
        }
        if (digit >= src){
            printf("Invalid input number!");
            return -1;
        }
        decimal_num += digit;
    }    

    printf("the number in base %d is:", dst);

    while (decimal_num > 0){
        digit = decimal_num % dst;
        decimal_num = decimal_num / dst;
        if (digit < 10){
            printf("%d", digit);
        }
        else{
            num = 'A' + digit - 10;
            printf("%s", num);
        }
    }

    return 0;
}
