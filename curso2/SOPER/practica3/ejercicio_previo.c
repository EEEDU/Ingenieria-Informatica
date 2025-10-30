/*	ejercicio_previo.c	*/

#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

#include "error.h"

int main()
{ 
    int pid_padre, pid;

    switch(fork()) {
        case -1:
    	    syserr("fork");
        case 0:
            // PROCESO HIJO
            printf("SOY EL HIJO\n");

            pid_padre = getppid();
            pid = getpid();

            printf("Mi pid es = %d\n",pid);
            printf("El pid de mi padre antes = %d\n",pid_padre);

            exit(15);

        default:
            // PROCESO PADRE
            printf("SOY EL PADRE\n");
            
            pid_padre = getppid();
            pid = getpid();

            printf("Mi PID: %d\n", getpid());
            printf("PID de mi padre: %d\n", getppid());
            

            wait(NULL);  // Espera a que el hijo termine
            printf("El hijo ha terminado\n");
            exit(0);
    } 

}
