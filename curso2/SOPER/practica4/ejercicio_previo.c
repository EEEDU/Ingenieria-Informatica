#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

int main(int argc, char *argv[]){
    char signal = atoi(argv[1]);

    // Mostrar mensaje
    printf("Se enviará a sí mismo la señal %d\n", signal);
    
    // Enviar la señal al proceso actual
    // getpid() devuelve el PID del proceso actual
    kill(getpid(), signal);
    
    // Esta línea probablemente no se ejecute si la señal termina el proceso
    printf("Señal enviada\n");
    
    return 0;

} 