#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>    // Libreria per le funzioni variadiche
#include <sys/types.h> /* For portability */
#include <sys/msg.h>
#include <sys/ipc.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/wait.h>

#define COLOR_RESET   "\x1b[0m"
#define COLOR_RED     "\x1b[31m"
#define COLOR_GREEN   "\x1b[32m"
#define COLOR_YELLOW  "\x1b[33m"
#define ICON_ERROR    "❌"
#define ICON_SUCCESS  "✅"
#define ICON_WARNING  "⚠️"

// Enum per il tipo di messaggio
typedef enum {
    MESSAGE_ERROR,
    MESSAGE_SUCCESS,
    MESSAGE_WARNING
} MessageType;

// Funzione generica per stampare messaggi colorati
void print_message(MessageType type, const char *format, ...) {
    const char *color;
    const char *icon;

    // Imposta colore e icona in base al tipo di messaggio
    switch (type) {
        case MESSAGE_ERROR:
            color = COLOR_RED;
            icon = ICON_ERROR;
            break;
        case MESSAGE_SUCCESS:
            color = COLOR_GREEN;
            icon = ICON_SUCCESS;
            break;
        case MESSAGE_WARNING:
            color = COLOR_YELLOW;
            icon = ICON_WARNING;
            break;
        default:
            color = COLOR_RESET;
            icon = "";
    }

    // Crea il messaggio formattato
    char buffer[256];
    va_list args;
    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);

    // Stampa il messaggio con colore e icona
    printf("%s%s %s%s\n", color, icon, buffer, COLOR_RESET);
}

// Struttura per i messaggi della coda
struct mymsg { 
    long mtype;   /* Tipo di messaggio */ 
    char mtext[100]; /* Corpo del messaggio */
} msg;

int main(int argc, const char* argv[]) {
    if(argc != 2){
        print_message(MESSAGE_ERROR, "Usage: %s <number of processes>", argv[0]);
        return 1;
    }

    FILE *f = fopen("msgq_id.txt", "r");
    if(f == NULL){
        print_message(MESSAGE_ERROR, "Error opening msgq_id.txt");
        return 1;
    }

    int msqid;
    fscanf(f, "%d", &msqid);

    struct msqid_ds buf;
    if (msgctl(msqid, IPC_STAT, &buf) == -1) {
        print_message(MESSAGE_ERROR, "Error getting message queue information");
        return 1;
    }

    printf("Message queue identifier: %d\n", msqid);

    for(int i = 0; i < atoi(argv[1]); i++) {
        pid_t pid;
        switch(pid = fork()) {
            case -1:
                print_message(MESSAGE_ERROR, "Error creating process with fork");
                return 1;
            case 0: {
                pid_t pid = getpid();
                msg.mtype = 1; 
                
                snprintf(msg.mtext, sizeof(msg.mtext), "%d %d", i, pid);
                print_message(MESSAGE_SUCCESS, "Message (%d) text: %s", i, msg.mtext);

                if (msgsnd(msqid, &msg, sizeof(msg) - sizeof(long), 0) < 0) {
                    print_message(MESSAGE_ERROR, "Error sending message to message queue");
                    return 1;
                }
                
                print_message(MESSAGE_WARNING, "Message (%d) sent to message queue. Waiting for response...", i);
                
                if (msgrcv(msqid, &msg, sizeof(msg) - sizeof(long), pid, 0) != -1) {
                    print_message(MESSAGE_SUCCESS, "Message response for (%d) received from message queue.", i);
                    printf("Response text: %s\n", msg.mtext);
                } else {
                    print_message(MESSAGE_ERROR, "Error receiving message from queue");
                    return 1;
                }
                
                return 1;
            }
            default:
                wait(NULL);
        }
    }

    for(int i = 0; i < argc-1; i++) {
        wait(NULL);
    }

    // Invia messaggio di chiusura
    msg.mtype = 1;
    strncpy(msg.mtext, "END", sizeof(msg.mtext) - 1);
    if (msgsnd(msqid, &msg, sizeof(msg) - sizeof(long), IPC_NOWAIT) < 0) {
        print_message(MESSAGE_ERROR, "Error sending END message");
        return 1;
    }
    
    print_message(MESSAGE_SUCCESS, "All messages sent. Closing connection...");
    return 0;
}
