#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

int main(void){
    string text = get_string("Text: ");

    int letters=0;
    int words = 1;
    int sentences = 0;
    for(int i=0, l=strlen(text); i<l; i++){
        char character= text[i];
        if(character==' '){
            words=words+1;
        }
        else if(character=='.'||character=='?'||character=='!'){
            sentences=sentences+1;
        }
        else if((character>='a'&&character<='z')||(character>='A'&&character<='Z')){
            letters=letters+1;
        }
    }


    float L= 100.0* letters / words; // average number of letters per 100 words
    float S= 100.0*sentences/words; //  average number of sentences per 100 words in the text

    float index_float =0.0588 * L - 0.296 * S - 15.8;
    int index=round(index_float);

    if(index<1){
        printf("Before Grade 1\n");
    }
    else if(index>=16){
        printf("Grade 16+\n");
    }
    else{
        printf("Grade %i\n", index);
    }

}
