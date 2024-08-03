#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

int main(int argc, string argv[]){
    //check if only one argument is provided
    if(argc!=2){
        printf("Invalid number of arguments\n");
        return 1;
    }

    string key=argv[1];
    //check if key has 26 characters
    if(strlen(key)!=26){
        printf("Invalid key. Key should contain 26 unique alphabetic characters.\n");
        return 1;
    }
    //check if they are alphabetical and unique
    for(int i=0; i<26; i++){
        if(key[i]<'A' || (key[i]>'Z' && key[i]<'a') || key[i]>'z'){
            printf("Invalid key. Key should only contain alphabetical values.\n");
            return 1;
        }
        for(int j=i+1;j<26;j++){
            if(key[i]==key[j]){
                printf("Invalid key. No repetition of characters is allowed.\n");
                return 1;
            }
        }
    }

    string alpha_lower="abcdefghijklmnopqrstuvwxyz";

    string plaintext= get_string("plaintext: ");
    string ciphertext=plaintext;

    for(int i=0, l=strlen(plaintext);i<(l);i++){
        char character=plaintext[i];
        if(character>='a' && character<='z'){
            for(int j=0; j<26;j++){
                if(character==alpha_lower[j]){
                    ciphertext[i]=tolower(key[j]);
                }
            }
        }
        else if(character>='A' && character<='Z'){
            for(int j=0; j<26;j++){
                if(tolower(character)==alpha_lower[j]){
                    ciphertext[i]=toupper(key[j]);
                }
            }
        }
        else{
            ciphertext[i]=plaintext[i];
        }
    }
    printf("ciphertext: %s\n",ciphertext);

}
