#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

int score(string word);

int main(void){
    string p1=get_string("Player 1: ");
    string p2=get_string("Player 2: ");

    int score_p1=score(p1);
    int score_p2=score(p2);

    //printf("Score Player 1 %i\n", score_p1);
    //printf("Score Player 2 %i\n", score_p2);

    if(score_p1>score_p2){
        printf("Player 1 Wins!\n");
    }
    else if(score_p1<score_p2){
        printf("Player 2 Wins!\n");
    }
    else if(score_p1==score_p2){
        printf("Tie!\n");
    }
}


int score(string word){
    int points=0;
    for(int i=0, l=strlen(word); i<l; i++){
        char letter=toupper(word[i]);
        if(letter=='A'|| letter=='E' || letter=='I' || letter=='L' || letter=='N' || letter=='O' || (letter>='R' && letter<='U')){
            points=points+1;
        }
        else if(letter=='D' || letter=='G'){
            points=points+2;
        }
        else if(letter=='B' || letter=='C' || letter=='M' || letter=='P'){
            points=points+3;
        }
        else if(letter=='F' || letter=='H' || letter=='V' || letter=='W' || letter=='Y'){
            points=points+4;
        }
        else if(letter=='K'){
            points=points+5;
        }
        else if(letter=='J' || letter=='X'){
            points=points+8;
        }
        else if(letter=='Q' || letter=='Z'){
            points=points+10;
        }
        else{
            points=points+0;
        }

    }
    return points;
}
