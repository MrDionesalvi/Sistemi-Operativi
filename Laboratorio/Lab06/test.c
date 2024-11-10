#include <stdio.h>

// Definizione delle sequenze di escape ANSI per i colori
#define RESET "\033[0m"
#define RED "\033[31m"
#define YELLOW "\033[33m"
#define GREEN "\033[32m"

int main() {
    // Testo normale
    printf("Questo è un testo normale.\n");

    // Testo di errore in rosso
    printf(RED "Questo è un messaggio di errore.\n" RESET);

    // Testo di avviso in giallo
    printf(YELLOW "Questo è un messaggio di avviso.\n" RESET);

    // Testo di successo in verde
    printf(GREEN "Questo è un messaggio di successo.\n" RESET);

    return 0;
}