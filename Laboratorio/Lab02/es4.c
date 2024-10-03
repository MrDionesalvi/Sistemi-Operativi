#include <stdio.h>
#include <ctype.h>
#include <string.h>

void stampaInvertitaMaiuscola(char *str) {
    if (*str) {
        stampaInvertitaMaiuscola(str + 1);
        putchar(toupper(*str));
    }
}

int main() {
    char frase[100];
    printf("Inserisci una frase: ");
    fgets(frase, sizeof(frase), stdin);

    printf("Frase iniziale: %s\n", frase);
    printf("Frase in caratteri maiuscoli e al contrario: ");
    stampaInvertitaMaiuscola(frase);
    printf("\n");

    return 0;
}