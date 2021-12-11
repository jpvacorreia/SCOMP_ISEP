#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

#define NR_OF_CHILDREN 5
#define ARRAY_SIZE 1000

int main() {
    int numbers[ARRAY_SIZE];/*Array with random numbers*/
    time_t t; /* needed to init. the random number generator (RNG)*/
    srand((unsigned) time(&t));
    int i;
    int index = 0;
    pid_t pids[NR_OF_CHILDREN];
    for (i = 0; i < ARRAY_SIZE; i++){
        numbers[i] = rand () % 256;
    }


    int fifth_part = (sizeof(numbers)/sizeof(int))/5;
    int maximum = -1;
    pid_t pid;
    int small_limit = 0 ;
    int big_limit = fifth_part;

    for (i=0;i<NR_OF_CHILDREN;i++){
        pid = fork();

        if (pid == 0){
            for (i=small_limit; i< big_limit; i++){
                if (numbers[i] > maximum){
                    maximum = numbers[i];
                }
            }
            exit(maximum);
        }
        big_limit += fifth_part;
        small_limit +=fifth_part;
        pids[index] = pid;
        index++;
    }

    int status;
    for (index = 0; index < NR_OF_CHILDREN; index++) {
        pid = waitpid(pids[index], &status, 0);
        if (WIFEXITED(status)) {
            printf("O %dº filho com o PID = %d terminou e encontrou o máximo = %d\n", index+1, pid, WEXITSTATUS(status));
            if (WEXITSTATUS(status)> maximum){
                maximum = WEXITSTATUS(status);
            }
        }
        printf("O maior deles todos é: %d\n",maximum);
    }

    pid = fork();

    int result[ARRAY_SIZE];
    if (pid==0){
        for (i=0;i<ARRAY_SIZE/2;i++){
            result[i]=((int) numbers[i]/maximum)*100;

            printf("[%d] - %d;\n",i,result[i]);
        }
        exit(&result);
    }
    sleep(1);
    for (i=ARRAY_SIZE/2; i< ARRAY_SIZE; i++){
        result[i]=((int) numbers[i]/maximum)*100;
        printf("[%d] - %d; \n",i,result[i]);
    }


    /*int * aux;
    waitpid(pid,&status,0);
    if(WIFEXITED(status)){
        aux = WEXITSTATUS(status);
        for (i=0;i<ARRAY_SIZE/2;i++){
            result[i] = aux[i];
        }
        printf("\nFull array: ");
        for (i=0; i<ARRAY_SIZE; i++){
            printf("%d ",result[i]);
        }
    }*/

    return 0;
}