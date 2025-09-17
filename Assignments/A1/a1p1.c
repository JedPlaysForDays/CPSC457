/*  CPSC 457 Assignment 1 Part 1
    Author: Jed Cravalho 30212116
*/

/* Includes */
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>
int main(int argc, char *argv[]) {

    pid_t fr;               // store child pid
    char line[1999];        // allocates memory for 1 line from treasure map
    char filename[100];     // allocates memory for name of file
    int noTreasure = 9999;  // return 9999 if no treasure is found
    int foundTreasure;
    int finishedSearching;
    int rows = 100;
    int cols = 1000;

    /* Error checking for valid number of arguments */
    if (argc == 3 && argv[1] == "<") {
        *filename = argv[2]; // use 2nd argument for filename
    } else {
        printf("Please use the following format: ./a1p1 < *text_file*\n");
        exit(1); // exit if format is wrong 
    }
    
    /* Open File and convert to 2D Array */
    int matrix[rows][cols];
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("There is an error with the file. Ending program.\n");
        exit(1);
        fclose(file);
    } else {
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                fscanf(file, "%d", &matrix[i][j]);
            }
        }
        fclose(file);

        /* Fork Chain */
        for (int i = 0; i < 100; i++) {
            fr = fork();
            if (fr < 0) {
                fprintf(stderr, "Fork Failed!\n");
                exit(-1);
            } else if (fr == 0) {
                printf("Child %d (PID %d): Searching row %d\n", i, getpid(), i);
                fgets(line, sizeof(line), stdin);
                char *token = strtok(line, "\n");
                for (int k = 0; k <= 1000; k++) {
                    line[k] = atoi(token);
                    token = strtok(NULL, " \n");
                }
                printf("%s", line);
                for (int j = 0; j <= 1000; j++) {
                    /* Insert Code to Find treasure here */
                    if ((int) line[j] == 1) {
                        printf("Found Treasure!!\n");
                        foundTreasure = j;
                        exit(foundTreasure);
                    }
                }
                exit(getpid());
            } else {
                int childPid = wait(&foundTreasure);
                printf("Parent: the treasure was found by child with PID %d at row %d and column %d\n", childPid, i, WEXITSTATUS(foundTreasure));
                exit(0);
            }
        }
        fclose(file);
    }
    
}