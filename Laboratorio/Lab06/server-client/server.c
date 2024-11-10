#include <stdio.h>
#include <sys/types.h> /* For portability */
#include <sys/msg.h>
#include <sys/ipc.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdarg.h>

// Definizioni di colore e icone
#define COLOR_RESET "\033[0m"
#define COLOR_INFO "\033[0;36m"    // Cyan
#define COLOR_SUCCESS "\033[0;32m" // Green
#define COLOR_WARNING "\033[0;33m" // Yellow
#define COLOR_ERROR "\033[0;31m"   // Red

void print_colored(const char* type, const char* format, ...) {
    const char* color;
    const char* icon;

    // Assegna colore e icona in base al tipo di messaggio
    if (strcmp(type, "info") == 0) {
        color = COLOR_INFO;
        icon = "[ℹ️]";
    } else if (strcmp(type, "success") == 0) {
        color = COLOR_SUCCESS;
        icon = "[✅]";
    } else if (strcmp(type, "warning") == 0) {
        color = COLOR_WARNING;
        icon = "[⚠️]";
    } else if (strcmp(type, "error") == 0) {
        color = COLOR_ERROR;
        icon = "[❌]";
    } else {
        color = COLOR_RESET;
        icon = "";
    }

    // Stampa messaggio formattato con colore e icona
    printf("%s%s ", color, icon);
    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);
    printf("%s", COLOR_RESET);  // Reset colore
}

struct mymsg { 
    long mtype;   /* Message type */ 
    char mtext[100]; /* Message body */
} msg;

int main() {
    int msqid;
    msqid = msgget(IPC_PRIVATE, IPC_CREAT | 0666);
    if (msqid == -1) {
        perror("msgget");
        return 1;
    }
    print_colored("success", "Message queue identifier: %d\n", msqid);

    FILE *f = fopen("msgq_id.txt", "w");
    if (f == NULL) {
        perror("fopen");
        return 1;
    }
    fprintf(f, "%d", msqid);
    fclose(f);
    print_colored("success", "Message queue identifier written to msgq_id.txt.\n\n");
    srand(time(NULL));

    struct mymsg response;
    while (msgrcv(msqid, &msg, sizeof(msg) - sizeof(long), 1, 0) != -1) {
        pid_t fork_pid;
        if (msg.mtype == 1 && strcmp(msg.mtext, "END") == 0) {
            break;
        }
        switch(fork_pid = fork()) {
            case -1:
                print_colored("error", "Failed to fork.\n");
                perror("fork");
                return 1;
            case 0:
                print_colored("success", "Child process created.\n");
                print_colored("info", "Message received from message queue.\n");
                print_colored("info", "Message text: %s\n", msg.mtext);
                int upper_limit;
                pid_t client_pid;
                
                if (msg.mtype == 1 && strcmp(msg.mtext, "END") == 0) {
                    break;
                }

                sscanf(msg.mtext, "%d %d", &upper_limit, &client_pid);

                int random_number = rand() % (upper_limit + 1);
                snprintf(response.mtext, sizeof(response.mtext), "Random number between 0 and %d: %d\n", upper_limit, random_number);
                
                response.mtype = client_pid;
                
                if (msgsnd(msqid, &response, sizeof(response) - sizeof(long), 0) < 0) {
                    print_colored("error", "Failed to send response to client.\n");
                    perror("msgsnd");
                    return 1;
                }
                print_colored("success", "Response sent to client.\n\n");
                return 1;
        }
    }

    // Rimozione della coda di messaggi
    msgctl(msqid, IPC_RMID, NULL);
    print_colored("success", "Message queue removed.\n");
    return 0;
}
