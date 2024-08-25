#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[]) {
    printf("Yo soy tu padre - dijo el proceso %d\n", getancestor(argc));
    exit(0);
}