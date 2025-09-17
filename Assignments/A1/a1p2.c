/*  Assignment 1 Part 2
    author: Jed Cravalho 30212116 */


/* Includes */
#include <stdio.h>


/* Primality function */
int is_prime(int num) {
    if (num < 2) return 0;
        for (int i = 2; i <= sqrt(num); i++) {
            if (num % i == 0) return 0;
        }
    return 1;
}

int main(int argc, char *argv[]) {

    /* Create shared memory layout */

    /* Parent Process */
    // allocate shared memory
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