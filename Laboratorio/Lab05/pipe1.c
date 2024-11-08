#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

#define BUF_SIZE 1024

int main(int argc, char *argv[])
{
    int pfd[2]; /* pipe file descriptors */
    char buf[BUF_SIZE];
    ssize_t numRead;
            
    if (argc != 2 || strcmp(argv[1], "--help") == 0){
        // Usage
        printf("%s string\n", argv[1]);
    }
    if (pipe(pfd) == -1) /* creo il pipe */
        printf("pipe error");                /* gestione errore */
    switch (fork()){
    case -1:
        printf("errore");                    /* gestione errore */
    case 0:          /* ------ figlio - legge dal pipe -------- */
        if (close(pfd[1]) == -1) /* chiusura write end */
            printf("not close");                /* gestione errore */
        for (;;){ /* legge dal pipe, e scrive su stdout */
            numRead = read(pfd[0], buf, BUF_SIZE);
            if (numRead == -1)
                printf("read error"); /* gestione errore */
            if (numRead == 0)
                break; /* End-of-file */
            if (write(STDOUT_FILENO, buf, numRead) != numRead)
                printf("child - partial/failed write"); /* gestione errore */
        }
        write(STDOUT_FILENO, "\n", 1);
        if (close(pfd[0]) == -1)
            printf("close"); /* gestione errore */
        exit(EXIT_SUCCESS);
        default:                          /* ------ padre - scrive sul pipe -------- */
            if (close(pfd[0]) == -1) /* chiusura del read end */
                printf("close fail");                                      /* gestione errore */
        if (write(pfd[1], argv[1], strlen(argv[1])) != strlen(argv[1]))
                printf("close write");                    /* gestione errore */
        if (close(pfd[1]) == -1) /* il figlio riceverà EOF */
                printf("EOF");                    /* gestione errore */
        wait(NULL);              /* attesa della terminazione del figlio */
        exit(EXIT_SUCCESS);
    } // end switch
}
