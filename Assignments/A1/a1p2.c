/*  Assignment 1 Part 2
    author: Jed Cravalho 30212116 */


/* Includes */
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <unistd.h>
#include <sys/shm.h>
#include <math.h>
#include <sys/wait.h>

/* Primality function given from Assignment */
int is_prime(int num) {
    if (num < 2) return 0;
        for (int i = 2; i <= sqrt(num); i++) {
            if (num % i == 0) return 0;
        }
    return 1;
}

int main(int argc, char *argv[]) {

    /* Variables */
    pid_t fr;                   // store child PID
    int LOWER_BOUND;            // store lower bound (given by argument)
    int UPPER_BOUND;            // store upper bound (given by argument)
    int N;                      // store number of ranges (processes)
    int totalRange;             // store calculated number of range
    int MAX_PRIMES_PER_CHILD;   // store calculated number of primes per child

    /* Get bounds and number of children */
    if (argc != 4) { // Checks if there are exactly 3 arguments
        printf("There must be 3 arguments in the format shown: ./a1p2 [UPPER_BOUND] [LOWER_BOUND] [NUM_PROCESSES]\n");
        exit(0);
    } else if (atoi(argv[3]) <= 0) { // checks if processes are greater than 0
        printf("The number of processes must be greater than 0.\n");
        exit(0);
    } else if (atoi(argv[2]) - atoi(argv[1]) <= 0) { // Checks if there are conflicting bounds
        printf("The difference between the bounds must be greater than 0\n");
        exit(0);
    } else { // calculates all values for later
        LOWER_BOUND = atoi(argv[1]);
        UPPER_BOUND = atoi(argv[2]);
        N = atoi(argv[3]);
        totalRange = UPPER_BOUND - LOWER_BOUND + 1; //+ 1 to count the starting number 
    }

    /* Parent Process */

    if (N > totalRange) { // Sets the highest number of children to the total range (can't have bigger range than children)
        N = totalRange;
    }
    // do floor function for max primes per child. This ensures that there is equal coverage for each process for the entire range of numbers.
    // Note: The last child will carry the remainder (see line 92)
    MAX_PRIMES_PER_CHILD = totalRange / N;

    /* Create shared memory layout */
    // shmid is the identifier of shared memory block
    int shmid = shmget(IPC_PRIVATE, (totalRange) * sizeof(int), IPC_CREAT | 0666); 

    // Basic error-checking for shmid (in case memory is full)
    if (shmid == -1) {
        printf("shmget failed\n");
        exit(1);
    }
    
    // creates pointer to shared memory 
    int *shm_ptr = (int *) shmat(shmid, NULL, 0); 

    // Basic error-checking for shm_ptr
    if (shm_ptr == (void *) -1) {
        printf("shmat failed");
        exit(1);
    }

    /* Forking Processes */
    for (int i = 0; i < N; i++) { // ensures that it will fork N times (and also numbers the children)
        fr = fork();
        if (fr < 0) { // error with child
            fprintf(stderr, "There was an error with child PID: %d. Shutting down.", getpid());
            exit(-1);
        } else if (fr == 0) {
            /* Child Process */
            // Computes a non-overlapping subrange of the total range.
            // The following is taken from the assignment description:
            int low = LOWER_BOUND + i * MAX_PRIMES_PER_CHILD;
            int high = LOWER_BOUND + (i + 1) * MAX_PRIMES_PER_CHILD - 1;

            // Ensures that the subrange never exceeds the upper bound
            // Also ensures that the last child carries the remainder
            if (high > UPPER_BOUND || i == N - 1) {
                high = UPPER_BOUND;
            } 
            printf("Child PID %d checking range [%d, %d]\n", getpid(), low, high);
        
            // Finds prime numbers in that subrange.
            int counter = 0; // counter ensures that each number is placed within its "chunk" in memory
            for (int j = low; j <= high; j++) {
                // Sets every element to 0 as a default. Only changes if prime number is detected. 
                // Makes easy parsing for parent parsing through the array.
                shm_ptr[i * MAX_PRIMES_PER_CHILD + counter] = 0;

                if (is_prime(j) == 1) {
                    shm_ptr[i * MAX_PRIMES_PER_CHILD + counter] = j;
                }
                counter++;
            }
            // Exits.
            _exit(0);
        } 
    }

    /* Parent process again */
    int status; // Status of child process
    int counter = 0;
    for (int i = 0; i < N; i++) { // waits N times (and makes sure they all exit successfully)
        wait(&status);

        // Basic error-checking
        if (WEXITSTATUS(status) != 0) {
            fprintf(stderr, "There was a problem with one of the children. Ending program.\n");
            exit(-1);
        } else {
            counter++; // counter increments for every successful child
        }
    }
    if (counter == N) { // If all children return success, print primes.
        printf("All children finished. Primes found:\n");

        // The for loop runs through all array indexes looking for primes and prints each one.
        for (int i = 0; i < totalRange; i++) {
            if (shm_ptr[i] != 0) {
                printf("%d ", shm_ptr[i]);
            }
        }
    }

    // free up shared memory
    shmdt(shm_ptr);
    shmctl(shmid, IPC_RMID, NULL);
    // exit successfully
    exit(0);
}