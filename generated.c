#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main() {
    system("echo Hello, World!");
    system("echo Running on Linux");
    system("ls -l");
    if (strcmp("linux", "win") == 0) {
    system("echo This is Windows");
    } else {
    system("echo This is not Windows");
    }
    int i;
    for (i = 1; i <= 3; i++) {
    char buff[256];
    sprintf(buff, "echo Iteration %d", i);
    system(buff);
    }
    void myfunc(void) {
    system("echo Inside function");
    }
    myfunc();
    fprintf(stderr, "Error: Something went wrong\n");
    exit(1);
    return 0;
}

