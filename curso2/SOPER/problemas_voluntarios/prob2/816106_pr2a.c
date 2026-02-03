#include <stdio.h>      // printf, fprintf
#include <stdlib.h>     // atoi, exit
#include <unistd.h>     // fork, write
#include <sys/wait.h>   // wait
#include <signal.h>

int proceso_num = 0;
int segundos = 0;

void captura_alarma(int c) {
    printf("Proceso P%d: %d segundos transcurridos\n", proceso_num, segundos);
    segundos += 3;
    alarm(3); // Reprogramar alarma para 3 segundos después
}

void captura_exit(int c) {
    printf("\nProceso P%d terminando...\n", proceso_num);
    exit(0);
}

int main(){
    int procesos = 3;


    signal(SIGALRM, captura_alarma);
    signal(SIGINT, captura_exit);

    for(int i = 1; i<=procesos; i++){
        if(fork() == 0) {
            proceso_num = i;
            segundos = i;
            alarm(i);
            while(1){
                pause();
            }
            exit(0);
        }
    }
    for(int i = 0; i<procesos; i++){
        wait(NULL);
    }
    exit(0);
}