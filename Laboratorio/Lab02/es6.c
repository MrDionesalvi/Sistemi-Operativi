#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int isComputational(char* line){
    if(strstr(line, "computational") != NULL){
        return 1;
    }
    return 0;
}

int main(){
    FILE* file = fopen("complexity.txt", "r");
    if(file == NULL){
        printf("Error opening file\n");
        return 1;
    }
    char line[100];
    printf("File without modify:\n");
    while(fgets(line, sizeof(line), file)){
        printf("%s", line);
    }
    rewind(file);
    int counter = 0;
    printf("\n\nComputational lines:\n");
    while(fgets(line, sizeof(line), file)){
        if(isComputational(line)){
            printf("%s", line);
            counter++;
        }
    }
    printf("\nNumber of computational lines: %d\n", counter);
}