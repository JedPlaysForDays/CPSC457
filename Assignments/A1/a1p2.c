/*  Assignment 1 Part 2
    author: Jed Cravalho 30212116 */


/* Includes */
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <unistd.h>
#include <sys/shm.h>
#include <math.h>

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
    int MAX_PRIMES_PER_CHILD;

    /* Get bounds and number of children */
    if (argc != 4) {
        printf("There must be 3 arguments in the format shown: ./a1p2 [UPPER_BOUND] [LOWER_BOUND] [NUM_PROCESSES]\n");
        exit(0);
    } else if (atoi(argv[3]) <= 0) {
        printf("The number of processes must be greater than 0.\n");
        exit(0);
    } else if (atoi(argv[2]) - atoi(argv[1]) <= 0) {
        printf("The difference between the bounds must be greater than 0\n");
        exit(0);
    } else {
        LOWER_BOUND = atoi(argv[1]);
        UPPER_BOUND = atoi(argv[2]);
        N = atoi(argv[3]);
        totalRange = UPPER_BOUND - LOWER_BOUND + 1; //+1 to correct 0-based numbering
    }

    /* Parent Process */
    // allocate shared memory using shm___
    if (N > totalRange) { // Sets the highest number of children to the total range (can't have bigger range than children)
        N = totalRange;
    }
    // do ceiling function for max primes per child. This ensures that there is equal coverage for each process for the entire range of numbers.
    MAX_PRIMES_PER_CHILD = (totalRange + N - 1) / N;

    /* Create shared memory layout */
    int shmid = shmget(IPC_PRIVATE, N * MAX_PRIMES_PER_CHILD * sizeof(int) + 1, IPC_CREAT | 0666); 
    //shmid is the identifier of shared memory block
    // +1 makes room for the index counter in case range is perfectly divisible by N
    // first "block" (i.e. shm_ptr[0]) reserved for index counter (num primes)
    int *shm_ptr = (int *) shmat(shmid, NULL, 0); //pointer to shared memory 

    for (int i = 0; i < N; i++) {

        fr = fork();
        if (fr < 0) {
            fprintf(stderr, "There was an error with child PID: %d. Shutting down.", getpid());
            exit(-1);
        } else if (fr == 0) {
            /* Child Process */
            // Computes a non-overlapping subrange of the total range.

            int base = 1 + i * MAX_PRIMES_PER_CHILD;
            int count = 0;

            int low = LOWER_BOUND + i * MAX_PRIMES_PER_CHILD;
            int high = LOWER_BOUND + (i + 1) * MAX_PRIMES_PER_CHILD - 1;
            if (high > UPPER_BOUND) {
                high = UPPER_BOUND;
            }

            printf("Child PID %d checking range [%d, %d]\n", getpid(), low, high);
            
            for (int n = LOWER_BOUND; n <= UPPER_BOUND; n++) {
                if (is_prime(n)) {
                    shm_ptr[base + count] = n;
                    count++;
                }
            }
            shm_ptr[base + count] = -1; // sentinel to mark end
            // for (int j = low; j < high; j++) {
            
            // }
            // Finds prime numbers in that subrange.
            // Stores them in the shared memory in a thread-safe manner (e.g., using an offset scheme).
            // Exits.
            exit(0);
        } else {
            
        }
    }
    // spawn children
    // each child only writes to the next following index




    /* Parent process again */

    // wait for all children
    // read shared memory
    // print out all primes (hopefully in order?)
    // free up shared memory
    shmdt(shm_ptr);
    shmctl(shmid, IPC_RMID, NULL);

    exit(0);
}