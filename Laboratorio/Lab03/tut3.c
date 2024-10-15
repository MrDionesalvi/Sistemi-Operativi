#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

pid_t pid;

int main(int argc, char *argv[]) {

    for (int i = 1; i < argc; i++) {
        pid = fork();
        if(pid == 0){
            printf("PID=%ld || Aspetto per %ss\n", (long)getpid(), argv[i]);
            sleep(atoi(argv[i]));
            exit(EXIT_SUCCESS);
        }
    }
    for (int i = 1; i < argc; i++) {
        wait(NULL);
    }
    int idx = 0; // numero errore
    for( idx = 0; idx < sys_nerr; idx++ ) 
        printf( "Error #%3d: %s\n", idx, strerror( idx ) );
    return 0;
}