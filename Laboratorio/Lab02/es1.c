#include <stdio.h>
#include <stdlib.h>
#include <string.h>


char** leggi_lista(FILE* file_in, int* nof_elements); 
void stampa_lista(char** mio_ar, int n_elems);


int main(int argc, char* argv[]) {
    FILE* file_in;
    char** mio_ar;
    int n_elems;

    file_in = fopen("listanomi.txt", "r");
    
    if(file_in == NULL){
        printf("Error");
        return 0;
    }

    mio_ar = leggi_lista(file_in, &n_elems);
    fclose(file_in);

    if(mio_ar == NULL) {
        printf("Errore: impossibile leggere il file\n");
        return 1;
    }

    stampa_lista(mio_ar, n_elems);

    return 0;
}

char **leggi_lista(FILE* file_in, int* nof_elements){
    if(file_in == NULL) {
        return NULL;
    }
    char value[100];
    int i = 0;
    while(fscanf(file_in, "%99s\n", value) != EOF){
        i++;
    }

    *nof_elements = i;
    i = 0;
    char** result = malloc(sizeof(char*) * (*nof_elements));

    rewind(file_in);
    while(fscanf(file_in, "%99s\n", value) != EOF){
        result[i] = malloc(sizeof(char) * (strlen(value) + 1));
        strcpy(result[i], value);
        i++;
    }
    return result;
}

void stampa_lista(char** mio_ar, int n_elems){
    for(int i = 0; i < n_elems; i++) {
        printf("%s \n", mio_ar[i]);
    }
}