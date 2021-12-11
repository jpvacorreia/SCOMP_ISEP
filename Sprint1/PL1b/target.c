#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <time.h>

volatile sig_atomic_t USR1_COUNTER = 0;
volatile sig_atomic_t USR2_COUNTER = 0;


void handle_SIGUSR1(int signo, siginfo_t * sinfo, void * env){
    struct timespec ts;
    
    ts.tv_sec = 3;
    ts.tv_nsec = 0000000;
    
    USR1_COUNTER++;
    
    write(STDOUT_FILENO, "SIGUSR1 signal captured!\n", 25);
    write(STDOUT_FILENO, "I can't be interrupted!\n", 24);
    
    nanosleep(&ts, NULL);
    
    write(STDOUT_FILENO, "SIGUSR1 done!\n", 14);
}


void handle_SIGUSR2(int signo, siginfo_t * sinfo, void * env){
    struct timespec ts;
    
    ts.tv_sec = 0;
    ts.tv_nsec = 20000000;
    
    USR2_COUNTER++;
    write(STDOUT_FILENO, "\tSIGUSR2 signal captured!\n", 26);
    nanosleep(&ts, NULL);
    
    write(STDOUT_FILENO, "\tSIGUSR2 done!\n", 15);
}


int main(){
    struct sigaction act1;
    struct sigaction act2;
    
    memset(&act1, 0, sizeof(struct sigaction));
    act1.sa_sigaction = handle_SIGUSR1;
    act1.sa_flags = SA_SIGINFO;
    sigfillset(&act1.sa_mask);
    
    
    memset(&act2, 0, sizeof(struct sigaction));
    act2.sa_sigaction = handle_SIGUSR2;
    act2.sa_flags = SA_SIGINFO;
    sigemptyset(&act2.sa_mask);
    
    /* Setting signal handlers. */
    sigaction(SIGUSR1, &act1, NULL);
    sigaction(SIGUSR2, &act2, NULL);
    
    
    for(;;){
        printf("I’m working!\n");
        sleep(5);
        printf("USR1_COUNTER = %d |\t", USR1_COUNTER);
        printf("USR2_COUNTER = %d\n", USR2_COUNTER);
    }
    
}


