// main_pipe.c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/wait.h>

#define BUFFER_SIZE 1024

int main() {
    int pipe_fd[2];
    pid_t pid;
    char buffer[BUFFER_SIZE];

    // Creazione della pipe
    if (pipe(pipe_fd) == -1) {
        perror("Errore nella creazione della pipe");
        exit(1);
    }

    // Creazione del processo figlio
    pid = fork();
    if (pid == -1) {
        perror("Errore nella fork");
        exit(1);
    }

    if (pid == 0) { // Processo figlio
        // Chiude l'estremità di scrittura della pipe
        close(pipe_fd[1]);

        // Redirige stdin dalla pipe
        dup2(pipe_fd[0], STDIN_FILENO);
        close(pipe_fd[0]);

        // Esegue il programma `filter`
        execl("./filter", "filter", NULL);
        perror("Errore nell'esecuzione di filter");
        exit(1);

    } else { // Processo genitore
        // Chiude l'estremità di lettura della pipe
        close(pipe_fd[0]);

        // Chiede l'input da tastiera
        printf("Inserisci una stringa: ");
        fgets(buffer, BUFFER_SIZE, stdin);

        // Scrive la stringa nella pipe
        write(pipe_fd[1], buffer, strlen(buffer));

        // Chiude l'estremità di scrittura per inviare EOF
        close(pipe_fd[1]);

        // Attende la fine del processo figlio
        wait(NULL);
    }

    return 0;
}
