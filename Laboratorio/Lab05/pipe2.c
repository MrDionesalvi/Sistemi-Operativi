#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define PATH_MAX 1024
int main(int argc, char **argv){
    FILE *fp;
    int status;
    char path[PATH_MAX];
    fp = popen("ls", "r");
    if (fp == NULL)
        ; // gestione errore
    while (fgets(path, PATH_MAX, fp) != NULL)
        printf("%s", path);
    status = pclose(fp);
    if (status == -1)
    {
        ; // gestione errore
    }
    else{
        // analisi dell'exit status
        if (WIFEXITED(status))
            printf("Exit status: %d\n", WEXITSTATUS(status));
        else
            printf("Terminated by signal %d\n", WTERMSIG(status));
    }
    return (0);
}