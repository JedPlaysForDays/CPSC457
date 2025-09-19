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
    int rowOfTreasure = -1; // store the row of the winning treasure (-1 for no treasure found)
    int winnerPID;          // store winning child's PID   
    int rows = 100;         // set number of rows to 100 (since it's a 100x1000 matrix)
    int cols = 1000;        // set number of rows to 1000
    int matrix[rows][cols]; // create a matrix of 100x1000

    /* Error checking for valid number of arguments */
    if (argc == 3 && strcmp(argv[1], "<") == 0) {
        /* Open File and convert to 2D Array */
        char *filename = argv[2]; // use 2nd argument for filename
        FILE *file = fopen(filename, "r"); 
        if (file == NULL) { // If there's an error with the file, print error message and end program
            printf("There is an error with the file. Ending program.\n");
            exit(1);
            fclose(file);
        } else {
            /* Fill Matrix from test file */
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
                        /* Find Treasure */
                        if (matrix[i][j] == 1) {
                            _exit(i+1); // +1 to avoid issues if treasure in row 0
                        }
                    }
                    _exit(0); // exit 0 if no treasure was found in row
                }
            }
            
            /* Check each child for success/failure */
            for (int j = 0; j < 100; j++) {
                int status;
                int childPID = wait(&status);
                if (WEXITSTATUS(status) > 0) { // if child process exits and succeeds finding the treasure
                    rowOfTreasure = WEXITSTATUS(status) - 1; // -1 to fix offset from exit code, save winning row
                    winnerPID = childPID; // save winning child PID
                }
            }
            // Error-checking if no treasure is in the matrix
            if (rowOfTreasure == -1) {
                printf("Parent: No treasure was found in the matrix\n"); // If no treasure was found, print and end program
                exit(0);     
            }                           
            // Find column of treasure given winning row
            for (int j = 0; j < 1000; j++) { 
                if (matrix[rowOfTreasure][j] == 1) {
                    printf("Parent: the treasure was found by child with PID %d at row %d and column %d\n", winnerPID, rowOfTreasure, j);
                    exit(0);
                } 
            }
        }
    } else {
        /* Error checking for incorrect input */
        printf("Please use the following format: ./a1p1 < [text_file]\n");
        printf("Number of Arguments: %d\n", argc-1);
        for (int i = 0; i < argc; i++) {
            printf("Argument %d: %s\n", i, argv[i]);
        }
        exit(1); // exit if format is wrong 
    }
}