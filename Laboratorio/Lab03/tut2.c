#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


int main(){
    FILE* file = fopen("file.txt", "w+");
    if(file == NULL){
        printf("Error opening file\n");
        return 1;
    }
    pid_t pid;
    switch (pid = fork()){
    case -1:
        return 0;
        break;
    case 0:
        fprintf(file, "Carr scrive dentro al fileeeeee\n");
        printf("Processo figlio dead\n");
        exit(0);
        break;
    default:
        wait(NULL);
        rewind(file);
        if(file == NULL){
            printf("Error opening file\n");
            return 1;
        }
        char c;
        printf("Contenuto file:\n");
        while((c = fgetc(file)) != EOF){
            printf("%c", c);
        }
        fclose(file);
        break;
    }
}