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
    int rowOfTreasure;      // store the row of the winning treasure
    int winnerPID;
    int fromChild;
    int rows = 100;
    int cols = 1000;
    int matrix[rows][cols];

    /* Error checking for valid number of arguments */
    if (argc == 3 && strcmp(argv[1], "<") == 0) {
        /* Open File and convert to 2D Array */
        char *filename = argv[2]; // use 2nd argument for filename
        FILE *file = fopen(filename, "r"); 
        if (file == NULL) {
            printf("There is an error with the file. Ending program.\n");
            exit(1);
            fclose(file);
        } else {

            /* Fill Matrix from test file*/

            for (int i = 0; i < rows; i++) {
                for (int j = 0; j < cols; j++) {
                    fscanf(file, "%d", &matrix[i][j]);
                }
            }
            fclose(file);

            /* Fork Chain */
            for (int i = 0; i < rows; i++) { // create 100 forks, 1 for each row
                fr = fork();
                if (fr < 0) { // if any fork fails to create, kill program
                    fprintf(stderr, "Fork Failed!\n");
                    exit(-1);

                } else if (fr == 0) { 

                    /* Child Process */

                    printf("Child %d (PID %d): Searching row %d\n", i, getpid(), i);
                    for (int j = 0; j < cols; j++) {
                        /* Insert Code to Find Treasure */
                        if (matrix[i][j] == 1) {
                            exit(i+1); // +1 to avoid issues if treasure in row 0
                        }
                    }
                    exit(0);
                }
            }
            
            /* Check each child for success/failure */
            for (int j = 0; j < 100; j++) {
                int status;
                int childPID = wait(&status);
                if (WIFEXITED(status) && WEXITSTATUS(status) > 0) { // if child process exits and succeeds finding the treasure
                    rowOfTreasure = WEXITSTATUS(status) - 1; // -1 to fix offset from exit code
                    winnerPID = childPID;
                }
            }                        
            for (int j = 0; j < 1000; j++) { // Find column of treasure given winning row
                if (matrix[rowOfTreasure][j] == 1) {
                    printf("Parent: the treasure was found by child with PID %d at row %d and column %d\n", winnerPID, rowOfTreasure, j);
                    exit(0);
                }
            }
            printf("Parent: No treasure was found in the matrix."); // If no treasure was found, print and end program
            exit(0);
        }
    } else {
        /* Error checking for incorrect input */

        printf("Please use the following format: ./a1p1 < [text_file]\n");
        printf("%d\n", argc);
        for (int i = 0; i < argc; i++) {
            printf("Argument %d: %s\n", i, argv[i]);
        }
        exit(1); // exit if format is wrong 
    }
}