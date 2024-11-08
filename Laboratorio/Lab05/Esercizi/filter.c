// filter.c
#include <stdio.h>
#include <ctype.h>

int main() {
    int c;
    while ((c = getchar()) != EOF) { // Legge un carattere alla volta
        if (islower(c)) {            // Controlla se è minuscolo
            c = toupper(c);          // Lo converte in maiuscolo
        }
        putchar(c);                  // Scrive il carattere convertito
    }
    return 0;
}
