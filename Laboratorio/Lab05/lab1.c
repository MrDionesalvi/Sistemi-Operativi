int main(int argc, char **argv)
{
    int n;
    int fd[2];
    pid_t pid;
    char line[MAXLINE];
    if (pipe(fd) < 0)
    ddErrExit("pipe error");
    if ((pid = fork()) < 0) {
                ddErrExit("fork error");
    } else if (pid > 0) { // --------- padre ---
                close(fd[0]);
                write(fd[1], "hello world\n", 12);
    } else { // ---------------------- figlio ---
                close(fd[1]);
                n = read(fd[0], line, MAXLINE);
                write(STDOUT_FILENO, line, n);
    }
    exit(EXIT_SUCCESS);
}
