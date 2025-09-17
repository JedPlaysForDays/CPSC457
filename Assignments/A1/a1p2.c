/*  Assignment 1 Part 2
    author: Jed Cravalho 30212116 */


/* Includes */
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <unistd.h>
#include <sys/shm.h>

/* Primality function */
int is_prime(int num) {
    if (num < 2) return 0;
        for (int i = 2; i <= sqrt(num); i++) {
            if (num % i == 0) return 0;
        }
    return 1;
}

int main(int argc, char *argv[]) {

    /* Variables */
    pid_t fr;
    int LOWER_BOUND;
    int UPPER_BOUND;
    int N;
    int totalRange;

    /* Get bounds and number of children */
    if (argc == 4) {
        LOWER_BOUND = argv[1];
        UPPER_BOUND = argv[2];
        N = argv[3];
        totalRange = UPPER_BOUND - LOWER_BOUND;

    } else {
        printf("There must be 3 arguments in the format shown: ./a1p2 [UPPER_BOUND] [LOWER_BOUND] [NUM_PROCESSES]\n");
    }


    /* Create shared memory layout */
    int shmid = shmget(IPC_PRIVATE, totalRange * sizeof(int), IPC_CREAT | 0666);
    int *shm_ptr = (int *) shmat(shmid, NULL, 0);

    /* Parent Process */
    // allocate shared memory using shm___

    /* Proposed Memory Allocation */
    // shared array index

    // spawn children

    /* Child Process */
    // Computes a non-overlapping subrange of the total range.
    // Finds prime numbers in that subrange.
    // Stores them in the shared memory in a thread-safe manner (e.g., using an offset scheme).
    // Exits.


    /* Parent process again */

    // wait for all children
    // read shared memory
    // print out all primes (hopefully in order?)
    // free up shared memory
}