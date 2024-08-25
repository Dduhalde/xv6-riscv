#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        exit(1);
    }
    
    int pid = atoi(argv[1]);
    printf("Yo soy tu ancestro - dijo el proceso %d\n", getancestor(pid));
    exit(0);
}