#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main(){
    pid_t n;

    printf("\nProcesso padre\n");
    for (int i=0; i<3; i++) {
        n = fork();
        if (n == 0) printf("sono un figlio\n");
        else printf("sono un padre\n");
    }
}  