#include <stdio.h>
#include <unistd.h>

int main() {
    for (int i = 1; i <= 10; i++) {
        fprintf(stdout, "STDOUT: linea %d\n", i);
        fprintf(stderr, "STDERR: error %d\n", i);
        fflush(stdout);
        fflush(stderr);
        sleep(4);
    }
    return 0;
}