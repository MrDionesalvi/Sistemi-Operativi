#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void handler_int(int s)
{
    printf("\n ricevuto segnale SIGTERM \n");
    signal(SIGTERM, SIG_DFL); // reset dell'associazione
}
int main(int argc, char *argv[])
{
    int i;
    // associazione segnale-handler
    signal(SIGTERM, handler_int);
    while (1)
        sleep(1);
    exit(EXIT_SUCCESS);
}
