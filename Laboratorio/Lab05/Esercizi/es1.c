#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

#define BUF_SIZE 1024

int main(int argc, char *argv[])
{
    int pfd[2]; /* pipe file descriptors */
    int pfd2[2];
    char buf[BUF_SIZE];
    int secondPipe = 0;
    ssize_t numRead;
            
    if (argc < 2 || argc > 3 || strcmp(argv[1], "--help") == 0){
        // Printf usage
        printf("usage: %s <string> <string>\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    if (argc == 3){
        secondPipe = 1;
        if (pipe(pfd2) == -1)
            printf("pipe error");
    }

    if (pipe(pfd) == -1)
        printf("pipe error");               
    switch (fork()){
        case -1:
            printf("errore");                 
        case 0:        
            if (close(pfd[1]) == -1) 
                printf("not close");               
            while (1){ 
                numRead = read(pfd[0], buf, BUF_SIZE);
                if (numRead == -1)
                    printf("read error");
                if (numRead == 0)
                    break; /* End-of-file */
                if (write(STDOUT_FILENO, buf, numRead) != numRead)
                    printf("child - partial/failed write"); 
            }
            write(STDOUT_FILENO, "\n", 1);
            if (close(pfd[0]) == -1)
                printf("close"); 
            if (secondPipe){
                if (close(pfd2[1]) == -1)
                    printf("not close");
                while (1){
                    numRead = read(pfd2[0], buf, BUF_SIZE);
                    if (numRead == -1)
                        printf("read error");
                    if (numRead == 0)
                        break; /* End-of-file */
                    if (write(STDOUT_FILENO, buf, numRead) != numRead)
                        printf("child - partial/failed write");
                }
                write(STDOUT_FILENO, "\n", 1);
                if (close(pfd2[0]) == -1)
                    printf("close");
            }
            exit(EXIT_SUCCESS);
        default:                          
            if (close(pfd[0]) == -1) 
                printf("close fail");                                     
            if (write(pfd[1], argv[1], strlen(argv[1])) != strlen(argv[1]))
                    printf("close write");            
            if (close(pfd[1]) == -1) 
                    printf("EOF");
            if (secondPipe){
                if (close(pfd2[0]) == -1)
                    printf("close fail");
                if (write(pfd2[1], argv[2], strlen(argv[2])) != strlen(argv[2]))
                    printf("close write");
                if (close(pfd2[1]) == -1)
                    printf("EOF");
            }      
            wait(NULL);             
            exit(EXIT_SUCCESS);
    } // end switch
}
