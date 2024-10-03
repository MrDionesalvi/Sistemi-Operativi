#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* parse_nome(char* stringa_completa){
    char* nome = strtok(stringa_completa, ",");
    return nome;
}

int main(){
    char* nome = (char*)malloc(100);
    char* cognome = (char*)malloc(100);
    printf("Inserisci nome e cognome: ");
    scanf("%s %s", nome, cognome);

    char* stringa = (char*)malloc(200);
    sprintf(stringa, "%s, %s", nome, cognome);

    printf("Il nome della stringa è: %s\n", parse_nome(stringa));

    return 0;
}