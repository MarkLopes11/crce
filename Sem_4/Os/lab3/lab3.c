#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
    pid_t child_pid = fork();

    if (child_pid > 0) {
        // Parent process
        printf("Parent process (PID=%d)\n", getpid());
        sleep(10); // Sleep to give the child process time to become a Zombie
    } else if (child_pid == 0) {
        // Child process
        printf("Child process (PID=%d)\n", getpid());
        exit(0); // Child process exits immediately
    } else {
        // Fork failed
        perror("Fork failed");
        exit(1);
    }

    return 0;
}
