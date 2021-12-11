#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <time.h>
#include <fcntl.h> 
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/time.h>


#define TOTAL 1000
#define SMALL 10
#define LEITURA 0
#define ESCRITA 1


void fill_array(int *array, int size){
    int i;
	srand(time(NULL));
    for (i = 0; i < size; i++) {
		array[i] = (rand() % 1000 + 1);
		//printf("array[%d]: %d\n", i, array[i]);
	}
}


int make_children(int n) {
    pid_t pid;
    int i;
	pid_t pidF = getpid();
    for (i = 0; i < n; i++) {
        pid = fork();
        if (pid < 0)
            return -1;
        else if (pid == 0)
            return i;
		else if (getpid() == pidF && i == n - 1)
			return pidF;
    }
    return 0;
}

int main(void) {
	int array1[TOTAL];

	fill_array(array1, TOTAL);

/*
	shm_unlink ("/ex09");
	int fd = shm_open("/ex09", O_CREAT|O_EXCL|O_RDWR, S_IRUSR|S_IWUSR);
	
    if (fd == -1) {
        printf("Error opening shared memory. Please check writer.c!\n");
        exit(EXIT_FAILURE);
    }
	*/
	
	int array2[SMALL];
	
	/*
	ftruncate(fd, SMALL * sizeof(int));
	array2 = (int*) mmap(NULL, SMALL * sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	
	if(array2 == NULL) {
		printf("Could not retreive info from shared memory! -Exiting-\n");
		return -1;
	}
	*/
	int fdPipe[2];
	if (pipe(fdPipe) == -1) {
        perror("Pipe failed");
        return 1;
    }
	
	int id = make_children(SMALL);  
	
	int i, j;
	for (i = 0; i < SMALL; i++) {
		if (id == i) {
			int maximo_array1 = 0;
			for (j = 100*i; j < (100*i)+99; j++) {
				if (array1[j] > maximo_array1) {
					maximo_array1 = array1[j];
					
				}
			}
			close(fdPipe[LEITURA]);
			write(fdPipe[ESCRITA], &maximo_array1, sizeof(int));
			close(fdPipe[ESCRITA]);
			//printf("Maximo local childs[%d]: %d\n", i, maximo_array1);

			exit(EXIT_SUCCESS);
		}
	}

	int max_transf;
    int status;

	for (j = 0; j < SMALL; j++) {
		wait(&status);
		//printf("waited for %d childs\n", j+1);
	}

	close(fdPipe[ESCRITA]);

	for (j = 0; j < SMALL; j++) {
		read(fdPipe[LEITURA], &max_transf, sizeof(int));
		array2[j] = max_transf;
		printf("Maximo local retirado do pipe [%d]: %d\n", j, array2[j]);
    }
	close(fdPipe[LEITURA]); 
	
	int maximo_array2 = 0;
	for (i = 0; i < SMALL; i++) {
		if (maximo_array2 < array2[i]) {
			maximo_array2 = array2[i];
		}

	}
	printf("\nMaximo global: %d\n", maximo_array2);
	

/*
    if (munmap((void *) array2, SMALL * sizeof(int)) < 0) {
        printf("Error unmapping at munmap()!\n");
        exit(EXIT_FAILURE);
    }

    if (close(fd) < 0) {
        printf("Error at close()!\n");
        exit(EXIT_FAILURE);
    }
*/

	return 0;
	
}

