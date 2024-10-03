#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void recur_to_up(char* stringa, int i){
    if(stringa[i] == '\0'){
        return;
    }
    if(stringa[i] >= 'a' && stringa[i] <= 'z'){
        stringa[i] = stringa[i] - 32;
    }
    recur_to_up(stringa, i + 1);
}
int main(){
    char* stringa = (char*)malloc(100);
    printf("Inserisci una stringa: ");
    scanf("%s", stringa);
    recur_to_up(stringa, 0);
    printf("La stringa in maiuscolo è: %s\n", stringa);
    return 0;
}