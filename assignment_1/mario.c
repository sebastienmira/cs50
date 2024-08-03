#include <stdio.h>
#include <cs50.h>

int main(void){
    //prompt user for pos integer
    int n;
    do{
        n = get_int("Size: ");
    }
    while(n<1);
    //n by n grid
    for(int i=0; i<n; i++){
        for(int j=1; j<n-i;j++){
            printf(" ");
        }
        for(int j=0; j<=i;j++){
            printf("#");
        }
        printf("  ");
        for(int j=0; j<=i;j++){
            printf("#");
        }
        printf("\n");
    }
}
