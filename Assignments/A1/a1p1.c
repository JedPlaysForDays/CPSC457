#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {

    pid_t fr;
    int toParent;
    int toChild;

    char filename[100];
    printf("Enter the file to scan:\n");
    fgets(filename, sizeof(filename), stdin);

    // Open file
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("An error occured while reading the file\n");
    }


    for (int i = 0; i < 100; i++) {
        fr = fork();
        if (fr < 0) {
            fprintf(stderr, "error\n");
        } else if (fr == 0) {
            printf("I am child %d!\n", i);

        } else {
            printf("Child %d (PID %d): Searching row %d\n", i, fr, i);
        }
    }


    return 0;
}