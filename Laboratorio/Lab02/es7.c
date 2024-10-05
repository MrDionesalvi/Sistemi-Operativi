#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int containsWord(char* line, char* word){
    if(strstr(line, word) != NULL){
        return 1;
    }
    return 0;
}

int main(int argc, char* argv[], char* envp[]){
    int index = 0;
    while(envp[index] != NULL){
        if(containsWord(envp[index], "LOGNAME=") || containsWord(envp[index], "HOME=") || containsWord(envp[index], "PATH=")){
            printf("%s\n", envp[index]);
        }
        index++;
    }
}