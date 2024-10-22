#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
static void dd_signal_handler(int sig){
    printf("ahiaaaaaa!\n");
}

void errExit(char *msg){
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[]){
    int j;
    if (signal(SIGINT, dd_signal_handler) == SIG_ERR)
        errExit("signal (SIG_ERR) error");
    for (j = 0;; j++)
    {
        printf("%d\n", j);
        sleep(1);
    }
}