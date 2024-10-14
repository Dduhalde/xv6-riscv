#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void test_priority_boost(int processes) {
    // Create the number of processes specified
    for (int i = 0; i < processes; i++) {
        // Create a new process
        int pid = fork();
        // Check if the process is the child
        if (pid == 0) {
            sleep(i+1);
            printf("Process %d with priority %d and boost %d\n", getpid(), getpriority(), getboost());
            exit(0);
        }
    }
}

int main(int argc, char *argv[]) {
    // Check the number of arguments
    if (argc != 2) { // 2 arguments: the program name and the number of processes
        // In case of wrong number of arguments, print the usage of the program
        printf("Usage: %s <number_of_processes>\n", argv[0]);
        exit(1);
    } else {
        printf("Test priority boost for %d process\n", atoi(argv[1]));
        test_priority_boost(atoi(argv[1]));
        exit(0);
    }
}