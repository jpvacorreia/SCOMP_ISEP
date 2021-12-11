#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <time.h>

int main(int argc, char * argv[]){
    pid_t pidp;
    int i;
    struct timespec ts;
    
    //printf("pidp: %d!\n", pidp);
    printf("argv[1]: %s!\n", argv[1]);
    
    if(argc != 2){
        printf("Já foste!\n");
        exit(EXIT_FAILURE);
    }
    
    pidp = atoi(argv[1]);
    printf("pidp: %d!\n", pidp);
    
    
    ts.tv_sec = 0;
    ts.tv_nsec = 10000000;
    
    kill(pidp, SIGUSR1);
    printf("Sent SIGUSR1 to %d!\n", pidp);
    x§§nanosleep(&ts, NULL);
    kill(pidp, SIGUSR2);
    printf("Sent SIGUSR2 to %d!\n", pidp);
    kill(pidp, SIGUSR1);
    printf("Sent SIGUSR1 to %d!\n", pidp);
    kill(pidp, SIGUSR2);
    printf("Sent SIGUSR2 to %d!\n", pidp);
    kill(pidp, SIGUSR2);
    printf("Sent SIGUSR2 to %d!\n", pidp);
    kill(pidp, SIGUSR2);
    printf("Sent SIGUSR2 to %d!\n", pidp);
    kill(pidp, SIGUSR2);
    printf("Sent SIGUSR2 to %d!\n", pidp);
    kill(pidp, SIGUSR2);
    printf("Sent SIGUSR2 to %d!\n", pidp);

}
