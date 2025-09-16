#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {

    pid_t fr;


    printf("");
    char filename[] = scanf(stdin);
    FILE *file = fopen(filename, "r");
    if (file != NULL) {
        
    }



    for (int i = 0; i < 100; i++) {
        fr = fork();
        if (fr < 0) {
            fprintf(stderr, "error\n");
            exit(-1);
        } else if (fr == 0) {
        
            
        } else {
            wait(NULL);
            printf("Success!!\n");
            exit(0);
        }
        
    }
    printf("Starting treasure hunt:\n");

    

    
}