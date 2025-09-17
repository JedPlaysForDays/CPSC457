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
    int noTreasure = 9999;  // return 9999 if no treasure is found
    int rowOfTreasure;
    int winnerPID;
    int fromChild;
    int rows = 100;
    int cols = 1000;

    /* Error checking for valid number of arguments */
    if (argc == 3 && strcmp(argv[1], "<") == 0) {
        /* Open File and convert to 2D Array */
        char *filename = argv[2]; // use 2nd argument for filename
        FILE *file = fopen(filename, "r");
        int matrix[rows][cols];
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
            for (int i = 0; i < rows; i++) {
                fr = fork();
                if (fr < 0) {
                    fprintf(stderr, "Fork Failed!\n");
                    exit(-1);
                } else if (fr == 0) {
                    printf("Child %d (PID %d): Searching row %d\n", i, getpid(), i);
                    for (int j = 0; j < cols; j++) {
                        /* Insert Code to Find treasure here */
                        if (matrix[i][j] == 1) {
                            exit(i+1);
                        }
                    }
                    exit(0);
                }
            }
            
            for (int j = 0; j < 100; j++) {
                int status;
                int childPID = wait(&status);
                if (WIFEXITED(status) && WEXITSTATUS(status) > 0) {
                    rowOfTreasure = WEXITSTATUS(status) - 1;
                    winnerPID = childPID;
                }
            }                        
            for (int j = 0; j < 1000; j++) {
                if (matrix[rowOfTreasure][j] == 1) {
                    printf("Parent: the treasure was found by child with PID %d at row %d and column %d\n", winnerPID, rowOfTreasure, j);
                    exit(0);
                }
            }
        
            fclose(file);
        }
    } else {
        printf("Please use the following format: ./a1p1 < [text_file]\n");
        printf("%d\n", argc);
        for (int i = 0; i < argc; i++) {

            printf("Argument %d: %s\n", i, argv[i]);
        }
        exit(1); // exit if format is wrong 
    }
    
}