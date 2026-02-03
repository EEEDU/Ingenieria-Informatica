#include <stdio.h>      // printf, fprintf
#include <stdlib.h>     // atoi, exit
#include <unistd.h>     // fork, write
#include <sys/wait.h>   // wait
#include <signal.h>

int proceso_num = 0;
int segundos = 0;
volatile sig_atomic_t alarma_recibida = 0;
volatile sig_atomic_t sigint_recibida = 0;

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
    sigset_t mask, oldmask, zeromask;

    signal(SIGALRM, captura_alarma);
    signal(SIGINT, captura_exit);

    sigemptyset(&zeromask);
    sigemptyset(&mask);
    sigaddset(&mask, SIGALRM);
    sigaddset(&mask, SIGINT);
    
    sigprocmask(SIG_BLOCK, &mask, &oldmask);


    for(int i = 1; i<=procesos; i++){
        if(fork() == 0) {
            proceso_num = i;
            segundos = i;
            alarm(i);
            while(1){
                sigsuspend(&zeromask);
                sigprocmask(SIG_BLOCK, &mask, NULL);
                if(sigint_recibida) {
                    printf("\nProceso P%d terminando...\n", proceso_num);
                    fflush(stdout);
                    exit(0);
                }
                if(alarma_recibida) {
                    alarma_recibida = 0;
                    segundos += 3;
                    printf("Proceso P%d: %d segundos transcurridos\n", proceso_num, segundos);
                    fflush(stdout);
                    alarm(3);
                }
                sigprocmask(SIG_UNBLOCK, &mask, NULL);

            }
            exit(0);
        }
    }
    for(int i = 0; i<procesos; i++){
        wait(NULL);
    }
    exit(0);
}