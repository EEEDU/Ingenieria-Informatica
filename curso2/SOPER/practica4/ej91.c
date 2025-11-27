#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

volatile sig_atomic_t alarma_recibida = 0;


void manejador_alarma(int signum) {
    alarma_recibida = 1;
}

// Función duerme: bloquea el proceso durante 'segundos' segundos
void duerme(int segundos) {
    // Programar la captura de SIGALRM
    signal(SIGALRM, manejador_alarma);
    
    // Resetear la bandera
    alarma_recibida = 0;
    
    // Programar la alarma
    alarm(segundos);
    
    // Bloquear hasta que llegue la señal
    // pause() se interrumpe cuando llega cualquier señal capturada
    while (!alarma_recibida) {
        pause();
    }
}

int main(int argc, char *argv[]){
    int tiempoX, tiempoY;
    int pid;

    if (argc != 3) {
        fprintf(stderr, "Uso: %s <X_segundos> <Y_segundos>\n", argv[0]);
        fprintf(stderr, "  X: intervalo entre mensajes del hijo\n");
        fprintf(stderr, "  Y: tiempo entre acciones del padre\n");
        return 1;
    }
    tiempoX = atoi(argv[1]);
    tiempoY = atoi(argv[2]);
    if (tiempoX <= 0 || tiempoY <= 0 || tiempoX > tiempoY) {
        fprintf(stderr, "Error: X e Y deben ser mayores que 0 y X debe ser menor a Y\n");
        return 1;
    }

    switch(pid = fork()) {
        case -1:
            // Error
    	    perror("fork");
        case 0:
            // Caso hijo
            int contador = 0;

            while (1)
            {
                printf("[HIJO PID=%d] Mensaje #%d (cada %d segundos)\n", getpid(), ++contador, tiempoX);
                duerme(tiempoX);
            }
            exit(0);
            
        default:
            printf("[PADRE PID=%d] Hijo creado con PID=%d\n", getpid(), pid);
            
            // Fase 1: Dejar al hijo ejecutarse durante Y segundos
            printf("[PADRE] Dejando al hijo ejecutarse durante %d segundos...\n", tiempoY);
            duerme(tiempoY);

            // Fase 2: Suspender al hijo durante Y segundos
            printf("\n[PADRE] Suspendiendo al hijo (SIGTSTP) durante %d segundos...\n", tiempoY);
            kill(pid, SIGTSTP);
            duerme(tiempoY);
            
            // Fase 3: Reanudar al hijo durante Y segundos
            printf("\n[PADRE] Reanudando al hijo (SIGCONT) durante %d segundos...\n", tiempoY);
            kill(pid, SIGCONT);
            duerme(tiempoY);
            
            // Fase 4: Matar al hijo y finalizar
            printf("\n[PADRE] Matando al hijo (SIGKILL) y finalizando...\n");
            kill(pid, SIGKILL);
            
            // Esperar a que el hijo termine (evitar zombies)
            wait(NULL);
            
            printf("[PADRE] Programa terminado.\n");
    } 

    return 0;
}

