#include <cs50.h>
#include <stdio.h>
#include <math.h>

int main(void){
    long card_number = get_long("Card number:");

    int number_of_digits=log10(card_number)+1;

    int first_digits=card_number%((long) pow(10,number_of_digits))/ (long) pow(10,number_of_digits-2);

    //printf("number of digits %i\n",number_of_digits);
    //printf("first digits %i\n",first_digits);

    //check for card type
    string card_type;

    if(number_of_digits==15 && (first_digits==34 || first_digits==37)){
        card_type="AMEX";
    }

    else if(number_of_digits==16 && (first_digits>=51 && first_digits<=55)){
        card_type="MASTERCARD";
    }

    else if((number_of_digits==13 || number_of_digits==16) && (first_digits>=40 && first_digits<=49)){
        card_type="VISA";
    }

    else{
        card_type="INVALID";
        printf("%s\n",card_type);
        return 0;
    }


    //Luhn's Algorithm
    int sum=0;
    //Start in 2nd to last digit
    for(int i=0; i<number_of_digits; i++){
        int digit=card_number%((long) pow(10,(i+1)))/ (long) pow(10,i);
        if (i==0||i%2==0){
            sum=sum+digit;
        }
        else if(i%2!=0){
            if(digit*2<10){
                sum=sum+digit*2;
            }
            else{
                sum=sum+(digit*2)%10+(digit*2)%100/10;
            }
        }
    }
    //printf("Value of sum: %i\n",sum);
    if(sum%10!=0){
        card_type="INVALID";
    }

    printf("%s\n",card_type);
}
