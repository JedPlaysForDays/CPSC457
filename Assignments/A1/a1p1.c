/*  CPSC 457 Assignment 1 Part 1
    Author: Jed Cravalho 30212116
*/

/* Includes */
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
int main() {

    pid_t fr;           // store child pid
    char line[1000];    // allocates memory for 1 line from treasure map
    char filename[100]; // allocates memory for name of file

    printf("Enter the file to scan:\n");
    scanf("%s", filename);

    /* Open File */
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("There is an error with the file. Ending program.\n");
        exit(0);
    }

    for (int i = 0; i < 100; i++) {
        fr = fork();
        if (fr < 0) {
            fprintf(stderr, "Fork Failed!\n");
            exit(-1);
        } else if (fr == 0) {
            for (int j = 0; j <= 1000; j++) {
            }
        } else {
            printf("Child %d (PID %d): Searching row %d\n", i, fr, i);
            
        }
    }

    return 0;
}